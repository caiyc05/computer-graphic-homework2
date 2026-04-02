#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {
        normal = Vector3f(0,0,1);
        d = 0;
        material = nullptr;
    }

    Plane(const Vector3f &normal, float d, Material *m) :
    Object3D(m),normal(normal),d(d) {

    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f dir = r.getDirection();
        Vector3f origin = r.getOrigin();
        
        if(Vector3f::dot(dir,normal) == 0){
            return false;
        }

        float t = -1*(d + Vector3f::dot(normal,origin)) / Vector3f::dot(dir,normal);

        if(tmin > t || t < 0){
            return false;
        }

        if(h.getT() < t){
            return false;
        }
        // 确保法线指向外部（与视线方向夹角大于90度）
        if (Vector3f::dot(normal, dir) > 0) {
            normal = -normal;
        }
        //计算法线:由于是平面所以法线恒定为normal
        h.set(t,material,normal);
        return true;
    }

protected:
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

