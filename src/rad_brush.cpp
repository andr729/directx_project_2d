#include "rad_brush.hpp"

namespace detail {
	void auxMakeRadBrush(ID2D1RadialGradientBrush** brush,
		ID2D1HwndRenderTarget* drt, D2D1_GRADIENT_STOP* stop_data,
		int num_rad_stops, ID2D1GradientStopCollection** stops,
		D2D1_POINT_2F center, D2D1_POINT_2F offset, FLOAT rx, FLOAT ry) {
		drt->CreateGradientStopCollection(
			stop_data, num_rad_stops, stops);

		if (stops) {
			drt->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(center, offset, rx, ry),
				*stops, brush);
		}
		else {
			exit(1);
		}
	}
}