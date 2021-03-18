#include <Timer.hpp>
#include <Position2D.hpp>
#include <Path2D.hpp>
#include <PathFollow2D.hpp>
#include <Curve2D.hpp>
#include <SceneTree.hpp>
#include <cmath>

#include "Player.h"
#include "Mob.h"
#include "HUD.h"

#include "Main.h"

using namespace godot;

void Main::_register_methods() {
  register_method("game_over", &Main::game_over);
  register_method("new_game", &Main::new_game);
  register_method("_on_StartTimer_timeout", &Main::_on_StartTimer_timeout);
  register_method("_on_ScoreTimer_timeout", &Main::_on_ScoreTimer_timeout);
  register_method("_on_MobTimer_timeout", &Main::_on_MobTimer_timeout);

  register_property<Main>("Mob", &Main::set_mob, &Main::get_mob, {});
}

Main::Main() {}
Main::~Main() {}

void Main::_on_StartTimer_timeout() {
  get_node<Timer>("ScoreTimer")->start();
  get_node<Timer>("MobTimer")->start();
}

void Main::_on_ScoreTimer_timeout() {
  ++_score;
  get_node<HUD>("HUD")->update_score(_score);
}

const float PI = 3.14159f;
void Main::_on_MobTimer_timeout() {
  auto mob_path = get_node<Path2D>("MobPath");
  auto mob_spawn_location = get_node<PathFollow2D>("MobPath/MobSpawnLocation");
  mob_spawn_location->set_offset(
    fmod((real_t)rand(), mob_path->get_curve()->get_baked_length())
  );
  auto mob_node = _mob->instance();
  auto mob = Object::cast_to<Mob>(mob_node);
  add_child(mob_node);
  mob->set_position(mob_spawn_location->get_position());
  auto direction = mob_spawn_location->get_rotation() + PI / 2;
  direction += (float)rand() / (float)RAND_MAX * PI / 2 - PI / 4;
  mob->set_rotation(direction);
  mob->set_linear_velocity(
    Vector2(
      (float)rand() / (float)RAND_MAX *
        (float)(mob->get_max_speed() - mob->get_min_speed()) +
        (float)mob->get_min_speed(),
      0)
    .rotated(direction)
  );
}

void Main::game_over() {
  get_node<Timer>("ScoreTimer")->stop();
  get_node<Timer>("MobTimer")->stop();
  get_node<HUD>("HUD")->show_game_over();
  get_tree()->call_group("mobs", "queue_free");
}

void Main::new_game() {
  _score = 0;
  get_node<Player>("Player")->start(get_node<Position2D>("StartPosition")->get_position());
  get_node<Timer>("StartTimer")->start();
  auto hud = get_node<HUD>("HUD");
  hud->update_score(_score);
  hud->show_message("Get Ready");
}

void Main::_init() {}
