#pragma once

#include "ofMain.h"
#include "ofUtils.h"
#include "ofxGui.h"
#include "ofxJSONElement.h"
#include "ofxAutoReloadedShader.h"
#include "ofxSyphon.h"
#include "ofxXmlSettings.h"
#include "ofxFFTLive.h"

class ofApp : public ofBaseApp{
    struct UniformVals {
        float min;
        float max;
        float start;
        ofVec3f vecMin;
        ofVec3f vecMax;
        ofVec3f vecStart;
        string type;
        string name;
        int index;
    };
    
    struct ShaderUI{
        string name;
        vector <UniformVals> uniforms;
        ofParameterGroup pGroup;
        vector<ofParameter<float>> myFloatSliders;
        vector<ofParameter<int>> myIntSliders;
        vector<ofParameter<bool>> myBoolSliders;
        ofxAutoReloadedShader shader;
        ofFbo fbo;
        ofxSyphonServer syphonServer;
        float counter;
        float tempoDivision;
        ofParameter<bool>* enabled;
    };
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        vector<float> buffer;
        //ofDirectory dir;
//        vector<ofAbstractParameter>abstractParams;
    
        ofxPanel gui;
        ofxJSONElement json;
    
//    ofxFFTLive fft;
    ofTexture mTexture;
    
    ofParameterGroup enableGroup;
    vector<ofParameter<bool>> shaderEnables;
    
    ofParameterGroup screenSize;
    ofParameter<int> render_width;
    ofParameter<int> render_height;
    
//    vector<ofParameter<float>>floatParams;
//    vector<ofParameter<int>>intParams;
//    vector<ofParameter<bool>>boolParams;
    

    
    ofParameterGroup* guiParamGroup;
    
//    vector<ofParameter<float>>allFloatParams;
		

    vector<ofParameter<bool>>enableBools;
    
//    vector<bool>textures;
//    vector<bool>sounds;
    
    ofImage theOnlyTexture;
    
//    vector< vector<ofParameter<float>> > myFloatVec;
//    vector< vector<ofParameter<int>> > myIntVec;
//    vector< vector<ofParameter<bool>> > myBoolVec;
//    
//    vector< vector<string> > uniformNamesFloat;
//    vector< vector<string> > uniformNamesInt;
//    vector< vector<string> > uniformNamesBool;

    vector<ShaderUI> shaderUIs;
    
//    vector<vector<SliderVals_float> > floatSliderVals;
//    vector<vector<SliderVals_int> > intSliderVals;
//    vector<vector<ofVec2f> > vec2SliderVals;
    
    vector<ofxSyphonServer> syphonServers;
    
    vector<bool>minMaxGui;
    
    ofxXmlSettings xml;

    int amountOfShaders;
};
