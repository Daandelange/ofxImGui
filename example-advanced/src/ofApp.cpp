#include "ofApp.h"
#include "RandomTheme.h"
#include "ImHelpers.h"
#include "imgui_internal.h" // GetKeyData()

// It is easy to extend ImGui making simple functions that use the ImGui API.
// If you need access to inner ImGui variables (eg: more complex tweaks), it's better to write an ImGui plugin and load that. (see imdrawlist_party.cpp)
// Code from ImGuiDemo.cpp
static void HelpMarker(const char* desc){
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
void FxTestBed(bool*); // Forward declared

ofApp::ofApp() : v(0) {

}

void ofApp::setup() {
    // Enable detailed logging
    ofSetLogLevel(OF_LOG_VERBOSE);

    // Enable viewports, enable state saving
    gui.setup(nullptr, false, ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable, true);
    //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // The backgroundColor is stored as an ImVec4 type but can handle ofColor
    backgroundColor = ofColor(114, 144, 154);

    show_test_window = true;
    show_another_window = false;

    // Textures stuff
    // Load your own ofImage
    //imageButtonSource.setUseTexture(false); // Fixes loading !
    imageButtonSource.load("of.png");
    imageButtonID = gui.loadImage(imageButtonSource);

    // Or have the loading done for you if you don't need the ofImage reference
    //imageButtonID = gui.loadImage("of.png");

    // You can also use ofPixels in same way
    ofLoadImage(pixelsButtonSource, "of_upside_down.png");
    pixelsButtonID = gui.loadPixels(pixelsButtonSource);

    // Pass in your own texture reference, and keep it for later usage
    textureSourceID = gui.loadTexture(textureSource, "of_upside_down.png");

    //or just pass a path
    //textureSourceID = gui.loadTexture("of_upside_down.png");

    ofLogVerbose() << "textureSourceID: " << textureSourceID;

    ofDirectory dataDirectory(ofToDataPath("", true));

    files = dataDirectory.getFiles();
    for(size_t i=0; i<files.size(); i++){
        fileNames.push_back(files[i].getFileName());

        // Create texture ID
        GLuint texID = gui.loadPixels(files[i].getFileName());
        fileTextures.push_back(texID);
    }
    imageButtonID = fileTextures.front();

    // It's nice to disable escape quitting your ofApp, as ImGui also listens to escape.
    ofSetEscapeQuitsApp(false);

    // Filter-out gui-consumed user input events
    eventHelper.setup();
}

void ofApp::draw() {
    // Precalcs
    #define plotSize 200
    static float realFPS[plotSize]={0};
    static float avgFPS=0;
    avgFPS=0;
    for(int i=0; i<plotSize-1; ++i){
        avgFPS+=realFPS[i];
        realFPS[i]=realFPS[i+1];
    }
    realFPS[plotSize-1]=ofGetFrameRate();
    avgFPS=(avgFPS+realFPS[plotSize-1])/plotSize;

    // BackgroundColor is stored as an ImVec4 type but is converted to ofColor automatically
    ofSetBackgroundColor(backgroundColor);

    // We have to use ofParameter::get() since this we are using an ofFloatColor.
//	if (!ofxImGui::IsMouseOverGui()) ofSetBackgroundColor(backgroundColor);
//	if (!ImGui::GetIO().WantCaptureMouse) ofSetBackgroundColor(backgroundColor);
//	else ofSetBackgroundColor(ofColor::white);//bg white if overGui

    // GUI variables
    static float speed1 = 0.072f;
    static float speed2 = 0.103f;
    static float variation1 = 0.0147f;
    static float variation2 = 0.032f;
    static float offset1 = 3.4f;
    static float offset2 = 5.1f;
    static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);
    glm::vec2 windowPos;
    glm::vec2 windowSize;
    static bool isWindowDragging = false;
    static bool isWindowFocused = false;

    // You can conditionally show your GUI
    bool bRenderGuiThisFrame = bShowGui;
    // Start drawing to ImGui (newFrame)
    gui.begin();

    if(bRenderGuiThisFrame){
        // Draw a menu bar
        if(ImGui::BeginMainMenuBar()){

            if(ImGui::BeginMenu( "Options")){

                ImGui::Checkbox("Show Another Window", &show_another_window);
                ImGui::SameLine(); HelpMarker("Opens a 2nd imgui window.");

                ImGui::Checkbox("Show ImGui Metrics", &showMetrics);
                ImGui::SameLine(); HelpMarker("The ImGui metrics is a gui window allowing you to inspect what is going on and how imgui is intagrated and interfaced with oF.");

                ImGui::Separator();
                ImGui::Checkbox("Show ImGui Demo", &showDemo);
                ImGui::SameLine(); HelpMarker("The ImGui Demo Window demonstrates many gui possibilities and serves as a reference for available gui widgets, together with its source code.");

                ImGui::Separator();
                ImGui::Checkbox("Show ofxImGui Debug Window", &showOfxImGuiDebugWindow);
                ImGui::SameLine(); HelpMarker("Helper window for debugging and optimising your ofxImGui instance within your ofApp.\nIt can also been seen as an interactive tutorial explaining how ofxmGui is setup (for avanced users).");

                ImGui::Separator();
                ImGui::Checkbox("Show ImGui FX", &showFX);
                ImGui::SameLine(); HelpMarker("Demoscene contest !\nDemonstrates :\n - how to make an ImGui plugin\n - How to use the imgui drawlist api");

                ImGui::Separator();
                ImGui::Checkbox("Draw Lines", &drawLines);
                ImGui::SameLine(); HelpMarker("An example of how to control your ofApp with ofxImGui.");

                ImGui::Separator();
                ImGui::Checkbox("Draw Camera Scene", &showCamWindow);
                ImGui::SameLine(); HelpMarker("Demonstrates how to handle mixed input between OpenFrameworks and ImGui.");

                ImGui::Separator();
                ImGui::Checkbox("Show Sample Window", &showTexturesWindow);
                ImGui::SameLine(); HelpMarker("Demonstrates showing textures within ImGui.");

                // Gui options
                ImGui::Separator();
                if(ImGui::BeginMenu("GUI")){
                    if(ImGui::MenuItem("Hide GUI")){
                        bShowGui = false;
                    }
                    ImGui::Dummy(ImVec2(10,10));
                    ImGui::SeparatorText("Theme");
                    if (ImGui::Button("Randomize THEME")){
                        gui.setTheme(new RandomTheme());

                    }
                    ImGui::SameLine();
                    if (ImGui::Button("DEFAULT THEME")){
                        gui.setTheme(new ofxImGui::DefaultTheme());
                    }
                    ImGui::EndMenu();
                }

                // Submenu
                if(ImGui::BeginMenu( "More..." )){
                    ImGui::TextDisabled("This is a sub-menu ;)");
                    ImGui::MenuItem( "Something" );
                    ImGui::MenuItem( "Something else" );
                    ImGui::MenuItem( "Something different" );
                    ImGui::EndMenu();
                }

                // Exit
                ImGui::Separator();
                if(ImGui::MenuItem( "Quit" )){
                    ofExit();
                }

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu( "Runtime")){

                // Full Screen
                static bool fullScreen = false;
                if(ImGui::Checkbox("Full screen", &fullScreen)){
                    ofSetFullscreen(fullScreen);
                }

                // Vertical Sync
                static bool vSync = false;
                if(ImGui::Checkbox("Vertical Sync", &vSync)){
                    ofSetVerticalSync(vSync);
                }

                ImGui::Separator();

                // Resolution changer
                static int resolution[2];
                resolution[0]=ofGetWidth();
                resolution[1]=ofGetHeight();
                std::string resString = ofToString(resolution[0]).append(" x ").append(ofToString(resolution[1]));
                if(ImGui::BeginCombo("Resolution", resString.c_str())){
                    if(ImGui::Selectable("800 × 600")){
                        ofSetWindowShape(800,600);
                    }
                    if(ImGui::Selectable("1024 × 768")){
                        ofSetWindowShape(1024, 768);
                    }
                    if(ImGui::Selectable("1366 × 768")){
                        ofSetWindowShape(1366, 768);
                    }
                    if(ImGui::InputInt2("Custom", resolution)){
                        ofSetWindowShape(resolution[0],resolution[1]);
                    }
                    ImGui::EndCombo();
                }

                // FPS
                static int appFPS=30;//ofGetTargetFrameRate();
                if(ImGui::DragInt("FPS Target", &appFPS, 1.f, 10)){ // Minimum set to 10 because double click is not detected below it
                    ofSetFrameRate(appFPS);
                    appFPS=ofGetTargetFrameRate(); // re-sync value with oF
                }
                if(ImGui::IsItemHovered()){
                    ImGui::BeginTooltip();
                    ImGui::Text("(double-click to edit)");
                    ImGui::EndTooltip();
                }
                static char avgFPSString[28]={0};
                sprintf(avgFPSString, "FPS : %.2f average", avgFPS);
                ImGui::PlotHistogram("FPS Histogram", realFPS, plotSize, 0, avgFPSString, 0, 60);

                ImGui::Dummy(ImVec2(10,10));
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::EndMenu();
            }
        } // End menu visible

        ImGui::EndMainMenuBar();

        // Start a transparent docking space to allow docked windows within the ofAppWindow
        ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        // Line drawing logic
        // With viewports enabled, coordinates are absolute
        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)  windowPos -= glm::vec2(ofGetWindowPositionX(), ofGetWindowPositionY());

        // Create a new window
        // ImGuiCond_Once => Set on launch, then let user use their size
        ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->Pos + ImVec2(((ofRandomf()+1.f)*.25f)*ofGetWidth(), ((ofRandomf()+1.f)*.25f)*ofGetHeight()), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(320,240), ImGuiCond_Once);

        if(ImGui::Begin("Main Window\n")){
            ImGui::Text("Example-Advanced");

            ImGui::Dummy({10,10});
            ImGui::TextWrapped(
                "State restoration\nAll windows except this one should restore their size and positions when re-opening this ofApp.\n\n"\
                "GUI window coordinates\nThe graphics around this window are pimped by oF. Use the options menu to draw lines instead.\n\n"\
                "Input events helper\nThe GuiEventHelper class intercepts user input events, preventing them to notify your ofApp when ImGui consumes them.\n\n"\
                "ofxImGuiDebugWindow\nInspect the internals of ofxImGui and gives hints on how to improve the ofxImGui integration within your ofApp."
            );

            ImGui::Dummy({10,10});
            ImGui::TextWrapped("Use the options menu to enable different demo components.");
            ImGui::Text("There are some help indicators around.");
            ImGui::SameLine(); HelpMarker("Bravo, you got it !");

            ImGui::Spacing();
            ImGui::SeparatorText("Layout");
#ifdef OFXIMGUI_BACKEND_GLFW
            ImGui::TextWrapped("ImGui windows can be popped-out of the OF window when using the GLFW engine.");
#elif defined(OFXIMGUI_BACKEND_OPENFRAMEWORKS)
            ImGui::TextWrapped("You're using the custom OpenFrameworks backend. Windows cannot pop-out of the ofAppWindow, but you can still combine and layout imgui windows.");
#else
            ImGui::TextDisabled("Unknown backend = Unknown layouts !");
#endif
            ImGui::TextWrapped("Try dragging around the imgui windows by their title and drop them within other windows to make the layout yours !");

        }
        // Query ImGui window state
        // Note: Helpers help convert ImVec2 to glm::Vec2
        windowPos         = ImGui::GetWindowPos();
        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)  windowPos -= glm::vec2(ofGetWindowPositionX(), ofGetWindowPositionY()); // with viewports enabled, coordinates change
        windowSize        = ImGui::GetWindowSize();
        isWindowDragging  = ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left);
        isWindowFocused   = ImGui::IsWindowFocused();

        ImGui::End(); // end Main Window

        // Line Settings window
        if( drawLines ){
            ImGui::Begin("Line Settings", &drawLines );

            ImGui::TextDisabled("This window is only visible then drawlines is.");

            ImGui::TreePush((void*)NULL);

            ImGui::Dummy(ImVec2(20,5));

            ImGui::SliderInt("Lines per swarm", &linesPerSide, 1, 1000);
            ImGui::DragFloat("Line Spacing", &linesSpacing, 0.0001f, 0.001f, 0.5f, "%.4f");
            ImGui::DragFloat("Line Thickness", &lineThickness, 0.01f, 0.01f, 2.f, "%.2f");
            ImGui::ColorEdit4("Color", (float*)&color);

            ImGui::Dummy(ImVec2(20,10));

            // It's also easy to add extra Gui interactions to fit your needs.
            ImGui::SliderFloat("Speed 1", &speed1, 0.001f, 1.5f);
            if(ImGui::IsItemActive() && ofGetKeyPressed(OF_KEY_SHIFT))
                speed2 = speed1;
            ImGui::SameLine(); HelpMarker("Hold shift to sync both speeds");
            ImGui::SliderFloat("Speed 2", &speed2, 0.001f, 1.5f);
            if(ImGui::IsItemActive() && ofGetKeyPressed(OF_KEY_SHIFT))
                speed1 = speed2;
            ImGui::SameLine(); HelpMarker("Hold shift to sync both speeds");

            ImGui::Separator();
            ImGui::SliderFloat("Speed Variation 1", &variation1, 0.001f, 1.5f);
            ImGui::SliderFloat("Speed Variation 2", &variation2, 0.001f, 1.5f);

            ImGui::Separator();
            ImGui::SliderFloat("Offset 1", &offset1, 0.001f, 10.f);
            ImGui::SliderFloat("Offset 2", &offset2, 0.001f, 10.f);

            ImGui::TreePop();

            // Close the line settings window
            ImGui::End();
        }

        if(showCamWindow){
            ImGui::Begin("Camera Window", &showCamWindow );

            ImGui::Dummy({10,10});
            ImGui::TextWrapped("When manipulating the GUI, the camera shall not move !");
            ImGui::BulletText("Drag (3 buttons)");
            ImGui::BulletText("Scroll");
            ImGui::BulletText("Double-Click");

           //ImGui::Text("IO wantcapturemouse=%i", ImGui::GetIO().WantCaptureMouse);

            ImGui::End();
        }

        if(showTexturesWindow){
            if(ImGui::Begin("Textures Window", &showTexturesWindow)){

                //ImGui::ShowStyleEditor();
                //ImGui::ShowFontSelector("label4FontSelector");

                // Texture loading
                ImGui::Dummy(ImVec2(10,10));
                if(!fileNames.empty()){
                    //ofxImGui::VectorListBox allows for the use of a vector<string> as a data source
                    static int currentListBoxIndex = 0;
                    if(ofxImGui::VectorListBox("VectorListBox", &currentListBoxIndex, fileNames)){
                        ofLog() << " VectorListBox FILE PATH: "  << files[currentListBoxIndex].getAbsolutePath();
                        imageButtonID = fileTextures[currentListBoxIndex];
                    }

                    //ofxImGui::VectorCombo allows for the use of a vector<string> as a data source
                    static int currentFileIndex = 0;
                    if(ofxImGui::VectorCombo("VectorCombo", &currentFileIndex, fileNames)){
                        ofLog() << "VectorCombo FILE PATH: "  << files[currentFileIndex].getAbsolutePath();
                        pixelsButtonID = fileTextures[currentFileIndex];
                    }
                }

                //GetImTextureID is a static function defined in Helpers.h that accepts ofTexture, ofImage, or GLuint
                ImGui::Dummy(ImVec2(10,10));
                if(ImGui::ImageButton("imagebutton", GetImTextureID(imageButtonID), ImVec2(200, 200))){
                       ofLog() << "PRESSED";
                }
                //or do it manually
                ImGui::SameLine();
                ImGui::Image(GetImTextureID(pixelsButtonID), ImVec2(200, 200));
                ImGui::Image((ImTextureID)(uintptr_t)textureSourceID, ImVec2(200, 200));
            }
            ImGui::End();
        }

        // Show another window, this time using an explicit ImGui::Begin and ImGui::End
        if (show_another_window){
            //note: ofVec2f and ImVec2f are interchangeable
            ImGui::SetNextWindowSize(ofVec2f(200,100), ImGuiCond_FirstUseEver);
            if(ImGui::Begin("Another Window", &show_another_window)){
                ImGui::Text("Hello from 2nd window.");
                ImGui::Dummy({10,10});
                ImGui::Text("You can create as many windows as you want.");
            }
            ImGui::End();
        }

        if(showMetrics){
            ImGui::SetNextWindowPos(ImVec2(ofGetWindowPositionX()+ofGetWidth()-350,ofGetWindowPositionY()+30), ImGuiCond_FirstUseEver );
            ImGui::ShowMetricsWindow( &showMetrics );
        }

        if(showDemo) ImGui::ShowDemoWindow(&showDemo);

        if(showOfxImGuiDebugWindow) gui.drawOfxImGuiDebugWindow(&showOfxImGuiDebugWindow);

        if(showFX){
            // Window position is only set on the first launch
            ImGui::SetNextWindowPos(ImVec2(ofGetWindowPositionX()+ofGetWidth()-350,ofGetWindowPositionY()+ofGetHeight()-230), ImGuiCond_FirstUseEver );
            FxTestBed(&showFX); // <-- ImGui extension from im_drawlist_party.cpp
        }

    }
    gui.end();

    // Line drawing variables
    glm::vec2 windowTopLeft     = windowPos;
    glm::vec2 windowTopRight    = windowTopLeft   + glm::vec2(windowSize.x,0);
    glm::vec2 windowBottomLeft  = windowTopLeft   + glm::vec2(0, windowSize.y);
    glm::vec2 windowBottomRight = windowTopLeft   + windowSize;

    glm::vec2 ofWindowTopLeft     = glm::vec2(0                 ,0);
    glm::vec2 ofWindowTopRight    = glm::vec2(ofGetWindowWidth(),0);
    glm::vec2 ofWindowBottomLeft  = glm::vec2(0                 ,ofGetWindowHeight());
    glm::vec2 ofWindowBottomRight = glm::vec2(ofGetWindowWidth(),ofGetWindowHeight());

    // Space changes ofWindow to imguiViewport.
    if( !ofGetKeyPressed(' ')){
        ofRectangle viewport = gui.getMainWindowViewportRect();
        ofWindowTopLeft     = viewport.getTopLeft();
        ofWindowTopRight    = viewport.getTopRight();
        ofWindowBottomLeft  = viewport.getBottomLeft();
        ofWindowBottomRight = viewport.getBottomRight();
    }

    // Draw camera ?
    if(showCamWindow){
        cam.begin();
        ofSetColor(ofColor::green);
        ofDrawBox(100,100,100);
        cam.end();
    }

    // Draw OF graphics
    if(drawLines){
        ofSetLineWidth(lineThickness);
        ofSetColor(color.x*255.f, color.y*255.f, color.z*255.f, color.w*255.f);
        const static int numSides = 4;
        for(int i=0; i < linesPerSide*numSides; i++){
            glm::vec2 fromA  = (i<linesPerSide)? ofWindowTopLeft  :((i<linesPerSide*2)? ofWindowTopRight    :((i<linesPerSide*3)? ofWindowBottomRight : ofWindowBottomLeft));
            glm::vec2 toA    = (i<linesPerSide)? windowTopLeft    :((i<linesPerSide*2)? windowTopRight      :((i<linesPerSide*3)? windowBottomRight   : windowBottomLeft  ));
            glm::vec2 fromB  = (i<linesPerSide)? ofWindowTopRight :((i<linesPerSide*2)? ofWindowBottomRight :((i<linesPerSide*3)? ofWindowBottomLeft  : ofWindowTopLeft   ));
            glm::vec2 toB    = (i<linesPerSide)? windowTopRight   :((i<linesPerSide*2)? windowBottomRight   :((i<linesPerSide*3)? windowBottomLeft    : windowTopLeft     ));
            unsigned int sideNum= (i<linesPerSide)? 0             :((i<linesPerSide*2)? 1                   :((i<linesPerSide*3)? 2                   : 3                 ));
            int lineNum      = i % linesPerSide;
            int variantA     = (sideNum+0)%numSides;
            int variantB     = (sideNum+1)%numSides;
            ofDrawLine(
              //                                                     |Speed  | A/B variation                    |       Spacing        | Differentiation
              ofInterpolateCosine( fromA , toA , ofGetElapsedTimef()*(speed1 + variation1 * variantA * 0.2f )   + lineNum*linesSpacing + ((variantA)%2)*5.3f*offset1),
              ofInterpolateCosine( fromB,  toB,  ofGetElapsedTimef()*(speed1 + variation1 * variantB * 0.2f )   + lineNum*linesSpacing + ((variantB)%2)*5.3f*offset1)
            );
            ofDrawLine(
              ofInterpolateCosine( fromA , toA , ofGetElapsedTimef()*(speed2 + variation2* variantA * 0.2f )   + lineNum*linesSpacing + ((variantB)%2)*5.3f*offset2),
              ofInterpolateCosine( fromB,  toB,  ofGetElapsedTimef()*(speed2 + variation2* variantB * 0.2f )   + lineNum*linesSpacing + ((variantA)%2)*5.3f*offset2)
            );
        }
        ofSetLineWidth(1.f);
    }
    // Visualise imgui window pos with openframeworks draw calls
    else if(bRenderGuiThisFrame) {
        ofSetColor(ofColor::white);
        // Connect Window corners with OF corners
        ofDrawLine(ofWindowTopLeft    , windowTopLeft    );
        ofDrawLine(ofWindowTopRight   , windowTopRight   );
        ofDrawLine(ofWindowBottomRight, windowBottomRight);
        ofDrawLine(ofWindowBottomLeft , windowBottomLeft );

        // Draw a rectangle behind the window
        ofFill();
        if(isWindowDragging) ofSetColor(255,0,0,126);
        else ofSetColor(255,isWindowFocused?80:40);
        ofDrawRectRounded(windowTopLeft-glm::vec2(10), windowSize.x+20, windowSize.y+20, 10);

        // Draw Circles on corners
        ofSetColor(255,255);
        ofNoFill();
        ofDrawCircle(windowTopLeft    , glm::mod( ofGetElapsedTimef()*10.f     , 10.f) );
        ofDrawCircle(windowTopRight   , glm::mod( ofGetElapsedTimef()*10.f+5.0f, 10.f) );
        ofDrawCircle(windowBottomLeft , glm::mod( ofGetElapsedTimef()*10.f+2.5f, 10.f) );
        ofDrawCircle(windowBottomRight, glm::mod( ofGetElapsedTimef()*10.f+7.5f, 10.f) );
    }

    gui.draw();
    if(bRenderGuiThisFrame){
        //gui.draw();
    }
    else{
        // You still need to call this when using pop-out viewports
        //gui.dontDraw();

        ofDrawBitmapStringHighlight("The GUI is hidden ! (press any key to show)", 10, 20);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // Ignore clicks that imgui consumes
    //if(gui.wantsCaptureKeyboard()) return;

    ofLogVerbose("ofApp::keyPressed(int)") << key;

}


//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& key){
    // Ignore presses that imgui consumes (if you don't use the GuiEventHelper)
    // if(gui.wantsCaptureKeyboard()) return;

    // Restore GUI visibility
    if(!bShowGui){
        bShowGui = true;
        return;
    }

    //ofLogVerbose(__FUNCTION__) << key.key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // Ignore presses that imgui consumes (if you don't use the GuiEventHelper)
    // if(gui.wantsCaptureKeyboard()) return;



    //ofLogVerbose(__FUNCTION__) << key;
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
    //ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int, int){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int , int , int){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int){
    //ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int, int, int){}

//--------------------------------------------------------------
void ofApp::windowResized(int, int){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo){}
