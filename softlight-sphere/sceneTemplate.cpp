
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
#include "softlight-sphere/loadAudioScene.hpp"
#include "softlight-sphere/soundObject.hpp"

// #include <dirent.h>
#include <string>
//#include <sys/types.h>
#include <vector>
#include "softlight-sphere/loadAudioScene.hpp"







using namespace al;

//testing trajectories

//Vec3f testTraj();

//synth voice for file loading and trajectories moved to header file

// void loadSceneAudio(std::vector<std::vector<std::string>>& storageVector, int sceneNumber) {
//    std::string path = "/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/audioFolder-" + std::to_string(sceneNumber);
//     //method using posix, this c++ version is too old for anything else
//     DIR *dir;
//     struct dirent *entry;
//     dir = opendir(path.c_str());
//     if (dir == nullptr) { //if the directory doesn't exist//
//         std::cerr << "Error: Directory not found.\n";
//         //return 1;
//     }

//     while ((entry = readdir(dir)) != nullptr) { //if directory exists//
//         std::string fileName = entry->d_name;
//         if (fileName != "." && fileName != "..") { // Skip . and ..
//             //std::cout << fileName << std::endl;
//             storageVector[sceneNumber-1].push_back(path + "/" + fileName);


//         }
//     }
//     closedir(dir);
//     //std::cout << song1files[0] << song1files[1] << song1files[2] << std::endl;
//     std::cout << storageVector[0][0].c_str() << std::endl;

// }


class MyApp : public App {
 public:

  //boiler plate:

  //std::vector<std::vector<std::string>> songFiles; //vector for each song scene / 

  ////INITIAL OBJECTS AND DECLARATIONS////
  // ->

  al::VAOMesh bodyMesh; //switch to vao mesh
  objParser newObjParser;
  al::Mesh boundarySphere;
  ///SoundObject track1;
  



  void onInit() override { 
    gam::sampleRate(audioIO().framesPerSecond()); 
    //function for loading in all sound files from a given folder
  

    }

  void onCreate() override {
    ////BIOLERPLATE////
    nav().pos(Vec3d(0, 0, 5));  // Set the camera to view the scene
    sequencer().playSequence();

    //BOUNDARY MESH
    addSphere(boundarySphere, 7.5); 
    boundarySphere.primitive((Mesh::LINES));


    ///////




    ////CREATE MY MESHES////
    bodyMesh.primitive(Mesh:: POINTS);

    newObjParser.parse("/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/assets/BaseMesh.obj", bodyMesh);
    bodyMesh.update();

   
    // addSphere(boundarySphere, 7.5); 
    //  boundarySphere.primitive((Mesh::LINES));

  }

  ////BASIC TRIGGERING////
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
  AudioLoader audioLoader;
  //std::vector<std::string> songs;
  std::vector<std::vector<std::string>> songFiles(5); //vector for each song scene / 
  
  //call for every scene
  audioLoader.loadSceneAudio(songFiles, 1);

    //function taking songfile index and spitting out correct file

  // std::string getTrackIndexPath(int sceneNumber, int fileNumber){

  // }

    //ENCAPSULATE INTO FUNCTION
    // std::string path = "/Users/lucian/Desktop/201B/allolib_playground/softlight-sphere/test-audio";
    // //method using posix, this c++ version is too old for anything else
    // DIR *dir;
    // struct dirent *entry;
    // dir = opendir(path.c_str());
    // if (dir == nullptr) { //if the directory doesn't exist//
    //     std::cerr << "Error: Directory not found.\n";
    //     return 1;
    // }

    // while ((entry = readdir(dir)) != nullptr) { //if directory exists//
    //     std::string fileName = entry->d_name;
    //     if (fileName != "." && fileName != "..") { // Skip . and ..
    //         //std::cout << fileName << std::endl;
    //         songFiles[1].push_back(fileName);

    //     }
    // }
    // closedir(dir);
    // //std::cout << song1files[0] << song1files[1] << song1files[2] << std::endl;
    // std::cout << songFiles[0][0].c_str() << std::endl;
  

  

  //constants for testing
  double g = 0.7;
  float a = 1.4;
  float b = 1.6;
  float c = 1.0;
  float d = 0.7;

  //assign trajectories in the sequencer!!
  app.sequencer().add<SoundObject>(0, 44000).set( 0, 0, 0, 0.5, (songFiles[0][0]).c_str(), [&](double t, const Vec3f& p) -> Vec3f { 
    return Vec3f(
    //body of lambda logic. will replace this will header calls
           (sin(a * p.y) + c * cos(a * p.x)),
          (sin(b * p.x) + d * cos(b * p.y)), 
            p.z
        );
  });
//app.sequencer().add<SoundObject>(0.5, 1).set( 0, 0.5, 0.5, "8.wav", [&](double t, const Vec3f& p) -> Vec3f { return p * g; });
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
