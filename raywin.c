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

ExWindow CreateExtraWindow(int width, int height, char *title) {
  ExWindow win = {0};
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

  //  BeginDrawing();
}

void EndDrawingOn(ExWindow *window) {
  if (!window->valid)
    return;

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

// Input handling
void UpdateWindowInput(ExWindow *window) {
  if (!window->valid)
    return;

  memcpy(window->prevKeys, window->currentKeys, sizeof(window->currentKeys));

  for (int i = 0; i < KEY_AMOUNT; i++) {
    window->currentKeys[i] = (glfwGetKey(window->handle, i) == GLFW_PRESS);
  }
}

bool IsKeyDownOn(ExWindow *window, int key) {
  return window->valid && window->currentKeys[key];
}

bool IsKeyPressedOn(ExWindow *window, int key) {
  return window->valid && window->currentKeys[key] && !window->prevKeys[key];
}

Vector2 GetMousePositionOn(ExWindow *window) {
  if (!window->valid)
    return (Vector2){0};
  double x, y;
  glfwGetCursorPos(window->handle, &x, &y);
  return (Vector2){x, y};
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
