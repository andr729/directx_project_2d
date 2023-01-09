#include "brush.hpp"
#include "base.hpp"

namespace detail {
	HRESULT auxMakeRadBrush(ID2D1RadialGradientBrush** brush,
	                        ID2D1HwndRenderTarget* drt, D2D1_GRADIENT_STOP* stop_data,
	                        int num_rad_stops, ID2D1GradientStopCollection** stops,
                            D2D1_POINT_2F center, D2D1_POINT_2F offset, FLOAT rx, FLOAT ry) {
		
		hr(drt->CreateGradientStopCollection(stop_data, num_rad_stops, stops));

		if (stops) {
			hr(drt->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(center, offset, rx, ry),
				*stops, brush));
		}
		else {
			return E_FAIL;
		}
	}

	HRESULT auxMakeLinBrush(ID2D1LinearGradientBrush** brush,
	                        ID2D1HwndRenderTarget* drt, D2D1_GRADIENT_STOP* stop_data,
	                        int num_rad_stops, ID2D1GradientStopCollection** stops,
                            Vector2D from, Vector2D to) {
		
		hr(drt->CreateGradientStopCollection(stop_data, num_rad_stops, stops));

		if (stops) {
			hr(drt->CreateLinearGradientBrush(
				D2D1::LinearGradientBrushProperties(from.toD2D1point(), to.toD2D1point()),
				*stops, brush));
		}
		else {
			return E_FAIL;
		}
	}
}