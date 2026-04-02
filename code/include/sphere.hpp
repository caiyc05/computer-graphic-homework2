#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        center = Vector3f(0.0,0.0,0.0);
        radius = 1.0;
        material = nullptr;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : 
    Object3D(material),center(center),radius(radius) {
        // 
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
    // 射线：O + t*D
    Vector3f O = r.getOrigin();
    Vector3f D = r.getDirection();

    // 球：|X - C|² = R²
    Vector3f C = center;
    float R = radius;

    // 构建一元二次方程 at² + bt + c = 0
    Vector3f OC = O - C;
    float a = Vector3f::dot(D, D);
    float b = 2 * Vector3f::dot(OC, D);
    float c = Vector3f::dot(OC, OC) - R * R;

    // 判别式
    float delta = b * b - 4 * a * c;

    // 无实根 → 不相交
    if (delta < 0)
        return false;

    // 求两个根
    float sqrtDelta = sqrt(delta);
    float t1 = (-b - sqrtDelta) / (2 * a);
    float t2 = (-b + sqrtDelta) / (2 * a);

    // 找最近的有效 t
    float t = -1;

    // 优先选小的 t（近交点）
    if (t1 >= tmin && t1 < h.getT())
        t = t1;
    else if (t2 >= tmin && t2 < h.getT())
        t = t2;
    else
        return false;

    // 计算交点 & 法线
    Vector3f hitPoint = r.pointAtParameter(t);
    Vector3f normal = hitPoint - center;
    normal.normalize();


    // 更新交点信息
    h.set(t, material, normal);
    return true;
}

protected:
    Vector3f center;
    float radius;

};


#endif
