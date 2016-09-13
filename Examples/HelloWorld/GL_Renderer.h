#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <map>


namespace Orion {
	class Gui;
}

//quickly written renderer to test the buffer data
class GL_Renderer {
public:
	GL_Renderer(Orion::Gui* gui);
	void update();
	void uploadTextureBuffer(const unsigned int& id);
private:
	Orion::Gui* m_gui;
	std::map<unsigned int, unsigned int> m_textureBuffers;
	unsigned int m_bufferId;
	unsigned int m_fontShaderId;
};

char* filetobuf(char *file);


#endif