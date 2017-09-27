#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD	0.0174532925199432957f
#define RADTODEG	57.295779513082320876f
#define M_PI		3.1415926535897932384f


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

//Utiles
//NOTE: tried to create as a safer version of delete
//#define void RELEASE(void* x){(x==nullptr)? LOG("trying to delete null variable"): delete x};

// Configuration -----------
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define CENTER_SCREEN_X 400
#define CENTER_SCREEN_Y 320
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "JoJo Engine"