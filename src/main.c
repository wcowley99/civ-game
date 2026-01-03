#include "render.h"
#include "world.h"

#include "raylib.h"

#include <math.h>
#include <stdio.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

Camera2D g_Camera = {0};

void init() {
  renderer_init(SCREEN_WIDTH, SCREEN_HEIGHT);

  g_Camera.offset =
      (Vector2){.x = (float)SCREEN_WIDTH / 2, .y = (float)SCREEN_HEIGHT / 2};
  g_Camera.zoom = 1.0f;
}

void handle_inputs() {
  g_Camera.zoom =
      expf(logf(g_Camera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
}

DrawInfo build_draw_info() {
  Vector2 mouse_pos = GetMousePosition();
  Vector2 world_pos = GetScreenToWorld2D(mouse_pos, g_Camera);
  Coord focus;
  world_to_axial(world_pos, &focus);

  return (DrawInfo){g_Camera, focus};
}

int main(int argc, char *argv[]) {
  init();

  World world = world_new(8);
  printf("back in main!\n");

  while (!WindowShouldClose()) {
    handle_inputs();

    DrawInfo info = build_draw_info();
    renderer_draw(&world, info);
  }

  world_free(&world);
  renderer_shutdown();

  return 0;
}
