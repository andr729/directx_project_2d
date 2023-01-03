#include "draw_tools.hpp"
#include "base.hpp"
#include "rad_brush.hpp"
#include "global_state.hpp"

namespace DT {
	IDWriteFactory* write_factory = nullptr;
	IDWriteTextFormat* button_text_format = nullptr;

	ID2D1SolidColorBrush* black_brush = nullptr;
	ID2D1SolidColorBrush* dark_gray_brush = nullptr;
	ID2D1SolidColorBrush* light_gray_brush = nullptr;
	ID2D1SolidColorBrush* customizable_brush = nullptr;
	ID2D1RadialGradientBrush* rad_brush = nullptr;
	
	const int RAD_BRUSH_STOPS = 5;
	const double RAD_BRUSH_DIST = 1.0f / (RAD_BRUSH_STOPS - 1);
	const int RAD_BRUSH_RADIUS = 500;
	const double RAD_BRUSH_OFFSET_PER_TICK = 0.01;


	RadBrushState<RAD_BRUSH_STOPS> rad_brush_data = {
						nullptr, nullptr,
				{
						{.position = 0 * RAD_BRUSH_DIST, .color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f) },
						{.position = 1 * RAD_BRUSH_DIST, .color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f) },
						{.position = 2 * RAD_BRUSH_DIST, .color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f) },
						{.position = 3 * RAD_BRUSH_DIST, .color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f) },
						{.position = 4 * RAD_BRUSH_DIST, .color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f) },
				}
	};

	D2D1_COLOR_F const color_black = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.0f };
	D2D1_COLOR_F const color_dark_gray = { .r = 0.4f, .g = 0.4f, .b = 0.4f, .a = 1.0f };
	D2D1_COLOR_F const color_light_gray = { .r = 0.625f, .g = 0.625f, .b = 0.625f, .a = 1.0f };

	// T¹ funckjê ChatGPT napisa³.
	D2D1_COLOR_F HSVToRGB(float h, float s, float v)
	{
		float r, g, b;

		int i = floor(h * 6);
		float f = h * 6 - i;
		float p = v * (1 - s);
		float q = v * (1 - f * s);
		float t = v * (1 - (1 - f) * s);

		switch (i % 6)
		{
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		return D2D1::ColorF(r, g, b);
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
			FONT_STROKE,
			L"en-us",
			&button_text_format
		));
		button_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	HRESULT recreateTools() {
		ID2D1HwndRenderTarget* render_target = global_state.render_target;
		hr(render_target->CreateSolidColorBrush(color_black, &black_brush));
		hr(render_target->CreateSolidColorBrush(color_dark_gray, &dark_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_light_gray, &light_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_black, &customizable_brush));
	}

	void changeBrushColor(D2D1_COLOR_F color) {
		customizable_brush->SetColor(color);
	}

	void updateRadialBrush() {
		static int ticks = 0;
		if (ticks == global_state.tick) {
			return;
		}
		ticks = global_state.tick;
		ID2D1HwndRenderTarget* render_target = global_state.render_target;

		for (int i = 0; i < RAD_BRUSH_STOPS; i++) {
			rad_brush_data.stops_data[i].color = HSVToRGB(std::fmod(ticks * RAD_BRUSH_OFFSET_PER_TICK + i * RAD_BRUSH_DIST, 1.0f), 1, 1);
		}

		makeRadBrush<RAD_BRUSH_STOPS>(
				rad_brush_data, render_target,
				{0, 0}, RAD_BRUSH_RADIUS, RAD_BRUSH_RADIUS
			);

		rad_brush = rad_brush_data.brush;
	}

	void drawText(const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush) {
		global_state.render_target->DrawText(
			text,
			sizeof(text)/sizeof(text[0]),
			button_text_format,
			rect,
			brush
		);
	}

	void EllipseDrawable::draw(ID2D1RenderTarget* rt, Vector2D pos, Vector2D dimensions) {
		rt->DrawEllipse({{pos.x, pos.y}, dimensions.x, dimensions.y}, black_brush);
	}

	void RectDrawable::draw(ID2D1RenderTarget* rt, Vector2D pos, Vector2D dimensions) {
		rt->DrawRectangle({pos.x - dimensions.x, pos.y - dimensions.y, pos.x + dimensions.x, pos.y + dimensions.y}, black_brush);
	}
}