#ifndef ATTRACTORS_HPP
#define ATTRACTORS_HPP

#include "al/graphics/al_VAOMesh.hpp"
#include "al/math/al_Complex.hpp"
#include "al/math/al_Vec.hpp"

struct Attractor {
  // basic thomas attractor

public:
  // float b = 0.208;
  // float speed = 0.1;

  al::VAOMesh makeNoiseCube(al::VAOMesh &mMesh, float radius = 1.0,
                            int nVertices = 100) {
    mMesh.primitive(al::Mesh::POINTS);
    mMesh.reset();

    for (int i = 0; i < nVertices; ++i) {
      float x = al::rnd::uniformS() * radius;
      float y = al::rnd::uniformS() * radius;
      float z = al::rnd::uniformS() * radius;

      mMesh.vertex(al::Vec3f(x, y, z));
      // mMesh.color(al::RGBA(1.0f, 1.0f, 1.0f, 1.0f ));
    }

    mMesh.update();
    return mMesh;
  }
  void processThomas(al::VAOMesh &mMesh, float time, float speed,
                     float b = 0.208) {
    auto &vertices = mMesh.vertices();
    for (int i = 0; i < mMesh.vertices().size(); ++i) {

      // switch this part to just be a pointer
      float x = vertices[i].x;
      float y = vertices[i].y;
      float z = vertices[i].z;
      float newX = x + (-b * x + sin(y)) * time * speed;
      float newY = y + (-b * y + sin(z)) * time * speed;
      float newZ = z + (-b * z + sin(x)) * time * speed;

      vertices[i] = al::Vec3f(newX, newY, newZ);
    }
  }

  void processChen(al::VAOMesh &mMesh, float time, float speed, float a = 35.0f,
                   float c = 28.0f, float d = 3.0f) {
    auto &vertices = mMesh.vertices();
    for (int i = 0; i < vertices.size(); ++i) {
      float x = vertices[i].x;
      float y = vertices[i].y;
      float z = vertices[i].z;

      float newX = x + (a * (y - x)) * time * speed;
      float newY = y + (c * x - x * z + c * y) * time * speed;
      float newZ = z + (x * y - d * z) * time * speed;

      vertices[i] = al::Vec3f(newX, newY, newZ);
    }
  }

  void processLorenz(al::VAOMesh &mMesh, float time, float speed,
                     float sigma = 10.0f, float rho = 28.0f,
                     float beta = 8.0f / 3.0f) {
    auto &vertices = mMesh.vertices();
    for (int i = 0; i < vertices.size(); ++i) {
      float x = vertices[i].x;
      float y = vertices[i].y;
      float z = vertices[i].z;

      float newX = x + sigma * (y - x) * time * speed;
      float newY = y + (x * (rho - z) - y) * time * speed;
      float newZ = z + (x * y - beta * z) * time * speed;

      vertices[i] = al::Vec3f(newX, newY, newZ);
    }
  }
  void processAizawa(al::VAOMesh &mMesh, float time, float speed,
                     float a = 0.95f, float b = 0.7f, float c = 0.6f,
                     float d = 3.5f, float e = 0.25f, float f = 0.1f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = (z - b) * x - d * y;
      float dy = d * x + (z - b) * y;
      float dz = c + a * z - (z * z * z) / 3.0f - x * x + f * z * x * x * x;
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processChenLee(al::VAOMesh &mMesh, float time, float speed,
                      float a = 5.0f, float b = -10.0f, float d = -0.38f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = a * x - y * z;
      float dy = b * y + x * z;
      float dz = d * z + (x * y) / 3.0f;
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processRossler(al::VAOMesh &mMesh, float time, float speed,
                      float a = 0.2f, float b = 0.2f, float c = 5.7f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = -y - z;
      float dy = x + a * y;
      float dz = b + z * (x - c);
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processHalvorsen(al::VAOMesh &mMesh, float time, float speed,
                        float a = 1.4f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = -a * x - 4.0f * y - 4.0f * z - y * y;
      float dy = -a * y - 4.0f * z - 4.0f * x - z * z;
      float dz = -a * z - 4.0f * x - 4.0f * y - x * x;
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processArneodo(al::VAOMesh &mMesh, float time, float speed,
                      float a = -5.5f, float b = 3.5f, float d = -1.0f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = y;
      float dy = z;
      float dz = -a * x - b * y - z + d * x * x * x;
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processSprottB(al::VAOMesh &mMesh, float time, float speed,
                      float a = 0.4f, float b = 1.2f, float c = 1.0f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = a * y * z;
      float dy = x - b * y;
      float dz = c - x * y;
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processSprottLinzF(al::VAOMesh &mMesh, float time, float speed,
                          float a = 0.5f) {
    auto &vertices = mMesh.vertices();
    for (auto &v : vertices) {
      float x = v.x, y = v.y, z = v.z;
      float dx = y + z;
      float dy = -x + a * y;
      float dz = x * x - z;
      v += al::Vec3f(dx, dy, dz) * time * speed;
    }
  }

  void processDadras(al::VAOMesh &mMesh, float time, float speed,
                     float a = 3.0f, float b = 2.7f, float c = 1.7f,
                     float d = 2.0f, float e = 9.0f) {
    auto &vertices = mMesh.vertices();
    for (int i = 0; i < vertices.size(); ++i) {
      float x = vertices[i].x;
      float y = vertices[i].y;
      float z = vertices[i].z;

      float newX = x + (y - a * x + b * y * z) * time * speed;
      float newY = y + (c * y - x * z + z) * time * speed;
      float newZ = z + (d * x * y - e * z) * time * speed;

      vertices[i] = al::Vec3f(newX, newY, newZ);
    }
  }

  void processTSUCS(al::VAOMesh &mMesh, float time, float speed,
                    float a = 40.0f, float b = 0.833f, float c = 0.5f,
                    float d = 20.0f, float e = 0.65f) {
    auto &vertices = mMesh.vertices();
    for (int i = 0; i < vertices.size(); ++i) {
      float x = vertices[i].x;
      float y = vertices[i].y;
      float z = vertices[i].z;

      float newX = x + (a * (y - x) + d * x * z) * time * speed;
      float newY = y + (c * x - x * z + e * y) * time * speed;
      float newZ = z + (b * z + x * y - x * x) * time * speed;

      vertices[i] = al::Vec3f(newX, newY, newZ);
    }
  }

  void processRabinovich(al::VAOMesh &mMesh, float time, float speed,
                         float a = 0.14f, float b = 0.10f) {
    auto &vertices = mMesh.vertices();
    for (int i = 0; i < vertices.size(); ++i) {
      float x = vertices[i].x;
      float y = vertices[i].y;
      float z = vertices[i].z;

      float newX = x + (y * (z - 1 + x * x) + b * x) * time * speed;
      float newY = y + (x * (3 * z + 1 - x * x) + b * y) * time * speed;
      float newZ = z + (-2 * z * (a + x * y)) * time * speed;

      vertices[i] = al::Vec3f(newX, newY, newZ);
    }
  }
};

#endif