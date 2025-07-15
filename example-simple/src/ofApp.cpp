#include "ofApp.h"

void ofApp::setup() {
    // (optional) To show all debug info
    //ofSetLogLevel(OF_LOG_VERBOSE);

    // (required) Setup GUI instance
#ifdef USE_AUTODRAW
    gui.setup(nullptr, true);
#else
    gui.setup(nullptr, false);
#endif

    // (optional) Listen to GUI render event, to draw stuff afterwards.
    gui.afterDraw.add(this, &ofApp::afterDraw, OF_EVENT_ORDER_APP);

    // Alternatively to above, you can register against the ofEvent too :
    //ofEvents().draw.add(this, &ofApp::afterDraw, OF_EVENT_ORDER_AFTER_APP);

    // Note: There's also a beforeDraw event which will render right before the GUI layer is rendered.
}

void ofApp::exit(){
    // Unregister any events that you registered !
    gui.afterDraw.remove(this, &ofApp::afterDraw, OF_EVENT_ORDER_APP);
    //ofEvents().draw.remove(this, &ofApp::afterDraw, OF_EVENT_ORDER_AFTER_APP);
}

void ofApp::draw() {
	// Set background-color
	ofSetBackgroundColor(backGroundColor[0]*100, backGroundColor[1]*100, backGroundColor[2]*100);

	// Start drawing to ImGui (newFrame)a
	gui.begin();

    // Create a new window
    ImGui::Begin("ofxImGui example-simple");
    ImGui::TextWrapped("This example can be compiled with or without `USE_AUTODRAW`, to demonstrate 2 different behaviours.");
    bool autoDrawOn = gui.isAutoDrawEnabled(); // Make variable editable
    ImGui::BeginDisabled();
    ImGui::Checkbox("Autodraw", &autoDrawOn);
    ImGui::EndDisabled();

    ImGui::Dummy({10,10});

    ImGui::SeparatorText("Sample widgets");
    ImGui::TextWrapped("Have a look at the source code to see how widgets connect with your app data.");

    // Method 1 - Passing variables to ImGui
    // In ImGui code, you'll often find static variables, as below.
    // They are gui-only variables : initialised once in a stack and remain accessible there, unlike non-static variables which get destructed once the stack closes.
    // Many arguments you pass to ImGui functions need to remain valid between frames. (they are passed by reference [&])
    // Note: Prefer method 2 (below), unless you understand how static variables work within scopes.
    static bool staticBool = false;
    ImGui::Checkbox("Checkbox", &staticBool);

    // You can hide and show Gui parts on demand : just call the parts that you need
    if(staticBool){
        ImGui::Text("The checkbox above is checked.");
        ImGui::SameLine();
        ImGui::TextDisabled("<-- conditional gui section !");
    }

    ImGui::Dummy(ImVec2(10,10));
    ImGui::SeparatorText("App Settings");

    // Method 2 - Passing variables to ImGui
    // This method is closer to OF practises, defining member variables in your ofApp, and passing them to ImGui as a pointer.
    ImGui::Checkbox("Draw lines", &drawLines);
    ImGui::ColorEdit3("Background color", &backGroundColor[0]);
    ImGui::SliderFloat("Num lines", &numLines, 1, 100);

    // Close the main window
    ImGui::End();

    // End our ImGui Frame.
    // From here on, no GUI components can be submitted anymore !
    gui.end();

    // Use our GUI variables
    // The GUI hasn't been rendered yet : we can still draw below it
    if(drawLines){
        auto halfWidth = ofGetWidth()*.5f;
        auto halfHeight = ofGetHeight()*.5f;
        for(unsigned int i=0u; i<numLines;i++){
            ofDrawLine( halfWidth+ofRandomf()*halfWidth, halfHeight+ofRandomf()*halfHeight, halfWidth+ofRandomf()*halfWidth, halfHeight+ofRandomf()*halfHeight );
        }
    }

    // Render the GUI
    // In manual mode, you can choose to render imgui at a given moment in your rendering pipeline
#ifdef USE_AUTODRAW
    // Nothing: The GUI automatically renders then ofApp::draw() is done.
#else
    gui.draw();
#endif

    // What happens when you draw here ?
#ifdef USE_AUTODRAW
    ofDrawBitmapStringHighlight( "Draw after GUI: I'm below the Gui ! (in autodraw mode, the GUI is always above)", 10, 20); // <-- This text will be over below the gui, except in shared mode
#else
    ofDrawBitmapStringHighlight( "Draw after GUI: I'm over the Gui thanks to manual draw !", 10, 20); // <-- This text will be drawn over the gui
#endif

    // If shared mode is on together with autodraw, rendering will happen after this scope, using the ofApp::draw event callback.
}

void ofApp::update(){
    // Gui variables are also accessible outside of the draw() loop.
    int numLinesCopy = numLines; // do something with numLines...
    numLinesCopy*=1;// silence "unused variable" warning !
}

void ofApp::afterDraw(ofEventArgs&){
    ofDrawBitmapStringHighlight( "The gui.afterDraw() event always lets you draw above the GUI.", 10, 45);
}
