#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// TODO: Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        this->fx = (this->height/2.0f)/tanf(angle/2.0f);
        this->fy = this->fx;
    }

    Ray generateRay(const Vector2f &point) override {
        // 
        //计算射线
        float cx = this->width / 2;
        float cy = this->height / 2;
        float u = point[0];
        float v = point[1];
        float x = (u-cx)/fx;
        float y = (cy-v)/fy;
        float z = 1;
        Vector3f d = Vector3f(x,y,z).normalized();
        Matrix3f R = Matrix3f(horizontal,-up,direction);
        Vector3f dir = R * d;
        //坐标变换
        Vector3f origin = center;
        
        return Ray(origin,dir);
    }
    float fx,fy;
};

#endif //CAMERA_H
