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
#include "al/graphics/al_Mesh.hpp"
#include "al/graphics/al_Graphics.hpp"
#include "al_ext/assets3d/al_Asset.hpp"
#include <iostream>
#include <string>

// MY CUSTOM INCLUDES:
#include "softlight-sphere/eoys-mesh-fx/ripple.hpp"
#include "softlight-sphere/eoys-mesh-fx/scatter.hpp"
#include "softlight-sphere/eoys-mesh-fx/vfxUtility.hpp"
#include "softlight-sphere/attractors.hpp"

// Shader Program
al::ShaderProgram glowShader;

class MyApp : public al::App {
public:

    al::Nav head;
    al::Vec3f target;
    float t{0};
    float a{0};
    al::VAOMesh ribbon{al::Mesh::TRIANGLES};
    // Meshes and Effects
    
    al::Parameter width{"Width", 0.1, 0, 0.2};
    al::VAOMesh mainMesh;
    Attractor mainAttractor;
    float updatedSpeed = 2.0;

     al::Light light;
  //Light light;
    al::Material material;


    // Global Time
    double globalTime = 0;
    double sceneTime = 0;
    float pointSize = 5.0f; // Particle size

    void onInit() override {
        gam::sampleRate(audioIO().framesPerSecond());
    }

    void onCreate() override {
        nav().pos(al::Vec3d(0, 0, 0)); // Move the camera back for view

        // Initialize Mesh
        mainAttractor.makeNoiseCube(mainMesh, 20.0, 2);
        mainMesh.primitive(al::Mesh::POINTS);

        head.moveF(1.0);
        head.spinF(0.3);
        //head
        //target.set(al::rnd::uniformS(), al::rnd::uniformS(), al::rnd::uniformS());
   
        ribbon.vertex(width, 0, 0);
        ribbon.normal(width, 0, 0);
        ribbon.vertex(0, 0, 0);
        ribbon.normal(0, 1, 0);
        ribbon.vertex(-width, 0, 0);
        ribbon.normal(-width, 0, 0);

        mainMesh.update();
        
    }

    void onAnimate(double dt) override {
        globalTime += dt;
        sceneTime += dt;

        // Apply Attractor Effect
        mainAttractor.processLorenz(mainMesh, dt, 0.1);
        target = mainMesh.vertices()[1];
        //head.faceToward(target);

        //karl code: 
        //t += dt;
        // std::cout << dt << sceneTime << std::endl;
        // head.step(dt);
        // a += 0.1;

    //      if (t > 11) {
    //   t = 0;
    //   target.set(al::rnd::uniformS(), al::rnd::uniformS(), al::rnd::uniformS());
    //   target *= 2.7;
    // }

    head.faceToward(target, 1.0);

    // if (ribbon.vertices().size() == 1000) {
    //   ribbon.vertices().back();
    // }

    ribbon.vertex(head.pos() + head.ur() * width);
    ribbon.vertex(head.pos());
    ribbon.vertex(head.pos() - head.ur() * width);
    ribbon.color(1.0,1.0,1.0,0.5);
    ribbon.color(1.0,1.0,1.0,0.1);
    ribbon.color(1.0,1.0,1.0,0.5);
    

    if (true) {
      // sort of curved surface shading
      ribbon.normal(al::Vec3f(head.uu()).lerp(head.ur(), 0.1));
      ribbon.normal(head.uu());
      ribbon.normal(al::Vec3f(head.uu()).lerp(head.ur(), -0.1));
    } else {
      // flat shading....
      ribbon.normal(head.uu());
      ribbon.normal(head.uu());
      ribbon.normal(head.uu());
    }

    const int n = ribbon.vertices().size();
    if (n > 3) {
      ribbon.index(n - 1);
      ribbon.index(n - 2);
      ribbon.index(n - 4);
      ribbon.index(n - 2);
      ribbon.index(n - 5);
      ribbon.index(n - 4);
      ribbon.index(n - 2);
      ribbon.index(n - 3);
      ribbon.index(n - 5);
      ribbon.index(n - 3);
      ribbon.index(n - 6);
      ribbon.index(n - 5);
    }
       
        head.step(dt * updatedSpeed);
        ribbon.update();
        
        mainMesh.update();
    }

    void onDraw(al::Graphics& g) override {
         glEnable(GL_BLEND);
      g.blendTrans();
      g.depthTesting(true); 
        g.clear(0.0, 0.0, 0.5, 1.0);
       // g.depthTesting(true);
        g.blending(true);
        g.blendAdd(); // Additive blending for glowing effect
        g.lighting(true);
    // lighting from karl's example
        light.globalAmbient(al::RGB(0.5, (1.0), 1.0));
        light.ambient(al::RGB(0.5, (1.0), 1.0));
        light.diffuse(al::RGB(1, 1, 0.5));
        g.light(light);
        material.specular(light.diffuse() * 0.3);
        material.shininess(50);
        g.material(material);

        // Use Custom Glow Shader
        // glowShader.begin();
        // glowShader.uniform("u_time", (float)globalTime);
        // glowShader.uniform("u_resolution", al::Vec2f(width(), height()));
        g.pointSize(2.5);
        g.meshColor();
        //g.color (1.0, 1.0, 1.0, 0.5);
        g.pointSize(10.0);
        g.draw(mainMesh);
        g.draw(ribbon);
        //glowShader.end();
    }

    void onSound(al::AudioIOData& io) override { mSequencer.render(io); }
    al::SynthSequencer& sequencer() { return mSequencer; }
    al::SynthSequencer mSequencer;
};

int main() {
    MyApp app;
    app.start();
    return 0;
}
