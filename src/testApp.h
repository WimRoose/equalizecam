#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"


class testApp : public ofBaseApp{

	public:
        testApp();
		void setup();
		void update();
		void draw();
		void drawBlobs();
		void slider1Changed(int & canny);
		void slider2Changed(int & canny);
		void slider3Changed(int & th);
		void applyCanny();
        void keyPressed(int key);

		ofImage edge;
        ofxCvColorImage rgb,hsb;
        ofxCvGrayscaleImage bri,greyImg;
        ofxCvBlob blob;
        ofxCvContourFinder contours;
        ofVideoGrabber vidGrabber;
        ofxIntSlider slider1;
        ofxIntSlider slider2;
        ofxIntSlider slider3;
        ofxPanel gui;

        int cannyTh_1;  // Canny low threshold
        int cannyTh_2;  // Canny high threshold (normaal 2 a 3 x low threshold
        int thrshld;    // thresholdwaarde voor .threshold functie na absDiff
        int w;          // width & height
        int h;
        bool canny;     // Canny aan/uit
        bool invt;      // invert aan/uit (zwart wordt wit en omgekeerd)
        bool showContours;
        float bass;

		ofSoundPlayer 		beat;

		float 				* fftSmoothed;
		// we will draw a rectangle, bouncing off the wall:


		int nBandsToGet;
		float prevx, prevy;
};

