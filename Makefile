CC = emcc
all: main.cpp
	$(CC) main.cpp -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s USE_SDL_TTF=2 -s EXPORTED_FUNCTIONS=["_main"] --preload-file assets -o scribblie.js

