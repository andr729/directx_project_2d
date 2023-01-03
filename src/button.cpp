#include <d2d1_3.h>
#include "base.hpp"
#include "button.hpp"
#include "draw_tools.hpp"
#include "global_state.hpp"

void Button::draw() {
	ID2D1HwndRenderTarget* render_target = global_state.render_target;
	ID2D1Brush *font_brush, *bg_brush;

	switch (state) {
	case ButtonState::Unclickable:
		font_brush = DT::dark_gray_brush;
		bg_brush = DT::light_gray_brush;
		break;
	case ButtonState::Clickable:
		font_brush = DT::black_brush;
		DT::changeBrushColor(color);
		bg_brush = DT::customizable_brush;
		break;
	case ButtonState::Hovered:
		font_brush = DT::black_brush;
		bg_brush = DT::rad_brush;
	}

	D2D1_RECT_F offset_position;
	if (state == ButtonState::Hovered) {
		TransformationMatrix old_transform, new_transform;
		render_target->GetTransform(&old_transform.getInner());
		Vector2D mouse_position = global_state.mouse_position;
		new_transform = old_transform * D2D1::Matrix3x2F::Translation(mouse_position.x, mouse_position.y);
		render_target->SetTransform(new_transform.getInner());
		offset_position = {
			.left = mouse_position.x - position.left,
			.top = mouse_position.y - position.top,
			.right = position.right - mouse_position.x,
			.bottom = position.bottom - mouse_position.y
		};
		render_target->FillRectangle(offset_position, bg_brush);
		render_target->SetTransform(old_transform.getInner());
	}
	else {
		render_target->FillRectangle(position, bg_brush);
	}


	render_target->DrawRectangle(position, font_brush, 5);
	offset_position = {
		.left = position.left,
		.top = (position.top + position.bottom - DT::BUTTON_FONT_STROKE) / 2,
		.right = position.right,
		.bottom = (position.top + position.bottom + DT::BUTTON_FONT_STROKE) / 2
	};
	DT::drawText(text, offset_position, font_brush, DT::button_text_format);
}