﻿#include "pch.hpp"
#include "ray.hpp"

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE Ray::Ray()
{
    d.normalize();
    dinv = d.invertedSafe(std::numeric_limits <float>::max());
    sign[0] = (d.x() < 0.0f);
    sign[1] = (d.y() < 0.0f);
    sign[2] = (d.z() < 0.0f);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE Ray::Ray(
    Vec3 pos,
    Vec3 dir,
    float awaveLength,
    float amint,
    float amaxt)
    :o(pos),
    d(dir),
    waveLength(awaveLength),
    mint(amint),
    maxt(amaxt)
{
    d.normalize();
    dinv = d.invertedSafe(std::numeric_limits <float>::max());
    sign[0] = (d.x() < 0.0f);
    sign[1] = (d.y() < 0.0f);
    sign[2] = (d.z() < 0.0f);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE Ray::Ray(const Ray& other)
    :o(other.o),
    d(other.d),
    dinv(other.dinv),
    waveLength(other.waveLength),
    mint(other.mint),
    maxt(other.maxt),
    sign(other.sign)
{
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE Intersect::Intersect()
{}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE void Intersect::clear()
{
    *this = Intersect();
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE Vec3 Intersect::uppserSideOrigin() const
{
    // HACK: 本当にこの実装であっているのかかなり怪しい
    return position + normal * rayEpsilon;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE Vec3 Intersect::belowSideOrigin() const
{
    // HACK: 本当にこの実装であっているのかかなり怪しい
    return position - normal * rayEpsilon;
}
