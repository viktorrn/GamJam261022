#include "rect.h"

#include "GL/glew.h"

void rect_create(rect* rect)
{
    float positions[12] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    float textures[8] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int p_vbo;
    glGenBuffers(1, &p_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, p_vbo);

    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    unsigned int t_vbo;
    glGenBuffers(1, &t_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);

    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textures, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    rect->vao = vao;
    rect->p_vbo = p_vbo;
    rect->t_vbo = t_vbo;
    rect->ibo = ibo;
    rect->indices_count = 6;
}

void rect_draw(rect* rect)
{
    glBindVertexArray(rect->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, rect->indices_count, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void rect_destroy(rect* rect)
{
    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glDeleteBuffers(1, &(rect->p_vbo));
    glDeleteBuffers(1, &(rect->t_vbo));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &(rect->ibo));

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &(rect->vao));
}