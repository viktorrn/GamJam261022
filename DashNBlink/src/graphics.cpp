#include "graphics.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "rect.h"
#include "shader.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

static GLFWwindow* s_window;

static rect s_rect;

static shader s_tile_shader;
static unsigned int s_tile_uniform_coords = 0;

bool graphics_init()
{
    if (!glfwInit())
        return 0;

    s_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dash & Blink", NULL, NULL);
    if (!s_window)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(s_window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Glew could not be initialized" << std::endl;
        return 0;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glClearColor(43.0f / 255.0f, 15.0f / 255.0f, 84.0f / 255.0f, 1.0f);

    shader_create(&s_tile_shader, "res/shaders/tile.shader");
    s_tile_uniform_coords = shader_find_location(&s_tile_shader, "u_room_coords");

    rect_create(&s_rect);

    return 1;
}

void graphics_prepare()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void graphics_present()
{
    glfwSwapBuffers(s_window);
    glfwPollEvents();
}

void graphics_clean_up()
{
    rect_destroy(&s_rect);

    shader_destroy(&s_tile_shader);

    glfwTerminate();
}

bool graphics_window_closed()
{
    return glfwWindowShouldClose(s_window);
}

void graphics_tile_draw(const tile* t, float x, float y)
{
    shader_bind(&s_tile_shader);

    shader_set_uniform2f(s_tile_uniform_coords, x, y);

    rect_draw(&s_rect);

    shader_unbind();
}