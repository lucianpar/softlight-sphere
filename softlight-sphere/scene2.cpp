
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
  al::VAOMesh blobMesh; 
  // al::VAOMesh bodyCloud;
  // objParser newObjParser;
  // al::Mesh boundarySphere;
  // NewColorBuffer scene1ColorBuffer;


  std::vector<al::Nav> blobs;
  std::vector<al::Vec3f> velocity;
  std::vector<al::Vec3f> force;
  float scene2Boundary = 10.0f;


  //MESH EFFECTS//
  //for body 
  


  //// END DECLARATIONS FOR SCENE 1 ////



  //GLOBAL TIME PARAMS//
   double globalTime = 0;
   double sceneTime = 0;
   //int sceneIndex; -- moved out of app to be global
  




   ////DECLARE VALUES FOR EVENT TIMES////

   //SCENE 1 
    
    
    //karl code -- need to figure out where to put this

    al::Vec3f randomVec3f(float scale) {
      return al::Vec3f(al::rnd::uniformS(), al::rnd::uniformS(), al::rnd::uniformS()) * scale;
    }

  


  




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

    // al::Vec3f randomVec3f(float scale) {
    //   return Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * scale;
    // }

    // //INITIALIZE LIGHTING 

    nav().pos(0, 0, 0);
    addSphere(blobMesh, 0.5, 50, 50);
    blobMesh.primitive(al::Mesh::TRIANGLES);

    blobMesh.generateNormals();

    // blobMesh.update();

    for (int b = 0; b < 4; ++b) {
      al::Nav p;
      p.pos() = randomVec3f(5);
      p.quat()
          .set(al::rnd::uniformS(), al::rnd::uniformS(), al::rnd::uniformS(),
               al::rnd::uniformS())
          .normalize();
      // p.set(randomVec3f(5), randomVec3f(1));
      blobs.push_back(p);
      velocity.push_back(al::Vec3f(0));
      force.push_back(al::Vec3f(0));
    }

    blobMesh.update();
   



    //end lighting test




    //BOUNDARY MESH
    // addSphere(boundarySphere, 7.5); 
    // boundarySphere.primitive((al::Mesh::LINES));


    ///////


    // SCENE 1 STUFF INITS

    ////CREATE MY MESHES////

    // NEED TO RESET TO RELATIVE PATHS, NOT HARDCODED
    



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
//  float newSpeed = 0.0f;
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
    // bodyEffectChain.process(bodyMesh, globalTime);
    //openingSphereEffectChain.process(openingSphereMesh, globalTime);
    //openingSphereScatter.stop(true);
    //bodyScatter.triggerOut(true, bodyMesh);

    //bodyAttractor.processThomas(bodyMesh, globalTime, 0.001);


    for (int k = 0; k < blobs.size(); ++k) {
      
      al::Vec3f a = blobs[k].pos();
      al::Vec3f origin = al::Vec3f(0,0,0);
      
      al::Vec3f displacement = origin - a;
      float distance = displacement.mag();
      al::Vec3f f = displacement.normalize() * (distance - 1);
      force[k] +=f;



      }
  

// velocity[k] += force[k] * dt;
// blobs[k].pos() += velocity[k] * dt;
    


    //MESH EFFECT SEQUENCING//

    //SCENE 1 -- from 

    // if (globalTime == 1){
    //   bodyScatter.triggerOut(true, bodyMesh);
    // }

    // bodyAttractor.processThomas(bodyMesh, globalTime, 0.0001);
    // bodyMesh.scale(1.01);
    
    float newAmplitude;
    // newSpeed;
    
  
    //SCENE 2 -- from

  }
  //END OF ANIMATE CALLBACK 

  //// INCREMENT VALUES FOR DRAW ////
  float shellIncrementScene1;
  float particleIncrementScene1;
  float pointSize = 3.0;
  void onDraw(al::Graphics& g) override {



    //// SCENE 1 START OF DRAW /////
    if (sceneIndex == 1){

    //THIS SEQUENCE MAKES THE SHELL APPEAR 

    g.clear(0.0,0.0,0.09);
   
  


    //PARTICLES SEQUENCE 1 
    
    g.pointSize(pointSize);
    g.color(1.0,1.0,1.0,0.2);

    for (int i = 0; i < blobs.size(); ++i) {
      g.pushMatrix();
     // g.color(1.0,sin(0.0+fearColorReact),0.5);
      g.translate(blobs[i].pos());
      g.rotate(blobs[i].quat());
      g.draw(blobMesh);
      g.popMatrix();
      //blobMesh.update();
    }
    //g.meshColor();

    //if (sceneTime >= particlesAppearEvent) {
    //g.meshColor();
    g.draw(blobMesh);
   // }
    

    // MAIN COLOR SEQUENCE //
    //color sequence
    // if(sceneTime<=particlesAppearEvent){
    // g.color(0+(sceneTime/particlesAppearEvent));}
    // else{
    //   //g.color(1.0);
    // }


   //only draw once particles have dispersed


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
