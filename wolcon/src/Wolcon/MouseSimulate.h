#pragma once

#include "MouseButtonCodes.h"

class MouseSimulate{

public:

    virtual void simulateDown(int keycode) = 0;

    virtual void simulateUp(int keycode) = 0;

    virtual void simulatePress(int keycode) = 0;

    virtual ~MouseSimulate(){}

};
