#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

HRESULT init(HWND hwnd);
void tick();
HRESULT recreateRenderTarget(HWND hwnd);
void destroyRenderTarget();
void destroy();
HRESULT onPaint(HWND hwnd);
void onMouseMove(FLOAT x, FLOAT y);
void onClick();
