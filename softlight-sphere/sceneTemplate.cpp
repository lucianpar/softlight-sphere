
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
#include <iostream>
#include <string>



// MY CUSTOM INCLUDES:
#include "softlight-sphere/eoys-mesh-fx/ripple.hpp"
#include "softlight-sphere/eoys-mesh-fx/scatter.hpp"
#include "softlight-sphere/eoys-mesh-fx/vfxUtility.hpp"
#include "softlight-sphere/parseObj.hpp"
#include "softlight-sphere/loadAudioScene.hpp"
#include "softlight-sphere/soundObject.hpp"
#include "softlight-sphere/loadAudioScene.hpp"
#include "softlight-sphere/eoys-mesh-fx/vfxMain.hpp"
#include "softlight-sphere/eoys-mesh-fx/scatter.hpp"



/* TO DO:
*implement fx 
*work on scatter effect
*fix stereo
*scene 1 and 2 graphics
*have option for sphere configuration 




* set up containers for everything?

*/








//using namespace al;


class MyApp : public al::App {
 public:

  ////INITIAL OBJECTS AND DECLARATIONS////
  // ->

  //MESHES//
  al::VAOMesh bodyMesh; 
  objParser newObjParser;
  al::Mesh boundarySphere;

  //MESH EFFECTS//
  VertexEffectChain bodyEffectChain;
  RippleEffect bodyRipple;
  ScatterEffect bodyScatter;




   ////DECLARE VALUES FOR EVENT TIMES////
    float moveInEvent = 5.0;
    // i.e. -- moveOutEvent1, StartRipplingEvent1
  



  void onInit() override { 
    gam::sampleRate(audioIO().framesPerSecond()); 
  

    }

  void onCreate() override {
    ////BIOLERPLATE////
    nav().pos(al::Vec3d(0, 0, 5));  // Set the camera to view the scene
    sequencer().playSequence();

    //BOUNDARY MESH
    addSphere(boundarySphere, 7.5); 
    boundarySphere.primitive((al::Mesh::LINES));


    ///////




    ////CREATE MY MESHES////
    bodyMesh.primitive(al::Mesh:: POINTS);

    newObjParser.parse("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/assets/BaseMesh.obj", bodyMesh);
    bodyMesh.translate(0,1.5,1.5);
    bodyMesh.update();



    ////SET MESH EFFECTS////
    bodyScatter.setBaseMesh(bodyMesh.vertices());
    bodyScatter.setParams(1.0, 5.0);
    bodyScatter.setScatterVector(bodyMesh);
    bodyRipple.setParams(5.0, 0.5, 4.0, 'y');



    bodyEffectChain.pushBack(&bodyRipple);
    bodyEffectChain.pushBack(&bodyScatter);
    //bodyScatter.triggerOut(true, bodyMesh);




   

   

  }

  ////BASIC TRIGGERING////
  // bool onKeyDown(const Keyboard& k) override {
  //   if (k.key() == ' ') {
  //     // If the space key is pressed, we will trigger the sequencer
  //     sequencer().playSequence();
  //   }
  //   return true;
  // }

  double t = 0;
  void onAnimate(double dt) override {
     t += dt;
    // Update the sequencer
    sequencer().update(t); // XXX important to call this
    std::cout << "global time: " << t << std::endl;
    // should we call in the audio callback instead?


    //// PROCESS MESH EFFECTS ////
    bodyEffectChain.process(bodyMesh, t);
    bodyScatter.triggerOut(true, bodyMesh);

    //MESH EFFECT SEQUENCING//

    if (t>=moveInEvent){
      bodyScatter.setParams(5.0, 5.0);
      bodyScatter.triggerIn(true);
      if(t<=10.0){
        bodyRipple.setParams(5.0, (0.5-((t-moveInEvent)*0.1)), 4.0, 'y');
      }
    }

  }

  void onDraw(al::Graphics& g) override {
    g.clear(0);
    g.color(1.0);
    g.pointSize( 4.0);
    g.draw(bodyMesh);
    g.draw(boundarySphere);

    
    mSequencer.render(g);
  }

  void onSound(al::AudioIOData& io) override { mSequencer.render(io); }

  al::SynthSequencer& sequencer() { return mSequencer; }
  al::SynthSequencer mSequencer;
};

int main() {
  MyApp app;


  //can this move to onCreate or initial app declarations??

  AudioLoader audioLoader;
  std::vector<std::vector<std::string>> songFiles(5); //vector for each song scene . //NOTE -- VEC IS 0 INDEX, BUT SONG FOLDERS ARE 1 INDEX FOR CLARITY
  audioLoader.loadSceneAudio(songFiles, 1);
  //audioLoader.loadSceneAudio(songFiles, 2);
  //audioLoader.loadSceneAudio(songFiles, 3);
  //audioLoader.loadSceneAudio(songFiles, 4);
  //audioLoader.loadSceneAudio(songFiles, 5);

  // ^^^



  //constants for testing
  double g = 0.7;
  float a = 1.4;
  float b = 1.6;
  float c = 1.0;
  float d = 0.7;

  //assign trajectories in the sequencer!!
  // app.sequencer().add<SoundObject>(0, 44000).set( 0, 0, 0, 0.5, (songFiles[0][0]).c_str(), [&](double t, const al::Vec3f& p) -> al::Vec3f { 
  //   return al::Vec3f(
  //   //body of lambda logic. will replace this will header calls
  //          (sin(a * p.y) + c * cos(a * p.x)),
  //         (sin(b * p.x) + d * cos(b * p.y)), 
  //           p.z
  //       );
  // });
  // app.sequencer().add<SoundObject>(0, 44000).set( 0, 0, 0, 0.5, (songFiles[0][1]).c_str(), [&](double t, const al::Vec3f& p) -> al::Vec3f { 
  //   return al::Vec3f(
  //   //body of lambda logic. will replace this will header calls
  //          (cos(a * p.y) + c * cos(a * p.x)),
  //         (sin(b * p.x) + d * sin(b * p.y)), 
  //           p.z
  //       );
  // });


  app.start();
  return 0;
}
