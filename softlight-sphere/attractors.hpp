#ifndef ATTRACTORS_HPP
#define ATTRACTORS_HPP

#include "al/graphics/al_VAOMesh.hpp"
#include "al/math/al_Complex.hpp"
#include "al/math/al_Vec.hpp"


struct Attractor {
    //basic thomas attractor

    public:
    //float b = 0.208;
    //float speed = 0.1;
    void processThomas(al::VAOMesh& mMesh, float time, float speed, float b = 0.208){
    auto& vertices = mMesh.vertices();
    for (int i = 0; i < mMesh.vertices().size(); ++i){

        //switch this part to just be a pointer
        float x = vertices[i].x;
        float y = vertices[i].y;
        float z = vertices[i].z;
        float newX = x + (-b * x + sin(y)) * time * speed;
        float newY = y + (-b * y + sin(z)) * time * speed;
        float newZ = z + (-b * z + sin(x)) * time * speed;

        vertices[i] = al::Vec3f(newX,newY,newZ); 

    }


}

};






#endif