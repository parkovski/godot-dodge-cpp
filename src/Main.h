#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <PackedScene.hpp>

namespace godot {

class Main : public Node {
  GODOT_CLASS(Main, Node)

  Ref<PackedScene> _mob_scene;
  int _score;

public:
  static void _register_methods();

  Main();
  ~Main();

  void _on_StartTimer_timeout();
  void _on_ScoreTimer_timeout();
  void _on_MobTimer_timeout();

  void set_mob(Ref<PackedScene> mob) { _mob_scene = mob; }
  Ref<PackedScene> get_mob() const { return _mob_scene; }

  void game_over();
  void new_game();

  void _init();
};

}
