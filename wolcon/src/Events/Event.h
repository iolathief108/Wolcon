#pragma once

#include "Core.h"
#include "wlpch.h"

namespace wolcon{
    enum class EventType
    {
        None = 0,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryKeyboard       = BIT(1),
        EventCategoryMouse          = BIT(2),
        EventCategoryMouseButton    = BIT(3)
    };

    class WOLCON_API Event
    {
        public:
            bool Handled = false;

            virtual EventType GetEventType() const = 0;
            virtual const char* GetName() const = 0;
            virtual int GetCategoryFlags() const = 0;
            virtual std::string ToString() const { return GetName(); }

            inline bool IsInCategory(EventCategory category)
            {
                return static_cast<bool>(GetCategoryFlags() & category);
            }
    };

}