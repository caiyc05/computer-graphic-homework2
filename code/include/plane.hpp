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
        
        if(fabs(Vector3f::dot(dir,normal)) < 1e-6){
            return false;
        }

        float t = (d - Vector3f::dot(normal,origin)) / Vector3f::dot(dir,normal);

        if(tmin > t || t < 0){
            return false;
        }

        if(h.getT() <= t){
            return false;
        }
        
        h.set(t, material, normal);
        
        return true;
    }

protected:
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

