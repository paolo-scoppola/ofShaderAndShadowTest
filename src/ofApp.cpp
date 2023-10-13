#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofxGuiSetDefaultWidth(400);
    
    parameters.setName("settings");
    
    planeParams.setName("Plane");
    planeParams.add(useMaterial.set("Use Material",true));
    planeParams.add(wireframe.set("Wireframe",true));
    planeParams.add(noiseAmplitude.set("Displacement",0.0f, 0.0f, 1.0f));
    planeParams.add(noiseScale.set("Noise Scale",0.0f, 0.0f, 0.1f));
    planeParams.add(noiseSpeed.set("Noise Speed",0.0f, 0.0f, 1.0f));
    
    parameters.add(planeParams);
    
    materialParams.setName("Material");
    materialParams.add(displacementStrength.set("Displacement Strength", 0.0f, 0.0f, 1.0f));
    materialParams.add(displacementNormalsStrength.set("Displacement Normals Strength", 0.0, 0.0, 10.0));
    materialParams.add(normalGeomToNormalMapMix.set("Normal Geom To Normal Map Mix", 0.0, 0.0, 1.0));
    
    materialParams.add(shininess.set("Shininess", 0.0, 0.0, 1.0));
    materialParams.add(roughness.set("Roughness", 0.0, 0.0, 1.0));
    materialParams.add(metallic.set("Metallic", 0.0, 0.0, 1.0));
    materialParams.add(reflectance.set("Reflectance", 0.0, 0.0, 1.0));
   
    materialParams.add(ambientColor.set("Ambient Color", ofColor::white));
    materialParams.add(specularColor.set("Specular Color",ofColor::white));
    materialParams.add(diffuseColor.set("Diffuse Color", ofColor::white));
    materialParams.add(emissiveColor.set("Emissive Color",ofColor::white));
    
    materialParams.add(clearCoat.set("Clear Coat", false));
    materialParams.add(clearCoatRoughness.set("Clear Coat Roughness", 0.0001, 0.0001, 10.0));
    materialParams.add(clearCoatStrength.set("Clear Coat Strength", 0.0001, 0.0001, 10.0));
    
    parameters.add(materialParams);
    
    lightParams.setName("Lights");
    lightParams.add(x.set("X", 0, -1000, 1000));
    lightParams.add(y.set("Y", 0.0, -1000, 1000));
    lightParams.add(z.set("Z", 0.0, -1000, 1000));
    parameters.add(lightParams);
    
    shadowParams.setName("Shadow");
    shadowParams.add(shadowBias.set("Bias", 0.07, 0.0, 1.0));
    shadowParams.add(shadowNormalBias.set("Normal Bias", -4.0, -10.0, 10.0));
    parameters.add(shadowParams);
   
    
    gui.setup(parameters);
    gui.loadFromFile("settings.xml");
    
    ofDisableArbTex();
    shader.load("shadersGL3/shader");
    img.allocate(800, 600, OF_IMAGE_GRAYSCALE);
    plane.set(800, 600, 80, 60);
    plane.mapTexCoordsFromTexture(img.getTexture());
    
    for( int i = 0; i < 2; i++ ) {
        auto light = make_shared<ofLight>();
        light->enable();
        if( i == 0 ) {
            light->setPointLight();
        } else {
            light->setSpotlight(60, 20);
            light->getShadow().setNearClip(200);
            light->getShadow().setFarClip(2000);
            light->setPosition( 210, 330.0, 750 );
            light->setAmbientColor(ofFloatColor(0.4));
        }

        light->getShadow().setStrength(0.6f);
        
        if( light->getType() != OF_LIGHT_POINT ){
            glm::quat xq = glm::angleAxis(glm::radians(-30.0f), glm::vec3(1,0,0));
            glm::quat yq = glm::angleAxis(glm::radians(20.0f), glm::vec3(0,1,0));
            light->setOrientation(yq*xq);
        }
        
        lights.push_back(light);
    }
    
    ofShadow::enableAllShadows();
    ofShadow::setAllShadowTypes(OF_SHADOW_TYPE_PCF_LOW);
    
    boxesMaterial.setDiffuseColor( ofFloatColor(0.85) );
    boxesMaterial.setShininess(60);
    boxesMaterial.setSpecularColor(ofFloatColor(1));

}

//--------------------------------------------------------------

void ofApp::update(){
    
  
    float n = noiseSpeed * ofGetElapsedTimef();
    
    ofPixels & pixels = img.getPixels();
    int w = img.getWidth();
    int h = img.getHeight();
    for(int y=0; y<h; y++) {
        for(int x=0; x<w; x++) {
            int i = y * w + x;
            pixels[i] = noiseAmplitude * ofNoise(x * noiseScale, y * noiseScale, n);
        }
    }
    
    img.update();
    
    material.setDisplacementTexture(img.getTexture());
    material.setDisplacementStrength(displacementStrength);
    material.setDisplacementNormalsStrength(displacementNormalsStrength);
    
    material.setDiffuseColor(diffuseColor);
    material.setAmbientColor(ambientColor);
    material.setEmissiveColor(emissiveColor);
    material.setSpecularColor(specularColor);
   
    material.setNormalGeomToNormalMapMix(normalGeomToNormalMapMix);
    
    material.setRoughness(roughness);
    material.setMetallic(metallic);
    material.setReflectance(reflectance);
    material.setShininess(shininess);
    
  //  material.setTexCoordScale(scaleX, scaleY);
    material.setClearCoatEnabled(clearCoat);
    material.setClearCoatRoughness(clearCoatRoughness);
    material.setClearCoatStrength(clearCoatStrength);
    
    if( lights.size() > 0 ){
        if( lights[0]->getType() == OF_LIGHT_POINT ) {
            float tangle = ofGetElapsedTimef() * 1.05;
            lights[0]->setPosition(x, y, z);
            lights[0]->setAmbientColor(ofColor::red);
        }
    }
    
    ofShadow::setAllShadowBias(shadowBias);
    ofShadow::setAllShadowNormalBias(shadowNormalBias);
}

//--------------------------------------------------------------
void ofApp::renderScene() {
    ofRotateDeg(-45, 1, 0, 0);
    if (useMaterial) {
        
        material.begin();
         
    } else {
        
        shader.begin();
        shader.setUniformTexture("displacement", img.getTexture(), 1);
    }
    
    if (wireframe)
        plane.drawWireframe();
    plane.draw();
    
    if (useMaterial) {
        
        material.end();
        
    } else {
        
        shader.end();
    }
    
     // add another material so the displacement is not applied to the boxes
    boxesMaterial.begin();
    ofDrawBox(0, 0, 0, 100);
    ofDrawBox(120, 80, 0, 100);
    ofDrawBox(-120, 80, 0, 100);
    boxesMaterial.end();
}

//--------------------------------------------------------------

void ofApp::draw(){
    
    ofSetColor(ofColor::white);
    ofEnableDepthTest();
    
    for( int i = 0; i < lights.size(); i++ ) {
        auto& light = lights[i];
        
        if( light->shouldRenderShadowDepthPass() ) {
            
            int numShadowPasses = light->getNumShadowDepthPasses();
            for( int j = 0; j < numShadowPasses; j++ ) {
                light->beginShadowDepthPass(j);
                renderScene();
                light->endShadowDepthPass(j);
            }
        }
    }
    
    camera.begin();
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    renderScene();
    glDisable(GL_CULL_FACE);
    
    for( int i = 0; i < lights.size(); i++ ){
        auto& light = lights[i];
        
        ofSetColor(light->getDiffuseColor());
        if(light->getType() == OF_LIGHT_POINT ) {
            
            ofDrawSphere( light->getPosition(), 12 );
            
        } else {
            light->draw();
        }
    }
    
    camera.end();
    ofDisableDepthTest();
    
    gui.draw();
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    

}
