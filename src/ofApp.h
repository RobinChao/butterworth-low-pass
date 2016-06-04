#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include "Filter.h"
class ofApp : public ofBaseApp{

	public:
        ~ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioRequested (float * input, int bufferSize, int nChannels); /* output method */
        void audioReceived (float * input, int bufferSize, int nChannels); /* input method */
        
        int		initialBufferSize; /* buffer size */
        int		sampleRate;
    

        
    int numberOfOrder2;
    Filter* theFilter[400];
    double** buffers;
    double outputs[2];
    ofxMaxiOsc osc;

    maxiFFT fft;

    ofxFloatSlider cuttoff;
    ofxFloatSlider atFreq;
    ofxFloatSlider at;
    ofxButton button;
    ofxPanel gui;
    
    void changeFilter();
    
    bool play=true;
};
