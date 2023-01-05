#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d2d1_3.h>
#include <array>
#include <tuple>
#include <vector>

template<UINT NUM_STOPS>
struct RadBrushState {
	ID2D1RadialGradientBrush* brush = nullptr;
	ID2D1GradientStopCollection* stops = nullptr;
	D2D1_GRADIENT_STOP stops_data[NUM_STOPS];
};


namespace detail {
	HRESULT auxMakeRadBrush(ID2D1RadialGradientBrush** brush,
		ID2D1HwndRenderTarget* drt, D2D1_GRADIENT_STOP* stop_data,
		int num_rad_stops, ID2D1GradientStopCollection** stops,
		D2D1_POINT_2F center, D2D1_POINT_2F offset, FLOAT rx, FLOAT ry);
}

template<UINT NUM_STOPS>
[[no_discard]]
HRESULT makeRadBrush(RadBrushState<NUM_STOPS>& state, ID2D1HwndRenderTarget* drt,
                     D2D1_POINT_2F center, FLOAT rx, FLOAT ry) {
	return detail::auxMakeRadBrush(
		&state.brush,
		drt, state.stops_data, NUM_STOPS, &state.stops,
		center, { 0, 0 },
		rx, ry
	);
}
