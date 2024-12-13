// SPDX-License-Identifier: MIT
// Copyright (c) 2024, Advanced Micro Devices, Inc. All rights reserved.

#include <hip/hip_runtime.h>

#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>

#include "ck_tile/core.hpp"
#include "ck_tile/ops/epilogue.hpp"
#include "ck_tile/ops/gemm.hpp"
#include "ck_tile/host.hpp"
#include "batched_gemm.hpp"

template <typename ALayout, typename BLayout, typename CLayout>
float batched_gemm(const batched_gemm_kargs& args, const ck_tile::stream_config& s)
{
    // The kPadM, kPadN, kPadK & kBlockPerCu should also come from the Codegen part.
    constexpr bool kPadM        = false;
    constexpr bool kPadN        = false;
    constexpr bool kPadK        = false;
    constexpr bool kTilePermute = false;
    // The rank and permutation will also be generate out by the CodeGen part.
    constexpr ck_tile::index_t kOutputRank = 2;

    constexpr int kBlockPerCu = 1;

    // This part comes from the Codegen
    constexpr ck_tile::index_t M_Tile = 128;
    constexpr ck_tile::index_t N_Tile = 128;
    constexpr ck_tile::index_t K_Tile = 32;

    constexpr ck_tile::index_t M_Warp = 2;
    constexpr ck_tile::index_t N_Warp = 2;
    constexpr ck_tile::index_t K_Warp = 1;

    constexpr ck_tile::index_t M_Warp_Tile = 32;
    constexpr ck_tile::index_t N_Warp_Tile = 32;
    constexpr ck_tile::index_t K_Warp_Tile = 8;

    // Whether doing the CShuffle (transpose before the global memory), depending on the output
    // layout.
    constexpr bool CShuffleEpilogue =
        std::is_same_v<CLayout, ck_tile::tensor_layout::gemm::ColumnMajor>;

    using CodegenGemmShape =
        ck_tile::TileGemmShape<ck_tile::sequence<M_Tile, N_Tile, K_Tile>,
                               ck_tile::sequence<M_Warp, N_Warp, K_Warp>,
                               ck_tile::sequence<M_Warp_Tile, N_Warp_Tile, K_Warp_Tile>>;

    using TilePartitioner = ck_tile::GemmTilePartitioner<CodegenGemmShape>;

    using GemmEpilogue = std::conditional_t<
        CShuffleEpilogue,
        ck_tile::CShuffleEpilogue<ck_tile::CShuffleEpilogueProblem<AccDataType,
                                                                   CDataType,
                                                                   kPadM,
                                                                   kPadN,
                                                                   kTilePermute,
                                                                   kOutputRank,
                                                                   1,
                                                                   0,
                                                                   TilePartitioner::kM,
                                                                   TilePartitioner::kN>>,
        ck_tile::Default2DEpilogue<
            ck_tile::Default2DEpilogueProblem<AccDataType, CDataType, kPadM, kPadN>>>;

    using CodegenGemmTraits =
        ck_tile::TileGemmTraits<kPadM, kPadN, kPadK, ALayout, BLayout, CLayout>;

    using CodegenPipelineProblem = ck_tile::
        GemmPipelineProblem<ADataType, BDataType, AccDataType, CodegenGemmShape, CodegenGemmTraits>;

    using CodegenGemmPipeline = ck_tile::GemmPipelineAGmemBGmemCRegV1<CodegenPipelineProblem>;
    // ToDo: Will add the codegen part to test different pipeline policies in GEMM.
    // Now we only use the BlockGemmASmemBSmemCRegV1DefaultPolicy.
    using Kernel = ck_tile::BatchedGemmKernel<TilePartitioner, CodegenGemmPipeline, GemmEpilogue>;

    auto kargs = Kernel::MakeKargs(args);

    const dim3 grids      = Kernel::GridSize(args);
    constexpr dim3 blocks = Kernel::BlockSize();

    if(s.log_level_ > 0)
    {
        std::cout << "Launching kernel with args:"
                  << " grid: {" << grids.x << ", " << grids.y << ", " << grids.z << "}"
                  << ", blocks: {" << blocks.x << ", " << blocks.y << ", " << blocks.z << "}"
                  << std::endl;
    }

    float ave_time = ck_tile::launch_kernel(
        s, ck_tile::make_kernel<blocks.x, kBlockPerCu>(Kernel{}, grids, blocks, 0, kargs));

    return ave_time;
}

#include "run_batched_gemm_example.inc"

int main(int argc, char* argv[]) { return !run_batched_gemm_example(argc, argv); }