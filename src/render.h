#pragma once

#include "world.h"

#include "raylib.h"

typedef struct DrawInfo {
  Camera2D main_camera;
  Coord focus;
} DrawInfo;

void renderer_init(int width, int height);
void renderer_shutdown();

void renderer_draw(World *world, DrawInfo info);
void renderer_draw_world(World *world, Coord focus);
