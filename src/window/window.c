#include "window.h"
#include "raylib.h"

void draw_field(int field_offset_x, int field_offset_y) {
  DrawRectangle(64, 64, GRID_SIZE * FIELD_HORIZONTAL_UNITS,
                GRID_SIZE * FIELD_VERTICAL_UNITS, RED);
  for (int x = 0; x < FIELD_HORIZONTAL_UNITS; x++) {
    for (int y = 0; y < FIELD_VERTICAL_UNITS; y++) {
      DrawRectangleLines(field_offset_x + x * GRID_SIZE,
                         field_offset_y + y * GRID_SIZE, GRID_SIZE, GRID_SIZE,
                         BLACK);
    }
  }
}

void make_window() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Colored blocks");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_field(64, 64);
    EndDrawing();
  }

  CloseWindow();
}