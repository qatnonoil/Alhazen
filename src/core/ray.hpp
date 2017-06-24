﻿#pragma once

#include "pch.hpp"
#include "core/math.hpp"
#include "bsdf/bsdf.hpp"
//#include "shape/shape.hpp"

/*
-------------------------------------------------
Ray
TODO: 一度作ったらconstなのでそれを明言するために全体をconstにしたい
-------------------------------------------------
*/
struct Ray
{
public:
    Vec3 o;
    Vec3 d;
    Vec3 dinv;
    float mint;
    float maxt;
    // 波長(m単位)
    float waveLength;
    std::array<int32_t, 3> sign;
public:
    Ray();
    Ray(
        const Vec3& pos,
        const Vec3& dir,
        float waveLength = WAVE_LENGTH_D,
        float mint = 0.0f,
        float maxt = std::numeric_limits < float >::max());
    Ray(const Ray& other);
};

/*
-------------------------------------------------
-------------------------------------------------
*/
struct Ray4
{
public:
    Vec3 o[4];
    Vec3 d[4];
    Vec3 dinv[4];
    float mint[4];
    float maxt[4];
    // 波長(m単位)
    float waveLength[4];
    std::array<int32_t, 3> sign[4];
public:
};

/*
-------------------------------------------------
-------------------------------------------------
*/
struct RaySIMD
{
public:
    Ray origRay;
    // 
    __m128 o[3]; // (x,x,x,x) (y,y,y,y) (z,z,z,z)
    __m128 d[3]; // (x,x,x,x) (y,y,y,y) (z,z,z,z)
    __m128 dinv[3]; // (x,x,x,x) (y,y,y,y) (z,z,z,z)
    __m128 mint; // (mnt,mnt,mnt,mnt)
    std::array<int32_t, 3> sign; // (sx,sy,sz)
    // 波長
    __m128 lamda; // (lamda0,lamda1,lamda2,lamda3)
    //
    RaySIMD(const Ray& ray)
        :origRay(ray)
    {
        const Vec3& ro = ray.o;
        const Vec3& rdi = ray.dinv;
        o[0] = _mm_set_ps1(ro.x());
        o[1] = _mm_set_ps1(ro.y());
        o[2] = _mm_set_ps1(ro.z());
        d[0] = _mm_set_ps1(ray.d.x());
        d[1] = _mm_set_ps1(ray.d.y());
        d[2] = _mm_set_ps1(ray.d.z());
        dinv[0] = _mm_set_ps1(rdi.x());
        dinv[1] = _mm_set_ps1(rdi.y());
        dinv[2] = _mm_set_ps1(rdi.z());
        sign[0] = ray.sign[0];
        sign[1] = ray.sign[1];
        sign[2] = ray.sign[2];
        mint = _mm_set_ps1(ray.mint);
    }
    void upateMintMaxt(float aMint, float aMaxt)
    {
        mint = _mm_set_ps1(aMint);
    }
};

/*
-------------------------------------------------
Intersect
-------------------------------------------------
*/
struct Intersect
{
public:
    float t = std::numeric_limits<float>::max();
    float rayEpsilon = 0.0f;
    Vec3 normal = Vec3(1.0f, 0.0f, 0.0);
    Vec3 position = Vec3(0.0f);
    Vec2 uv = Vec2(0.0f, 0.0f);
    BSDFPtr bsdf = nullptr;
    Spectrum emission = Spectrum::createAsBlack();
    // 交差したのがライトか
    bool isLight = false;
    // HACK: shape.hppをincludeするとビルドできないので一時的に回避するためclass宣言している。
    const class Shape* shape = nullptr;
public:
    Intersect();
    void clear();
    //
    Vec3 uppserSideOrigin() const;
    Vec3 belowSideOrigin() const;
};

/*
-------------------------------------------------
IntersectSIMD
-------------------------------------------------
*/
class IntersectSIMD
{
public:
    __m128 t;
public:
    void upadte(float at)
    {
        t = _mm_set_ps1(at);
    }
};

#include "ray.inl"
