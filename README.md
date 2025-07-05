# Raywin
A library for creating multiple windows in raylib. (Raywin currently doesn't work. come back later)

## Dependency
Raywin uses GLFW. You need to install GLFW and link it when compiling:  
`gcc game.c raywin.c -o game -lraylib -lglfw`

## Usage
To use raywin, download `raywin.h` and `raywin.c`. (Make sure to also compile raywin.c when compiling your game)  

Call the `InitRaywin()` function before any other raywin function.  
Then create your windows:  
```c
int main(){
  // Create the raylib window as usual
  InitWindow(600, 600, "Title");

  InitRaywin();
  ExWindow win2 = CreateExtraWindow(600, 600, "Window 2");
  ExWindow win3 = CreateExtraWindow(600, 600, "Window 3");
}
```

For Drawing, use the `BeginDrawingOn()` function. And make sure to call UpdateExtraWindows() once per frame.  

```c
while(!WindowShouldClose()) { // You can use ExtraWindowShouldClose() to check other windows
  UpdateExtraWindows();
  // Also add UpdateWindowInput() if you want to handle input for other windows

  // Draw as usual
  BeginDrawing();
  ClearBackground(SKYBLUE);

  // Draw for other windows
  BeginDrawingOn(&win2);
  ClearBackground(RED);
  EndDrawingOn(&win2);

  BeginDrawingOn(&win3);
  ClearBackground(ORANGE);
  EndDrawingOn(&win3);

  EndDrawing();
} // Make sure to close all the windows then call DeinitRaywin()
```

See the raywin.h file to see the list of functions
