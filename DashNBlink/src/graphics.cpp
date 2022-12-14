#include "graphics.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "rect.h"
#include "shader.h"

#include "keyboard.h"
#include "mouse.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

static GLFWwindow* s_window;

static rect s_rect;

static shader s_start_shader;
static unsigned int s_start_uniform_offset = 0;
static unsigned int s_start_uniform_opacity = 0;
static unsigned int s_start_uniform_tex0 = 0;

static sprite_sheet s_start_tex0;
static sprite_sheet s_start_tex1;
static sprite_sheet s_start_tex2;
static sprite_sheet s_start_tex3;

static shader s_tile_shader;
static unsigned int s_tile_uniform_id = 0;
static unsigned int s_tile_uniform_coords = 0;
static unsigned int s_tile_uniform_tex = 0;

static shader s_player_shader;
static unsigned int s_player_uniform_id = 0;
static unsigned int s_player_uniform_coords = 0;
static unsigned int s_player_uniform_rotation = 0;
static unsigned int s_player_uniform_tex = 0;

static sprite_sheet s_play_tex0;
static sprite_sheet s_tile_sheet;

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

    keyboard_init(s_window);
    mouse_init(s_window);

    glClearColor(43.0f / 255.0f, 15.0f / 255.0f, 84.0f / 255.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);

    shader_create(&s_start_shader, "res/shaders/start.shader");
    s_start_uniform_offset = shader_find_location(&s_start_shader, "u_offset");
    s_start_uniform_opacity = shader_find_location(&s_start_shader, "u_opacity");
    s_start_uniform_tex0 = shader_find_location(&s_start_shader, "u_texture0");

    sprite_sheet_create(&s_start_tex0, "res/fire.png");
    sprite_sheet_create(&s_start_tex1, "res/lines.png");
    sprite_sheet_create(&s_start_tex2, "res/header.png");
    sprite_sheet_create(&s_start_tex3, "res/press_start.png");

    shader_create(&s_tile_shader, "res/shaders/tile.shader");
    s_tile_uniform_id = shader_find_location(&s_tile_shader, "u_id");
    s_tile_uniform_coords = shader_find_location(&s_tile_shader, "u_room_coords");
    s_tile_uniform_tex = shader_find_location(&s_tile_shader, "u_texture");

    shader_create(&s_player_shader, "res/shaders/player.shader");
    s_player_uniform_id = shader_find_location(&s_player_shader, "u_id");
    s_player_uniform_coords = shader_find_location(&s_player_shader, "u_room_coords");
    s_player_uniform_rotation = shader_find_location(&s_player_shader, "u_rotation");
    s_player_uniform_tex = shader_find_location(&s_player_shader, "u_texture");

    rect_create(&s_rect);

    sprite_sheet_create(&s_play_tex0, "res/fireRIm.png");
    sprite_sheet_create(&s_tile_sheet, "res/tile_sheet.png");

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
    sprite_sheet_destroy(&s_start_tex0);
    sprite_sheet_destroy(&s_start_tex1);
    sprite_sheet_destroy(&s_start_tex2);
    sprite_sheet_destroy(&s_start_tex3);

    sprite_sheet_destroy(&s_play_tex0);
    sprite_sheet_destroy(&s_tile_sheet);

    rect_destroy(&s_rect);

    shader_destroy(&s_tile_shader);

    glfwTerminate();
}

bool graphics_window_closed()
{
    return glfwWindowShouldClose(s_window);
}

void graphics_start_draw(float offset, bool switched, float opacity)
{
    shader_bind(&s_start_shader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_set_uniform1f(s_start_uniform_tex0, 0);

    shader_set_uniform1f(s_start_uniform_opacity, 1.0f);

    shader_set_uniform1f(s_start_uniform_offset, offset);

    texture_bind(&(s_start_tex1.tex), 0);
    rect_draw(&s_rect);

    shader_set_uniform1f(s_start_uniform_offset, 0);

    texture_bind(&(s_start_tex0.tex), 0);
    rect_draw(&s_rect);

    shader_set_uniform1f(s_start_uniform_opacity, opacity);

    shader_set_uniform1f(s_start_uniform_offset, offset + 100.0f);

    if (!switched)
    {
        texture_bind(&(s_start_tex2.tex), 0);
        rect_draw(&s_rect);
    }

    else
    {
        texture_bind(&(s_start_tex3.tex), 0);
        rect_draw(&s_rect);
    }

    glDisable(GL_BLEND);

    texture_unbind(0);

    shader_unbind();
}

void graphics_play_draw()
{
    shader_bind(&s_start_shader);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_set_uniform1f(s_start_uniform_tex0, 0);

    shader_set_uniform1f(s_start_uniform_opacity, 1.0f);

    shader_set_uniform1f(s_start_uniform_offset, 0);

    texture_bind(&(s_play_tex0.tex), 0);
    rect_draw(&s_rect);

    glDisable(GL_BLEND);

    texture_unbind(0);

    shader_unbind();
}

void graphics_tile_draw(const tile* t, float x, float y)
{
    shader_bind(&s_tile_shader);

    shader_set_uniform1f(s_tile_uniform_id, (float)t->index);
    shader_set_uniform2f(s_tile_uniform_coords, x, y);

    texture_bind(&(s_tile_sheet.tex), 0);
    shader_set_uniform1f(s_tile_uniform_tex, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    rect_draw(&s_rect);

    glDisable(GL_BLEND);

    texture_unbind(0);

    shader_unbind();
}


void graphics_player_draw(const player* p)
{
    shader_bind(&s_player_shader);

    shader_set_uniform1f(s_player_uniform_id, (float)p->character);
    shader_set_uniform2f(s_player_uniform_coords, p->position.x, p->position.y);

    texture_bind(&(s_tile_sheet.tex), 0);
    shader_set_uniform1f(s_tile_uniform_tex, 0);

    float cos_r = cosf(p->rotation);
    float sin_r = sinf(p->rotation);

    float mat2[4] = { cos_r, -sin_r, sin_r, cos_r };

    shader_set_uniform2x2f(s_player_uniform_rotation, mat2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    rect_draw(&s_rect);

    glDisable(GL_BLEND);

    texture_unbind(0);

    shader_unbind();
}