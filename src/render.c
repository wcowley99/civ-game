#include "render.h"
#include "world.h"

#include <raylib.h>

Texture2D g_TileAtlas;

Texture2D g_VikingWarrior;
Texture2D g_VikingArcher;

void renderer_init(int width, int height) {
  InitWindow(width, height, "Civ Game");

  g_TileAtlas = LoadTexture("resources/sprite-sheet.png");

  g_VikingWarrior = LoadTexture("resources/viking-warrior.png");
  g_VikingArcher = LoadTexture("resources/viking-archer.png");
}

void renderer_shutdown() { UnloadTexture(g_TileAtlas); }

void renderer_draw(World *world, DrawInfo info) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  BeginMode2D(info.main_camera);

  renderer_draw_world(world, info.focus);

  EndMode2D();

  EndDrawing();
}

void renderer_draw_world(World *world, Coord focus) {
  Rectangle sprite_rec = {0.0, 0.0, 32.0, 32.0};
  for (int r = -world->num_rings; r <= world->num_rings; r += 1) {
    for (int i = 0; i < world->num_tiles; i += 1) {
      Tile *tile = &world->tiles[i];

      if (tile->coord.r != r)
        continue;

      Vector2 pos;
      axial_to_world(tile->coord, &pos);

      if (coord_eq(focus, tile->coord))
        pos.y -= 4;
      DrawTextureRec(g_TileAtlas, sprite_rec, pos, WHITE);
    }

    // for (int i = 0; i < world->num_units; i += 1) {
    //   Unit *unit = &world->units[i];
    //
    //   if (unit->pos.r != r)
    //     continue;
    //
    //   Vector2 pos;
    //   axial_to_world(unit->pos, &pos);
    //   pos.y -= 16;
    //
    //   if (coord_eq(focus, unit->pos))
    //     pos.y -= 4;
    //
    //   switch (unit->type) {
    //   case WARRIOR:
    //     DrawTexture(g_VikingWarrior, pos.x, pos.y, WHITE);
    //     break;
    //   case ARCHER:
    //     DrawTexture(g_VikingArcher, pos.x, pos.y, WHITE);
    //     break;
    //   }
    // }
  }
}
