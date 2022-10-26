#include "graphics.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

static GLFWwindow* s_window;

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

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

    return 1;
}

void graphics_prepare()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_present()
{
    glfwSwapBuffers(s_window);
    glfwPollEvents();
}

bool graphics_window_closed()
{
    return glfwWindowShouldClose(s_window);
}
