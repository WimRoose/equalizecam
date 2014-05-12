#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"


class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void drawBlob(ofxCvBlob& blob);
		void slider1Changed(int & canny);
		void slider2Changed(int & canny);
		void slider3Changed(int & th);
		void applyCanny();
        void keyPressed(int key);

		ofImage im;


        ofxCvColorImage rgb,hsb;


        ofxIntSlider slider1;
        ofxIntSlider slider2;
        ofxIntSlider slider3;
        ofxPanel gui;

        int cannyTh;
        int findHue;
        float bass;

		ofSoundPlayer 		beat;

		float 				* fftSmoothed;
		// we will draw a rectangle, bouncing off the wall:


		int nBandsToGet;
		float prevx, prevy;
};

