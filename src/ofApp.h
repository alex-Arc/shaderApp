#pragma once

#include "ofMain.h"
#include "ofUtils.h"
#include "ofxGui.h"
#include "ofxJSON.h"
#include "ofxAutoReloadedShader.h"
#include "ofxSyphon.h"
#include "ofxXmlSettings.h"
#include "ofxFFTLive.h"

class ofApp : public ofBaseApp{

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
        vector<ofAbstractParameter>abstractParams;
        ofxPanel gui;
        ofxJSONElement json;
    
    ofxFFTLive fft;
    ofTexture mTexture;
    ofParameterGroup screenSize;
    ofParameter<int> render_width;
    ofParameter<int> render_height;
    
    vector<ofParameter<float>>floatParams;
    vector<ofParameter<int>>intParams;
    vector<ofParameter<bool>>boolParams;
    
    ofxAutoReloadedShader** shaders;
    ofFbo** renders;
    
    ofParameterGroup* guiParamGroup;
    
    vector<ofParameter<float>>allFloatParams;
		
    vector<float>time;
    vector<float>tempoDivision;
    vector<string>names;
    vector<ofParameter<bool>>enableBools;
    
    vector<bool>textures;
    vector<bool>sounds;
    
    ofImage theOnlyTexture;
    
    vector< vector<ofParameter<float>> > myFloatVec;
    vector< vector<ofParameter<int>> > myIntVec;
    vector< vector<ofParameter<bool>> > myBoolVec;
    
    vector< vector<string> > uniformNamesFloat;
    vector< vector<string> > uniformNamesInt;
    vector< vector<string> > uniformNamesBool;
    
    vector<vector<ofVec2f> > floatSliderVals;
    vector<vector<ofVec2f> > intSliderVals;
    vector<vector<ofVec2f> > vec2SliderVals;
    
    ofxSyphonServer** syphons;
    
    vector<bool>minMaxGui;
    
    ofxXmlSettings xml;

    int amountOfShaders;
};
