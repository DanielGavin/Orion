#include "GL_Renderer.h"

#include <DrawQueue.h>
#include <Gui.h>
#include <gl\glew.h>
#include <SDL_opengl.h> 

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

char* filetobuf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr = fopen(file, "rb"); /* Open file for reading */
	if (!fptr) /* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
	length = ftell(fptr); /* Find out how many bytes into the file we are */
	buf = new char[length + 1]; /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
	fclose(fptr); /* Close the file */
	buf[length] = 0; /* Null terminator */

	return buf; /* Return the buffer */
}

GL_Renderer::GL_Renderer(Orion::Gui* gui)
	: m_gui(gui)
{
	glGenBuffers(1, &m_bufferId);

	//extract from opengl 2 tutorial
	int IsCompiled_VS, IsCompiled_FS;
	int IsLinked;
	int maxLength;
	char *vertexInfoLog;
	char *fragmentInfoLog;
	char *shaderProgramInfoLog;

	auto vertexsource = filetobuf("font.vertex");
	auto fragmentsource = filetobuf("font.fragment");

	auto vertexshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	glCompileShader(vertexshader);

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == false)
	{
		glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);

		vertexInfoLog = (char *)malloc(maxLength);

		glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);

		free(vertexInfoLog);
		return;
	}

	auto fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == false)
	{
		glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);

		fragmentInfoLog = (char *)malloc(maxLength);

		glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);

		free(fragmentInfoLog);
		return;
	}

	m_fontShaderId = glCreateProgram();

	glAttachShader(m_fontShaderId, vertexshader);
	glAttachShader(m_fontShaderId, fragmentshader);

	glBindAttribLocation(m_fontShaderId, 0, "vertex");
	glBindAttribLocation(m_fontShaderId, 1, "uv");

	glLinkProgram(m_fontShaderId);


	glGetProgramiv(m_fontShaderId, GL_LINK_STATUS, (int *)&IsLinked);
	if (IsLinked == false)
	{
		glGetProgramiv(m_fontShaderId, GL_INFO_LOG_LENGTH, &maxLength);

		shaderProgramInfoLog = (char *)malloc(maxLength);

		glGetProgramInfoLog(m_fontShaderId, maxLength, &maxLength, shaderProgramInfoLog);

		free(shaderProgramInfoLog);
		return;
	}

	delete vertexsource;
	delete fragmentsource;
}

void GL_Renderer::update()
{
	//test fixed opengl
	auto queue = m_gui->queue();
	auto scene = queue->createScene();

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	glBufferData(m_bufferId, scene.first.size(), scene.first.data(), GL_DYNAMIC_DRAW);

	for (Orion::CmdInfo info : scene.second) {

		//triangles
		if (info.textureId == 0) {

		}

		//texture rect
		else {
			auto texture = m_gui->texture(info.textureId);

			if (texture.components == 1) {

				glUseProgram(m_fontShaderId);

				glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);

				glEnableVertexAttribArray(0); //vertex
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*5, BUFFER_OFFSET(info.offset));
				glEnableVertexAttribArray(1); //uv
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*5, BUFFER_OFFSET(info.offset+3*4));

				glDrawArrays(GL_TRIANGLES, 0, info.size / (3 * 4 * 5));
			}

		}

	}


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
}
