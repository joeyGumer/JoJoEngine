#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD	0.0174532925199432957f
#define RADTODEG	57.295779513082320876f
#define M_PI		3.1415926535897932384f

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

#define GPU_TOTAL_AVAILABLE_MEM 0x9048
#define GPU_CURRENT_AVAILABLE_MEM 0x9049



//Directories
#define LIBRARY "Library/"
#define ASSETS "Assets/"
#define LIBRARY_MESHES "Library/Meshes/"
#define LIBRARY_MATERIALS "Library/Materials/"