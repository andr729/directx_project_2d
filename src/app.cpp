#include <d2d1_3.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <stdexcept>
#include <chrono>
#include <wincodec.h>
#include "app.hpp"
#include "base.hpp"
#include "physics.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"

// using D2D1::BitmapProperties;
// using D2D1::PixelFormat;
// using D2D1::Point2F;
// using D2D1::RectF;
using D2D1::Matrix3x2F;
// using std::sin;
// using std::array;


namespace {
	ID2D1Factory7* factory = nullptr;
}

void tick() {
	global_state.tick++;
	global_state.handler.simulateTick();
}

HRESULT init(HWND hwnd) {
	global_state.scene = Scene::MenuScene;

	global_state.game_state.upgrades[L"Multiplier"] = 1.2f;
	global_state.game_state.upgrades[L"Number of circles"] = 15.f;
	global_state.game_state.upgrades[L"Circle speed"] = 300.f;
	global_state.game_state.upgrades[L"Explosion radius"] = 50.f;
	global_state.game_state.upgrades[L"Number of squares"] = 15.f;
	global_state.game_state.upgrades[L"Square speed"] = 300.f;
	global_state.game_state.upgrades[L"Number of bullets"] = 2.f;
	global_state.game_state.upgrades[L"Bullet duration"] = 0.5f;
	global_state.game_state.money = 0;

	hr(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory));
	if (factory == nullptr) {
		return E_FAIL;
	}

	hr(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));
	
	hr(DT::initTools(factory))
	global_state.shop_scene.init();
	global_state.menu_scene.init();
	hr(recreateRenderTarget(hwnd));

	return S_OK;
}

HRESULT recreateRenderTarget(HWND hwnd) {
	HRESULT hr;
	RECT rc;
	if (GetClientRect(hwnd, &rc) == 0) {
		return E_FAIL;
	}

	FLOAT window_size_x = static_cast<FLOAT>(rc.right - rc.left);
	FLOAT window_size_y = static_cast<FLOAT>(rc.bottom - rc.top);

	hr(factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(
				static_cast<UINT32>(rc.right) -
				static_cast<UINT32>(rc.left),
				static_cast<UINT32>(rc.bottom) -
				static_cast<UINT32>(rc.top))),
		&global_state.render_target));

	if (global_state.render_target == nullptr) {
		return E_FAIL;
	}

	if ((window_size_x / window_size_y) > (16.0f / 9.0f)) {
		FLOAT ratio = window_size_y / 900.f;
		FLOAT offset = (window_size_x - 1600.f * ratio) / 2;
		global_state.render_target->SetTransform(TransformationMatrix(
			Matrix3x2F::Scale(ratio, ratio, { 0, 0 }) * Matrix3x2F::Translation(offset, 0)).getInner());
	}
	else if ((window_size_x / window_size_y) < (16.0f / 9.0f)) {
		FLOAT ratio = window_size_x / 1600.f;
		FLOAT offset = (window_size_y - 900.f * ratio) / 2;
		global_state.render_target->SetTransform(TransformationMatrix(
			Matrix3x2F::Scale(ratio, ratio, { 0, 0 }) * Matrix3x2F::Translation(0, offset)).getInner());
	}

	hr(DT::recreateTools());

	return S_OK;
}

void destroyRenderTarget() {
	SafeRelease(&global_state.render_target);
}

void destroy() {
	destroyRenderTarget();
	SafeRelease(&factory);
}

HRESULT onPaint(HWND hwnd) {
	if (!global_state.render_target) {
		hr(recreateRenderTarget(hwnd));
	}	
	
	global_state.render_target->BeginDraw();
	global_state.render_target->Clear(DT::color_clear);

	switch (global_state.scene) {
	case Scene::GameScene:
		global_state.game_scene.draw();
		break;
	case Scene::ShopScene:
		hr(global_state.shop_scene.draw());
		break;
	case Scene::MenuScene:
		hr(global_state.menu_scene.draw());
		break;
	case Scene::WinScene:
		hr(global_state.win_scene.draw());
		break;
	}

	if (global_state.render_target->EndDraw() == D2DERR_RECREATE_TARGET) {
		destroyRenderTarget();
		return onPaint(hwnd);
	}
	return  S_OK;
}

void onMouseMove(FLOAT x, FLOAT y) {
	global_state.onMouseMove(x, y);
}

void onClick() {
	switch (global_state.scene) {
	case Scene::ShopScene:
		global_state.shop_scene.onClick();
		break;
	case Scene::GameScene:
		global_state.game_scene.onClick();
		break;	
	case Scene::MenuScene:
		global_state.menu_scene.onClick();
		break;
	case Scene::WinScene:
		global_state.win_scene.onClick();
		break;
	default:
		break;
	}
}