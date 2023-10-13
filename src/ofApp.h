#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
    
	public:
		
	void setup();
	void update();
	void draw();
    void keyPressed(int key);
	
	ofShader shader;
	ofPlanePrimitive plane;
	ofFloatImage img;
    
     vector< shared_ptr<ofLight> > lights;
    ofEasyCam camera;
    ofMaterial material;
    ofMaterial boxesMaterial;
    
    ofVideoGrabber vidGrabber;
    int camWidth;
    int camHeight;

    void renderScene();
    
    ofParameterGroup planeParams;
    ofParameter<bool> useMaterial;
    ofParameter<bool> useCamera;
    ofParameter<float>  noiseAmplitude;
    ofParameter<float>  noiseScale;
    ofParameter<float>  noiseSpeed;
    
    ofParameterGroup materialParams;
    ofParameter<float> displacementStrength;
    ofParameter<float> displacementNormalsStrength;
    ofParameter<float> normalGeomToNormalMapMix;
    
    ofParameter<ofFloatColor> diffuseColor;
    ofParameter<ofFloatColor> specularColor;
    ofParameter<ofFloatColor> ambientColor;
    ofParameter<ofFloatColor> emissiveColor;
    
    ofParameter<float> shininess;
    ofParameter<float> metallic;
    ofParameter<float> reflectance;
    ofParameter<float> roughness;
    
    ofParameter<float> clearCoatRoughness;
    ofParameter<float> clearCoatStrength;
    ofParameter<bool> clearCoat;
    
    ofParameterGroup lightParams;
    ofParameter<float> x;
    ofParameter<float> y;
    ofParameter<float> z;
    
    ofParameterGroup shadowParams;
    ofParameter<float> shadowBias;
    ofParameter<float> shadowNormalBias;
    
    
    ofParameterGroup parameters;
    ofxPanel gui;
    ofXml settings;
};
