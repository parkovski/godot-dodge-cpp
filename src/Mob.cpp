#include "Mob.h"
#include <AnimatedSprite.hpp>
#include <SpriteFrames.hpp>
#include <cstdlib>

using namespace godot;

void Mob::_register_methods() {
  register_method("_ready", &Mob::_ready);
  register_method("_on_VisibilityNotifier2D_screen_exited", &Mob::_on_VisibilityNotifier2D_screen_exited);

  register_property("min_speed", &Mob::set_min_speed, &Mob::get_min_speed, _default_min_speed);
  register_property("max_speed", &Mob::set_max_speed, &Mob::get_max_speed, _default_max_speed);
}

Mob::Mob() {}
Mob::~Mob() {}

void Mob::_on_VisibilityNotifier2D_screen_exited() {
  queue_free();
}

void Mob::_init() {}

void Mob::_ready() {
  auto animated_sprite = get_node<AnimatedSprite>("AnimatedSprite");
  auto mob_types = animated_sprite
    ->get_sprite_frames()
    ->get_animation_names();
  auto index = rand() % mob_types.size();
  animated_sprite->set_animation(mob_types[index]);
}
