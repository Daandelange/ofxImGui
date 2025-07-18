#include "GuiEventHelper.h"

#include "ofxImGuiConstants.h"

#include "imgui.h"

namespace ofxImGui
{

    GuiEventHelper::GuiEventHelper(){

    }

    GuiEventHelper::~GuiEventHelper(){
        // Destroy listeners
        // Up to user ! ?
        unbindFiltersToOfCoreEvents();
    };

#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
    void GuiEventHelper::unbindCoreOfEvents(ofBaseApp& app, ofEventOrder prio){
        ofCoreEvents& ce = ofEvents();

        // Note: Virtual methods will resolve the ofBaseApp sub-class, which wil be notified !
        ofRemoveListener(ce.keyPressed,     &app, &ofBaseApp::keyPressed    , prio);
        ofRemoveListener(ce.keyReleased,    &app, &ofBaseApp::keyReleased   , prio);

        ofRemoveListener(ce.mouseMoved,     &app, &ofBaseApp::mouseMoved    , prio);
        ofRemoveListener(ce.mouseDragged,   &app, &ofBaseApp::mouseDragged  , prio);
        ofRemoveListener(ce.mousePressed,   &app, &ofBaseApp::mousePressed  , prio);
        ofRemoveListener(ce.mouseReleased,  &app, &ofBaseApp::mouseReleased , prio);
        ofRemoveListener(ce.mouseScrolled,  &app, &ofBaseApp::mouseScrolled , prio);

        ofRemoveListener(ce.touchDown,      &app, &ofBaseApp::touchDown     , prio);
        ofRemoveListener(ce.touchUp,        &app, &ofBaseApp::touchUp       , prio);
        ofRemoveListener(ce.touchMoved,     &app, &ofBaseApp::touchMoved    , prio);
        ofRemoveListener(ce.touchDoubleTap, &app, &ofBaseApp::touchDoubleTap, prio);
        ofRemoveListener(ce.touchCancelled, &app, &ofBaseApp::touchCancelled, prio);

        ofRemoveListener(ce.fileDragEvent,  &app, &ofBaseApp::dragged       , prio);
        //ofRemoveListener(ce.charEvent,      &app, &ofBaseApp::XXXX, prio );
    }

    void GuiEventHelper::bindFilteredEvents(ofBaseApp& app, ofEventOrder prio){
        ofAddListener(keyPressed,     &app, &ofBaseApp::keyPressed    , prio);
        ofAddListener(keyReleased,    &app, &ofBaseApp::keyReleased   , prio);

        ofAddListener(mouseMoved,     &app, &ofBaseApp::mouseMoved    , prio);
        ofAddListener(mouseDragged,   &app, &ofBaseApp::mouseDragged  , prio);
        ofAddListener(mousePressed,   &app, &ofBaseApp::mousePressed  , prio);
        ofAddListener(mouseReleased,  &app, &ofBaseApp::mouseReleased , prio);
        ofAddListener(mouseScrolled,  &app, &ofBaseApp::mouseScrolled , prio);

        ofAddListener(touchDown,      &app, &ofBaseApp::touchDown     , prio);
        ofAddListener(touchUp,        &app, &ofBaseApp::touchUp       , prio);
        ofAddListener(touchMoved,     &app, &ofBaseApp::touchMoved    , prio);
        ofAddListener(touchDoubleTap, &app, &ofBaseApp::touchDoubleTap, prio);
        ofAddListener(touchCancelled, &app, &ofBaseApp::touchCancelled, prio);
    }

    void GuiEventHelper::unbindFilteredEvents(ofBaseApp& app, ofEventOrder prio){
        ofRemoveListener(keyPressed,     &app, &ofBaseApp::keyPressed    , prio);
        ofRemoveListener(keyReleased,    &app, &ofBaseApp::keyReleased   , prio);

        ofRemoveListener(mouseMoved,     &app, &ofBaseApp::mouseMoved    , prio);
        ofRemoveListener(mouseDragged,   &app, &ofBaseApp::mouseDragged  , prio);
        ofRemoveListener(mousePressed,   &app, &ofBaseApp::mousePressed  , prio);
        ofRemoveListener(mouseReleased,  &app, &ofBaseApp::mouseReleased , prio);
        ofRemoveListener(mouseScrolled,  &app, &ofBaseApp::mouseScrolled , prio);

        ofRemoveListener(touchDown,      &app, &ofBaseApp::touchDown     , prio);
        ofRemoveListener(touchUp,        &app, &ofBaseApp::touchUp       , prio);
        ofRemoveListener(touchMoved,     &app, &ofBaseApp::touchMoved    , prio);
        ofRemoveListener(touchDoubleTap, &app, &ofBaseApp::touchDoubleTap, prio);
        ofRemoveListener(touchCancelled, &app, &ofBaseApp::touchCancelled, prio);

        ofRemoveListener(fileDragEvent,  &app, &ofBaseApp::dragged       , prio);
        //ofRemoveListener(charEvent,      &app, &ofBaseApp::XXXX );
    }

    void GuiEventHelper::disable(){
//		setup.disable();
//		draw.disable();
//		update.disable();
//		exit.disable();
		keyPressed.disable();
		keyReleased.disable();
		mouseDragged.disable();
		mouseReleased.disable();
		mousePressed.disable();
		mouseMoved.disable();
		mouseScrolled.disable();
//		mouseEntered.disable();
//		mouseExited.disable();
		touchDown.disable();
		touchUp.disable();
		touchMoved.disable();
		touchDoubleTap.disable();
		touchCancelled.disable();
//		messageEvent.disable();
//		fileDragEvent.disable();
	}

	void GuiEventHelper::enable(){
//		setup.enable();
//		draw.enable();
//		update.enable();
//		exit.enable();
		keyPressed.enable();
		keyReleased.enable();
		mouseDragged.enable();
		mouseReleased.enable();
		mousePressed.enable();
		mouseMoved.enable();
		mouseScrolled.enable();
//		mouseEntered.enable();
//		mouseExited.enable();
		touchDown.enable();
		touchUp.enable();
		touchMoved.enable();
		touchDoubleTap.enable();
		touchCancelled.enable();
//		messageEvent.enable();
//		fileDragEvent.enable();
	}
#endif

    void GuiEventHelper::bindFiltersToOfCoreEvents(ofEventOrder prio){
        ofCoreEvents& ce = ofEvents();

        ofAddListener(ce.keyPressed,     this, &GuiEventHelper::filterKeyPressed   , prio);
        ofAddListener(ce.keyReleased,    this, &GuiEventHelper::filterKeyReleased  , prio);

        ofAddListener(ce.mouseMoved,     this, &GuiEventHelper::filterMouseMoved   , prio);
        ofAddListener(ce.mouseDragged,   this, &GuiEventHelper::filterMouseDragged , prio);
        ofAddListener(ce.mousePressed,   this, &GuiEventHelper::filterMousePressed , prio);
        ofAddListener(ce.mouseReleased,  this, &GuiEventHelper::filterMouseReleased, prio);
        ofAddListener(ce.mouseScrolled,  this, &GuiEventHelper::filterMouseScrolled, prio);

        ofAddListener(ce.touchDown,      this, &GuiEventHelper::filterTouchDown     , prio);
        ofAddListener(ce.touchUp,        this, &GuiEventHelper::filterTouchUp       , prio);
        ofAddListener(ce.touchMoved,     this, &GuiEventHelper::filterTouchMoved    , prio);
        ofAddListener(ce.touchDoubleTap, this, &GuiEventHelper::filterTouchDoubleTap, prio);
        ofAddListener(ce.touchCancelled, this, &GuiEventHelper::filterTouchCancelled, prio);

        ofAddListener(ce.fileDragEvent,  this, &GuiEventHelper::filterFileDragEvent , prio);
        ofAddListener(ce.charEvent,      this, &GuiEventHelper::filterCharEvent     , prio);
    }

    void GuiEventHelper::unbindFiltersToOfCoreEvents(ofEventOrder prio){
        ofCoreEvents& ce = ofEvents();

        ofRemoveListener(ce.keyPressed,     this, &GuiEventHelper::filterKeyPressed    , prio);
        ofRemoveListener(ce.keyReleased,    this, &GuiEventHelper::filterKeyReleased   , prio);

        ofRemoveListener(ce.mouseMoved,     this, &GuiEventHelper::filterMouseMoved    , prio);
        ofRemoveListener(ce.mouseDragged,   this, &GuiEventHelper::filterMouseDragged  , prio);
        ofRemoveListener(ce.mousePressed,   this, &GuiEventHelper::filterMousePressed  , prio);
        ofRemoveListener(ce.mouseReleased,  this, &GuiEventHelper::filterMouseReleased , prio);
        ofRemoveListener(ce.mouseScrolled,  this, &GuiEventHelper::filterMouseScrolled , prio);

        ofRemoveListener(ce.touchDown,      this, &GuiEventHelper::filterTouchDown     , prio);
        ofRemoveListener(ce.touchUp,        this, &GuiEventHelper::filterTouchUp       , prio);
        ofRemoveListener(ce.touchMoved,     this, &GuiEventHelper::filterTouchMoved    , prio);
        ofRemoveListener(ce.touchDoubleTap, this, &GuiEventHelper::filterTouchDoubleTap, prio);
        ofRemoveListener(ce.touchCancelled, this, &GuiEventHelper::filterTouchCancelled, prio);

        ofRemoveListener(ce.fileDragEvent,  this, &GuiEventHelper::filterFileDragEvent , prio);
        ofRemoveListener(ce.charEvent,      this, &GuiEventHelper::filterCharEvent     , prio);
    }

	bool GuiEventHelper::filterKeyPressed(ofKeyEventArgs &key){
		if (ImGui::GetIO().WantCaptureKeyboard) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		keyPressed.notify(key);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}
	bool GuiEventHelper::filterKeyReleased(ofKeyEventArgs &key){
		if (ImGui::GetIO().WantCaptureKeyboard) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		keyReleased.notify(key);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterMouseMoved( ofMouseEventArgs & mouse ){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		mouseMoved.notify(mouse);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterMouseDragged( ofMouseEventArgs & mouse ){
		if (ImGui::GetIO().WantCaptureMouse) return true;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		mouseDragged.notify(mouse);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterMousePressed( ofMouseEventArgs & mouse ){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		mousePressed.notify(mouse);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterMouseReleased(ofMouseEventArgs & mouse){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		mouseReleased.notify(mouse);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterMouseScrolled( ofMouseEventArgs & mouse ){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		mouseScrolled.notify(mouse);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterTouchDown(ofTouchEventArgs & touch){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		touchDown.notify(touch);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterTouchMoved(ofTouchEventArgs & touch){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		touchMoved.notify(touch);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterTouchUp(ofTouchEventArgs & touch){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		touchUp.notify(touch);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterTouchDoubleTap(ofTouchEventArgs & touch){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		touchDoubleTap.notify(touch);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterTouchCancelled(ofTouchEventArgs & touch){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		touchCancelled.notify(touch);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterFileDragEvent(ofDragInfo &info){
		if (ImGui::GetIO().WantCaptureMouse) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		fileDragEvent.notify(info);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

	bool GuiEventHelper::filterCharEvent(uint32_t &c){
		if (ImGui::GetIO().WantCaptureKeyboard) return OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT;
#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		charEvent.notify(c);
#endif
		return OFXIMGUI_EVENT_HELPER_RETURN_PASS;
	}

} // end namespace ofxImGui
