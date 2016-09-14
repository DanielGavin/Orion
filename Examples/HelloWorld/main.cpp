#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>

#include <Gui.h>
#include <DefaultStyle.h>
#include "GL_Renderer.h"

using namespace Orion;

const unsigned int screen_width = 500;
const unsigned int screen_height = 400;

//test code
int main(int argc, char *argv[])
{
	DefaultStyle style;
	Gui gui(screen_width, screen_height, &style);

	SDL_Window *window = SDL_CreateWindow(
		"Hello world", 100, 100, screen_width, screen_height,
		SDL_WINDOW_OPENGL);

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	glewInit();

	bool exit = 0;

	auto ttf_buffer = filetobuf("c:/windows/fonts/times.ttf");

	auto font_handle = gui.createFont(ttf_buffer);

	gui.setFont(font_handle);

	GL_Renderer renderer(&gui);

	renderer.uploadTextureBuffer(font_handle);

	while (!exit) {

		SDL_Event e;
		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_WINDOWEVENT) {
				if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
					exit = 1;
				}
			}

			switch (e.type) {
			case SDLK_ESCAPE:
				exit = 1;
				break;
			default:
				break;
			}
		}

		gui.label(100, 100, 16, "hello world", Color(255,255,255));

		renderer.update();


		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glcontext);

	return 0;
}