#include <GLFW/glfw3.h>
#include <raylib.h>
#include <rlgl.h>
#include <stdbool.h>

typedef enum {
  EXWIN_FLAG_FULLSCREEN = 1 << 0,
  EXWIN_FLAG_RESIZEABLE = 1 << 1,
  EXWIN_FLAG_UNDECORATED = 1 << 2,
  EXWIN_FLAG_TRANSPARENT = 1 << 3,
  EXWIN_FALG_ALWAYS_ON_TOP = 1 << 4
} ExWindowFlag;

#define KEY_AMOUNT 256

typedef struct {
  void *handle;
  int width, height;
  char *title;
  bool valid;

  bool currentKeys[KEY_AMOUNT];
  bool prevKeys[KEY_AMOUNT];

  ExWindowFlag flags;
} ExWindow;

// General window functions
void InitRaywin();
ExWindow CreateExtraWindow(int width, int height, char *title);
void UpdateExtraWindows();
void BeginDrawingOn(ExWindow *window);
void EndDrawingOn(ExWindow *window);
bool ExtraWindowShouldClose(ExWindow *window);
void CloseExtraWindow(ExWindow *window);

// Window Inputs
void UpdateWindowInput(ExWindow *window);
bool IsKeyDownOn(ExWindow *window, int key);
bool IsKeyPressedOn(ExWindow *window, int key);
Vector2 GetMousePositionOn(ExWindow *window);

// Window editing
void SetExtraWindowPosition(ExWindow *window, Vector2 pos);
void SetExtraWindowSize(ExWindow *window, Vector2 size);
void SetExtraWindowFlag(ExWindow *window, ExWindowFlag flag);
void ExWindowHint(
    ExWindowFlag flag); // Call before creating a window
                        // (more reliable than SetExtraWindowFlag())
void ClearExtraWindowFlag(ExWindow *window);
