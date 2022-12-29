#include "draw_tools.hpp"
#include "base.hpp"

namespace DT {
	IDWriteFactory* write_factory = nullptr;
	IDWriteTextFormat* text_format = nullptr;

	ID2D1SolidColorBrush* black_brush = nullptr;
	ID2D1SolidColorBrush* customizable_brush = nullptr;

	D2D1_COLOR_F const color_black = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.0f };

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
			150.0f,
			L"en-us",
			&text_format
		));

	}

	HRESULT recreateTools(ID2D1HwndRenderTarget* render_target) {
		hr(render_target->CreateSolidColorBrush(color_black, &black_brush));
		hr(render_target->CreateSolidColorBrush(color_black, &customizable_brush));
	}

	void drawText(ID2D1HwndRenderTarget* render_target, const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush) {
		render_target->DrawText(
			text,
			sizeof(text)/sizeof(text[0]),
			text_format,
			rect,
			brush
		);
	}

	void EllipseDrawable::draw(ID2D1RenderTarget* rt, Vector2D pos, Vector2D dimensions) {
		rt->DrawEllipse({{pos.x, pos.y}, dimensions.x, dimensions.y}, black_brush);
	}
}