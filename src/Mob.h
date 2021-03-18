#pragma once

#include <Godot.hpp>
#include <RigidBody2D.hpp>

namespace godot {

class Mob : public RigidBody2D {
  GODOT_CLASS(Mob, RigidBody2D)

  static const int _default_min_speed = 150;
  static const int _default_max_speed = 250;
  int _min_speed = _default_min_speed;
  int _max_speed = _default_max_speed;

public:
  static void _register_methods();

  Mob();
  ~Mob();

  void _on_VisibilityNotifier2D_screen_exited();

  void set_min_speed(int v) { _min_speed = v; }
  int get_min_speed() const { return _min_speed; }
  void set_max_speed(int v) { _max_speed = v; }
  int get_max_speed() const { return _max_speed; }

  void _init();
  void _ready();
};

}
