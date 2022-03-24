#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cstdlib>
#include <cstring>

int profile_gemm(int, char*[]);
int profile_gemm_bias_2d(int, char*[]);
int profile_gemm_bias_relu(int, char*[]);
int profile_gemm_bias_relu_add(int, char*[]);
int profile_gemm_reduce(int, char*[]);
int profile_batched_gemm(int, char*[]);
int profile_grouped_gemm(int, char*[]);
int profile_conv_fwd(int, char*[]);
int profile_conv_fwd_bias_relu(int, char*[]);
int profile_conv_fwd_bias_relu_add(int, char*[]);
int profile_conv_fwd_bias_relu_atomic_add(int, char*[]);
int profile_conv_bwd_data(int, char*[]);
int profile_reduce(int, char*[]);

int main(int argc, char* argv[])
{
    if(strcmp(argv[1], "gemm") == 0)
    {
        return profile_gemm(argc, argv);
    }
    else if(strcmp(argv[1], "gemm_bias_2d") == 0)
    {
        return profile_gemm_bias_2d(argc, argv);
    }
    else if(strcmp(argv[1], "gemm_bias_relu") == 0)
    {
        return profile_gemm_bias_relu(argc, argv);
    }
    else if(strcmp(argv[1], "gemm_bias_relu_add") == 0)
    {
        return profile_gemm_bias_relu_add(argc, argv);
    }
    else if(strcmp(argv[1], "gemm_reduce") == 0)
    {
        return profile_gemm_reduce(argc, argv);
    }
    else if(strcmp(argv[1], "batched_gemm") == 0)
    {
        return profile_batched_gemm(argc, argv);
    }
    else if(strcmp(argv[1], "grouped_gemm") == 0)
    {
        profile_grouped_gemm(argc, argv);
    }
    else if(strcmp(argv[1], "conv_fwd") == 0)
    {
        return profile_conv_fwd(argc, argv);
    }
    else if(strcmp(argv[1], "conv_fwd_bias_relu") == 0)
    {
        return profile_conv_fwd_bias_relu(argc, argv);
    }
    else if(strcmp(argv[1], "conv_fwd_bias_relu_add") == 0)
    {
        return profile_conv_fwd_bias_relu_add(argc, argv);
    }
    else if(strcmp(argv[1], "conv_fwd_bias_relu_atomic_add") == 0)
    {
        return profile_conv_fwd_bias_relu_atomic_add(argc, argv);
    }
    else if(strcmp(argv[1], "conv_bwd") == 0)
    {
        return profile_conv_bwd_data(argc, argv);
    }
    else if(strcmp(argv[1], "reduce") == 0)
    {
        return profile_reduce(argc, argv);
    }
    else
    {
        // clang-format off
        printf("arg1: tensor operation (gemm: GEMM\n"
               "                        gemm_bias_2d: GEMM+Bias(2D)\n"
               "                        gemm_bias_relu: GEMM+Bias+ReLU\n"
               "                        gemm_bias_relu_add: GEMM+Bias+ReLU+Add\n"
               "                        gemm_reduce: GEMM+Reduce\n"
               "                        grouped_gemm: Grouped Gemm\n"
               "                        conv_fwd: ForwardConvolution\n"
               "                        conv_fwd_bias_relu: ForwardConvolution+Bias+ReLU\n"
               "                        conv_fwd_bias_relu_add: ForwardConvolution+Bias+ReLU+Add\n"
               "                        conv_fwd_bias_relu_atomic_add: ForwardConvolution+Bias+ReLU+AtomicAdd\n"
               "                        conv_bwd: BackwardConvolution\n"
               "                        reduce: Reduce\n");
        // clang-format on

        return 0;
    }
}