
#include "Gamma/Domain.h"
#include "Gamma/Envelope.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/graphics/al_VAO.hpp"
#include "al/graphics/al_VAOMesh.hpp"
#include "al/math/al_Random.hpp"
#include "al/math/al_Vec.hpp"
#include "al/scene/al_SynthSequencer.hpp"
#include "al/ui/al_ControlGUI.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/ui/al_PresetSequencer.hpp"
//#include "al/graphics/al_Asset.hpp"
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Graphics.hpp"
#include "al_ext/assets3d/al_Asset.hpp"
#include "softlight-sphere/parseObj.hpp"
#include "softlight-sphere/soundObject.hpp"



using namespace al;

//testing trajectories

//Vec3f testTraj();

//synth voice for file loading and trajectories moved to header file

class MyApp : public App {
 public:
  al::VAOMesh bodyMesh; //switch to vao mesh
  objParser newObjParser;
  al::Mesh boundarySphere;
  ///SoundObject track1;
  



  void onInit() override { 
    gam::sampleRate(audioIO().framesPerSecond()); 
    //function for loading in all sound files from a given folder

    }

  void onCreate() override {
    nav().pos(Vec3d(0, 0, 5));  // Set the camera to view the scene
    sequencer().playSequence();
    bodyMesh.primitive(Mesh:: POINTS);

    newObjParser.parse("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/assets/BaseMesh.obj", bodyMesh);
    bodyMesh.update();

   
    addSphere(boundarySphere, 7.5); 
     boundarySphere.primitive((Mesh::LINES));



  }

  // bool onKeyDown(const Keyboard& k) override {
  //   if (k.key() == ' ') {
  //     // If the space key is pressed, we will trigger the sequencer
  //     sequencer().playSequence();
  //   }
  //   return true;
  // }

  void onAnimate(double dt) override {
    // Update the sequencer
    sequencer().update(dt); // XXX important to call this
    // should we call in the audio callback instead?
  }

  void onDraw(Graphics& g) override {
    g.clear(0);
    g.color(1.0);
    g.pointSize( 4.0);
    g.draw(bodyMesh);
    g.draw(boundarySphere);

    
    mSequencer.render(g);
  }

  void onSound(AudioIOData& io) override { mSequencer.render(io); }

  SynthSequencer& sequencer() { return mSequencer; }
  SynthSequencer mSequencer;
};

int main() {
  MyApp app;
  

  //constants for testing
  double g = 0.7;
  float a = 1.4;
  float b = 1.6;
  float c = 1.0;
  float d = 0.7;

  //assign trajectories in the sequencer!!
  app.sequencer().add<SoundObject>(0, 44000).set( 0, 0, 0, 0.5, "/Users/lucian/Desktop/falling master 2.wav", [&](double t, const Vec3f& p) -> Vec3f { 
    return Vec3f(
    //body of lambda logic. will replace this will header calls
           (sin(a * p.y) + c * cos(a * p.x)),
          (sin(b * p.x) + d * cos(b * p.y)), 
            p.z
        );
  });
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
