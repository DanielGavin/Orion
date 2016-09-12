#include "Gui.h"
#include "Style.h"
#include "DrawQueue.h"

namespace Orion {

	Gui::Gui(const unsigned int& width, const unsigned int& height, Style * style)
	{
		m_style = style;

		Clip clip;
		clip.width = width;
		clip.height = height;
		clip.x = 0;
		clip.y = 0;
		m_clips.push(clip);

		//allocate the queue and set the clip to be the size of the window.
		m_queue = new DrawQueue();
		m_queue->setClipRect(clip);
	}

	Gui::~Gui()
	{
		delete m_queue;
	}

	int Gui::createFontAtlas(void* data)
	{
		//test bitmap code
		FontAtlas font;

		font.bitmap = new unsigned char[1024 * 1024];

		auto ret = stbtt_BakeFontBitmap((const unsigned char*)data, 0, 32.0, font.bitmap, 1024, 1024, 32, 96, font.cdata);

		int i = (int)m_fonts.size();
		m_fonts[i] = font;
		return i;
	}

	void Gui::setFont(const unsigned int& i)
	{

	}

	void Gui::label(const unsigned int & x, const unsigned int & y, const unsigned int& size, const char * text)
	{
		Vec2<float> p(x, y);
		m_style->drawLabel(m_queue, p, size, text);
	}

}
