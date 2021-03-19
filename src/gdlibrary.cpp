#include "Player.h"
#include "Mob.h"
#include "Main.h"
#include "HUD.h"

#include <Engine.hpp>

#include <exception>

#define DEBUGGER_TIMEOUT 5
#define LONG_WAIT_LIMIT 30

#ifndef NDEBUG
# include <cstdio>
# if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>

static void wait_for_debugger() {
  int count = DEBUGGER_TIMEOUT;
  bool has_debugger = false;
  bool long_wait = false;
  printf("Press any key to wait for debugger (%ds).", DEBUGGER_TIMEOUT);
  fflush(stdout);
  auto hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  auto hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwInMode, dwOutMode;
  GetConsoleMode(hStdIn, &dwInMode);
  GetConsoleMode(hStdOut, &dwOutMode);
  SetConsoleMode(hStdIn, ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS);
  SetConsoleMode(hStdOut, dwOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

  struct RestoreConsoleMode {
    HANDLE _handle;
    DWORD _mode;
    RestoreConsoleMode(HANDLE handle, DWORD mode) noexcept :
      _handle(handle), _mode(mode) {}
    ~RestoreConsoleMode() {
      SetConsoleMode(_handle, _mode);
    }
  };
  RestoreConsoleMode restoreOutputMode(hStdOut, dwOutMode);
  RestoreConsoleMode restoreInputMode(hStdIn, dwInMode);

  while (!(has_debugger = IsDebuggerPresent()) && count > 1) {
    if (WaitForSingleObject(hStdIn, 1000) == WAIT_OBJECT_0) {
      long_wait = true;
    }
    printf(".");
    fflush(stdout);
    --count;
  }

  if (long_wait) {
    count = LONG_WAIT_LIMIT;
    printf(" %d", count);
    fflush(stdout);
    while (!(has_debugger = IsDebuggerPresent()) && count > 1) {
      if (WaitForSingleObject(hStdIn, 1000) == WAIT_OBJECT_0) {
        break;
      }
      printf("\033[%dD%d", count >= 10 ? 2 : 1, count - 1);
      fflush(stdout);
      --count;
    }
    printf("\033[%dD", count >= 10 ? 2 : 1);
    fflush(stdout);
  }

  if (has_debugger) {
    printf(" Attached.\n");
  } else {
    printf(" Missing.\n");
  }
  fflush(stdout);
}

# else // _WIN32
static void wait_for_debugger() {
  printf("wait_for_debugger not implemented on this platform.\n");
}
# endif
#else // !NDEBUG
static void wait_for_debugger() {}
#endif

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
  godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
  godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
  godot::Godot::nativescript_init(handle);

  if (!godot::Engine::get_singleton()->is_editor_hint()) {
    printf("dodgethecreeps init/%s.\n",
#ifdef NDEBUG
           "optimized"
#else
           "debug"
#endif
          );
    wait_for_debugger();
  } else {
    printf("dodgethecreeps init/editor.\n");
  }

  godot::register_class<godot::Player>();
  godot::register_class<godot::Mob>();
  godot::register_class<godot::Main>();
  godot::register_class<godot::HUD>();
}