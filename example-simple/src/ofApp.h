#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

// Comment below to use manual drawing
#define USE_AUTODRAW

class ofApp : public ofBaseApp{

	public:
        ofApp() {}

        void setup() override;
        void exit() override;
        void draw() override;
        void update() override;

        void afterDraw(ofEventArgs&);
    private:
        ofxImGui::Gui gui;

        // Variables exposed to ImGui
        float numLines = 10;
        float backGroundColor[3] = {1,1,1};
        bool drawLines = false;
};
