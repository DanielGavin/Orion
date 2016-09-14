#include "GL_Renderer.h"

#include <DrawQueue.h>
#include <Gui.h>
#include <gl\glew.h>
#include <SDL_opengl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

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

	int IsCompiled_VS, IsCompiled_FS;
	int IsLinked;

	auto vertexsource = filetobuf("font.vertex");
	auto fragmentsource = filetobuf("font.fragment");

	auto vertexshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	glCompileShader(vertexshader);

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
	if (IsCompiled_VS == false)
	{
		char vertexInfoLog[3000];
		int maxLength;

		glGetShaderInfoLog(vertexshader, 3000, &maxLength, vertexInfoLog);

		printf("%s", vertexInfoLog);

		return;
	}

	auto fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
	if (IsCompiled_FS == false)
	{
		char fragmentInfoLog[3000];
		int maxLength;

		glGetShaderInfoLog(fragmentshader, 3000, &maxLength, fragmentInfoLog);

		printf("%s", fragmentInfoLog);

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
		char shaderProgramInfoLog[3000];
		int maxLength;
		glGetProgramiv(m_fontShaderId, GL_INFO_LOG_LENGTH, &maxLength);


		glGetProgramInfoLog(m_fontShaderId, 3000, &maxLength, shaderProgramInfoLog);

		printf("%s", shaderProgramInfoLog);

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
	glBufferData(GL_ARRAY_BUFFER, scene.first.size()*sizeof(float), scene.first.data(), GL_STATIC_DRAW);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	glm::mat4 projMat = glm::ortho(0.f, 500.f, 400.f, 0.f, -1.f, 2.f);
	//glm::mat4 projMat;

	for (Orion::CmdInfo info : scene.second) {

		//triangles
		if (info.textureId == 0) {

		}

		//texture triangles
		else {
			auto texture = m_gui->texture(info.textureId);

			if (texture.components == 1) {
				
				glUseProgram(m_fontShaderId);

				GLint myLoc = glGetUniformLocation(m_fontShaderId, "atlas");
				glUniform1i(myLoc, 0);

				myLoc = glGetUniformLocation(m_fontShaderId, "ortho");
				glUniformMatrix4fv(myLoc, 1, GL_FALSE, glm::value_ptr(projMat));


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_textureBuffers[info.textureId]);


				glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);

				glEnableVertexAttribArray(0); //vertex
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(info.offset));
				glEnableVertexAttribArray(1); //uv
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, BUFFER_OFFSET(info.offset + 3 * sizeof(float)));

				glDrawArrays(GL_TRIANGLES, 0, info.size / (sizeof(float)*5));

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
