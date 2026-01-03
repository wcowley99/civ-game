#include "world.h"

#include "utils.h"

#include "raylib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void axial_to_world(Coord c, Vector2 *pos) {
  pos->y = 16 * c.r - 16;
  pos->x = 32 * c.q + 16 * c.r - 16;
}

void world_to_axial(Vector2 pos, Coord *c) {
  float frac_q = (pos.x - pos.y - 4) / 32;
  float frac_r = (pos.y + 4) / 16;
  float frac_s = -frac_q - frac_r;

  int q1 = roundf(frac_q);
  int r1 = roundf(frac_r);
  int s1 = roundf(frac_s);

  float dq = fabsf(q1 - frac_q);
  float dr = fabsf(r1 - frac_r);
  float ds = fabsf(s1 - frac_s);

  if (dq > dr && ds > ds) {
    c->q = -r1 - s1;
    c->r = r1;
  } else if (dr > ds) {
    c->q = q1;
    c->r = -q1 - s1;
  } else {
    c->q = q1;
    c->r = r1;
  }
}

bool coord_eq(Coord c1, Coord c2) { return (c1.r == c2.r) && (c1.q == c2.q); }

World world_new(int num_rings) {
  int num_tiles = 1 + 3 * num_rings * (num_rings + 1);

  Tile *tiles = malloc(sizeof(Tile) * num_tiles);

  int n = num_rings;
  int i = 0;
  for (int q = -n; q <= n; q += 1) {
    int r_min = max(-n, -q - n);
    int r_max = min(n, -q + n);

    for (int r = r_min; r <= r_max; r += 1) {
      Coord c = {q, r};
      tiles[i] = (Tile){.coord = c, .biome = Plains};

      i += 1;
    }
  }

  printf("here!\n");
  Unit *units = malloc(sizeof(Unit) * 2);
  units[0] = (Unit){(Coord){-2, 0}, ARCHER, 100};
  units[1] = (Unit){(Coord){2, 0}, WARRIOR, 100};
  printf("here!\n");

  return (World){tiles, num_tiles, num_rings, units, 2};
}

void world_free(World *world) {
  free(world->tiles);
  free(world->units);

  world->tiles = NULL;
  world->num_tiles = 0;
  world->num_units = 0;
}
