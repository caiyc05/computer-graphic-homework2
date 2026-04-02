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
        // 球心到射线起点
        Vector3f oc = center - r.getOrigin();    
        // 获取方向向量      
        Vector3f dir = r.getDirection();   
        
        // 计算球心到射线上的投影长度
        float projectionLength = Vector3f::dot(oc, dir);
        
        // 如果投影长度为负，且起点在球外，则射线背离球体
        if (projectionLength < 0 && oc.length() > radius) {
            return false;
        }
        
        // 计算球心到射线的垂直距离
        Vector3f projectPoint = r.getOrigin() + dir * projectionLength;
        float distanceSquared = (projectPoint - center).squaredLength();
        
        // 垂直距离大于半径，不相交
        if (distanceSquared > radius * radius) {
            return false;
        }
        
        // 计算交点距离
        float halfChord = sqrt(radius * radius - distanceSquared);
        //划归为以direction长度为单位的t值
        halfChord /= dir.length();
        
        // 分类讨论：
        float t;
        if(oc.length() > radius){
            //origin 在球外。此时projectionLength一定是正的
            t = projectionLength - halfChord;
        }
        else if(oc.length() < radius){
            //origin在球内
            t = projectionLength + halfChord;
        }
        else{
            //说明origin正好在球面上
            t = 0;
        }
        //判断合理性
        if(tmin > t){
            return false;
        }
        //不是离相机最近的
        if(t > h.getT()){
            return false;
        }
        
        // 计算法线
        Vector3f intersectionPoint = r.pointAtParameter(t);
        Vector3f normal = (intersectionPoint - center) / radius;  // 归一化
        
        // 确保法线指向外部（与视线方向夹角大于90度）
        if (Vector3f::dot(normal, dir) > 0) {
            normal = -normal;
        }
        
        h.set(t, material, normal);
        return true;
}

protected:
    Vector3f center;
    float radius;

};


#endif
