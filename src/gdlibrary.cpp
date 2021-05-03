#include "Player.h"
#include "Mob.h"
#include "Main.h"
#include "HUD.h"

//#include <Engine.hpp>
#include <GDNative.hpp>

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
  godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
  godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
  godot::Godot::nativescript_init(handle);

  godot::register_class<godot::Player>();
  godot::register_class<godot::Mob>();
  godot::register_class<godot::Main>();
  godot::register_class<godot::HUD>();
}