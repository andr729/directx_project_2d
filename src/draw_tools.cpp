#include "draw_tools.hpp"
#include "base.hpp"
#include "rad_brush.hpp"

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

	HRESULT recreateTools(ID2D1HwndRenderTarget* render_target) {
		hr(render_target->CreateSolidColorBrush(color_black, &black_brush));
		hr(render_target->CreateSolidColorBrush(color_dark_gray, &dark_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_light_gray, &light_gray_brush));
		hr(render_target->CreateSolidColorBrush(color_black, &customizable_brush));
	}

	void changeBrushColor(D2D1_COLOR_F color) {
		customizable_brush->SetColor(color);
	}

	void updateRadialBrush(ID2D1HwndRenderTarget* render_target) {
		static int ticks = 0;
		ticks++;



		makeRadBrush<RAD_BRUSH_STOPS>(
				rad_brush_data, render_target,
				{0, 0}, RAD_BRUSH_RADIUS, RAD_BRUSH_RADIUS
			);
	}

	void drawText(ID2D1HwndRenderTarget* render_target, const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush) {
		render_target->DrawText(
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
		rt->DrawRectangle({pos.x, pos.y, pos.x + dimensions.x, pos.y + dimensions.y}, black_brush);
	}
}