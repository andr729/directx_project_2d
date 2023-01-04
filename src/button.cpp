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
	if (state == ButtonState::Hovered) {
		TransformationMatrix old_transform, new_transform;
		render_target->GetTransform(&old_transform.getInner());
		Vector2D mouse_position = global_state.mouse_position;
		new_transform = old_transform * D2D1::Matrix3x2F::Translation(mouse_position.x, mouse_position.y);
		render_target->SetTransform(new_transform.getInner());
		D2D1_RECT_F offset_position = {
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
	
	if (text == L"+") {
		const float plus_height_ratio = 0.6;
		const float plus_width_ratio = 0.15;
		
		float center_x = (position.left + position.right) / 2;
		float center_y = (position.top + position.bottom) / 2;
		float plus_height = (position.top - position.bottom) * plus_height_ratio;
		float plus_width = plus_height * plus_width_ratio;
		D2D1_RECT_F plus_position = {
			.left = center_x - plus_width / 2,
			.top =  center_y - plus_height / 2,
			.right = center_x + plus_width / 2,
			.bottom = center_y + plus_height / 2
		};
		render_target->FillRectangle(plus_position, font_brush);
		plus_position = {
			.left = center_x - plus_height / 2,
			.top = center_y - plus_width / 2,
			.right = center_x + plus_height / 2,
			.bottom = center_y + plus_width / 2
		};
		render_target->FillRectangle(plus_position, font_brush);
	}
	else {
		DT::drawText(text.c_str(), position, font_brush, DT::button_text_format);
	}

}