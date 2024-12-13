// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2024, Advanced Micro Devices, Inc. All rights reserved.

#include "gtest/gtest.h"
#include "ck/utility/data_type.hpp"
#include "ck/utility/type_convert.hpp"

using ck::bf8_fnuz_t;
using ck::f8_convert_rne;
using ck::f8_convert_sr;
using ck::half_t;
using ck::type_convert;

TEST(BF8FNUZ, NumericLimits)
{
    // constants given for negative zero nan mode
    EXPECT_EQ(ck::NumericLimits<bf8_fnuz_t>::Min(), type_convert<bf8_fnuz_t>(0x04));
    EXPECT_EQ(ck::NumericLimits<bf8_fnuz_t>::Max(), type_convert<bf8_fnuz_t>(0x7F));
    EXPECT_EQ(ck::NumericLimits<bf8_fnuz_t>::Lowest(), type_convert<bf8_fnuz_t>(0xFF));
    EXPECT_EQ(ck::NumericLimits<bf8_fnuz_t>::QuietNaN(), type_convert<bf8_fnuz_t>(0x80));
}

TEST(BF8FNUZ, ConvertFP32Nearest)
{
    // fix the tolerance value
    float abs_tol = 1e-6;
    // convert 0 float to bf8 and back, check if holds
    ASSERT_NEAR(0.0f, type_convert<float>(f8_convert_rne<bf8_fnuz_t>(0.0f)), abs_tol);
    // don't run the next test on gfx11 devices
#ifndef CK_SKIP_FLAKY_F8_TEST
    // convert minimal float to bf8 and back, check if holds
    ASSERT_NEAR(std::numeric_limits<float>::min(),
                type_convert<float>(f8_convert_rne<bf8_fnuz_t>(std::numeric_limits<float>::min())),
                abs_tol);
#endif

    const auto max_bf8_t_float = type_convert<float>(ck::NumericLimits<bf8_fnuz_t>::Max());
    // convert maximal bf8_fnuz_t to float and check if equal to 57344.0
    ASSERT_NEAR(
        max_bf8_t_float, type_convert<float>(f8_convert_rne<bf8_fnuz_t>(max_bf8_t_float)), abs_tol);
    // convert maximal float to bf8 and back, check if clipped to 57344.0
    ASSERT_NEAR(max_bf8_t_float,
                type_convert<float>(f8_convert_rne<bf8_fnuz_t>(std::numeric_limits<float>::max())),
                abs_tol);
    // convert inf float to bf8_fnuz_t and check if it is qNan
    ASSERT_NEAR(ck::NumericLimits<bf8_fnuz_t>::QuietNaN(),
                f8_convert_rne<bf8_fnuz_t>(std::numeric_limits<float>::infinity()),
                abs_tol);
    // positive norm float value to bf8 and back, check if holds
    float pos_float = 0.0000762939f;
    ASSERT_NEAR(pos_float, type_convert<float>(f8_convert_rne<bf8_fnuz_t>(pos_float)), abs_tol);
    // negative norm float value to bf8 and back, check if holds
    float neg_float = -0.0000610351f;
    ASSERT_NEAR(neg_float, type_convert<float>(f8_convert_rne<bf8_fnuz_t>(neg_float)), abs_tol);
    // positive subnorm float value to bf8 and back, check if holds
    pos_float = 0.0000305175f;
    ASSERT_NEAR(pos_float, type_convert<float>(f8_convert_rne<bf8_fnuz_t>(pos_float)), abs_tol);
    // negative subnorm float value to bf8 and back, check if holds
    neg_float = -0.0000152587f;
    ASSERT_NEAR(neg_float, type_convert<float>(f8_convert_rne<bf8_fnuz_t>(neg_float)), abs_tol);
}

TEST(BF8FNUZ, ConvertFP32Stochastic)
{
    // fix the tolerance value
    float abs_tol = 1e-6;
    // convert 0 float to bf8 and back, check if holds
    ASSERT_NEAR(0.0f, type_convert<float>(f8_convert_sr<bf8_fnuz_t>(0.0f)), abs_tol);
    // convert minimal float to bf8 and back, check if holds
    ASSERT_NEAR(std::numeric_limits<float>::min(),
                type_convert<float>(f8_convert_sr<bf8_fnuz_t>(std::numeric_limits<float>::min())),
                abs_tol);

    const auto max_bf8_t_float = type_convert<float>(ck::NumericLimits<bf8_fnuz_t>::Max());
    // convert maximal bf8_fnuz_t to float and check if equal to 57344.0
    ASSERT_NEAR(
        max_bf8_t_float, type_convert<float>(f8_convert_sr<bf8_fnuz_t>(max_bf8_t_float)), abs_tol);
    // convert maximal float to bf8 and back, check if clipped to 57344.0
    ASSERT_NEAR(max_bf8_t_float,
                type_convert<float>(f8_convert_sr<bf8_fnuz_t>(std::numeric_limits<float>::max())),
                abs_tol);
    // convert inf float to bf8_fnuz_t and check if it is qNan
    ASSERT_NEAR(ck::NumericLimits<bf8_fnuz_t>::QuietNaN(),
                f8_convert_sr<bf8_fnuz_t>(std::numeric_limits<float>::infinity()),
                abs_tol);
    // positive norm float value to bf8 and back, check if holds
    float pos_float = 0.0000762939f;
    ASSERT_NEAR(pos_float, type_convert<float>(f8_convert_sr<bf8_fnuz_t>(pos_float)), abs_tol);
    // negative norm float value to bf8 and back, check if holds
    float neg_float = -0.0000610351f;
    ASSERT_NEAR(neg_float, type_convert<float>(f8_convert_sr<bf8_fnuz_t>(neg_float)), abs_tol);
    // positive subnorm float value to bf8 and back, check if holds
    pos_float = 0.0000305175f;
    ASSERT_NEAR(pos_float, type_convert<float>(f8_convert_sr<bf8_fnuz_t>(pos_float)), abs_tol);
    // negative subnorm float value to bf8 and back, check if holds
    neg_float = -0.0000152587f;
    ASSERT_NEAR(neg_float, type_convert<float>(f8_convert_sr<bf8_fnuz_t>(neg_float)), abs_tol);
}

TEST(BF8FNUZ, ConvertFP16Nearest)
{
    // fix the tolerance value
    float abs_tol = 1e-3;
    // convert 0 fp16 to bf8 and back, check if holds
    ASSERT_NEAR(
        half_t{0.0}, type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(half_t{0.0})), abs_tol);
    // convert minimal fp16 to bf8 and back, check if holds
    ASSERT_NEAR(ck::NumericLimits<half_t>::Min(),
                type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(ck::NumericLimits<half_t>::Min())),
                abs_tol);

    const auto max_bf8_t_half = type_convert<half_t>(ck::NumericLimits<bf8_fnuz_t>::Max());
    // convert maximal bf8_fnuz_t to fp16 and check if equal to 57344.0
    ASSERT_NEAR(
        max_bf8_t_half, type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(max_bf8_t_half)), abs_tol);
    // convert maximal fp16 to bf8 and back, check if clipped to 57344.0
    ASSERT_NEAR(max_bf8_t_half,
                type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(ck::NumericLimits<half_t>::Max())),
                abs_tol);
    // convert QuietNaN fp16 to bf8_fnuz_t and check if it is QuietNaN
    ASSERT_NEAR(ck::NumericLimits<bf8_fnuz_t>::QuietNaN(),
                f8_convert_rne<bf8_fnuz_t>(ck::NumericLimits<half_t>::QuietNaN()),
                abs_tol);
    // positive norm fp16 value to bf8 and back, check if holds
    half_t pos_half = half_t{0.0000762939};
    ASSERT_NEAR(pos_half, type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(pos_half)), abs_tol);
    // negative norm fp16 value to bf8 and back, check if holds
    half_t neg_half = half_t{-0.0000610351};
    ASSERT_NEAR(neg_half, type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(neg_half)), abs_tol);
    // positive subnorm fp16 value to bf8 and back, check if holds
    pos_half = half_t{0.0000305175};
    ASSERT_NEAR(pos_half, type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(pos_half)), abs_tol);
    // negative subnorm fp16 value to bf8 and back, check if holds
    neg_half = half_t{-0.0000152587};
    ASSERT_NEAR(neg_half, type_convert<half_t>(f8_convert_rne<bf8_fnuz_t>(neg_half)), abs_tol);
}

TEST(BF8FNUZ, ConvertFP16Stochastic)
{
    // fix the tolerance value
    float abs_tol = 1e-3;
    // convert 0 fp16 to bf8 and back, check if holds
    ASSERT_NEAR(half_t{0.0}, type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(half_t{0.0})), abs_tol);
    // convert minimal fp16 to bf8 and back, check if holds
    ASSERT_NEAR(ck::NumericLimits<half_t>::Min(),
                type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(ck::NumericLimits<half_t>::Min())),
                abs_tol);

    const auto max_bf8_t_half = type_convert<half_t>(ck::NumericLimits<bf8_fnuz_t>::Max());
    // convert maximal bf8_fnuz_t to fp16 and check if equal to 57344.0
    ASSERT_NEAR(
        max_bf8_t_half, type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(max_bf8_t_half)), abs_tol);
    // convert maximal fp16 to bf8 and back, check if clipped to 57344.0
    ASSERT_NEAR(max_bf8_t_half,
                type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(ck::NumericLimits<half_t>::Max())),
                abs_tol);
    // convert QuietNaN fp16 to bf8_fnuz_t and check if it is QuietNaN
    ASSERT_NEAR(ck::NumericLimits<bf8_fnuz_t>::QuietNaN(),
                f8_convert_sr<bf8_fnuz_t>(ck::NumericLimits<half_t>::QuietNaN()),
                abs_tol);
    // positive norm fp16 value to bf8 and back, check if holds
    half_t pos_half = half_t{0.0000762939};
    ASSERT_NEAR(pos_half, type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(pos_half)), abs_tol);
    // negative norm fp16 value to bf8 and back, check if holds
    half_t neg_half = half_t{-0.0000610351};
    ASSERT_NEAR(neg_half, type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(neg_half)), abs_tol);
    // positive subnorm fp16 value to bf8 and back, check if holds
    pos_half = half_t{0.0000305175};
    ASSERT_NEAR(pos_half, type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(pos_half)), abs_tol);
    // negative subnorm fp16 value to bf8 and back, check if holds
    neg_half = half_t{-0.0000152587};
    ASSERT_NEAR(neg_half, type_convert<half_t>(f8_convert_sr<bf8_fnuz_t>(neg_half)), abs_tol);
}