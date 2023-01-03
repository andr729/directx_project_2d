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




EntityHandler handler;
DT::RectDrawable rect_drawable;
DT::EllipseDrawable ellipse_drawable;

void add_rect(Vector2D pos, Vector2D v, Float dx, Float dy, Float mass) {
	auto rect = new RectangleEntity(pos, v, dx, dy);
	rect->mass = mass;
	rect->drawable = &rect_drawable;
	handler.addEntity(rect);
}

void add_circ(Vector2D pos, Vector2D v, Float r, Float mass) {
	auto circ = new CircleEntity(pos, v, r);
	circ->mass = mass;
	circ->drawable = &ellipse_drawable;
	handler.addEntity(circ);
}

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
	handler.simulateTick();
	handler.collideAll(1);
}

HRESULT init(HWND hwnd) {

	add_rect({500, 100}, {0, 0}, 400, 20, 10000);
	add_rect({500, 600}, {0, 0}, 400, 20, 10000);
	add_rect({930, 350}, {0, 0}, 20, 280, 10000);
	add_rect({70, 350}, {0, 0}, 20, 280, 10000);

	add_circ({400, 400}, {100, 123}, 10, 0.1);
	add_circ({500, 500}, {10, -23}, 10, 0.1);
	add_circ({300, 300}, {120, -23}, 10, 0.1);
	
	auto hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	// THROW_IF_FAILED(hr, "D2D1CreateFactoryfailed");
	if (factory == nullptr) {
		exit(1);
	}

	hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	// THROW_IF_FAILED(hr, "CoInitializeEx failed");

	// Create WIC factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&img_factory)
	);
	// THROW_IF_FAILED(hr, "Creating WIC factory failed");

	recreateRenderTarget(hwnd);
	return 0;
}

HRESULT recreateRenderTarget(HWND hwnd) {
	HRESULT hr;
	RECT rc;
	GetClientRect(hwnd, &rc);
	// window_size_x = static_cast<FLOAT>(rc.right - rc.left);
	// window_size_y = static_cast<FLOAT>(rc.bottom - rc.top);

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
	// TransformationMatrix transform = Matrix3x2F::Identity();
	global_state.render_target->BeginDraw();
	global_state.render_target->Clear(color);

	handler.drawAll();

	if (global_state.render_target->EndDraw() == D2DERR_RECREATE_TARGET) {
		destroyRenderTarget();
		onPaint(hwnd);
	}
	return 0;
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