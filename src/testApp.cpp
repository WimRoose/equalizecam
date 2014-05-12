#include "testApp.h"
using namespace ofxCv;
using namespace cv;

#define _USE_LIVE_VIDEO

ofxCvGrayscaleImage hue,sat,bri,greyImg;
ofImage edge;
ofxCvBlob blob;
ofxCvContourFinder contours;
ofVideoGrabber vidGrabber;
int cannyTh_1=50;
int cannyTh_2=150;
int thrshld;
int w = 640;
int h = 480;
bool canny=false;
bool invt=false;
bool showContours=false;

//--------------------------------------------------------------
void testApp::setup(){

    gui.setup();
    gui.add(slider1.setup("Canny", 10, 0, w/3));
    gui.add(slider2.setup("Canny2", 10, 0, w/3));
    gui.add(slider3.setup("Threshold", 1, 0, 200));
    slider1.addListener(this, &testApp::slider1Changed);
    slider2.addListener(this, &testApp::slider2Changed);
    slider3.addListener(this, &testApp::slider3Changed);
    #ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(640,480);
	#else
        im.allocate(w,h,OF_IMAGE_COLOR);
        rgb.allocate(w,h);
        greyImg.allocate(w,h);
        bri.allocate(w,h);
        im.loadImage("/home/lou/Pictures/equalizer2.jpg");
        cout << "image loaded" << endl;
        rgb.setFromPixels(im.getPixelsRef());
        cout << "rgb set" << endl;
        greyImg=rgb;
        cout << "greyImg set" << endl;
        greyImg.blurGaussian(3);
	#endif
    // load in sounds:
	beat.loadSound("/home/lou/Music/easy.wav");
    beat.play();
	// the fft needs to be smoothed out, so we create an array of floats
	// for that purpose:
	fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}

	nBandsToGet = 128;
}

void testApp::drawBlob(ofxCvBlob& blob){

    ofFill();
    float x = 0;
    float y = 0;
    ofSetColor(0x0000FF);
    ofBeginShape();
        for (int i = 0; i < blob.nPts; i++){
            ofVertex(x + blob.pts[i].x, y + blob.pts[i].y);
        }
    ofEndShape(true);
}
//--------------------------------------------------------------

void testApp::applyCanny() {

    #ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
       bool bNewFrame = vidGrabber.isFrameNew();
	   if (bNewFrame)   {
            rgb.setFromPixels(vidGrabber.getPixels(), w,h);
	   }
	   greyImg=rgb;
    #endif

    Canny(greyImg,edge, cannyTh_1, cannyTh_2, 3);
    edge.update();
    bri.setFromPixels(edge.getPixelsRef());
    if (canny) {
        bri.absDiff(greyImg,bri);
        bri.threshold(thrshld);
    }
    if (invt) {
        bri.threshold(0,true);
    }


    contours.findContours(bri, 1000, w*h/2, 10, true,true);
    cout << contours.nBlobs << endl;
    if (contours.nBlobs > 0){
        blob = contours.blobs.at(0);
        cout << contours.nBlobs << endl;
    }
    //cout << contours.nBlobs << endl;

}

void testApp::slider1Changed(int & canny){
    cannyTh_1=canny;
    //applyCanny();
}
void testApp::slider2Changed(int & canny){
    cannyTh_2=canny;
    //applyCanny();
}
void testApp::slider3Changed(int & th){
    thrshld=th;
    //applyCanny();
}

void testApp::update(){
    applyCanny();
	ofBackground(80,80,20);

	// update the sound playing system:
	ofSoundUpdate();


	// (5) grab the fft, and put in into a "smoothed" array,
	//		by taking maximums, as peaks and then smoothing downward
	float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
   bass = fftSmoothed[0] *= 0.96f;
	for (int i = 0;i < nBandsToGet; i++){

		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.96f;
        //cout << fftSmoothed[0] << endl;
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];

	}

}

//--------------------------------------------------------------
void testApp::draw(){

    gui.draw();
    greyImg.draw(0,100);
    bri.draw(640,100);
    //edge.draw(640,500);
    if (showContours)   {
        contours.draw(0,100);
    }

    if (bass > 0.4f)    {
        ofEnableAlphaBlending();
    ofSetColor(255,0,0,100);
    if (contours.nBlobs > 0){


    ofFill();

            float x = 0;
            float y = 100;
            for (int t=0;t<contours.nBlobs;t++) {
                    blob = contours.blobs.at(t);
            ofBeginShape();
            for (int i = 0; i < blob.nPts; i++){
                   ofVertex(x + blob.pts[i].x, y + blob.pts[i].y);
                   cout << contours.nBlobs << endl;
            }
            ofEndShape(true);
            }
            ofDisableAlphaBlending();


    }
    //cout << contours.nBlobs << endl;
	ofEnableAlphaBlending();
		ofSetColor(255,255,255,100);
		ofRect(0,600,5*128,200);
	ofDisableAlphaBlending();
    }
	// draw the fft resutls:
	ofSetColor(255,255,255,255);

	float width = (float)(5*128) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofRect(100+i*width,ofGetHeight()-100,width,-(fftSmoothed[i] * 200));
	}


}
void testApp::keyPressed(int key){

	switch (key){
		case 'a':
			if (canny) {
                canny = false;
                applyCanny();
			}
			else    {
                canny = true;
                applyCanny();
			}
        break;
        case 'q':
        if (invt) {
                invt = false;
                applyCanny();
			}
			else    {
                invt = true;
                applyCanny();
			}
        break;
        case 'c':
        if (showContours) {
                showContours = false;

			}
			else    {
                showContours = true;

			}
        break;
	}
}

