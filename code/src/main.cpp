#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        std::cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    SceneParser sceneParser = SceneParser(inputFile.c_str());
    // Then loop over each pixel in the image, shooting a ray
    Camera* camera = sceneParser.getCamera();
    int width = camera->getWidth();
    int height = camera->getHeight();
    Image image(width,height);

    for(int x=0; x < width;x++){
        for(int y=0;y < height;y++){
            //shooting a ray
            Ray ray = camera->generateRay(Vector2f(x,y));
            Group* basegroup = sceneParser.getGroup();
            Hit hit;
            //判断ray和basegroup是否有交点。并且将结果存储在hit中
            bool isIntersect = basegroup->intersect(ray,hit,0);
            if(isIntersect){
                //说明有交点
                //累加来自所有光源的光强影响
                Vector3f finalColor = Vector3f::ZERO;
                for(int li = 0; li < sceneParser.getNumLights();li++){
                    Light* light = sceneParser.getLight(li);
                    //获得光照以及光照
                    Vector3f L,lightColor;
                    light->getIllumination(ray.pointAtParameter(hit.getT()),L,lightColor);
                    finalColor += hit.getMaterial()->Shade(ray,hit,L,lightColor);
                }
                image.SetPixel(x,y,finalColor);
                
            }else{
                //说明不存在交点
                image.SetPixel(x,y,sceneParser.getBackgroundColor());
            }

        }

    }

    image.SaveImage(outputFile.c_str());
    
    std::cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

