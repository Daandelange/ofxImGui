#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofEasyCam.h"
#include "GuiEventHelper.h"

class ofApp : public ofBaseApp {

    public:
        ofApp();

        void setup() override;
        void draw() override;

        //--------------------------------------------------------------
        void keyPressed(int key) override;
        void keyPressed(ofKeyEventArgs&) override;
        void keyReleased(int key) override;
        void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
        void mouseMoved(int, int) override;
        void mouseDragged(int , int , int) override;
        void mousePressed(int, int, int) override;
        void mouseReleased(int, int, int) override;
        void windowResized(int, int) override;
        void gotMessage(ofMessage) override;
        void dragEvent(ofDragInfo) override;

    private:
        ofxImGui::Gui gui;

        // Variables exposed to ImGui
        bool showMetrics        = true;
        bool showDemo           = false;
        bool showFX             = false;
        bool showTexturesWindow = true;
        bool showOfxImGuiDebugWindow = false;
        bool bShowGui           = true;
        bool show_test_window;
        bool show_another_window;

        // Line + draw settings
        float v = 0;
        int   linesPerSide  = 15;
        float linesSpacing  = 0.02f;
        float lineThickness = 0.8f;
        bool  drawLines     = false;
        ImVec4 backgroundColor;

        // IO event helper
        bool showCamWindow    = true;
        ofxImGui::GuiEventHelper eventHelper;
        ofEasyCam cam;

        // Texture stuff
        ofImage   imageButtonSource;
        GLuint    imageButtonID;
        ofPixels  pixelsButtonSource;
        GLuint    pixelsButtonID;
        ofTexture textureSource;
        GLuint    textureSourceID;

        std::vector<std::string> fileNames;
        std::vector<ofFile> files;
        std::vector<GLuint> fileTextures;
};
