#pragma once

#ifdef WL_PLATFORM_WINDOWS
    #ifdef WL_BUILD_DLL
        #define WOLCON_API __declspec(dllexport)
    #else
        #define WOLCON_API __declspec(dllimport)
    #endif
#else
#error Wolcon only supports Windows!
#endif


#define BIT(x) (1 << x)