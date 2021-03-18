#include "Player.h"
#include <Input.hpp>
#include <AnimatedSprite.hpp>
#include <CollisionShape2D.hpp>

using namespace godot;

void Player::_register_methods() {
  register_method("_process", &Player::_process);
  register_method("_ready", &Player::_ready);
  register_method("_on_Player_body_entered", &Player::_on_Player_body_entered);

  register_property("_speed", &Player::set_speed, &Player::get_speed, _defaultSpeed);

  register_signal<Player>("hit", {});
}

Player::Player() {}

Player::~Player() {}

void Player::_on_Player_body_entered(Node *body) {
  hide();
  emit_signal("hit");
  get_node<CollisionShape2D>("CollisionShape2D")->set_deferred("disabled", true);
}

void Player::start(Vector2 pos) {
  set_position(pos);
  show();
  get_node<CollisionShape2D>("CollisionShape2D")->set_disabled(false);
}

void Player::_init() {
  //_speed = 0;
}

void Player::_ready() {
  _screenSize = get_viewport_rect().get_size();
  hide();
}

void Player::_process(float delta) {
  auto input = Input::get_singleton();
  auto velocity = Vector2();
  if (input->is_action_pressed("ui_right")) {
    velocity.x += 1;
  }
  if (input->is_action_pressed("ui_left")) {
    velocity.x -= 1;
  }
  if (input->is_action_pressed("ui_down")) {
    velocity.y += 1;
  }
  if (input->is_action_pressed("ui_up")) {
    velocity.y -= 1;
  }
  auto sprite = this->get_node<AnimatedSprite>("AnimatedSprite");
  if (velocity.length() > 0) {
    velocity = velocity.normalized() * (float)_speed;
    sprite->play();
  } else {
    sprite->stop();
  }

  auto pos = get_position();
  pos += velocity * delta;
  pos.x = Math::clamp(pos.x, 0.f, _screenSize.x);
  pos.y = Math::clamp(pos.y, 0.f, _screenSize.y);
  this->set_position(pos);

  if (velocity.x) {
    sprite->set_animation("walk");
    sprite->set_flip_v(false);
    sprite->set_flip_h(velocity.x < 0);
  } else if (velocity.y) {
    sprite->set_animation("up");
    sprite->set_flip_v(velocity.y > 0);
  }
}