#include "global_state.hpp"

GlobalState global_state;

void GlobalState::onMouseMove(FLOAT x, FLOAT y) {
	internal_mouse_position = { x, y };
	// TODO: Transform internal_mouse_position by current transform and get mouse_position this way.
	mouse_position = { x, y };
}
