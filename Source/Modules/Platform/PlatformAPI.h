#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#if DLL_EXPORT
#define PLATFORM_API __declspec(dllexport)
#else
#define PLATFORM_API
#endif

#else
#define PLATFORM_API
#endif
