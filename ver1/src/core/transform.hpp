﻿#pragma once

#include "core/object.hpp"
#include "math.hpp"

/*
-------------------------------------------------
Transform
WorldとLocalの間での全ての線形変換を扱う
WorldとLocalの間で回転だけを扱いたい場合はOrthonormalBasisクラスを使う
-------------------------------------------------
*/
class Transform AL_FINAL
{
public:
    Transform();
    Transform(const ObjectProp& objectProp);
    const Vec3 toWorld(Vec3 pos) const;
    const Vec3 toLocal(Vec3 pos) const;
    const Vec3 toWorldDir(Vec3 dir) const;
    const Vec3 toLocalDir(Vec3 dir) const;
    //
    Vec3 cameraOrigin() const;
    Vec3 cameraUp() const;
    Vec3 cameraDir() const;
    Vec3 cameraRight() const;

private:
    Matrix4x4 constructToWorldMatrix(const ObjectProp& objectProp) const;

public: // HACK:
    // 一部内部パラメーターを触っている箇所があるのでそれが解消されるまで開いておく
    Matrix4x4 toWorld_;
    Matrix4x4 toLocal_;
    Matrix3x3 toWorldDir_;
    Matrix3x3 toLocalDir_;
};
