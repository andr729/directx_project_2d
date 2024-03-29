#include "global_state.hpp"

GlobalState global_state;

void GlobalState::onMouseMove(FLOAT x, FLOAT y) {
	Vector2D internal_mouse_position = { x, y };

	if (render_target) {
		TransformationMatrix tm;
		render_target->GetTransform(&tm.getInner());
		mouse_position = internal_mouse_position * tm.inverted();
	}
}
