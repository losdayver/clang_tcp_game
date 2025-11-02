#include "window.h"
#include "client.h"
#include "raylib.h"
#include <stdio.h>

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

void start_window(struct Client *client) {
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Colored blocks");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_field(64, 64);
    if (client->state == CLIENT_STATE_REGISTERED) {
      DrawText("connected", 200, 80, 20, BLACK);
    }
    EndDrawing();
    Client_pop(client);
  }

  CloseWindow();
}