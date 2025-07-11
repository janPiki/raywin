#include <GLFW/glfw3.h>
#include <raylib.h>
#include <rlgl.h>
#include <stdbool.h>

typedef enum {
  EXWIN_FLAG_FULLSCREEN = 1 << 0,
  EXWIN_FLAG_RESIZEABLE = 1 << 1,
  EXWIN_FLAG_UNDECORATED = 1 << 2,
  EXWIN_FLAG_TRANSPARENT = 1 << 3,
  EXWIN_FLAG_ALWAYS_ON_TOP = 1 << 4
} ExWindowFlag;

#define KEY_AMOUNT 256

typedef struct {
  void *handle;
  int width, height;
  char *title;
  bool valid;

  bool currentKeys[KEY_AMOUNT];
  bool prevKeys[KEY_AMOUNT];

  bool currentMouseButton[GLFW_MOUSE_BUTTON_LAST + 1];
  bool prevMouseButton[GLFW_MOUSE_BUTTON_LAST + 1];

  bool firstMouseUpdate;
  Vector2 lastMouse;
  Vector2 mouseDelta;

  ExWindowFlag flags;
  
  rlRenderBatch renderBatch;
  Shader defaultShader;
} ExWindow;

// General window functions
void InitRaywin();
ExWindow CreateExtraWindow(int width, int height, char *title);
void UpdateExtraWindows();
void SetupExtraWindow(); // does not do what you think it does, but I couldn't find a better name
void BeginDrawingOn(ExWindow *window);
void EndDrawingOn(ExWindow *window);
bool ExtraWindowShouldClose(ExWindow *window);
void CloseExtraWindow(ExWindow *window);
void DeinitRaywin();

// Window Inputs
void UpdateWindowInput(ExWindow *window);
bool IsKeyDownOn(ExWindow *window, int key);
bool IsKeyPressedOn(ExWindow *window, int key);
bool IsKeyReleasedOn(ExWindow *window, int key);
Vector2 GetMousePositionOn(ExWindow *window);
bool IsMouseButtonDownOn(ExWindow *window, int button);
bool IsMouseButtonPressedOn(ExWindow *window, int button);
bool IsMouseButtonReleasedOn(ExWindow *window, int button);
Vector2 GetMouseDeltaOn(ExWindow *window);
void EnableCursorOn(ExWindow *window);
void DisableCursorOn(ExWindow *window);

// Window editing
void SetExtraWindowPosition(ExWindow *window, Vector2 pos);
void SetExtraWindowSize(ExWindow *window, Vector2 size);
void SetExtraWindowTitle(ExWindow *window, char *title);
void SetExtraWindowFlag(ExWindow *window, ExWindowFlag flag);
void ExWindowHint(
    ExWindowFlag flag); // Call before creating a window
                        // (more reliable than SetExtraWindowFlag())
void ClearExtraWindowFlag(ExWindow *window);
void MaximizeExtraWindow(ExWindow *window);
void MinimizeExtraWindow(ExWindow *window);
void RestoreExtraWindow(ExWindow *window);
void SetExtraWindowOpacity(ExWindow *window, float opacity);

// Window info
Vector2 GetExtraWindowPos(ExWindow *window);
Vector2 GetExtraWindowSize(ExWindow *window);
bool IsExtraWindowFocused(ExWindow *window);
bool IsExtraWindowMinimized(ExWindow *window);
float GetExtraWindowOpacity(ExWindow *window);
Vector2 GetExtraWindowScaleDPI(ExWindow *window);
