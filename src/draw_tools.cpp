#include "draw_tools.hpp"
#include "base.hpp"
#include "brush.hpp"
#include "global_state.hpp"

namespace DT {
	IDWriteFactory* write_factory = nullptr;
	IDWriteTextFormat* button_text_format = nullptr;
	IDWriteTextFormat* normal_text_format = nullptr;
	IDWriteTextFormat* shop_text_format = nullptr;

	ID2D1SolidColorBrush* black_brush = nullptr;
	ID2D1SolidColorBrush* dark_gray_brush = nullptr;
	ID2D1SolidColorBrush* light_gray_brush = nullptr;
	ID2D1SolidColorBrush* white_brush = nullptr;
	ID2D1SolidColorBrush* customizable_brush = nullptr;
	ID2D1RadialGradientBrush* rad_brush = nullptr;
	ID2D1LinearGradientBrush* lin_brush = nullptr;

	IWICImagingFactory* img_factory = nullptr;
	ID2D1Bitmap* title_bitmap = nullptr;

	
	constexpr int RAD_BRUSH_STOPS = 100;
	constexpr float RAD_BRUSH_DIST = 1.0f / (RAD_BRUSH_STOPS - 1);
	constexpr float RAD_BRUSH_CYCLES = 3.0f;
	constexpr int RAD_BRUSH_RADIUS = 500;
	constexpr double RAD_BRUSH_OFFSET_PER_TICK = 0.01;

	constexpr int LIN_BRUSH_STOPS = 100;
	constexpr float LIN_BRUSH_DIST = 1.0f / (LIN_BRUSH_STOPS - 1);
	constexpr float LIN_BRUSH_CYCLES = 3.0f;
	constexpr int LIN_BRUSH_RADIUS = 500;

	RadBrushState<RAD_BRUSH_STOPS> rad_brush_data;
	LinBrushState<LIN_BRUSH_STOPS> lin_brush_data;

	D2D1_COLOR_F color_clear = { .r = 0.5f, .g = 0.5f, .b = 0.5f, .a = 1.f };
	constexpr D2D1_COLOR_F color_black = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.0f };
	constexpr D2D1_COLOR_F color_dark_gray = { .r = 0.4f, .g = 0.4f, .b = 0.4f, .a = 1.0f };
	constexpr D2D1_COLOR_F color_light_gray = { .r = 0.625f, .g = 0.625f, .b = 0.625f, .a = 1.0f };
	constexpr D2D1_COLOR_F color_white = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.0f };


	struct CubicBezier {
		D2D1_POINT_2F control1;
		D2D1_POINT_2F control2;
		D2D1_POINT_2F target;
	};

	struct Path {
		D2D1_POINT_2F start;
		std::vector<CubicBezier> segments;
	};

	Path title_path_data = {
		{703.00,290.00},
		{
			{{852.00,319.00},{1015.00,311.00},{1100.00,267.00}},
			{{1185.00,223.00},{1294.00,124.00},{1350.00,170.00}},
			{{1406.00,216.00},{1446.00,268.00},{1455.00,446.00}},
			{{1464.00,624.00},{1434.00,737.00},{1346.00,795.00}},
			{{1258.00,853.00},{916.00,750.00},{787.00,754.00}},
			{{658.00,758.00},{322.00,800.00},{243.00,760.00}},
			{{164.00,720.00},{136.00,644.00},{120.00,569.00}},
			{{104.00,494.00},{112.00,243.00},{226.00,188.00}},
			{{340.00,133.00},{554.00,261.00},{703.00,290.0}}
		}
	};
	ID2D1RectangleGeometry* title_rect = nullptr;
	ID2D1PathGeometry* title_path = nullptr;
	ID2D1GeometrySink* title_path_sink = nullptr;
	ID2D1GeometryGroup* title_group = nullptr;

	ID2D1Geometry* title_geometries[2];

	// To jest przerobiona funkcja stąd: https://www.codespeedy.com/hsv-to-rgb-in-cpp/.
	D2D1_COLOR_F HSVToRGB(float H, float S, float V) {
		H = std::fmod(H, 1.0f);
		if (H < 0) H += 1.0f;

		while (H < 0) {
			H += 1;
		}
		H *= 360;
		S *= 100;
		V *= 100;

		float s = S / 100;
		float v = V / 100;
		float C = s * v;
		float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
		float m = v - C;
		float r, g, b;
		if (H >= 0 && H < 60) {
			r = C, g = X, b = 0;
		}
		else if (H >= 60 && H < 120) {
			r = X, g = C, b = 0;
		}
		else if (H >= 120 && H < 180) {
			r = 0, g = C, b = X;
		}
		else if (H >= 180 && H < 240) {
			r = 0, g = X, b = C;
		}
		else if (H >= 240 && H < 300) {
			r = X, g = 0, b = C;
		}
		else {
			r = C, g = 0, b = X;
		}
		float R = (r + m);
		float G = (g + m);
		float B = (b + m);
		return D2D1_COLOR_F({ R, G, B, 1.0f });
	}

	HRESULT initTools(ID2D1Factory7* factory) {
		hr(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&write_factory)
		));

		hr(write_factory->CreateTextFormat(
			L"Trebuchet MS",
			nullptr,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			BUTTON_FONT_STROKE,
			L"en-us",
			&button_text_format
		));
		button_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		hr(write_factory->CreateTextFormat(
			L"Trebuchet MS",
			nullptr,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			TEXT_FONT_STROKE,
			L"en-us",
			&normal_text_format
		));

		hr(write_factory->CreateTextFormat(
			L"Arial",
			nullptr,
			DWRITE_FONT_WEIGHT_HEAVY,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			SHOP_FONT_STROKE,
			L"en-us",
			&shop_text_format
		));
		shop_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		hr(CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&img_factory)
		));


		hr(factory->CreateRectangleGeometry({ 0,0,1600,900 }, &title_rect));
		hr(factory->CreatePathGeometry(&title_path));
		title_path->Open(&title_path_sink);
		title_path_sink->BeginFigure(title_path_data.start, D2D1_FIGURE_BEGIN_FILLED);
		for (auto segment : title_path_data.segments) {
			title_path_sink->AddBezier(D2D1::BezierSegment(
				segment.control1, segment.control2, segment.target));
		}
		title_path_sink->EndFigure(D2D1_FIGURE_END_OPEN);
		title_path_sink->Close();

		title_geometries[0] = title_rect;
		title_geometries[1] = title_path;
		hr(factory->CreateGeometryGroup(
			D2D1_FILL_MODE_ALTERNATE,
			title_geometries,
			2,
			&title_group
		));
	}

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

	HRESULT recreateTools() {
		ID2D1HwndRenderTarget* render_target = global_state.render_target;
		hr(render_target->CreateSolidColorBrush(color_black, &black_brush));
		hr(render_target->CreateSolidColorBrush(color_dark_gray, &dark_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_light_gray, &light_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_white, &white_brush));
		hr(render_target->CreateSolidColorBrush(color_black, &customizable_brush));
		hr(makeRadBrush<RAD_BRUSH_STOPS>(
			rad_brush_data, render_target,
			{ 0, 0 }, RAD_BRUSH_RADIUS, RAD_BRUSH_RADIUS
		));
		rad_brush = rad_brush_data.brush;
		hr(makeLinBrush<LIN_BRUSH_STOPS>(
			lin_brush_data, render_target,
			Vector2D(0, 450), Vector2D(1600, 450)
		));
		lin_brush = lin_brush_data.brush;

		hr(LoadBitmapFromFile(
			render_target,
			img_factory,
			L"assets/RainbowExplosion.png",
			1031,
			338,
			&title_bitmap
		));
	}

	void changeBrushColor(D2D1_COLOR_F color) {
		customizable_brush->SetColor(color);
	}

	HRESULT updateRadialBrush() {
		static int ticks = -1;
		if (ticks == global_state.tick) {
			return S_OK;
		}
		ticks = global_state.tick;
		ID2D1HwndRenderTarget* render_target = global_state.render_target;

		for (int i = 0; i < RAD_BRUSH_STOPS; i++) {
			rad_brush_data.stops_data[i].position = FLOAT(i) / (RAD_BRUSH_STOPS - 1);
			rad_brush_data.stops_data[i].color = HSVToRGB(std::fmod(ticks * RAD_BRUSH_OFFSET_PER_TICK - i * RAD_BRUSH_DIST * RAD_BRUSH_CYCLES, 1.0f), 1, 1);
		}

		hr(makeRadBrush<RAD_BRUSH_STOPS>(
			rad_brush_data, render_target,
			{0, 0}, RAD_BRUSH_RADIUS, RAD_BRUSH_RADIUS
		));

		rad_brush = rad_brush_data.brush;
	}
	
	void initLinearBrush() {
		ID2D1HwndRenderTarget* render_target = global_state.render_target;

		for (int i = 0; i < LIN_BRUSH_STOPS; i++) {
			lin_brush_data.stops_data[i].position = FLOAT(i) / (LIN_BRUSH_STOPS - 1);
			lin_brush_data.stops_data[i].color = HSVToRGB(std::fmod(i * LIN_BRUSH_DIST * LIN_BRUSH_CYCLES, 1.0f), 1, 1);
			lin_brush_data.stops_data[i].color.a = 0.01f;
		}
	}

	HRESULT updateLinearBrush() {
		static int upgrades = 0;
		const int max_upgrades = 75;
		ID2D1HwndRenderTarget* render_target = global_state.render_target;

		upgrades++;
		double completion = double(upgrades) / max_upgrades;

		for (int i = 0; i < LIN_BRUSH_STOPS; i++) {
			lin_brush_data.stops_data[i].color.a = completion;
		}

		hr(makeLinBrush<LIN_BRUSH_STOPS>(
			lin_brush_data, render_target,
			Vector2D(0, 450), Vector2D(1600, 450)
			));

		lin_brush = lin_brush_data.brush;
	}

	std::wstring floatToWstring(float x) {
		std::wstring f = std::to_wstring(x);
		bool has_dot = f.find('.') != f.npos;
		if (!has_dot) {
			return f;
		}
		while (f[f.size() - 1] == '0') {
			f.resize(f.size() - 1);
		}
		if (f[f.size() - 1] == '.') {
			f.resize(f.size() - 1);
		}
		return f;
	}

	void drawText(const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush, IDWriteTextFormat* format) {
		float font_size = format->GetFontSize();
		D2D1_RECT_F centered_rect = {
			.left = rect.left,
			.top = (rect.top + rect.bottom - font_size) / 2,
			.right = rect.right,
			.bottom = (rect.top + rect.bottom + font_size) / 2
		};
		global_state.render_target->DrawText(
			text,
			std::wcslen(text),
			format,
			centered_rect,
			brush
		);
	}

	void drawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F rect) {
		ID2D1HwndRenderTarget* render_target = global_state.render_target;

		render_target->DrawBitmap(
			bitmap,
			&rect,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			nullptr
		);
	}

	void drawTitleGeometry() {
		ID2D1HwndRenderTarget* render_target = global_state.render_target;
		changeBrushColor(color_clear);
		render_target->FillGeometry(title_group, customizable_brush);
	}

	void EllipseDrawable::draw(Vector2D pos, Vector2D dimensions) {
		global_state.render_target->DrawEllipse({{pos.x, pos.y}, dimensions.x, dimensions.y}, black_brush);
	}

	void RectDrawable::draw(Vector2D pos, Vector2D dimensions) {
		global_state.render_target->DrawRectangle({pos.x - dimensions.x, pos.y - dimensions.y, pos.x + dimensions.x, pos.y + dimensions.y}, black_brush);
	}
}
