
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
#include "al/graphics/al_Light.hpp"
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
//#include "softlight-sphere/meshMorph.hpp"
#include "softlight-sphere/parseObj.hpp"
#include "softlight-sphere/loadAudioScene.hpp"
#include "softlight-sphere/soundObject.hpp"
#include "softlight-sphere/loadAudioScene.hpp"
#include "softlight-sphere/eoys-mesh-fx/vfxMain.hpp"
#include "softlight-sphere/eoys-mesh-fx/scatter.hpp"
//#include "softlight-sphere/imageToMesh.hpp"
//#include "softlight-sphere/imageToSphere.hpp"
#include "softlight-sphere/attractors.hpp"
#include "softlight-sphere/imageColorToMesh.hpp"



/* TO DO:

*figure out setting up bodyMesh as a uv for rotation
* set up newer sequence -
* make main shell transition to body - setting up heaeder
*ask karl about lighting
*scene 1 and 2 graphics 

- START ACTUALLY SEQUENCING BASED ON TRACKS 


- change how the sequencer works
- SET UP TRACK ANALYZER / STRING SEQUENCE GENERATOR 
*have option for sphere configuration 

* set up scene containers for everything?

*/





int sceneIndex = 1;


//using namespace al;


class MyApp : public al::App {
 public:

  ////INITIAL OBJECTS AND DECLARATIONS////
  // ->
  al::Light light;

  //// START DECLARATIONS FOR SCENE 1 ////

  //MESHES//
  al::VAOMesh bodyMesh; 
  objParser newObjParser;
  al::Mesh boundarySphere;
  NewColorBuffer scene1ColorBuffer;

  //MESH EFFECTS//
  //for body 
  VertexEffectChain bodyEffectChain;
  RippleEffect bodyRippleY;
  RippleEffect bodyRippleX;
  ScatterEffect bodyScatter;
  Attractor bodyAttractor;


  //// END DECLARATIONS FOR SCENE 1 ////



  //GLOBAL TIME PARAMS//
   double globalTime = 0;
   double sceneTime = 0;
   //int sceneIndex; -- moved out of app to be global
  




   ////DECLARE VALUES FOR EVENT TIMES////

   //SCENE 1 
    float rippleAmplitudeTrack1 = 1.0;

    float shellTurnsWhiteEvent = 7.0f;
  
    float particlesAppearEvent = 7.0f;

    float particlesSlowRippleEvent = 10.0f;

    float rippleSpeedUpEvent = 30.0f;

    float stopSpeedUpEvent = 62.0f;

    float startTurnShellBlack = 62.0f;

    float stopTurnShellBlack = 64.0f;

    float startAttractor = 70.0f;

    float startScaleIn = 80.0f;

    float startTransitionToBody = 85.0f;

     float stopRippleEvent = 10.0f;


     float moveInEvent = 70.0;




  //SCENE 2

    //float track2Start = 119;
    
    // i.e. -- moveOutEvent1, StartRipplingEvent1
  



  void onInit() override { 
    gam::sampleRate(audioIO().framesPerSecond()); 
  

    }

  void onCreate() override {
     ////BIOLERPLATE////
    nav().pos(al::Vec3d(0, 0, 0));  // Set the camera to view the scene
    sequencer().playSequence();

    // //INITIALIZE LIGHTING 
   



    //end lighting test




    //BOUNDARY MESH
    addSphere(boundarySphere, 7.5); 
    boundarySphere.primitive((al::Mesh::LINES));


    ///////


    // SCENE 1 STUFF INITS

    ////CREATE MY MESHES////

    // NEED TO RESET TO RELATIVE PATHS, NOT HARDCODED
    bodyMesh.primitive(al::Mesh:: POINTS);

    newObjParser.parse("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/assets/BaseMesh.obj", bodyMesh);
    bodyMesh.translate(0,3.5,-4);

    scene1ColorBuffer.imageToNewMesh("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/assets/9400image.png", bodyMesh);

    
    bodyMesh.update();

    
    



    ////SET MESH EFFECTS////
    //for body
    bodyScatter.setBaseMesh(bodyMesh.vertices());
    bodyScatter.setParams(1.0, 20.0);
    bodyScatter.setScatterVector(bodyMesh);
    bodyRippleY.setParams(5.0, rippleAmplitudeTrack1, 4.0, 'y');
    bodyRippleX.setParams(10.0, rippleAmplitudeTrack1, 6.0, 'x');



    bodyEffectChain.pushBack(&bodyRippleY);
    bodyEffectChain.pushBack(&bodyRippleX);
    bodyEffectChain.pushBack(&bodyScatter);

  
    //bodyScatter.triggerOut(true, bodyMesh);



     // SCENE 2 STUFF INITS
   

   

  }

  ////BASIC TRIGGERING////
  bool onKeyDown(const al::Keyboard& k) override {

    //if (k.key() == '1') {
      // If the space key is pressed, we will trigger the sequencer
      //sequencer().playSequence();
      if (k.key() >=49 && k.key() <= 54){
      sceneIndex = k.key()-48;
      std::cout << "pressed key: " << sceneIndex << std::endl;
      }
    
    return true;
  }

  void onAnimate(double dt) override {

    // SET SCENES AND TIME TRANSITIONS ///
     globalTime += dt;
     sceneTime += dt;
     if (globalTime == 118) {
      sceneIndex = 2;
      sceneTime = 0;
     }
     if (globalTime == 334) {
      sceneIndex = 3;
      sceneTime = 0;
     }


    // Update the sequencer
    sequencer().update(globalTime); // XXX important to call this
    std::cout << "global time: " << globalTime << std::endl;
     fflush(stdout);
    // should we call in the audio callback instead?


    //// PROCESS MESH EFFECTS ////
    bodyEffectChain.process(bodyMesh, globalTime);
    //openingSphereEffectChain.process(openingSphereMesh, globalTime);
    //openingSphereScatter.stop(true);
    bodyScatter.triggerOut(true, bodyMesh);

    bodyAttractor.processThomas(bodyMesh, globalTime, 0.001);




    //MESH EFFECT SEQUENCING//

    //SCENE 1 -- from 
    
    float newAmplitude;
    // if (globalTime<=moveInEvent){newAmplitude = 0;}
    // if (globalTime>=moveInEvent){
    //   bodyScatter.setParams(5.0, 20.0);
    //   bodyScatter.triggerIn(true);

  
   
    //     if (globalTime<=stopRippleEvent) { //slows down rippling
    //      newAmplitude = (rippleAmplitudeTrack1-((globalTime-moveInEvent) /(stopRippleEvent-moveInEvent)));
    //     }
    //     else{ newAmplitude = 0.0f; //once time is past ripple event, keep at 0 so amp doesn't go negative
    //     }
    //     bodyRippleY.setParams(5.0, newAmplitude , 4.0,'y'); //
    //     bodyRippleX.setParams(10.0, newAmplitude, 4.0, 'x');


    // }
  
    //SCENE 2 -- from

  }
  //END OF ANIMATE CALLBACK 

  //// INCREMENT VALUES FOR DRAW ////
  float shellIncrementScene1;
  float particleIncrementScene1;
  void onDraw(al::Graphics& g) override {



    //// SCENE 1 START OF DRAW /////
    if (sceneIndex == 1){

    //THIS SEQUENCE MAKES THE SHELL APPEAR 
    g.clear(0);
    // if (globalTime < shellTurnsWhiteEvent) {
    // shellIncrementScene1 += (globalTime / (shellTurnsWhiteEvent*60));
    // g.clear(shellIncrementScene1);
    // }
    // if (globalTime >= shellTurnsWhiteEvent){
    // g.clear(1.0);
    // }
    // if (globalTime >= startTurnShellBlack && globalTime<= (startTurnShellBlack + 3)){
    //   shellIncrementScene1 -=  (globalTime / (shellTurnsWhiteEvent*60));
    //   g.clear(1.0-shellIncrementScene1);
    // }
  


    //PARTICLES SEQUENCE 1 
    g.pointSize(2.0);
    //g.color(1.0);
    g.meshColor();

    // if (globalTime >= particlesAppearEvent) {
    // g.meshColor();
    // }
    

    // MAIN COLOR SEQUENCE //
    //color sequence
    // if(globalTime<=particlesAppearEvent){
    // g.color(0+(globalTime/particlesAppearEvent));}
    // else{
    //   //g.color(1.0);
    // }


    g.draw(bodyMesh); //only draw once particles have dispersed


    //g.draw(boundarySphere);

  } //// SCENE 1 END OF DRAW /////



    
    

    
    mSequencer.render(g);
  }

  void onSound(al::AudioIOData& io) override { mSequencer.render(io); }

  al::SynthSequencer& sequencer() { return mSequencer; }
  al::SynthSequencer mSequencer;
};

int main() {
  MyApp app;
  //int sceneIndex;


  //can this section move to onCreate or initial app declarations?? or is it fine here//

  bool soundObjectVisual = false; 
  AudioLoader audioLoader;
  std::vector<std::vector<std::string>> songFiles(5); //vector for each song scene . //NOTE -- VEC IS 0 INDEX, BUT SONG FOLDERS ARE 1 INDEX FOR CLARITY
  audioLoader.loadSceneAudio(songFiles, 1);
  audioLoader.loadSceneAudio(songFiles, 2);
  //audioLoader.loadSceneAudio(songFiles, 2);
  //audioLoader.loadSceneAudio(songFiles, 3);
  //audioLoader.loadSceneAudio(songFiles, 4);
  //audioLoader.loadSceneAudio(songFiles, 5);
  //audioLoader.loadSceneAudio(songFiles, 6);

  // ^^^



  //constants for testing
  double g = 0.7;
  float a = 1.4;
  float b = 1.6;
  float c = 1.0;
  float d = 0.7;



  //group audio by every scene. make multiple sequencers to trigger when every scene index is switched
  
  //assign trajectories in the sequencer!!

  // UNCOMMENT AUDIO !!!!!!
  // this logic doesn't work, will  have to change
  if (sceneIndex ==1){
  app.sequencer().add<SoundObject>(0, 44000).set( 0, 0, 0, 0.5,soundObjectVisual ,(songFiles[0][0]).c_str(), [&](double t, const al::Vec3f& p) -> al::Vec3f { 
    return al::Vec3f(
    //body of lambda logic. will replace this will header calls
           (sin(a * p.y) + c * cos(a * p.x)),
          (sin(b * p.x) + d * cos(b * p.y)), 
            p.z
        );
  });
  }
  if (sceneIndex == 2) {
  app.sequencer().add<SoundObject>(0, 44000).set( 0, 0, 0, 0.5,soundObjectVisual ,(songFiles[1][0]).c_str(), [&](double t, const al::Vec3f& p) -> al::Vec3f { 
    return al::Vec3f(
    //body of lambda logic. will replace this will header calls
           (cos(a * p.y) + c * cos(a * p.x)),
          (sin(b * p.x) + d * sin(b * p.y)), 
            p.z
        );
  });
  }


  app.start();
  return 0;
}
