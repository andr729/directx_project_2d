#include <d2d1_3.h>
#include <dwrite_3.h>

// DrawTools
namespace DT {
	extern ID2D1SolidColorBrush* black_brush;
	extern ID2D1SolidColorBrush* dark_gray_brush;
	extern ID2D1SolidColorBrush* light_gray_brush;
	extern ID2D1SolidColorBrush* customizable_brush;
	extern ID2D1RadialGradientBrush* rad_brush;

	const float FONT_STROKE = 150.0f;

	HRESULT initTools();

	HRESULT recreateTools(ID2D1HwndRenderTarget* render_target);

	void changeBrushColor(D2D1_COLOR_F color);

	void drawText(ID2D1HwndRenderTarget* render_target, const WCHAR* text, D2D1_RECT_F rect, ID2D1Brush* brush);

	void destroyTools();
}
