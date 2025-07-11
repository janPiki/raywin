#include "raywin.h"
#include <raylib.h>

Color win3Col = ORANGE;

int main() {
  InitWindow(600, 600, "Main Window");
  SetTargetFPS(60);

  InitRaywin();
  ExWindow win2 = CreateExtraWindow(300, 300, "Second Window");
  ExWindow win3 = CreateExtraWindow(150, 150, "Third Window");

  Vector2 win3Pos = {683, 384};
  double win3Speed = 5;
  Vector2 win3Vel = {win3Speed, win3Speed};
  SetExtraWindowPosition(&win3, win3Pos);

  Texture2D Logo = LoadTexture("RaylibLogoBadlyDrawn.png");

  Rectangle r = {100, 100, 50, 50};

  while (!WindowShouldClose() && !ExtraWindowShouldClose(&win2) &&
         !ExtraWindowShouldClose(&win3)) {
    UpdateExtraWindows();
    UpdateWindowInput(&win2);

    if (IsKeyDownOn(&win2, KEY_SPACE)) {
      r.y -= 25;
    }
    r.y += 1;

    win3Pos.x += win3Vel.x;
    if (win3Pos.x >= 1366 - 150 || win3Pos.x <= 0) {
      win3Vel.x *= -1;
      if (win3Col.b == ORANGE.b) {
        win3Col = VIOLET;
      } else {
        win3Col = ORANGE;
      }
    }
    win3Pos.y += win3Vel.y;
    if (win3Pos.y >= 768 - 150 || win3Pos.y <= 0) {
      win3Vel.y *= -1;
      if (win3Col.b == ORANGE.b) {
        win3Col = VIOLET;
      } else {
        win3Col = ORANGE;
      }
    }
    SetExtraWindowPosition(&win3, win3Pos);

    BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawRectangle(0, 0, 50, 50, DARKBLUE);
    EndDrawing();
    
    SetupExtraWindow();

    BeginDrawingOn(&win2);
    ClearBackground(LIME);
    DrawRectangle(r.x, r.y, r.width, r.height, RED);
    EndDrawingOn(&win2);

    BeginDrawingOn(&win3);
    ClearBackground(win3Col);
    DrawTexture(Logo, 0, 0, WHITE);
    EndDrawingOn(&win3);
  }
  UnloadTexture(Logo);

  CloseExtraWindow(&win3);
  CloseExtraWindow(&win2);
  CloseWindow();
}
