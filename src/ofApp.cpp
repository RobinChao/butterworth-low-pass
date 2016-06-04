/* Implementation of a cascade of Butterworth filters. The maximum order achievable is 120, beyond this value the filters become unstable. It has a GUI to control the filter parameters and a visualisation of the filter response (the input signal is white noise). Careful when using speakers!
*/


#include "ofApp.h"
#include<vector>

//-------------------------------------------------------------
ofApp::~ofApp() {
    
}


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    sampleRate 			= 44100; /* Sampling Rate */
    initialBufferSize	= 1024;	/* Buffer Size. you have to fill this buffer with sound*/

    fft.setup(initialBufferSize, 1024, 256);
    
    // Filter setup
    double a=30;
    double fa=1400;
    double fc=1320;
    string type="lowPass";
    // calculate which order is needed
    theFilter[0]=new Filter(a,fa,fc,type);
    // calculate how many filters of order 2 are needed
    numberOfOrder2=floor(theFilter[0]->order/2.0);
    // at least we need 1 filter
    if(numberOfOrder2==0){
        numberOfOrder2++;
    }
    // if numberOfOrder2 is not even, then add 1
    if(numberOfOrder2%2!=0){
        numberOfOrder2++;
    }
    // if the order is higher than 120 the filter becomes unstable
    if(theFilter[0]->order>120){
        numberOfOrder2=120/2;
        printf("%s","Max order reached (order=120)");
    }
    // make numberOfOrder2 filters
    for(int i=0; i<numberOfOrder2;i++){
        theFilter[i]=new Filter(a,fa,fc,2,type);
    }
    
    // Initialise the buffers. Each filter needs a buffer. In theory we can cascade order 3 filters so each filter needs to know about the last 4 samples. We put all the buffers into a matrix.
    buffers=new double*[200];
    for (int i = 0; i < 200; ++i){
        buffers[i] = new double[4];
    }
    
    // GUI
    gui.setup();
    gui.setSize(400,100);
    // Change filter button
    button.addListener(this,&ofApp::changeFilter);
    // Cuttoff slider
    gui.add(cuttoff.setup("cuttoff",1320,0,22050));
    cuttoff.setSize(400, 20);
    // Attenuation frequency slider
    gui.add(atFreq.setup("attenuation frequency",1400,0,22050));
    atFreq.setSize(400, 20);
    // Attenuation level slider
    gui.add(at.setup("attenuation",30,0,60));
    at.setSize(400,20);
    gui.add(button.setup("Change filter"));
    
    
    ofSoundStreamSetup(2,2,this, sampleRate, initialBufferSize, 4);/* Call this last ! */
    
}

// Change filter parameters
void ofApp::changeFilter(){
    // stop playing
    play=false;
    // new parameters extracted from the GUI
    double a=at;
    double fa=atFreq;
    double fc=cuttoff;
    string type="lowPass";
    // calculate which order is needed
    theFilter[0]=new Filter(a,fa,fc,type);
    // calculate how many filters of order 2 are needed
    numberOfOrder2=floor(theFilter[0]->order/2.0);
    // if the order is higher than 120 the filter becomes unstable
    if(theFilter[0]->order>120){
        numberOfOrder2=120/2;
        printf("%s","Max order reached (order=120)");
    }
    // we need at neast 1 filter
    if(numberOfOrder2==0){
        numberOfOrder2++;
    }
    // if numberOfOrder2 is not even, then add 1
    if(numberOfOrder2%2!=0){
        numberOfOrder2++;
    }
    
    for(int i=0; i<numberOfOrder2;i++){
        theFilter[i]=new Filter(a,fa,fc,2,type);
    }
    // set play to true
    play=true;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw the fft
    for(int i=0; i<initialBufferSize/4;i++){
        ofRect(i*ofGetWidth()/(initialBufferSize/4),ofGetHeight()/2,2,-fft.magnitudes[i]*3);
    }
    // draw the gui
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::audioRequested 	(float * output, int bufferSize, int nChannels){
    
    for (int i = 0; i < bufferSize; i++){
        if(play){
            // generate noise
            buffers[0][0]=osc.noise();
            // fft
            if (fft.process(output[i])) {
                fft.magsToDB(); // calculate all the DBs
            }
            // do the filtering operation
            for(int i=0; i<numberOfOrder2;i++){
                theFilter[i]->doTheFiltering(theFilter[i],buffers[i], buffers[i+1], buffers[i+1]);
            }
            // reassign the buffer values to previous sample values
            for(int i=0; i<numberOfOrder2+1; i++){
                for(int j=4; j>0; j--){
                    buffers[i][j]=buffers[i][j-1];
                }
            }
            // audio out
            output[i*nChannels] = buffers[numberOfOrder2][0];
            output[i*nChannels + 1] = buffers[numberOfOrder2][0];
        }
    }
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    
    // This does not work.
    
    for (int i = 0; i < bufferSize; i++){
        // Too slow for input processing?
        //buffers[0][0]=input[i*nChannels];
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
void ofApp::windowResized(int w, int h){
    
}



//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
