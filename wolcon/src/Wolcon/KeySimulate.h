#pragma once

#include "keyCodes.h"
#include "Core.h"

// Interface representing a desktop system based key simulate
class WOLCON_API KeySimulate {

public:
    virtual void simulateDown(int keycode) = 0;

    virtual void simulateUp(int keycode) = 0;

    virtual void simulatePress(int keycode) = 0;

    virtual ~KeySimulate(){}

};

