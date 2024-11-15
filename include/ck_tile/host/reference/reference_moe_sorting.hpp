// SPDX-License-Identifier: MIT
// Copyright (c) 2024, Advanced Micro Devices, Inc. All rights reserved.

#pragma once

#include "ck_tile/core.hpp"
#include "ck_tile/host/host_tensor.hpp"

namespace ck_tile {

template <typename WeightType, typename IndexType = index_t>
CK_TILE_HOST void reference_moe_sorting(const HostTensor<IndexType>& topk_ids,
                                        const HostTensor<WeightType>& weights,
                                        HostTensor<IndexType>& p_sorted_token_ids,
                                        HostTensor<WeightType>& sorted_weight,
                                        HostTensor<IndexType>& sorted_expert_ids,
                                        index_t& unit_cnt,
                                        const index_t experts,
                                        const index_t unit_size)
{
    const index_t num_token = topk_ids.mDesc.get_lengths()[0];
    const index_t topk      = topk_ids.mDesc.get_lengths()[1];
    std::vector<std::vector<IndexType>> expert_tokens(experts,
                                                      std::vector<IndexType>(unit_size, num_token));
    std::vector<std::vector<WeightType>> expert_token_weights(
        experts, std::vector<WeightType>(unit_size, 0));
    std::vector<IndexType> expert_slices(experts, 1);
    std::vector<IndexType> expert_slice_idxs(experts, 0);

    for(index_t t = 0; t < num_token; t++)
    {
        for(index_t k = 0; k < topk; k++)
        {
            IndexType e  = topk_ids(t, k);
            WeightType w = weights(t, k);
            index_t idx  = expert_slice_idxs[e];
            if(idx > expert_slices[e] * unit_size - 1)
            {
                expert_slices[e]++;
                index_t new_size = expert_slices[e] * unit_size;
                expert_tokens[e].resize(new_size);
                expert_token_weights[e].resize(new_size);
                for(index_t i = (expert_slices[e] - 1) * unit_size; i < new_size; i++)
                {
                    expert_tokens[e][i]        = num_token;
                    expert_token_weights[e][i] = 0;
                }
            }

            expert_tokens[e][idx]        = t;
            expert_token_weights[e][idx] = w;
            expert_slice_idxs[e]++;
        }
    }

    IndexType* out_tokens    = p_sorted_token_ids.data();
    WeightType* out_weights  = sorted_weight.data();
    IndexType* out_expert_id = sorted_expert_ids.data();
    for(index_t e = 0; e < experts; e++)
    {
        memcpy(out_tokens, expert_tokens[e].data(), sizeof(index_t) * expert_slices[e] * unit_size);
        out_tokens += expert_slices[e] * unit_size;
        memcpy(out_weights,
               expert_token_weights[e].data(),
               sizeof(WeightType) * expert_slices[e] * unit_size);
        out_weights += expert_slices[e] * unit_size;

        for(index_t s = 0; s < expert_slices[e]; s++)
        {
            out_expert_id[s] = e;
            unit_cnt++;
        }
        out_expert_id += expert_slices[e];
    }
    unit_cnt *= unit_size;
    return;
}
} // namespace ck_tile