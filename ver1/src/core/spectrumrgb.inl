﻿#include "spectrum.hpp"

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB SpectrumRGB::createFromRGB(const std::array<float, 3>& rgb, bool asIllumination)
{
    (void)asIllumination;
    return SpectrumRGB(rgb[0], rgb[1], rgb[2]);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB SpectrumRGB::createFromColorTemp(const float kelvin)
{
    SpectrumRGB ret;
    SpectrumSampled::createFromColorTemp(kelvin).toRGB(ret);
    return ret;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE float SpectrumRGB::energyDif(const SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    const float rd = lhs.r - rhs.r;
    const float gd = lhs.g - rhs.g;
    const float bd = lhs.b - rhs.b;
    return rd*rd + gd*gd + bd*bd;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB::SpectrumRGB(float v)
    :r(v), g(v), b(v)
{}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB::SpectrumRGB(float ar, float ag, float ab)
    :r(ar), g(ag), b(ab)
{
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE void SpectrumRGB::clear()
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool SpectrumRGB::isBlack() const
{
    return
        r == 0.0f &&
        g == 0.0f &&
        b == 0.0f;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB SpectrumRGB::sqrt() const
{
    return
        SpectrumRGB(
            sqrtf(r),
            sqrtf(g),
            sqrtf(b));
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE float SpectrumRGB::luminance() const
{
    return
        0.2126f*r +
        0.7152f*g +
        0.0722f*b;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE void SpectrumRGB::toRGB(SpectrumRGB& argb) const
{
    argb = *this;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE void SpectrumRGB::clamp(float mn, float mx)
{
    const auto clampSub = [&mn, &mx](float v)
    {
        if (v > mx)
        {
            v = mx;
        }
        else if (v < mn)
        {
            v = mn;
        }
        return v;
    };
    r = clampSub(r);
    g = clampSub(g);
    b = clampSub(b);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool SpectrumRGB::hasNaN() const
{
    return
        std::isnan(r) ||
        std::isnan(g) ||
        std::isnan(b);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB& SpectrumRGB::operator =(const SpectrumRGB& other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator+(const SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    SpectrumRGB ret;
    ret.r = lhs.r + rhs.r;
    ret.g = lhs.g + rhs.g;
    ret.b = lhs.b + rhs.b;
    return ret;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator - (const SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    SpectrumRGB ret;
    ret.r = lhs.r - rhs.r;
    ret.g = lhs.g - rhs.g;
    ret.b = lhs.b - rhs.b;
    return ret;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator * (float scale, const SpectrumRGB& spectrum)
{
    return SpectrumRGB(
        spectrum.r * scale,
        spectrum.g * scale,
        spectrum.b * scale);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator * (const SpectrumRGB& spectrum, float scale)
{
    return SpectrumRGB(
        spectrum.r * scale,
        spectrum.g * scale,
        spectrum.b * scale);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator * (const SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    return SpectrumRGB(
        lhs.r * rhs.r,
        lhs.g * rhs.g,
        lhs.b * rhs.b);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator / (const SpectrumRGB& spectrum, float v)
{
    const float iv = 1.0f / v;
    return iv * spectrum;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB operator / (const SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    return SpectrumRGB(
        lhs.r / rhs.r,
        lhs.g / rhs.g,
        lhs.b / rhs.b);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool operator == (const SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    return
        (lhs.r == rhs.r) &&
        (lhs.g == rhs.g) &&
        (lhs.b == rhs.b);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB& operator += (SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    return lhs;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB& operator -= (SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    lhs.r -= rhs.r;
    lhs.g -= rhs.g;
    lhs.b -= rhs.b;
    return lhs;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB& operator *= (SpectrumRGB& spectrum, float scale)
{
    spectrum.r *= scale;
    spectrum.g *= scale;
    spectrum.b *= scale;
    return spectrum;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB& operator *= (SpectrumRGB& lhs, const SpectrumRGB& rhs)
{
    lhs.r *= rhs.r;
    lhs.g *= rhs.g;
    lhs.b *= rhs.b;
    return lhs;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB& operator /= (SpectrumRGB& spectrum, float div)
{
    const float scale = 1.0f / div;
    spectrum.r *= scale;
    spectrum.g *= scale;
    spectrum.b *= scale;
    return spectrum;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE SpectrumRGB lerp(const SpectrumRGB& lhs, const SpectrumRGB& rhs, float factor)
{
    const float t0 = 1.0f - factor;
    const float t1 = factor;
    return t0 * lhs + t1 * rhs;
}

