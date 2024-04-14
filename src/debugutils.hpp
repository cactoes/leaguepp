#ifndef __DEBUGUTILS_HPP__
#define __DEBUGUTILS_HPP__

#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef _DEBUG
    #define DEBUG_LOG(fmt, ...) printf("[%s@%i]: "##fmt##"\n", __FILENAME__, __LINE__, __VA_ARGS__)
#else
    #define DEBUG_LOG()
#endif

#endif // __DEBUGUTILS_HPP__