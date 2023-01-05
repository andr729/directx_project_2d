#include "draw_tools.hpp"
#include "base.hpp"
#include "rad_brush.hpp"
#include "global_state.hpp"

namespace DT {
	IDWriteFactory* write_factory = nullptr;
	IDWriteTextFormat* button_text_format = nullptr;
	IDWriteTextFormat* normal_text_format = nullptr;

	ID2D1SolidColorBrush* black_brush = nullptr;
	ID2D1SolidColorBrush* dark_gray_brush = nullptr;
	ID2D1SolidColorBrush* light_gray_brush = nullptr;
	ID2D1SolidColorBrush* white_brush = nullptr;
	ID2D1SolidColorBrush* customizable_brush = nullptr;
	ID2D1RadialGradientBrush* rad_brush = nullptr;
	
	constexpr int RAD_BRUSH_STOPS = 100;
	constexpr float RAD_BRUSH_DIST = 1.0f / (RAD_BRUSH_STOPS - 1);
	constexpr float RAD_BRUSH_CYCLES = 3.0f;
	constexpr int RAD_BRUSH_RADIUS = 500;
	constexpr double RAD_BRUSH_OFFSET_PER_TICK = 0.01;

	RadBrushState<RAD_BRUSH_STOPS> rad_brush_data;

	constexpr D2D1_COLOR_F color_black = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.0f };
	constexpr D2D1_COLOR_F color_dark_gray = { .r = 0.4f, .g = 0.4f, .b = 0.4f, .a = 1.0f };
	constexpr D2D1_COLOR_F color_light_gray = { .r = 0.625f, .g = 0.625f, .b = 0.625f, .a = 1.0f };
	constexpr D2D1_COLOR_F color_white = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.0f };

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

	HRESULT initTools() {
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
	}

	HRESULT recreateTools() {
		ID2D1HwndRenderTarget* render_target = global_state.render_target;
		hr(render_target->CreateSolidColorBrush(color_black, &black_brush));
		hr(render_target->CreateSolidColorBrush(color_dark_gray, &dark_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_light_gray, &light_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_white, &white_brush));
		hr(render_target->CreateSolidColorBrush(color_black, &customizable_brush));
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

	void EllipseDrawable::draw(Vector2D pos, Vector2D dimensions) {
		global_state.render_target->DrawEllipse({{pos.x, pos.y}, dimensions.x, dimensions.y}, black_brush);
	}

	void RectDrawable::draw(Vector2D pos, Vector2D dimensions) {
		global_state.render_target->DrawRectangle({pos.x - dimensions.x, pos.y - dimensions.y, pos.x + dimensions.x, pos.y + dimensions.y}, black_brush);
	}
}