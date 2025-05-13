#ifndef SOUND_OBJECT_HPP
#define SOUND_OBJECT_HPP

#include <functional>
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/graphics/al_VAOMesh.hpp"
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
#include "al/scene/al_SynthSequencer.hpp"




class SoundObject : public al::SynthVoice {
  gam::SamplePlayer<> mSource;
  al::VAOMesh mesh{al::VAOMesh()};
  al::Vec3f position{0.0f, 0.0f, 0.0f};
  float mSize{1.0f};
  double time = 0.0;

public:
  typedef std::function<al::Vec3f(double, const al::Vec3f&)> FuncType;
  FuncType trajectory;

  inline SoundObject() {
    al::addSphere(mesh);
    mesh.primitive(al::Mesh::POINTS);
  }

  void onProcess(al::AudioIOData& io) override {
    while (io()) {
      io.out(0) += mSource();
    }
    if (mSource.done()) {
      free();
    }
  }

  void update(double dt) override {
    time += dt;
    if (trajectory) {
      position = trajectory(time, position);
    }
  }

  void onProcess(al::Graphics& g) override {
    g.pushMatrix();
    g.translate(position);
    g.scale(mSize * mSource.pos() / mSource.max());
    g.draw(mesh);
    g.popMatrix();
  }

  void set(float x, float y, float size, const char* filename, FuncType func) {
    trajectory = func;
    if (!mSource.load(filename)) {
      std::cerr << "Failed to load sound file: " << filename << std::endl;
    }
    position.set(x, y, 0);
    mSize = size;
  }

  void onTriggerOn() override {
    time = 0;
    mSource.reset();
  }

  void onTriggerOff() override {
    mSource.finish();
  }
};

#endif  // SOUND_OBJECT_HPP