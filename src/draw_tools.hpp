#pragma once

#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <string>
#include "base.hpp"

// DrawTools
namespace DT {
	extern ID2D1SolidColorBrush* black_brush;
	extern ID2D1SolidColorBrush* dark_gray_brush;
	extern ID2D1SolidColorBrush* light_gray_brush;
	extern ID2D1SolidColorBrush* white_brush;
	extern ID2D1SolidColorBrush* customizable_brush;
	extern ID2D1RadialGradientBrush* rad_brush;
	extern ID2D1LinearGradientBrush* lin_brush;

	extern D2D1_COLOR_F color_clear;

	extern IDWriteTextFormat* button_text_format;
	extern IDWriteTextFormat* normal_text_format;
	extern IDWriteTextFormat* shop_text_format;

	extern ID2D1Bitmap* title_bitmap;

	constexpr float BUTTON_FONT_STROKE = 75.0f;
	constexpr float TEXT_FONT_STROKE = 40.0f;
	constexpr float SHOP_FONT_STROKE = 150.0f;

	HRESULT initTools(ID2D1Factory7* factory);

	D2D1_COLOR_F randomColor();

	void destroyTools();

	HRESULT recreateTools();

	void changeBrushColor(D2D1_COLOR_F color);

	HRESULT updateRadialBrush();

	void initLinearBrush();
	HRESULT updateLinearBrush();

	std::wstring floatToWstring(float x);

	void drawText(const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush, IDWriteTextFormat* format = normal_text_format);

	void drawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F rect);

	void drawTitleGeometry();

	struct Drawable {
		virtual void draw(Vector2D pos, Vector2D dimensions, D2D1_COLOR_F color) = 0;
		virtual ~Drawable() = default;
	};
	
	struct EllipseDrawable: public Drawable {
		void draw(Vector2D pos, Vector2D dimensions, D2D1_COLOR_F color) final;
		virtual ~EllipseDrawable() = default;
	};

	struct RectDrawable: public Drawable {
		void draw(Vector2D pos, Vector2D dimensions, D2D1_COLOR_F color) final;
		virtual ~RectDrawable() = default;
	};

	extern RectDrawable rect_drawable;
	extern EllipseDrawable ellipse_drawable;

}
