#include "raywin.h"
#include <GLFW/glfw3.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// General window functions
void InitRaywin() { glfwInit(); }

int nextWindowFlags = 0;

ExWindow CreateExtraWindow(int width, int height, char *title) {
  ExWindow win = {0};
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  win.handle = glfwCreateWindow(width, height, title, NULL, GetWindowHandle());

  if (!win.handle) {
    const char *desc;
    glfwGetError(&desc);
    TraceLog(LOG_ERROR, "GLFW Error: %s", desc ? desc : "Unknown");
    return (ExWindow){0};
  }

  glfwMakeContextCurrent(win.handle);
  rlglInit(width, height);
  rlLoadExtensions((void *)glfwGetProcAddress);

  glfwSwapInterval(0);

  int fbWidth, fbHeight;
  glfwGetFramebufferSize(win.handle, &fbWidth, &fbHeight);
  rlViewport(0, 0, fbWidth, fbHeight);

  win.width = width;
  win.height = height;
  win.title = strdup(title);
  win.valid = true;
  win.flags = nextWindowFlags;
  win.firstMouseUpdate = true;

  glfwMakeContextCurrent(GetWindowHandle());

  return win;
}

void UpdateExtraWindows() { glfwPollEvents(); }

bool ExtraWindowShouldClose(ExWindow *window) {
  return glfwWindowShouldClose(window->handle);
}

void BeginDrawingOn(ExWindow *window) {
  if (!window->valid)
    return;
  glfwMakeContextCurrent(window->handle);

  int fbWidth, fbHeight;
  glfwGetFramebufferSize(window->handle, &fbWidth, &fbHeight);
  rlViewport(0, 0, fbWidth, fbHeight);

  rlSetMatrixProjection(MatrixOrtho(0, fbWidth, fbHeight, 0, 0, 1));
  rlSetMatrixModelview(MatrixIdentity());

  // Removed to prevent lag
  //  BeginDrawing();
}

void EndDrawingOn(ExWindow *window) {
  if (!window->valid)
    return;

  // Removed to prevent lag
  //  EndDrawing();

  glfwSwapBuffers(window->handle);
  glfwMakeContextCurrent(GetWindowHandle());
}

void CloseExtraWindow(ExWindow *window) {
  if (!window->handle || !window->valid) {
    TraceLog(LOG_WARNING, "Tried to destroy a null or invalid window");
    return;
  }

  TraceLog(LOG_INFO, "Destroying extra window: handle = %p", window->handle);
  glfwDestroyWindow(window->handle);
  window->handle = NULL;

  if (window->title) {
    free(window->title);
    window->title = NULL;
  }
  window->valid = false;
}

void DeinitRaywin() { glfwTerminate(); }

// Input handling
void UpdateWindowInput(ExWindow *window) {
  if (!window->valid)
    return;

  memcpy(window->prevKeys, window->currentKeys, sizeof(window->currentKeys));

  for (int i = 0; i < KEY_AMOUNT; i++) {
    window->currentKeys[i] = (glfwGetKey(window->handle, i) == GLFW_PRESS);
  }

  memcpy(window->prevMouseButton, window->currentMouseButton,
         sizeof(window->currentMouseButton));

  for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
    window->currentMouseButton[i] =
        (glfwGetMouseButton(window->handle, i) == GLFW_PRESS);
  }

  double x, y;
  glfwGetCursorPos(window->handle, &x, &y);
  Vector2 current = {x, y};

  if (window->firstMouseUpdate) {
    window->lastMouse = current;
    window->mouseDelta = (Vector2){0, 0};
    window->firstMouseUpdate = false;
  } else {
    window->mouseDelta = Vector2Subtract(current, window->lastMouse);
    window->lastMouse = current;
  }
}

bool IsKeyDownOn(ExWindow *window, int key) {
  return window->valid && window->currentKeys[key];
}

bool IsKeyPressedOn(ExWindow *window, int key) {
  return window->valid && window->currentKeys[key] && !window->prevKeys[key];
}

bool IsKeyReleasedOn(ExWindow *window, int key) {
  return window->valid && !window->currentKeys[key] && window->prevKeys[key];
}

Vector2 GetMousePositionOn(ExWindow *window) {
  if (!window->valid)
    return (Vector2){0};
  double x, y;
  glfwGetCursorPos(window->handle, &x, &y);
  return (Vector2){x, y};
}

bool IsMouseButtonDownOn(ExWindow *window, int button) {
  return window->valid && window->currentMouseButton[button];
}

bool IsMouseButtonPressedOn(ExWindow *window, int button) {
  return window->valid && window->currentMouseButton[button] &&
         !window->prevMouseButton[button];
}

bool IsMouseButtonReleasedOn(ExWindow *window, int button) {
  return window->valid && !window->currentMouseButton[button] &&
         window->prevMouseButton[button];
}

Vector2 GetMouseDeltaOn(ExWindow *window) {
  return window->valid ? window->mouseDelta : (Vector2){0};
}

void EnableCursorOn(ExWindow *window) {
  if (window->valid)
    glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void DisableCursorOn(ExWindow *window) {
  if (window->valid)
    glfwSetInputMode(window->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Window editing
void SetExtraWindowPosition(ExWindow *window, Vector2 pos) {
  if (!window->valid)
    return;
  glfwSetWindowPos(window->handle, pos.x, pos.y);
}

void SetExtraWindowSize(ExWindow *window, Vector2 size) {
  if (!window->valid)
    return;
  if (size.x == window->width && size.y == window->height)
    return;
  glfwSetWindowSize(window->handle, size.x, size.y);
  window->width = size.x;
  window->height = size.y;
}

void SetExtraWindowTitle(ExWindow *window, char *title) {
  glfwSetWindowTitle(window->handle, title);
}

void SetExtraWindowFlag(ExWindow *window, ExWindowFlag flag) {
  if (!window->valid)
    return;

  window->flags |= flag;

  switch (flag) {
  case EXWIN_FLAG_FULLSCREEN: {
    const GLFWvidmode *vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(window->handle, glfwGetPrimaryMonitor(), 0, 0,
                         window->width, window->height, 60);
    break;
  }
  case EXWIN_FLAG_RESIZEABLE:
    glfwSetWindowAttrib(window->handle, GLFW_RESIZABLE, GLFW_TRUE);
    break;

  case EXWIN_FLAG_UNDECORATED:
    glfwSetWindowAttrib(window->handle, GLFW_DECORATED, GLFW_FALSE);
    break;

  case EXWIN_FLAG_TRANSPARENT:
    glfwSetWindowAttrib(window->handle, GLFW_TRANSPARENT_FRAMEBUFFER,
                        GLFW_TRUE);
    break;

  case EXWIN_FLAG_ALWAYS_ON_TOP:
    glfwSetWindowAttrib(window->handle, GLFW_FLOATING, GLFW_TRUE);
    break;
  }
}

void ExWindowHint(ExWindowFlag flag) {
  nextWindowFlags |= flag;

  switch (flag) {
  case EXWIN_FLAG_FULLSCREEN:
    // Does not work for fullscreen
    break;
  case EXWIN_FLAG_RESIZEABLE:
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    break;
  case EXWIN_FLAG_UNDECORATED:
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    break;
  case EXWIN_FLAG_TRANSPARENT:
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    break;
  case EXWIN_FLAG_ALWAYS_ON_TOP:
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  }
}

void ClearExtraWindowFlag(ExWindow *window) { window->flags = 0; }

void MaximizeExtraWindow(ExWindow *window) {
  glfwMaximizeWindow(window->handle);
}

void MinimizeExtraWindow(ExWindow *window) {
  glfwIconifyWindow(window->handle);
}

void RestoreExtraWindow(ExWindow *window) { glfwRestoreWindow(window->handle); }

void SetExtraWindowOpacity(ExWindow *window, float opacity) {
  glfwSetWindowOpacity(window->handle, opacity);
}

// Window info

Vector2 GetExtraWindowPos(ExWindow *window) {
  int x, y;
  glfwGetWindowPos(window->handle, &x, &y);
  return (Vector2){x, y};
}

Vector2 GetExtraWindowSize(ExWindow *window) {
  int x, y;
  glfwGetWindowSize(window->handle, &x, &y);
  return (Vector2){x, y};
}

bool IsExtraWindowFocused(ExWindow *window) {
  return glfwGetWindowAttrib(window->handle, GLFW_FOCUSED);
}

bool IsExtraWindowMinimized(ExWindow *window) {
  return glfwGetWindowAttrib(window->handle, GLFW_ICONIFIED);
}

float GetExtraWindowOpacity(ExWindow *window) {
  return glfwGetWindowOpacity(window->handle);
}

Vector2 GetExtraWindowScaleDPI(ExWindow *window) {
  float x, y;
  glfwGetWindowContentScale(window->handle, &x, &y);
  return (Vector2){x, y};
}
