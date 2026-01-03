#pragma once

#include "raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum Biome {
  Plains,
  Grasslands,
  Arctic,
  Coastal,
  Ocean,
} Biome;

typedef enum UnitType {
  WARRIOR,
  ARCHER,
} UnitType;

typedef struct Coord {
  int q;
  int r;
} Coord;

typedef struct Tile {
  Coord coord;
  Biome biome;
} Tile;

typedef struct Unit {
  Coord pos;
  UnitType type;
  int health;
} Unit;

void axial_to_world(Coord c, Vector2 *pos);

void world_to_axial(Vector2 pos, Coord *r);

bool coord_eq(Coord c1, Coord c2);

typedef struct World {
  Tile *tiles;
  int num_tiles;
  int num_rings;
  Unit *units;
  int num_units;
} World;

World world_new(int num_rings);
void world_free(World *world);

void world_draw(World *world, Coord focus);
