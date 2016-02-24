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
    theOnlyTexture.load("img.png");
    
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
    
    
    syphons = new ofxSyphonServer*[dir.size()];
    renders = new ofFbo*[dir.size()];
    shaders = new ofxAutoReloadedShader*[dir.size()];
    
    for(int j = 0 ; j<dir.size();j++){
        
        ofBuffer buffer = ofBufferFromFile(dir[j].path());
        
        string megaString = buffer.getText();
        
        vector<string> subString = ofSplitString(megaString, "//uniforms");
        //cout<<subString[1]<<endl;
        vector<string> u_strings = ofSplitString(subString[1], "]"); //vector of each float

        
        vector<string>ufloats;
        uniformNamesFloat.push_back(ufloats);
        uniformNamesInt.push_back(ufloats);
        uniformNamesBool.push_back(ufloats);
        
        vector<ofVec2f> f_vecArray;
        intSliderVals.push_back(f_vecArray);
        floatSliderVals.push_back(f_vecArray);
        tempoDivision.push_back(1);
        
        bool _b = false;
        textures.push_back(_b);
        
        for(int i = 0 ; i<u_strings.size();i++){
            if(ofIsStringInString(u_strings[i],"//tempodivision ")){
                vector<string>f_strings = ofSplitString(u_strings[i], " ");
                tempoDivision[j]=ofToFloat(f_strings[1]);
                cout<<tempoDivision[j]<<endl;
            }
            
            if(ofIsStringInString(u_strings[i]," sampler2D ")){
                vector<string>f_strings = ofSplitString(u_strings[i], " ");
                textures[j]=true;
                
            }
            

            if(ofIsStringInString(u_strings[i]," float ")){
                vector<string>f_strings = ofSplitString(u_strings[i], ";//");
                vector<string>f_name =ofSplitString(f_strings[0]," ");
                
                uniformNamesFloat[j].push_back(f_name[2]);
                
                vector<string>f_sliderVals = ofSplitString(f_strings[1], ",");
                //vector<ofVec2f> f_vecArray;
                
                ofVec2f f_vec;
                f_vec.set(ofToFloat(f_sliderVals[0]), ofToFloat(f_sliderVals[1]));
                //f_vecArray.push_back(f_vec);
                
                floatSliderVals[j].push_back(f_vec);
            }
            if(ofIsStringInString(u_strings[i]," int ")){
                vector<string>i_strings = ofSplitString(u_strings[i], ";//");
                vector<string>i_name =ofSplitString(i_strings[0]," ");
                
                uniformNamesInt[j].push_back(i_name[2]);
                
                vector<string>i_sliderVals = ofSplitString(i_strings[1], ",");
               //vector<ofVec2f> i_vecArray;
                ofVec2f i_vec;
                i_vec.set(ofToInt(i_sliderVals[0]), ofToFloat(i_sliderVals[1]));
               // i_vecArray.push_back(i_vec);
                intSliderVals[j].push_back(i_vec);            }
            
            if(ofIsStringInString(u_strings[i]," bool ")){
                vector<string>i_strings = ofSplitString(u_strings[i], ";//");
                vector<string>i_name =ofSplitString(i_strings[0]," ");
                uniformNamesBool[j].push_back(i_name[2]);
            }
        }
    }
    
    
    
    ofParameterGroup theGroup;
    
    ofParameterGroup enableGroup;
    enableGroup.setName("enable");
    for(int j = 0; j<dir.size();j++){
        ofParameter<bool> b;
        vector<string>temp = ofSplitString(dir.getPath(j), ".");
        vector<string>name =ofSplitString(temp[0], "/");
        b.set(name[1],true);
        enableBools.push_back(b);
        enableGroup.add(enableBools[j]);
        names.push_back(name[1]);
        
        
        //load shaders
        shaders[j] = new ofxAutoReloadedShader;
        vector<string>tempO = ofSplitString(dir.getPath(j), ".");
        shaders[j]->load(tempO[0]);
       
        
    }
    
    
    theGroup.add(enableGroup);

  
    
    screenSize.setName("Screen Size");
    screenSize.add(render_width.set("render_width",readWidth,10,3000));
    screenSize.add(render_height.set("render_height",readWidth,10,3000));
    
    render_width = readWidth;
    render_height = readHeight;
    
    theGroup.add(screenSize);
    //set up gui and parameters
    for(int j = 0; j<dir.size();j++){
        
        vector<ofParameter<float>>paraF;
        vector<ofParameter<int>>paraI;
        vector<ofParameter<bool>>paraB;
        myFloatVec.push_back(paraF);
        myIntVec.push_back(paraI);
        myBoolVec.push_back(paraB);
        
        
        ofParameterGroup paraGroup;
        paraGroup.setName(names[j]); //get shader name from jsonObject[j]
        
        for(int i = 0; i<uniformNamesFloat[j].size();i++){ //get num float uniforms from json
            ofParameter<float>para;
            para.set(uniformNamesFloat[j][i], floatSliderVals[j][i].x, floatSliderVals[j][i].x, floatSliderVals[j][i].y); //get setupvalues: name, from-too.
            floatParams.push_back(para);
            //allFloatParams.push_back(para);
        }
        myFloatVec[j]=floatParams;
        
        if(uniformNamesInt[j].size()>0){
        for(int i = 0; i<uniformNamesInt[j].size();i++){
            ofParameter<int>para;
            para.set(uniformNamesInt[j][i],intSliderVals[j][i].x, intSliderVals[j][i].x, intSliderVals[j][i].y);
            intParams.push_back(para);
        }
            
        myIntVec[j]=intParams;
        }
        for(int i = 0; i<uniformNamesBool[j].size();i++){
            ofParameter<bool>para;
            para.set(uniformNamesBool[j][i], true);
            boolParams.push_back(para);
        }
        myBoolVec[j]=boolParams;
        
        for(int i = 0; i<floatParams.size();i++){
            paraGroup.add(floatParams[i]);
        }
        for(int i = 0; i<intParams.size();i++){
            paraGroup.add(intParams[i]);
        }
        for(int i = 0; i<boolParams.size();i++){
            paraGroup.add(boolParams[i]);
        }
        floatParams.clear();
        boolParams.clear();
        intParams.clear();
        
        
        theGroup.add(paraGroup);
        
        cout<<myFloatVec.size()<<endl;
        
        
        renders[j] = new ofFbo;
        renders[j]->allocate(render_width, render_height);
        syphons[j] = new ofxSyphonServer;
        syphons[j]->setName(names[j]);
        
        
        time.push_back(0);
        bool _b = enableBools[j];
        minMaxGui.push_back(_b);
       
        
    }
    
 
    gui.setup(theGroup);
    
    gui.minimizeAll();
    ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>( gui.getControl("enable"));
    if(group){
        group->maximize();
    }
    
   // gui.loadFromFile("setting.xml");
   // ofAbstractParameter * param;
    
    
    // ofVec2f v = param->cast<ofVec2f>();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0 ; i<time.size(); i++) {
        time[i] += myFloatVec[i][0]/tempoDivision[i];
        
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    for(int i = 0; i<enableBools.size();i++){
        
        if(enableBools[i] && minMaxGui[i]){
            
            ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>( gui.getControl(names[i]));
            if(group){
                group->maximize();
            }
            minMaxGui[i]=false;
        }
        if(!enableBools[i] && !minMaxGui[i]){
            ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>( gui.getControl(names[i]));
            if(group){
                group->minimize();
            }
            minMaxGui[i]=true;
        }
    }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    //cout <<myFloatVec[0].size()<<endl;
    


    gui.draw();
    ofPushMatrix();
    for(int i = 0; i<enableBools.size();i++){
        
        if(enableBools[i]){
            
            renders[i]->begin();
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            ofSetColor(0);
            ofFill();
            ofDrawRectangle(0, 0, renders[i]->getWidth(), renders[i]->getHeight());
            // }
            
            
            
            shaders[i]->begin();
            
            shaders[i]->setUniform2f("u_resolution", render_width,render_height);
            shaders[i]->setUniform1f(uniformNamesFloat[i][0], time[i]); //tempo always nr 1
           
            for(int j = 1 ; j< uniformNamesFloat[i].size();j++){
                shaders[i]->setUniform1f(uniformNamesFloat[i][j],myFloatVec[i][j]);
            }
            //cout<<uniformNamesInt[i].size()<<endl;
            for(int j = 0 ; j< uniformNamesInt[i].size();j++){
                shaders[i]->setUniform1f(uniformNamesInt[i][j],myIntVec[i][j]);
            }
            for(int j = 0 ; j< uniformNamesBool[i].size();j++){
                shaders[i]->setUniform1i(uniformNamesBool[i][j],myBoolVec[i][j]);
            }
            
            if(textures[i]){
                theOnlyTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
                shaders[i]->setUniformTexture("tex0",theOnlyTexture,i);
            }
            
            ofSetColor(255,255,255);
            ofFill();
            ofDrawRectangle(0, 0, renders[i]->getWidth(), renders[i]->getHeight());
            
            
            shaders[i]->end();
            
            renders[i]->end();
            
            
            ofFill();
            ofSetColor(255);
            
            renders[i]->draw(250, 0, render_width/5, render_height/5);
            ofNoFill();
            ofSetColor(255,0,0);
            ofSetLineWidth(1);
            ofDrawRectangle(250, 0, render_width/5, render_height/5);
            
            ofTranslate(0, render_height/5+20);
        }
    }
    ofPopMatrix();
    
    for(int i = 0;i<amountOfShaders;i++){
        ofFill(); // 10.9 fix
        syphons[i]->publishTexture(&renders[i]->getTexture());
    }
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
