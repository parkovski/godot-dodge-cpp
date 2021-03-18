#include <Label.hpp>
#include <Timer.hpp>
#include <SceneTree.hpp>
#include <SceneTreeTimer.hpp>
#include <Button.hpp>

#include "HUD.h"

using namespace godot;

void HUD::_register_methods() {
  register_method("_on_StartButton_pressed", &HUD::_on_StartButton_pressed);
  register_method("_on_MessageTimer_timeout", &HUD::_on_MessageTimer_timeout);
  register_method("show_game_over", &HUD::show_game_over);
  register_method("show_game_over1", &HUD::show_game_over1);
  register_method("show_game_over2", &HUD::show_game_over2);

  register_signal<HUD>("start_game", {});
}

HUD::HUD() {}
HUD::~HUD() {}

void HUD::_on_StartButton_pressed() {
  get_node<Button>("StartButton")->hide();
  emit_signal("start_game");
}

void HUD::_on_MessageTimer_timeout() {
  get_node<Label>("Message")->hide();
}

void HUD::_init() {}

void HUD::show_message(String text) {
  auto message = get_node<Label>("Message");
  message->set_text(text);
  message->show();
  get_node<Timer>("MessageTimer")->start();
}

void HUD::show_game_over() {
  show_message("Game Over");
  auto timer = get_node<Timer>("MessageTimer");
  timer->connect("timeout", this, "show_game_over1",
                {}, CONNECT_ONESHOT);
  timer->start();
}

void HUD::show_game_over1() {
  auto message = get_node<Label>("Message");
  message->set_text("Dodge the Creeps!");
  message->show();
  get_tree()->create_timer(1)->connect("timeout", this, "show_game_over2", {},
                                       CONNECT_ONESHOT);
}

void HUD::show_game_over2() {
  get_node<Button>("StartButton")->show();
}

void HUD::update_score(int score) {
  get_node<Label>("ScoreLabel")->set_text(String::num_int64(score));
}
