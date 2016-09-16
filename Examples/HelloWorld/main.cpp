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

char* filetobuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "rb");

    if (!fptr) {
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    buf = new char[length + 1];
    fseek(fptr, 0, SEEK_SET);
    fread(buf, length, 1, fptr);
    fclose(fptr);
    buf[length] = 0;

    return buf;
}

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

    auto ttf_buffer = filetobuf("c:/windows/fonts/arial.ttf");

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


            if (e.type == SDL_MOUSEBUTTONDOWN) {

                gui.mouseMovement(e.button.x, e.button.y);

                if (e.button.button == SDL_BUTTON_LEFT) {
                    gui.mouseDown(MOUSE_BUTTON_LEFT);
                }

            }

            if (e.type == SDL_MOUSEBUTTONUP) {

                gui.mouseMovement(e.button.x, e.button.y);

                if (e.button.button == SDL_BUTTON_LEFT) {
                    gui.mouseUp(MOUSE_BUTTON_LEFT);
                }
            }


        }

        glClearColor(0.3, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gui.label(100, 100, 16, "The thread 0x155c has exited with code 0 (0x0).", Color(255, 255, 255));

        if (gui.button(20, 20, 30, 30, "hello")) {
            printf("hello world");
        }

        renderer.update();


        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glcontext);

    return 0;
}