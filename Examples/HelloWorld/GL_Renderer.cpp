#include "GL_Renderer.h"

#include <DrawQueue.h>
#include <Gui.h>
#include <gl/glew.h>
#include <SDL_opengl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#define BUFFER_OFFSET(i) ((char *)NULL + (i))



GL_Renderer::GL_Renderer(Orion::Gui* gui)
    : m_gui(gui)
{
    auto font_vertex =
        "#version 330\n"
        "layout(location = 0)in vec3 vertex;\n"
        "layout(location = 1)in vec3 color;\n"
        "layout(location = 2)in vec2 uv;\n"
        "out vec2 st;\n"
        "out vec3 fragColor;\n"
        "uniform mat4 ortho;\n"
        "void main(void) {\n"
        "    st = uv;\n"
        "    fragColor = color;\n"
        "    gl_Position = ortho * vec4(vertex, 1.0);\n"
        "}";


    auto font_fragment =
        "#version 330\n"
        "out vec4 outputColor;\n"
        "uniform sampler2D atlas;\n"
        "in vec2 st;\n"
        "in vec3 fragColor;\n"
        "void main(void) {\n"
        "    outputColor = vec4(fragColor, texture2D(atlas, st).a);\n"
        "}\n";

    auto triangles_vertex =
        "#version 330\n"
        "layout(location = 0)in vec3 vertex;\n"
        "layout(location = 1)in vec3 color;\n"
        "out vec3 frag_color;\n"
        "uniform mat4 ortho;\n"
        "void main(void) {\n"
        "    frag_color = color;\n"
        "    gl_Position = ortho * vec4(vertex, 1.0);\n"
        "}";

    auto triangles_fragment =
        "#version 330\n"
        "out vec4 outputColor;\n"
        "in vec3 frag_color;\n"
        "void main(void) {\n"
        "    outputColor = vec4(frag_color, 1.0f);\n"
        "}\n";

    m_fontShaderId = createProgram(font_vertex, font_fragment);
    m_triangleShaderId = createProgram(triangles_vertex, triangles_fragment);

    glm::mat4 projMat = glm::ortho(0.f, 900.f, 600.f, 0.f, -1.f, 2.f);


    //set the uniforms for the font program
    glUseProgram(m_fontShaderId);

    GLint myLoc = glGetUniformLocation(m_fontShaderId, "atlas");
    glUniform1i(myLoc, 0);

    myLoc = glGetUniformLocation(m_fontShaderId, "ortho");
    glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    //set the uniform for triangle program
    glUseProgram(m_triangleShaderId);

    myLoc = glGetUniformLocation(m_triangleShaderId, "ortho");
    glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(projMat));

}

void GL_Renderer::update()
{
    //test fixed opengl
    auto queue = m_gui->queue();
    auto scene = queue->createScene();


    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, scene.first.size() * sizeof(float), scene.first.data(), GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glEnable(GL_SCISSOR_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (Orion::CmdInfo info : scene.second) {

        glScissor(info.clip.x, info.clip.y, info.clip.width, info.clip.height);

        //triangles
        if (info.textureId == 0) {
            glUseProgram(m_triangleShaderId);

            glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);

            glEnableVertexAttribArray(0); //vertex
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, BUFFER_OFFSET(info.offset));
            glEnableVertexAttribArray(1); //color
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, BUFFER_OFFSET(info.offset + 3 * sizeof(float)));

            glDrawArrays(GL_TRIANGLES, 0, info.size / (sizeof(float) * 6));
        }

        //texture triangles
        else {
            auto texture = m_gui->texture(info.textureId);

            if (texture.components == 1) {

                glUseProgram(m_fontShaderId);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_textureBuffers[info.textureId]);


                glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);

                glEnableVertexAttribArray(0); //vertex
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, BUFFER_OFFSET(info.offset));
                glEnableVertexAttribArray(1); //color
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, BUFFER_OFFSET(info.offset + 3 * sizeof(float)));
                glEnableVertexAttribArray(2); //uv
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, BUFFER_OFFSET(info.offset + 6 * sizeof(float)));

                glDrawArrays(GL_TRIANGLES, 0, info.size / (sizeof(float) * 8));

                glBindTexture(GL_TEXTURE_2D, 0);

            }

        }

    }

    m_gui->clear();
}

//upload the texture to device
void GL_Renderer::uploadTextureBuffer(const unsigned int& id)
{
    auto tex = m_gui->texture(id);

    GLuint ftex;
    glGenTextures(1, &ftex);
    glBindTexture(GL_TEXTURE_2D, ftex);

    if (tex.components == 1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex.width, tex.height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex.data);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_textureBuffers[id] = ftex;
}

unsigned int GL_Renderer::createProgram(const char * vertex, const char * fragment)
{
    glGenBuffers(1, &m_bufferId);

    int IsCompiled_VS, IsCompiled_FS;
    int IsLinked;


    auto vertexshader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexshader, 1, (const GLchar**)&vertex, 0);

    glCompileShader(vertexshader);

    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);

    if (IsCompiled_VS == false)
    {
        char vertexInfoLog[3000];
        int maxLength;

        glGetShaderInfoLog(vertexshader, 3000, &maxLength, vertexInfoLog);

        printf("%s", vertexInfoLog);

        assert(true);
    }

    auto fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentshader, 1, (const GLchar**)&fragment, 0);

    glCompileShader(fragmentshader);

    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);

    if (IsCompiled_FS == false)
    {
        char fragmentInfoLog[3000];
        int maxLength;

        glGetShaderInfoLog(fragmentshader, 3000, &maxLength, fragmentInfoLog);

        printf("%s", fragmentInfoLog);

        assert(true);
    }

    GLint program = glCreateProgram();

    glAttachShader(program, vertexshader);
    glAttachShader(program, fragmentshader);


    glLinkProgram(program);


    glGetProgramiv(program, GL_LINK_STATUS, (int*)&IsLinked);

    if (IsLinked == false)
    {
        char shaderProgramInfoLog[3000];
        int maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);


        glGetProgramInfoLog(program, 3000, &maxLength, shaderProgramInfoLog);

        printf("%s", shaderProgramInfoLog);

        assert(true);
    }

    return program;
}
