#pragma once
#include "Events/Event.h"


class KeyHook{

    typedef void (*eventCallBack)(Event&);


public:
    virtual void SetEventCallback(const eventCallBack& callBack) = 0;



};

