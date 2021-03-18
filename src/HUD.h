#pragma once

#include <Godot.hpp>
#include <CanvasLayer.hpp>

namespace godot {

class HUD : public CanvasLayer {
  GODOT_CLASS(HUD, CanvasLayer)

public:
  static void _register_methods();

  HUD();
  ~HUD();

  void _on_StartButton_pressed();
  void _on_MessageTimer_timeout();

  void _init();

  void show_message(String text);
  void show_game_over();
  void show_game_over1();
  void show_game_over2();
  void update_score(int score);
};

}
