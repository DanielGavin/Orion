#include "Gui.h"

#include <memory>

#include "Style.h"
#include "DrawQueue.h"
#include "Font.h"

namespace Orion {

	Gui::Gui(const unsigned int& width, const unsigned int& height, Style * style)
		: m_style(style), m_counter(0), m_queue(new DrawQueue)
	{
		Clip clip;
		clip.width = (int16_t)width;
		clip.height = (int16_t)height;
		clip.x = 0;
		clip.y = 0;
		m_clips.push(clip);

		//set the clip to be the size of the window.
		m_queue->setClipRect(clip);
	}

	Gui::~Gui()
	{
	}

	DrawQueue* Gui::queue()
	{
		return m_queue.get();
	}

	void Gui::clear()
	{
		m_queue->clear();
	}

	unsigned int Gui::createFont(void* data)
	{
		Font* font = new Font(data);

		Texture texture;
		texture.components = 1; 
		texture.data = font->bitmap();
		texture.width = font->width();
		texture.height = font->height();

		m_fonts[m_counter] = std::unique_ptr<Font>(font);
		m_textures[m_counter] = texture;

		return m_counter++;
	}

	unsigned int Gui::createTexture(const unsigned int & width, const unsigned int & height, const unsigned int & components, void * data)
	{
		return m_counter++;
	}

	void Gui::setFont(const unsigned int& i)
	{
		m_currentAtlasId = i;
		m_queue->setFont(m_fonts[i].get());
	}

	Texture Gui::texture(unsigned int idx)
	{
		return m_textures[idx];
	}

	void Gui::label(const unsigned int & x, const unsigned int & y, 
					const unsigned int& size, const char * text, const Color& color)
	{
		Vec2<float> p((float)x, (float)y);
		beginTextureId(m_currentAtlasId);
		m_style->drawLabel(m_queue.get(), p, size, text, color);
		endTextureId();
	}

	void Gui::beginTextureId(const unsigned int & id)
	{
		m_textureId.push(id);
		m_queue->setTextureId(m_textureId.top());
	}

	void Gui::endTextureId()
	{
		m_textureId.pop();
		m_queue->setTextureId(m_textureId.top());
	}

}
