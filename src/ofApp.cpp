#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    string path = "shaders";
    ofDirectory dir(path);
    //only show frag files
    dir.allowExt("frag");
    //populate the directory object
    dir.listDir();
    amountOfShaders = dir.size();
    ofDisableArbTex();
    theOnlyTexture.load("img.png");
    ofEnableArbTex();
    
    int readWidth = 1000;
    int readHeight = 1000;
    xml.load("settings.xml");
    xml.pushTag("group");
    xml.pushTag("Screen_Size");
    if(xml.tagExists("render_width")){
        
        readWidth = xml.getValue("render_width", 0);
        //cout<<render_width<<endl;
        
    }
    if(xml.tagExists("render_height")){
        readHeight= xml.getValue("render_height", 0);
    }
    
    xml.popTag();
    xml.popTag();
    
    
    
    enableGroup.setName("enable");
    
    screenSize.setName("Screen Size");
    screenSize.add(render_width.set("render_width",readWidth,10,3000));
    screenSize.add(render_height.set("render_height",readWidth,10,3000));
    
    render_width = readWidth;
    render_height = readHeight;
    
    for(int j = 0 ; j<dir.size();j++){
        
        
        vector<string>temp = ofSplitString(dir.getPath(j), ".");
        vector<string>name =ofSplitString(temp[0], "/");
        
        //load shaders
        ofxAutoReloadedShader shader;
        vector<string>tempO = ofSplitString(dir.getPath(j), ".");
        
        ShaderUI shaderUI;
        shaderUI.name = name[1];
        shaderUI.shader.load(tempO[0]);
        
        ofBuffer buffer = ofBufferFromFile(dir[j].path());
        
        string megaString = buffer.getText();
        
        vector<string> subString = ofSplitString(megaString, "//uniforms");
        //cout<<subString[1]<<endl;
        vector<string> u_strings = ofSplitString(subString[1], "]"); //vector of each uniform
        
        
        // Read the uniforms
        
        for(int i = 0 ; i<u_strings.size();i++){
            
            if(ofIsStringInString(u_strings[i],"\n\n")){
                // cought emty lines
                
            }else if(ofIsStringInString(u_strings[i],"//tempodivision ")){
                vector<string>f_strings = ofSplitString(u_strings[i], " ");
                shaderUI.tempoDivision=ofToFloat(f_strings[1]);
                
            }else if(ofIsStringInString(u_strings[i]," tex0 ")){
                vector<string>f_strings = ofSplitString(u_strings[i], " ");
                //                textures[j]=true;
                
            }else if(ofIsStringInString(u_strings[i]," sound")){
                vector<string>f_strings = ofSplitString(u_strings[i], " ");
                //                sounds[j]=true;
                
            }else{
                vector<string>f_strings = ofSplitString(u_strings[i], ";//");
                vector<string>f_name =ofSplitString(f_strings[0]," ");
                
                UniformVals uniform;
                uniform.name = f_name[2];
                
                if(ofIsStringInString(u_strings[i]," float ")) uniform.type = "float";
                if(ofIsStringInString(u_strings[i]," int ")) uniform.type = "int";
                if(ofIsStringInString(u_strings[i]," bool ")) uniform.type = "bool";
                if(ofIsStringInString(u_strings[i]," vec2 ")) uniform.type = "vec2";
                if(ofIsStringInString(u_strings[i]," vec3 ")) uniform.type = "vec3";
                
                if(uniform.type == "float" || uniform.type == "int" || uniform.type == "bool"){
                    
                    vector<string>f_uniformVals = ofSplitString(f_strings[1], ",");
                    
                    if(f_uniformVals.size()>2){
                        uniform.start = ofToFloat(f_uniformVals[0]);
                        uniform.min = ofToFloat(f_uniformVals[1]);
                        uniform.max = ofToFloat(f_uniformVals[2]);
                        
                    }else if(f_uniformVals.size()>1){
                        uniform.min = ofToFloat(f_uniformVals[0]);
                        uniform.max = ofToFloat(f_uniformVals[1]);
                        uniform.start = uniform.min;
                        
                        
                    }else if(f_uniformVals.size()>0){
                        uniform.min = ofToFloat(f_uniformVals[0]);
                        uniform.max = uniform.min;
                        uniform.start = uniform.min;
                    }else{
                        uniform.start = 1;
                        uniform.min = 1;
                        uniform.max = 1;
                    }
                    shaderUI.uniforms.push_back(uniform);
                }
                else if(uniform.type == "vec2" || uniform.type == "vec3"){
                    vector<string>f_uniformVecVals = ofSplitString(f_strings[1], ";");
                    
                    if(f_uniformVecVals.size()>2){ // 3 vecX
                        vector<string>f_uniformVecComp1 = ofSplitString(f_strings[0], ",");
                        vector<string>f_uniformVecComp2 = ofSplitString(f_strings[1], ",");
                        vector<string>f_uniformVecComp3 = ofSplitString(f_strings[2], ",");
                        if(f_uniformVecComp1.size()>2){ // vec3
                            uniform.vecStart = ofVec3f(ofToFloat(f_uniformVecComp1[0]), ofToFloat(f_uniformVecComp1[1]),ofToFloat(f_uniformVecComp1[2]) );
                            uniform.vecMin = ofVec3f(ofToFloat(f_uniformVecComp2[0]), ofToFloat(f_uniformVecComp2[1]),ofToFloat(f_uniformVecComp2[2]) );
                            uniform.vecMax = ofVec3f(ofToFloat(f_uniformVecComp3[0]), ofToFloat(f_uniformVecComp3[1]),ofToFloat(f_uniformVecComp3[2]) );
                            
                        } else if(f_uniformVecComp1.size()>1){ // vec2
                            uniform.vecStart = ofVec3f(ofToFloat(f_uniformVecComp1[0]), ofToFloat(f_uniformVecComp1[1]), 0. );
                            uniform.vecMin = ofVec3f(ofToFloat(f_uniformVecComp2[0]), ofToFloat(f_uniformVecComp2[1]),0. );
                            uniform.vecMax = ofVec3f(ofToFloat(f_uniformVecComp3[0]), ofToFloat(f_uniformVecComp3[1]),0. );
                        }
                        if(f_uniformVecVals.size()>1){ // 2 vecX
                            vector<string>f_uniformVecComp1 = ofSplitString(f_strings[0], ",");
                            vector<string>f_uniformVecComp2 = ofSplitString(f_strings[1], ",");
                            vector<string>f_uniformVecComp3 = ofSplitString(f_strings[2], ",");
                            if(f_uniformVecComp1.size()>2){ // vec3
                                uniform.vecMin = ofVec3f(ofToFloat(f_uniformVecComp2[0]), ofToFloat(f_uniformVecComp2[1]),ofToFloat(f_uniformVecComp2[2]) );
                                uniform.vecMax = ofVec3f(ofToFloat(f_uniformVecComp3[0]), ofToFloat(f_uniformVecComp3[1]),ofToFloat(f_uniformVecComp3[2]) );
                                
                            } else if(f_uniformVecComp1.size()>1){ // vec2
                                uniform.vecStart = ofVec3f(ofToFloat(f_uniformVecComp1[0]), ofToFloat(f_uniformVecComp1[1]), 0. );
                                uniform.vecMin = ofVec3f(ofToFloat(f_uniformVecComp2[0]), ofToFloat(f_uniformVecComp2[1]),0. );
                            }
                            uniform.vecStart = uniform.vecMin;
                        }
                    }
                    shaderUI.uniforms.push_back(uniform);
                }
            }
        }
        
        shaderUI.pGroup.setName(shaderUI.name);
        
        int f = 0;
        int i = 0;
        int b = 0;
        
        // create the GUI
        
        for(auto & uniform : shaderUI.uniforms){
            
            if(uniform.type == "float"){
                ofParameter<float> p;
                p.set(uniform.name, uniform.start, uniform.min, uniform.max);
                shaderUI.myFloatSliders.push_back(p);
                uniform.index = f;
                f++;
                shaderUI.pGroup.add(shaderUI.myFloatSliders.back());
                
            }else if(uniform.type == "int"){
                ofParameter<int> p;
                p.set(uniform.name, uniform.start, uniform.min, uniform.max);
                shaderUI.myIntSliders.push_back(p);
                uniform.index = i;
                i++;
                shaderUI.pGroup.add(shaderUI.myIntSliders.back());
                
            }else if(uniform.type == "bool"){
                ofParameter<bool> p;
                p.set(uniform.name, uniform.start, uniform.min, uniform.max);
                shaderUI.myBoolSliders.push_back(p);
                uniform.index = b;
                b++;
                shaderUI.pGroup.add(shaderUI.myBoolSliders.back());
            }
            
        }
        
        shaderUI.counter = 0;
        
        shaderUI.enabled.set(shaderUI.name, true);
        shaderUI.isMaximised = false;
        
        enableGroup.add( shaderUI.enabled );
        
        shaderUI.fbo.allocate(render_width, render_height);

        
        shaderUIs.push_back(shaderUI);
        
        //        shaderUIs.back().syphonServer.setName(shaderUI.name);
    }
    
    
    
    ofParameterGroup paramGroups;
    
    paramGroups.add(screenSize);
    paramGroups.add(enableGroup);
    
    for (auto & shaderUI : shaderUIs) {
        paramGroups.add(shaderUI.pGroup);
        
        shaderUI.syphonServer.setName(shaderUI.name);

    }
    
    gui.setup(paramGroups);
    
    gui.minimizeAll();
    ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>( gui.getControl("enable"));
    if(group){
        group->maximize();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto & shaderUI : shaderUIs) {
        shaderUI.counter += shaderUI.myFloatSliders[0]/shaderUI.tempoDivision;
        
        if(shaderUI.enabled){
            
            shaderUI.fbo.begin();
            {
                glClearColor(0.0, 0.0, 0.0, 0.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                shaderUI.shader.begin();
                {
                    //                    ofTexture t;
                    //                    // if(textures[i]){
                    //                    t =theOnlyTexture.getTexture();
                    //                    t.generateMipmap();
                    //                    t.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
                    //                    t.setTextureWrap(GL_REPEAT,GL_REPEAT);
                    //                    t.bind();
                    
                    //}
                    
                    for(auto & uniform : shaderUI.uniforms){
                        
                        string uniformName = uniform.name;
                        int index = uniform.index;
                        
                        if(uniformName == "u_resolution"){
                            shaderUI.shader.setUniform2f("u_resolution", shaderUI.fbo.getWidth(), shaderUI.fbo.getHeight());
                            
                        }else if(uniformName == "u_time"){
                            shaderUI.shader.setUniform1f("u_time", shaderUI.counter); //tempo always nr 1
                            
                        }else if(uniform.type == "float"){
                            shaderUI.shader.setUniform1f(uniformName, shaderUI.myFloatSliders[index]);
                        }else if(uniform.type == "int"){
                            shaderUI.shader.setUniform1i(uniformName, shaderUI.myIntSliders[index]);
                        }else if(uniform.type == "bool"){
                            shaderUI.shader.setUniform1i(uniformName, shaderUI.myBoolSliders[index]);
                        }
                    }
                    
                    // if(textures[i]){
                    //                    t.unbind();
                    // }
                    
                    //   shaders[i]->setUniformTexture("tex0",theOnlyTexture,i);
                    
                    //            if(sounds[i]){
                    //                mTexture.bind();
                    //                shaders[i]->setUniformTexture("sounds",mTexture,i);
                    //                shaders[i]->setUniform1f("soundInput",buffer[0]);
                    //                mTexture.unbind();
                    //            }
                    
                    ofSetColor(255,255,255);
                    ofFill();
                    ofDrawRectangle(0, 0, shaderUI.fbo.getWidth(), shaderUI.fbo.getHeight());
                }
                shaderUI.shader.end();
            }
            shaderUI.fbo.end();
            
            // publish Syphon
            ofFill(); // 10.9 fix
            shaderUI.syphonServer.publishTexture(&shaderUI.fbo.getTexture());
            
        }
        
        
        
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    // minimize/maximize the running shaderUIs
    for(auto & shaderUI : shaderUIs){
        
        if(shaderUI.enabled && !shaderUI.isMaximised){
    
                ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>( gui.getControl(shaderUI.name));
                if(group){
                    group->maximize();
                }
                shaderUI.isMaximised=true;
            }
        if(!shaderUI.enabled && shaderUI.isMaximised){
                ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>( gui.getControl(shaderUI.name));
                if(group){
                    group->minimize();
                }
                shaderUI.isMaximised=false;
            }
        }
    
    //    fft.update();
    //    buffer.clear();
    //    buffer = fft.getFftRawData();
    //   // cout <<buffer[0]<<endl;
    //    unsigned char signal[1024];
    //    for (int i = 0; i < 512; i++) {
    //        float scaleFFT = ofMap(i,0,512,0.0,1.0);
    //        scaleFFT *= 255;
    //        signal[i] = (unsigned char) scaleFFT;
    //    }
    //    for (int i = 0; i < buffer.size(); i++) {
    //        signal[i] = (unsigned char) i;
    //        signal[512+i] = (unsigned char) (buffer.at(i)*255);
    //    }
    //    mTexture.loadData(signal, 512, 2, GL_LUMINANCE);
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    
    
    gui.draw();
    
    // draw previews
    ofPushMatrix();
    for (auto & shaderUI : shaderUIs) {
        if(shaderUI.enabled){
            
            ofFill();
            ofSetColor(255);
            shaderUI.fbo.draw(250, 0, render_width/5, render_height/5);
            // draw frame
            ofNoFill();
            ofSetColor(255,0,0);
            ofSetLineWidth(1);
            ofDrawRectangle(250, 0, render_width/5, render_height/5);
            ofTranslate(0, render_height/5+20);
        }
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if(key == 'l') {
        gui.loadFromFile("settings.xml");
        
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
