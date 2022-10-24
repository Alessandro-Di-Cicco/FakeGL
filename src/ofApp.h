#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void mouseMoved(int x, int y) override;
		void keyPressed(ofKeyEventArgs& key) override;
};
