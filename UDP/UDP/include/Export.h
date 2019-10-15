#pragma once


#pragma warning(disable:4251)

#ifdef UDP_CONTEXT
#define UDP_API __declspec(dllexport)
#else
#define UDP_API __declspec(dllimport)
#endif

