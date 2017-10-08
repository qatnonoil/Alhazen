﻿#include "pch.hpp"
#include "shape.hpp"
#include "core/orthonormalbasis.hpp"
#include "bsdf/bsdf.hpp"
#include "core/object.hpp"
#include "core/transform.hpp"

#include "pch.hpp"
#include "shape.hpp"
#include "core/orthonormalbasis.hpp"
#include "bsdf/bsdf.hpp"
#include "core/object.hpp"
#include "core/transform.hpp"



/*
-------------------------------------------------
-------------------------------------------------
*/
class BoxShape
    :public Shape
{
public:
    BoxShape(const ObjectProp& objectProp);
    AABB aabb() const override;
    bool intersect(const Ray& ray, Intersect* isect) const override;
    bool intersectCheck(const Ray& ray) const override;
private:
#if 0
    // ローカル座標系でのBox。ワールド座標系ではないことに注意。
    AABB aabbLocal_;
    // AABBを変換する
    Transform transform_;
#else
    struct Face
    {
        std::array<Vec3, 4> vs;
        Vec3 n;
    };
    std::array<Face, 6> faces_;
    //
    AABB aabb_;
#endif
};
REGISTER_OBJECT(Shape,BoxShape);
/*
-------------------------------------------------
-------------------------------------------------
*/
BoxShape::BoxShape(const ObjectProp& objectProp)
    :Shape(objectProp)
{
#if 0
    transform_ = Transform(objectProp.findChildByTag("transform"));
    /*
    AABB自体のサイズは指定しない。
    スケールも含めてTransformが吸収する
    */
    const float d = 0.5f;
    aabbLocal_.addPoint(Vec3(-d, -d, -d));
    aabbLocal_.addPoint(Vec3(-d, -d, +d));
    aabbLocal_.addPoint(Vec3(-d, +d, -d));
    aabbLocal_.addPoint(Vec3(-d, +d, +d));
    aabbLocal_.addPoint(Vec3(+d, -d, -d));
    aabbLocal_.addPoint(Vec3(+d, -d, +d));
    aabbLocal_.addPoint(Vec3(+d, +d, -d));
    aabbLocal_.addPoint(Vec3(+d, +d, +d));
#else
    Transform transform(objectProp.findChildByTag("transform"));
    //
    std::array<Vec3, 8> vs;
    vs[0] = transform.toWorld(Vec3(-1.0f, -1.0f, -1.0f));
    vs[1] = transform.toWorld(Vec3(+1.0f, -1.0f, -1.0f));
    vs[2] = transform.toWorld(Vec3(-1.0f, +1.0f, -1.0f));
    vs[3] = transform.toWorld(Vec3(+1.0f, +1.0f, -1.0f));
    vs[4] = transform.toWorld(Vec3(-1.0f, -1.0f, +1.0f));
    vs[5] = transform.toWorld(Vec3(+1.0f, -1.0f, +1.0f));
    vs[6] = transform.toWorld(Vec3(-1.0f, +1.0f, +1.0f));
    vs[7] = transform.toWorld(Vec3(+1.0f, +1.0f, +1.0f));
    //
    const auto createFace = [](
        const Vec3& v0,
        const Vec3& v1,
        const Vec3& v2,
        const Vec3& v3
        ) ->Face
    {
        Face f;
        f.vs[0] = v0;
        f.vs[1] = v1;
        f.vs[2] = v2;
        f.vs[3] = v3;
        f.n = Vec3::cross(v1- v0,v2-v0).normalized();
        return f;
    };
    // -Z
    faces_[0] = createFace(vs[0], vs[1], vs[2], vs[3]);
    // +Z
    faces_[1] = createFace(vs[4], vs[5], vs[6], vs[7]);
    // -Y
    faces_[2] = createFace(vs[0], vs[1], vs[4], vs[5]);
    // +Y
    faces_[3] = createFace(vs[2], vs[3], vs[6], vs[7]);
    // -X
    faces_[4] = createFace(vs[0], vs[2], vs[4], vs[6]);
    // +X
    faces_[5] = createFace(vs[1], vs[3], vs[5], vs[7]);
#endif
    //
    for (auto& v : vs)
    {
        aabb_.addPoint(v);
    }
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE AABB BoxShape::aabb() const
{
    return aabb_;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool BoxShape::intersect(const Ray& ray, _Inout_ Intersect* isect) const
{
#if 0
    // レイの位置と方向をLocal座標系に変換
    const Vec3 no = transform_.toLocal(ray.o);
    const Vec3 nd = transform_.toLocalDir(ray.d);
    const float ndl = nd.length();
    const float rd = ray.d.length();
    const float s = rd / ndl;
    // ローカル座標系でAABBと交差判定
    Intersect isectLocal;
    isectLocal.t = isect->t * s;
    if (aabbLocal_.intersect(Ray(no, nd), &isectLocal))
    {
        // ワールド座標系に戻す
        isect->position = transform_.toWorld(isectLocal.position);
        isect->normal = transform_.toWorldDir(isectLocal.normal);
        isect->t = isectLocal.t * s;
        isect->isHit = isectLocal.isHit;
        return true;
    }
#else
    //
    const auto intersectFace = [](const Ray& ray, const Face& face, _Inout_ Intersect* isect)
    {
        const auto& vs = face.vs;
        const Vec3& n = face.n;
        // HACK: UVは適当
        const Vec2 uv(0.0f);
        return
            intersectTriangle(ray, vs[0], vs[1], vs[2], n, n, n, uv, uv, uv, isect) ||
            intersectTriangle(ray, vs[2], vs[1], vs[3], n, n, n, uv, uv, uv, isect);
    };
    bool isHit = false;
    isHit |= intersectFace(ray, faces_[0], isect);
    isHit |= intersectFace(ray, faces_[1], isect);
    isHit |= intersectFace(ray, faces_[2], isect);
    isHit |= intersectFace(ray, faces_[3], isect);
    isHit |= intersectFace(ray, faces_[4], isect);
    isHit |= intersectFace(ray, faces_[5], isect);
    return isHit;
#endif
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool BoxShape::intersectCheck(const Ray& ray) const
{
    // TODO: より軽い方法があるならそれに切り替える
    Intersect isDummy;
    return intersect(ray, &isDummy);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
class RectangleShape
    :public Shape
{
public:
    RectangleShape(const ObjectProp& objectProp);
    AABB aabb() const override;
    bool intersect(const Ray& ray, Intersect* isect) const override;
    bool intersectCheck(const Ray& ray) const override;
private:
    std::array<Vec3, 4> verts_;
    std::array<Vec2, 4> uvs_;
    Vec3 n_;
    AABB aabb_;
};
REGISTER_OBJECT(Shape, RectangleShape);

/*
-------------------------------------------------
-------------------------------------------------
*/
RectangleShape::RectangleShape(const ObjectProp& objectProp)
    :Shape(objectProp)
{
    //
    verts_[0] = Vec3(-1.0f, -1.0f, 0.0f);
    verts_[1] = Vec3(1.0f, -1.0f, 0.0f);
    verts_[2] = Vec3(-1.0f, 1.0f, 0.0f);
    verts_[3] = Vec3(1.0f, 1.0f, 0.0f);
    uvs_[0] = Vec2(0.0f, 0.0f);
    uvs_[1] = Vec2(1.0f, 0.0f);
    uvs_[2] = Vec2(0.0f, 1.0f);
    uvs_[3] = Vec2(1.0f, 1.0f);
    //
    const Transform transform(objectProp.findChildByTag("transform"));
    verts_[0] = transform.toWorld(verts_[0]);
    verts_[1] = transform.toWorld(verts_[1]);
    verts_[2] = transform.toWorld(verts_[2]);
    verts_[3] = transform.toWorld(verts_[3]);
    n_ = Vec3::cross(verts_[0] - verts_[1], verts_[0] - verts_[2]);
    n_.normalize();
    //
    aabb_.addPoints(
        static_cast<Vec3*>(verts_.data()),
        static_cast<int32_t>(verts_.size()));
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE AABB RectangleShape::aabb() const
{
    return aabb_;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool RectangleShape::intersect(const Ray& ray, _Inout_ Intersect* isect) const
{
    const bool hit0 = intersectTriangle(ray, verts_[0], verts_[1], verts_[2], n_, n_, n_, uvs_[0], uvs_[1], uvs_[2], isect);
    const bool hit1 = intersectTriangle(ray, verts_[2], verts_[1], verts_[3], n_, n_, n_, uvs_[1], uvs_[2], uvs_[3], isect);
    if (!hit0 && !hit1)
    {
        return false;
    }
    isect->bsdf = bsdf_;
    return true;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
INLINE bool RectangleShape::intersectCheck(const Ray& ray) const
{
    // TODO: より軽い方法があるならそれに切り替える
    Intersect isDummy;
    return intersect(ray, &isDummy);
}


/*
-------------------------------------------------
Sphere
-------------------------------------------------
*/
class Sphere
    :public Shape
{
public:
    Sphere(const ObjectProp& prop);
    Sphere(const Vec3& pos, float r);
    AABB aabb() const override;
    bool intersect(const Ray& ray, Intersect* isect) const override;
    bool intersectCheck(const Ray& ray) const override;
private:
    Vec3 pos_;
    float r_;
    AABB aabb_;
};
REGISTER_OBJECT(Shape, Sphere);

/*
-------------------------------------------------
-------------------------------------------------
*/
Sphere::Sphere(const ObjectProp& prop)
    :Shape(prop)
{
    // 幾何情報を取得
    pos_ = prop.findChildBy("name", "center").asXYZ(Vec3(0.0f));
    r_ = prop.findChildBy("name", "radius").asFloat(1.0f);
    const Vec3 xdir = Vec3(r_, 0.0f, 0.0f);
    const Vec3 ydir = Vec3(0.0f, r_, 0.0f);
    const Vec3 zdir = Vec3(0.0f, 0.0f, r_);
    aabb_.addPoint(pos_ + xdir);
    aabb_.addPoint(pos_ - xdir);
    aabb_.addPoint(pos_ + ydir);
    aabb_.addPoint(pos_ - ydir);
    aabb_.addPoint(pos_ + zdir);
    aabb_.addPoint(pos_ - zdir);
}

/*
-------------------------------------------------
-------------------------------------------------
*/
AABB Sphere::aabb() const
{
    return aabb_;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
bool Sphere::intersect(const Ray& ray, _Inout_ Intersect* isect) const
{
    if (intersectSphere(ray, pos_, r_, isect))
    {
        isect->bsdf = bsdf_;
        return true;
    }
    return false;
}

/*
-------------------------------------------------
-------------------------------------------------
*/
bool Sphere::intersectCheck(const Ray& ray) const
{
    return intersectSphereCheck(ray, pos_, r_);
}