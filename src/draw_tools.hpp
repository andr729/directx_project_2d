#include <d2d1_3.h>
#include <dwrite_3.h>
#include "base.hpp"

// DrawTools
namespace DT {
	extern ID2D1SolidColorBrush* black_brush;
	extern ID2D1SolidColorBrush* dark_gray_brush;
	extern ID2D1SolidColorBrush* light_gray_brush;
	extern ID2D1SolidColorBrush* customizable_brush;
	extern ID2D1RadialGradientBrush* rad_brush;

	const float FONT_STROKE = 150.0f;

	HRESULT initTools();

	void destroyTools();

	HRESULT recreateTools();

	void changeBrushColor(D2D1_COLOR_F color);

	void updateRadialBrush();

	void drawText(const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush);

	struct Drawable {
		virtual void draw(ID2D1RenderTarget* rt, Vector2D pos, Vector2D dimensions) = 0;
		virtual ~Drawable() = default;
	};
	
	struct EllipseDrawable: public Drawable {
		void draw(ID2D1RenderTarget* rt, Vector2D pos, Vector2D dimensions) final;
		virtual ~EllipseDrawable() = default;
	};

	struct RectDrawable: public Drawable {
		void draw(ID2D1RenderTarget* rt, Vector2D pos, Vector2D dimensions) final;
		virtual ~RectDrawable() = default;
	};


}