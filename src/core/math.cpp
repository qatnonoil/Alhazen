﻿#include "pch.hpp"
#include "core/unittest.hpp"
#include "core/math.hpp"

/*
-------------------------------------------------
BoolInVecに関するテストコード
-------------------------------------------------
*/
AL_TEST_IMM(Math, BoolInVec)
{
    // 基本的な真偽の設定
    {
        BoolInVec v(false);
        AL_ASSERT_ALWAYS(!v.value());
        AL_ASSERT_ALWAYS(!(bool)v);
    }
    {
        BoolInVec v(true);
        AL_ASSERT_ALWAYS(v.value());
        AL_ASSERT_ALWAYS((bool)v);
    }
    // 各種代入
    {
        BoolInVec v0(false);
        BoolInVec v1(v0);
        AL_ASSERT_ALWAYS(!(bool)v1);
    }
    {
        BoolInVec v0(true);
        BoolInVec v1(v0);
        AL_ASSERT_ALWAYS((bool)v1);
    }
    {
        BoolInVec v0(false);
        BoolInVec v1(v0.v);
        AL_ASSERT_DEBUG(!(bool)v1);
    }
    {
        BoolInVec v0(true);
        BoolInVec v1(v0.v);
        AL_ASSERT_DEBUG((bool)v1);
    }
    // 非ゼロのtrueがちゃんと判定されるかを入れる
    {
        BoolInVec v;
        v.v = _mm_set1_epi32(2);
        AL_ASSERT_ALWAYS((bool)v);
    }
    // 他のレーンを汚した状態での真偽チェック
    {
        BoolInVec v;
        v.v = _mm_set_epi32(3,2,1,0);
        AL_ASSERT_ALWAYS(!(bool)v);
    }
    {
        BoolInVec v;
        v.v = _mm_set_epi32(3,2,1,1);
        AL_ASSERT_ALWAYS((bool)v);
    }
}

/*
-------------------------------------------------
FloatInVecに関するテスト
-------------------------------------------------
*/
AL_TEST_IMM(Math, FloatInVec)
{
    const float nan = std::numeric_limits<float>::quiet_NaN();
    // 基本的な設定
    {
        FloatInVec v(1.0f);
        AL_ASSERT_ALWAYS(v.value() == 1.0f);
        AL_ASSERT_ALWAYS((float)v == 1.0f);
    }
    // 他のレーンを汚染しても大丈夫かチェック
    {
        FloatInVec v;
        v.v = _mm_set_ps(2.0f,3.0f,4.0f,5.0f);
        AL_ASSERT_ALWAYS(float(v) == 5.0f);
    }
    {
        FloatInVec v;
        v.v = _mm_set_ps(2.0f,3.0f,4.0f,nan);
        AL_ASSERT_ALWAYS(v.isNan());
    }
    {
        FloatInVec v;
        v.v = _mm_set_ps(2.0f,3.0f,nan,1.0f);
        AL_ASSERT_ALWAYS(!v.isNan());
    }
}

/*
-------------------------------------------------
 Vec3に関するテスト
-------------------------------------------------
*/
AL_TEST(Math, Vec3)
{
    // 
    {
        const Vec3 v(1.0f);
        AL_TEST_CHECK(v.x == 1.0f);
        AL_TEST_CHECK(v.y == 1.0f);
        AL_TEST_CHECK(v.z == 1.0f);
    }
    // 
    {
        const float arr[] = { 1.0f,2.0f,3.0f };
        const Vec3 v(arr);
        AL_TEST_CHECK(v.x == 1.0f);
        AL_TEST_CHECK(v.y == 2.0f);
        AL_TEST_CHECK(v.z == 3.0f);
    }
    // zero
    {
        Vec3 v;
        v.zero();
        AL_TEST_CHECK(v.x == 0.0f);
        AL_TEST_CHECK(v.y == 0.0f);
        AL_TEST_CHECK(v.z == 0.0f);
    }
    // swizzle
    {
        const float x = 0.0f;
        const float y = 1.0f;
        const float z = 2.0f;
        const Vec3 v(x, y, z);
        AL_TEST_CHECK(v.xxx() == Vec3(x, x, x));
        AL_TEST_CHECK(v.zyx() == Vec3(z, y, x));
    }
    // TODO: isNan
    // TODO: any/all
}

/*
 -------------------------------------------------
 -------------------------------------------------
 */
AL_TEST_IMM(Math,Matrix3x3)
{
    // TODO: 実装
}

/*
-------------------------------------------------
-------------------------------------------------
*/
AL_TEST_IMM(Math,Matrix4x4)
{
    // TODO: 実装
}

/*
 -------------------------------------------------
 -------------------------------------------------
 */
AL_TEST_IMM(Math,AABB)
{
    // TODO: 実装
}

/*
 -------------------------------------------------
 Vec4に関するテスト
 -------------------------------------------------
 */
AL_TEST_IMM(Math,V4)
{
    // 初期設定テスト
    {
        const Vec4 v(1.0f);
        AL_TEST_CHECK(v.x == 1.0f);
        AL_TEST_CHECK(v.y == 1.0f);
        AL_TEST_CHECK(v.z == 1.0f);
        AL_TEST_CHECK(v.w == 1.0f);
    }
    {
        const Vec4 v(0.0f,1.0f,2.0f,3.0f);
        AL_TEST_CHECK(v.x == 0.0f);
        AL_TEST_CHECK(v.y == 1.0f);
        AL_TEST_CHECK(v.z == 2.0f);
        AL_TEST_CHECK(v.w == 3.0f);
    }
#if 0
    // 長さ
    {
        Vec4 v(1.0f,2.0f,3.0f,4.0f);
        AL_TEST_CHECK(v.lengthSq() == 30.0f);
        AL_TEST_CHECK(std::fabsf(v.length()-5.477225575f));
    }
#endif
    // TODO: swizzleのテストコード
    {
        const float x = 1.0f;
        const float y = 2.0f;
        const float z = 3.0f;
        const float w = 4.0f;
        const Vec4 v(x,y,z,w);
        AL_TEST_CHECK(v.xxxx() == Vec4(x,x,x,x));
        AL_TEST_CHECK(v.yxxx() == Vec4(y,x,x,x));
        AL_TEST_CHECK(v.zxxx() == Vec4(z,x,x,x));
        AL_TEST_CHECK(v.wxxx() == Vec4(y,x,x,x));
        AL_TEST_CHECK(v.xyxx() == Vec4(x,y,x,x));
        AL_TEST_CHECK(v.yyxx() == Vec4(y,y,x,x));
        AL_TEST_CHECK(v.zyxx() == Vec4(z,y,x,x));
        AL_TEST_CHECK(v.wyxx() == Vec4(y,y,x,x));
        AL_TEST_CHECK(v.xzxx() == Vec4(x,z,x,x));
        AL_TEST_CHECK(v.yzxx() == Vec4(y,z,x,x));
        AL_TEST_CHECK(v.zzxx() == Vec4(z,z,x,x));
        AL_TEST_CHECK(v.wzxx() == Vec4(y,z,x,x));
        AL_TEST_CHECK(v.xwxx() == Vec4(x,w,x,x));
        AL_TEST_CHECK(v.ywxx() == Vec4(y,w,x,x));
        AL_TEST_CHECK(v.zwxx() == Vec4(z,w,x,x));
        AL_TEST_CHECK(v.wwxx() == Vec4(y,w,x,x));
        
        // TODO: 参列目をyにするところから続き
        AL_TEST_CHECK(v.xxxx() == Vec4(x,x,x,x));
        AL_TEST_CHECK(v.yxxx() == Vec4(y,x,x,x));
        AL_TEST_CHECK(v.zxxx() == Vec4(z,x,x,x));
        AL_TEST_CHECK(v.wxxx() == Vec4(y,x,x,x));
        AL_TEST_CHECK(v.xyxx() == Vec4(x,y,x,x));
        AL_TEST_CHECK(v.yyxx() == Vec4(y,y,x,x));
        AL_TEST_CHECK(v.zyxx() == Vec4(z,y,x,x));
        AL_TEST_CHECK(v.wyxx() == Vec4(y,y,x,x));
        AL_TEST_CHECK(v.xzxx() == Vec4(x,z,x,x));
        AL_TEST_CHECK(v.yzxx() == Vec4(y,z,x,x));
        AL_TEST_CHECK(v.zzxx() == Vec4(z,z,x,x));
        AL_TEST_CHECK(v.wzxx() == Vec4(y,z,x,x));
        AL_TEST_CHECK(v.xwxx() == Vec4(x,w,x,x));
        AL_TEST_CHECK(v.ywxx() == Vec4(y,w,x,x));
        AL_TEST_CHECK(v.zwxx() == Vec4(z,w,x,x));
        AL_TEST_CHECK(v.wwxx() == Vec4(y,w,x,x));
    }
}












