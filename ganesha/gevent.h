#ifndef SPCGANESHAENGINE_GEVENT_H
#define SPCGANESHAENGINE_GEVENT_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

enum EEventType {
    EVENT_TYPE_NONE,
    
    EVENT_TYPE_WINDOW_CLOSE,
    EVENT_TYPE_WINDOW_RESIZE,
    EVENT_TYPE_WINDOW_FOCUS,
    EVENT_TYPE_WINDOW_LOST_FOCUS,
    EVENT_TYPE_WINDOW_MOVED,

    EVENT_TYPE_APP_TICK,
    EVENT_TYPE_APP_UPDATE,
    EVENT_TYPE_APP_RENDER,

    EVENT_TYPE_KEY_DOWN,
    EVENT_TYPE_KEY_UP,
    EVENT_TYPE_KEY_TYPED,
    
    EVENT_TYPE_MOUSE_BUTTON_DOWN,
    EVENT_TYPE_MOUSE_BUTTON_UP,
    EVENT_TYPE_MOUSE_MOVED,
    EVENT_TYPE_MOUSE_SCROLLED,
    
    EVENT_TYPE_VULKAN_UPDATE_PROJECTION,
    EVENT_TYPE_VULKAN_UPDATE_VIEWPORT
};

enum EEventCategory {
    EVENT_CATEGORY_NONE = 0b0,
    EVENT_CATEGORY_APPLICATION = 0b1,
    EVENT_CATEGORY_INPUT = 0b10,
    EVENT_CATEGORY_KEYBOARD = 0b100,
    EVENT_CATEGORY_MOUSE = 0b1000,
    EVENT_CATEGORY_MOUSE_BUTTON = 0b10000
};

class GEvent {
public:
    GEvent(const EEventType eventType);
    virtual ~GEvent();
    
    const EEventType getType() const;

private:
    EEventType eventType;
};

};

#endif  //  SPCGANESHAENGINE_GEVENT_H