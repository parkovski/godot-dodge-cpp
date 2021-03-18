#pragma once

#include <Godot.hpp>
#include <Area2D.hpp>

namespace godot {

class Player : public Area2D {
  GODOT_CLASS(Player, Area2D)

  static const int _defaultSpeed = 450;
  int _speed = _defaultSpeed;
  Vector2 _screenSize;

public:
  static void _register_methods();

  Player();
  ~Player();

  void _on_Player_body_entered(Node *body);

  void set_speed(int v) { _speed = v; }
  int get_speed() const { return _speed; }

  void start(Vector2 pos);

  void _init();
  void _ready();
  void _process(float delta);
};

}
