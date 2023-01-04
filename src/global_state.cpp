#include "global_state.hpp"

GlobalState global_state;

void GlobalState::onMouseMove(FLOAT x, FLOAT y) {
	Vector2D internal_mouse_position = { x, y };
	// TODO: Transform internal_mouse_position by current transform and get mouse_position this way.
	if (render_target) {
		TransformationMatrix tm;
		render_target->GetTransform(&tm.getInner());
		auto itm = tm.inverted();
		mouse_position = internal_mouse_position * itm;
	}
}
