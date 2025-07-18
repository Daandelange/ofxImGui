#pragma once

#include "ofEvents.h"
#include "ofBaseApp.h"

#include "ofxImGuiConstants.h"

// Todo: Add flags/bitmasks to optionally include/exclude some event bindings ?

// Uncomment to implement another way, by providing rebind functionality and separate events to listen to.
// If commented: Bind to OF with higher priority and block the event bubling further (more simple impl)
//#define OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs

// Note for ofEvents: return false to bubble, true to intercept/block.
#ifndef OFXIMGUI_EVENT_HELPER_RETURN_PASS
#	define OFXIMGUI_EVENT_HELPER_RETURN_PASS false // Pass false when GUI doesn't intercept
#endif

#ifndef OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT
#	ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
#		define OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT false // Never intercept events
#	else
#		define OFXIMGUI_EVENT_HELPER_RETURN_INTERCEPT true // Pass true to intercept/block
#	endif
#endif

namespace ofxImGui
{
	// Filters GUI events out of ofCoreEvents
	class GuiEventHelper {
		public:

		GuiEventHelper();
		~GuiEventHelper();

#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		void setup(ofBaseApp& app){
			// Unbind app
			unbindCoreOfEvents(app);

			// Bind self to core events
			bindFiltersToOfCoreEvents();

			// Bind app to filtered events
			bindFilteredEvents(app);
		}
#else
		void setup(){
			// Bind self to core events
			bindFiltersToOfCoreEvents();
		}
#endif

#ifdef OFXIMGUI_EVENT_HELPER_IMPL_ALTERNATE_EVENTs
		//ofEvent<ofEventArgs> onMouseDownPassThrough;
		ofEvent<ofKeyEventArgs> 	keyPressed;
		ofEvent<ofKeyEventArgs> 	keyReleased;

		ofEvent<ofMouseEventArgs> 	mouseMoved;
		ofEvent<ofMouseEventArgs> 	mouseDragged;
		ofEvent<ofMouseEventArgs> 	mousePressed;
		ofEvent<ofMouseEventArgs> 	mouseReleased;
		ofEvent<ofMouseEventArgs> 	mouseScrolled;
//		ofEvent<ofMouseEventArgs> 	mouseEntered;
//		ofEvent<ofMouseEventArgs> 	mouseExited;

		ofEvent<ofTouchEventArgs>	touchDown;
		ofEvent<ofTouchEventArgs>	touchUp;
		ofEvent<ofTouchEventArgs>	touchMoved;
		ofEvent<ofTouchEventArgs>	touchDoubleTap;
		ofEvent<ofTouchEventArgs>	touchCancelled;

		ofEvent<ofDragInfo>			fileDragEvent;
		ofEvent<uint32_t>			charEvent;

		//void bindOfEvents(ofCoreEvents& events = ofEvents());

		// Pause event notifications
		void disable();
		void enable();

		// (un)Binds ofApp from ofEvents()
		//void bindCoreOfEvents(ofBaseApp& app, ofEventOrder prio = OF_EVENT_ORDER_BEFORE_APP);
		void unbindCoreOfEvents(ofBaseApp& app, ofEventOrder prio = OF_EVENT_ORDER_BEFORE_APP);

		// (un)Binds ofApp to filteredEvents
		void bindFilteredEvents(ofBaseApp& app, ofEventOrder prio = OF_EVENT_ORDER_BEFORE_APP);
		void unbindFilteredEvents(ofBaseApp& app, ofEventOrder prio = OF_EVENT_ORDER_BEFORE_APP);
#endif
		// (un)Binds filter events to ofCoreEvents
		void bindFiltersToOfCoreEvents(ofEventOrder prio = OF_EVENT_ORDER_BEFORE_APP);
		void unbindFiltersToOfCoreEvents(ofEventOrder prio = OF_EVENT_ORDER_BEFORE_APP);

		private:
		//static ofEventArgs voidEventArgs;
		bool filterKeyPressed( ofKeyEventArgs& key );
		bool filterKeyReleased( ofKeyEventArgs& key );

		bool filterMouseMoved( ofMouseEventArgs& mouse );
		bool filterMouseDragged( ofMouseEventArgs& mouse );
		bool filterMousePressed( ofMouseEventArgs& mouse );
		bool filterMouseReleased(ofMouseEventArgs& mouse);
		bool filterMouseScrolled( ofMouseEventArgs& mouse );
//		bool filterMouseEntered( ofMouseEventArgs & mouse );
//		bool filterMouseExited( ofMouseEventArgs & mouse );

		bool filterTouchDown(ofTouchEventArgs& touch);
		bool filterTouchMoved(ofTouchEventArgs& touch);
		bool filterTouchUp(ofTouchEventArgs& touch);
		bool filterTouchDoubleTap(ofTouchEventArgs& touch);
		bool filterTouchCancelled(ofTouchEventArgs& touch);

		bool filterFileDragEvent(ofDragInfo& info);
		bool filterCharEvent(uint32_t& c);
	};
}
