/*
 * Copyright 2018 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <vector>
#include <array>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <GLES3/gl3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
using namespace std;
const unsigned int DISP_WIDTH = 600;
const unsigned int DISP_HEIGHT = 400;
int main() {
// The window
  SDL_Window *window = NULL;
  // The OpenGL context
  SDL_GLContext context = NULL;
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 10;
  } else{
    cout << "loaad" << endl;
  }

  // Setup the exit hook

  // Request OpenGL ES 3.0
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  // Want double-buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  cout << "errora"<< endl;
  // Create the window
  window = SDL_CreateWindow("GLES3+SDL2 Tutorial", SDL_WINDOWPOS_UNDEFINED,
  SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_OPENGL);
  cout << "erroraff"<< endl;


  context = SDL_GL_CreateContext(window);


  cout << "erroraffaa"<< endl;

  // Clear to black
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  // Update the window
  SDL_GL_SwapWindow(window);

  cout << "erroraffhtrht"<< endl;
  // Wait for the user to quit
  bool quit = false;
  while (!quit) {

  SDL_Event event;
  if (SDL_WaitEvent(&event) != 0) {
  if (event.type == SDL_QUIT) {
  // User wants to quit
  quit = true;
  }
  }
  }
  return EXIT_SUCCESS;
  }
