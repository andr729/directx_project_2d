#include "game_scene.hpp"
#include "global_state.hpp"
#include "entity_utils.hpp"
#include "random.hpp"
#include <numbers>


void GameScene::newLevel() {
	global_state.handler.clear();

	state_switch_tick = global_state.tick;
	state = GameSceneState::WaitToExplode;

	earned_money = 0;
	next_value = 1;

	setUpWalls(top_left_simulation, bottom_right_simulation, 30);

	auto circ_count = global_state.game_state.upgrades.at(L"Number of circles");
	auto rect_count = global_state.game_state.upgrades.at(L"Number of squares");
	
	auto circ_speed = global_state.game_state.upgrades.at(L"Circle speed");
	auto rect_speed = global_state.game_state.upgrades.at(L"Square speed");
	multiplier = global_state.game_state.upgrades.at(L"Multiplier");
	
	auto explosion_radius = global_state.game_state.upgrades.at(L"Explosion radius");
	this->explosion_speed = explosion_radius / explosion_life_time + 1;
	
	this->number_of_bullets = global_state.game_state.upgrades.at(L"Number of bullets");
	this->bullet_duration = global_state.game_state.upgrades.at(L"Bullet duration");

	for (int i = 0; i < circ_count; i++) {
		addCircObject(
			RD::randVector(top_left_simulation + Vector2D{80, 80}, bottom_right_simulation - Vector2D{90, 90}),
			randomVelocity(circ_speed)
		);
	}

	for (int i = 0; i < rect_count; i++) {
		addRectObject(
			RD::randVector(top_left_simulation + Vector2D{80, 80}, bottom_right_simulation - Vector2D{80, 80}),
			randomVelocity(rect_speed)
		);
	}

}

void GameScene::update() {
	if (state == GameSceneState::Exploding and global_state.handler.noExplosions()) {
		state_switch_tick = global_state.tick;
		state = GameSceneState::LastExplosionGone;
	}
	else if (state == GameSceneState::LastExplosionGone) {
		if (global_state.tick > (state_switch_tick + FPS * 2))
			switchToShop();
	}
};

void GameScene::draw() {
	update();

	DT::drawText((std::wstring(L"+") + std::to_wstring(size_t(earned_money)) + L"$").c_str(), {10, 10, 500, 100}, DT::black_brush);
	DT::drawText(
		(std::to_wstring(int64_t(global_state.game_state.money)) + L"$").c_str(),
		{75,  75 + DT::TEXT_FONT_STROKE, 400, 75 + 2 * DT::TEXT_FONT_STROKE},
		DT::black_brush);


	global_state.render_target->FillRectangle({ top_left_simulation.x,top_left_simulation.y,bottom_right_simulation.x,bottom_right_simulation.y}, DT::light_gray_brush);
	global_state.handler.drawAll();
}


void GameScene::explode(Vector2D position, D2D1_COLOR_F color) {
	global_state.handler.explodeCirc(position, color);
}


void GameScene::onClick() {
	// @TODO: check if is in game
	if (state == GameSceneState::WaitToExplode) {
		state_switch_tick = global_state.tick;
		state = GameSceneState::Exploding;
		
		explode(global_state.mouse_position, DT::randomColor(0.75f));
		earned_money = 0;
		next_value = 1;
	}
}

void GameScene::switchToShop() {
	global_state.handler.clear();
	global_state.scene = Scene::ShopScene;

	global_state.game_state.money += earned_money;
}


void GameScene::explosionWas() {
	earned_money += next_value;
	next_value *= multiplier;
} 