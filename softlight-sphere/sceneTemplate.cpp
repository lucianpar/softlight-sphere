
#include "Gamma/Domain.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/ui/al_PresetSequencer.hpp"
//#include "al/graphics/al_Asset.hpp"
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Graphics.hpp"
#include "al_ext/assets3d/al_Asset.hpp"
#include "softlight-sphere/parseObj.hpp"


// //might be using for parsing
// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include <string>

using namespace al;





//UPDATE THIS TO BE RELEVANT TO PROJECT //
class MyVoice : public SynthVoice {
  gam::SamplePlayer<> mSource;
  Mesh mesh;         // The mesh now belongs to the voice
  float mSize{1.0};  // This are the internal parameters
  Vec3f position;

 public:
  MyVoice() {
    addCone(mesh);  // Prepare mesh to draw a cone
    mesh.primitive(Mesh::LINE_STRIP);
  }

  void onProcess(AudioIOData& io) override {
    // XXX use the position of this voice to spatialize the sound!!
    while (io()) {
      io.out(0) += mSource();
    }
    if (mSource.done()) {
      free();
    }
  }

  double time = 0;

  typedef std::function<Vec3f(double, const Vec3f&)> FuncType;
  FuncType trajectory;

  void update(double dt) override {
    time += dt;

    // if the trajectory function is real, then call it
    if (trajectory) {
     printf("time: %f\n", time);
      fflush(stdout);
      // update our position based on this custom function
      position = trajectory(time, position);
    }
  }

  void onProcess(Graphics& g) override {
    g.pushMatrix();
    g.translate(position);
    g.scale(mSize * mSource.pos() / mSource.max());
    g.draw(mesh);  // Draw the mesh
    g.popMatrix();
  }

  void set(float x, float y, float size, const char* filename, FuncType func) {
    trajectory = func;       // Set the trajectory function
    mSource.load(filename);  // Load a sound file
    position.set(x, y, 0);   // Set the position of the voice
    mSize = size;
  }

  void onTriggerOn() override {
    time = 0;
    mSource.reset();
  }
  void onTriggerOff() override { mSource.finish(); }
};

// Main app functions



class MyApp : public App {
 public:
  al::Mesh bodyMesh; //switch to vao mesh
  objParser newObjParser;

  void onInit() override { 
    gam::sampleRate(audioIO().framesPerSecond()); 

  // Scene *ascene{nullptr};
  // Vec3f scene_min, scene_max, scene_center;
  // Texture tex;
  // std::vector<Mesh> meshes;
  // float a = 0.f;  // current rotation angle


    }

  void onCreate() override {
    nav().pos(Vec3d(0, 0, 8));  // Set the camera to view the scene
    //sequencer().playSequence();
    // if (!bodyMesh.load("softlight-sphere/BaseMesh.obj")) {
    // std::cerr << "Failed to load OBJ file." << std::endl;
    // return;
    //}
    //bodyMesh.primitive(Mesh:: POINTS);
    bodyMesh.primitive(Mesh:: TRIANGLE_FAN);
    //newObjParser.parse("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/BaseMesh.obj", bodyMesh);
    newObjParser.parse("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/lotus-materials.obj", bodyMesh);


  }

  bool onKeyDown(const Keyboard& k) override {
    if (k.key() == ' ') {
      // If the space key is pressed, we will trigger the sequencer
      sequencer().playSequence();
    }
    return true;
  }

  void onAnimate(double dt) override {
    // Update the sequencer
    sequencer().update(dt); // XXX important to call this
    // should we call in the audio callback instead?
  }

  void onDraw(Graphics& g) override {
    g.clear(0);
    g.color(1.0);
    g.pointSize( 10.0);
    g.draw(bodyMesh);

    
    mSequencer.render(g);
  }

  void onSound(AudioIOData& io) override { mSequencer.render(io); }

  SynthSequencer& sequencer() { return mSequencer; }
  SynthSequencer mSequencer;
};

int main() {
  MyApp app;

  

  double g = 0.7;

  app.sequencer().add<MyVoice>(0, 44000).set( 0, 0, 0.5, "/Users/lucian/Desktop/falling master 2.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(0.5, 1).set( 0, 0.5, 0.5, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1, 2).set( 0.5, 0.5, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.1, 2).set( 0.6, 0.5, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.2, 2).set( 0.3, 0.4, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.3, 2).set( 0.2, 0.3, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.4, 2).set( 0.1, 0.2, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.5, 2).set( 0.0, 0.2, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.6, 2).set( -0.1, 0.1, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.7, 2).set( -0.2, 0.0, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.8, 2).set( -0.3, -0.1, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(1.9, 2).set( -0.4, -0.2, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
//   app.sequencer().add<MyVoice>(2.0, 2).set( -0.5, -0.3, 0.7, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });

  app.start();
  return 0;
}
