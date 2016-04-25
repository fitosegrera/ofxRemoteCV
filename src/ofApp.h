#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"
#include "ofxOpenCv.h"
#include "ofxJSON.h"

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
		
		void getImage(string u, string usr, string pwd);
		void drawBlobs();
		void timerCompleteHandler(int &args);

		void drawStrokes(vector<ofPoint> strokes, int radius);
		
		ofxJSONElement config;

		ofxSimpleTimer timer;
		int period;
		ofImage snapshot;
		string url;
		bool frameNew;
		string user, password;
		int w, h;

		ofxCvColorImage	colorImg;
        ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;
        ofxCvContourFinder contourFinder;
		int threshold;
		bool bLearnBakground;
		int state;

		vector<ofPoint> positions;

};
