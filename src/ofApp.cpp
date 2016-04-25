#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::timerCompleteHandler( int &args ){
	getImage(url, user, password);
	frameNew = true;
}

//--------------------------------------------------------------
void ofApp::getImage(string u, string usr, string pwd){
	ofSystem("wget " + u + " --user=" + usr +" --password=" + pwd + " -O data/snapshot.png");
	// ofSystem("convert snapshot.cgi snapshot.png");
	snapshot.load("snapshot.png");
}

//--------------------------------------------------------------
void ofApp::drawBlobs(){
	ofFill();
	ofSetHexColor(0x333333);
	ofDrawRectangle(ofGetWidth()/2,0,w,h);
	for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(ofGetWidth()/2,0);
		ofSetColor(255);
		// draw over the centroid if the blob is a hole
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x + w,
				contourFinder.blobs[i].boundingRect.getCenter().y + h);
		}
    }
    ofSetHexColor(0xffffff);
}

//--------------------------------------------------------------
void ofApp::drawStrokes(vector<ofPoint> strokes, int radius){
	for (int i = 0; i < strokes.size(); i++){
		ofFill();
		if(i == strokes.size()-1){
			ofSetHexColor(0xff0000);
		}else{
			ofSetHexColor(0xffffff);
		}
		ofDrawCircle(strokes[i].x, strokes[i].y, radius);
		ofSetHexColor(0xffffff);
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	//load camera data from config.json
	std::string file = "config.json";
	bool parsingSuccessful = config.open(file);
	
	if (parsingSuccessful){
		url = config["url"].asString();
		user = config["user"].asString();;
		password = config["password"].asString();
	}else{
		cout<< "JSON error at reading file"<< endl;
	}

	//setup timer
	period = 3000;
	timer.setup(period);
	timer.start(true);
	ofAddListener(timer.TIMER_COMPLETE, this, &ofApp::timerCompleteHandler);

	//
	frameNew = false;
	w = 640;
	h = 480;
	state = 0;

	colorImg.allocate(w,h);
	grayImage.allocate(w,h);
	grayBg.allocate(w,h);
	grayDiff.allocate(w,h);

	bLearnBakground = true;
	threshold = 80;
}

//--------------------------------------------------------------
void ofApp::update(){
	timer.update();

	if(frameNew){
		colorImg.setFromPixels(snapshot.getPixels());
		grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;
			bLearnBakground = false;
		}
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
		
		for (int i = 0; i < contourFinder.nBlobs; i++){
        	contourFinder.blobs[i].draw(ofGetWidth()/2,0);
			ofPoint p;
			p.x = contourFinder.blobs[i].boundingRect.getCenter().x;
			p.y = contourFinder.blobs[i].boundingRect.getCenter().y;
			positions.push_back(p);
	    }

		frameNew = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	int half = ofGetWidth()/2;
	int ymargin = 500;
	int spacing = 20;
	string imgType = "";

	snapshot.draw(0, 0);
	
	if (state == 0){
		grayImage.draw(half, 0);
		imgType = "Gray Raw";
	}else if (state == 1){
		grayBg.draw(half, 0);
		imgType = "Gray Background";
	}else if (state == 2){
		grayDiff.draw(half, 0);
		ofNoFill();
		ofSetHexColor(0xffffff);
		ofDrawRectangle(ofGetWidth()/2,0,w,h);
		imgType = "Gray Diff";
	}else if (state == 3){
		drawBlobs();
		imgType = "Blob Detection";
	}else if (state == 4){
		ofNoFill();
		ofSetHexColor(0xffffff);
		ofDrawRectangle(ofGetWidth()/2,0,w,h);

		ofPushMatrix();
			ofTranslate(half,0);
			drawStrokes(positions, 3);
		ofPopMatrix();
		
		imgType = "Strokes View";
	}

	std::stringstream s;

	s << "------- URL: " << url << std::endl;
	s << "------ USER: " << user << std::endl;
	s << "-- LOCATION: " << "M50 / Shanghai" << std::endl;
	s << "- FRAMERATE: " << "1 frame every " << period/1000 << " seconds" << std::endl;
	s << "- NEW FRAME: " << frameNew << std::endl;
	s << "------ SIZE: " << snapshot.getWidth() << " X " << snapshot.getHeight() << std::endl;
	ofDrawBitmapString(s.str(), 8, ymargin);
    timer.draw(0, ymargin + spacing*4.3);

    std::stringstream ss;
	ss << "- IMAGE TYPE: " << imgType << std::endl;
	ss << "-- THRESHOLD: " << threshold << std::endl;
	ss << "---- STROKES: " << positions.size() << std::endl;
	
	ofDrawBitmapString(ss.str(), half, ymargin);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case OF_KEY_RIGHT:
			state ++;
			if (state > 4){
				state = 0;
			}
			break;
		case OF_KEY_LEFT:
			state --;
			if (state < 0){
				state = 4;
			}
			break;
		case 'r':
			positions.clear();
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
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
