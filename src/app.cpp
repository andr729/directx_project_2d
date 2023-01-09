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

// using D2D1::BitmapProperties;
// using D2D1::PixelFormat;
// using D2D1::Point2F;
// using D2D1::RectF;
using D2D1::Matrix3x2F;
// using std::sin;
// using std::array;


HRESULT LoadBitmapFromFile(
	ID2D1RenderTarget* pRenderTarget,
	IWICImagingFactory* pIWICFactory,
	PCWSTR uri,
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap** ppBitmap);

namespace {
	ID2D1Factory7* factory = nullptr;
	IWICImagingFactory* img_factory;
	
	D2D1_COLOR_F color(0.5f, 0.5f, 0.5f, 1.f);
}

void tick() {
	global_state.tick++;
	global_state.handler.simulateTick();
}

HRESULT init(HWND hwnd) {
	global_state.shop_scene.init();
	global_state.scene = Scene::ShopScene;

	//TODO: Remove.
	global_state.game_state.money = 1e4;

	hr(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory));
	if (factory == nullptr) {
		return E_FAIL;
	}

	hr(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED));

	// Create WIC factory
	hr(CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&img_factory)
	));
	
	//TODO: handle HRESULT.
	hr(DT::initTools());

	hr(recreateRenderTarget(hwnd));
	return S_OK;
}

HRESULT recreateRenderTarget(HWND hwnd) {
	HRESULT hr;
	RECT rc;
	GetClientRect(hwnd, &rc);
	FLOAT window_size_x = static_cast<FLOAT>(rc.right - rc.left);
	FLOAT window_size_y = static_cast<FLOAT>(rc.bottom - rc.top);


	hr = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(
				static_cast<UINT32>(rc.right) -
				static_cast<UINT32>(rc.left),
				static_cast<UINT32>(rc.bottom) -
				static_cast<UINT32>(rc.top))),
		&global_state.render_target);

	// THROW_IF_FAILED(hr, "CreateHwndRenderTarget failed");

	if (global_state.render_target == nullptr) {
		throw "TODO";
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

	DT::recreateTools();

	return 0;
}

HRESULT destroyRenderTarget() {
	if (global_state.render_target) {
		global_state.render_target->Release();
		global_state.render_target = nullptr;
	}
	return 0;
}

HRESULT destroy() {
	if (global_state.render_target) global_state.render_target->Release();
	if (factory) factory->Release();
	return 0;
}

HRESULT onPaint(HWND hwnd) {
	if (!global_state.render_target) recreateRenderTarget(hwnd);
	
	global_state.render_target->BeginDraw();
	global_state.render_target->Clear(color);

	switch (global_state.scene) {
	case Scene::GameScene:
		global_state.game_scene.draw();
		break;
	case Scene::ShopScene:
		global_state.shop_scene.draw();
		break;
	}

	if (global_state.render_target->EndDraw() == D2DERR_RECREATE_TARGET) {
		destroyRenderTarget();
		onPaint(hwnd);
	}
	return 0;
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
	default:
		break;
	}
}

// @TODO: move to draw tools
HRESULT LoadBitmapFromFile(
		ID2D1RenderTarget* pRenderTarget,
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight,
		ID2D1Bitmap** ppBitmap) {

	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICStream* pStream = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	IWICBitmapScaler* pScaler = nullptr;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if (SUCCEEDED(hr)) {
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr)) {
		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);

	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr)) {

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}