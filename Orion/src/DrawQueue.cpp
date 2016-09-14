#include "DrawQueue.h"

#define STB_TRUETYPE_IMPLEMENTATION

#include "stb_truetype.h"

#include "Font.h"

namespace Orion {

	DrawQueue::DrawQueue()
		: m_depth(0), m_currentTextureId(0)
	{
	}

	//currently all the commands have seperate vectors and then copied to one vector
	std::pair<std::vector<float>&, std::vector<CmdInfo>&> DrawQueue::createScene()
	{
		auto it1 = m_cmdTriangles.begin();

		for (; it1 != m_cmdTriangles.end(); ++it1) {

			auto cmd = it1->second;
			auto textureId = it1->first;
			auto it2 = cmd.begin();

			for (; it2 != cmd.end(); ++it2) {

				auto v = it2->second;
				Clip clip = *(Clip*)&it2->first;

				CmdInfo info;
				info.offset = (unsigned int)m_buffer.size()*sizeof(float);
				info.size = (unsigned int)v.size()*sizeof(float);
				info.textureId = textureId;
				info.clip = clip;

				//copy the content of the buffer to the end of the final buffer
				m_buffer.insert(m_buffer.end(), v.begin(), v.end());
				m_commands.push_back(info);
			}
		}

		return std::pair<std::vector<float>&, std::vector<CmdInfo>&>(m_buffer, m_commands);
	}

	void DrawQueue::clear()
	{
		m_buffer.clear();
		m_commands.clear();
		m_cmdTriangles.clear();
	}


	void DrawQueue::drawPrimTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Color& color)
	{
		CmdDataVector* v = &m_cmdTriangles[0][(int64_t)&m_currentClip];

		v->push_back(p1.x());
		v->push_back(p1.y());
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());

		v->push_back(p2.x());
		v->push_back(p2.y());
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());

		v->push_back(p3.x());
		v->push_back(p3.y());
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());
	}

	void DrawQueue::drawPrimRect(const Vec2<float>& p1, const float& width, const float& height, const Color& color)
	{
		//could store the pointer to the current command per texture id and clip change.
		CmdDataVector* v = &m_cmdTriangles[0][(int64_t)&m_currentClip];

		/* p1      p4*/
		/* p3      p2*/
		float x_2 = p1.x();  
		float y_2 = p1.y() + height;
		
		float x_3 = p1.x() + width;
		float y_3 = p1.y() + height;

		float x_4 = p1.x() + width;
		float y_4 = p1.y();

		//half rects
		v->push_back(p1.x());
		v->push_back(p1.y()); 
		v->push_back((float)m_depth); 
		v->push_back(color.rn()); 
		v->push_back(color.gn()); 
		v->push_back(color.bn());

		v->push_back(x_3);
		v->push_back(y_3);
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());

		v->push_back(x_2);
		v->push_back(y_2);
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());

		v->push_back(p1.x());
		v->push_back(p1.y());
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());

		v->push_back(x_2);
		v->push_back(y_2);
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());

		v->push_back(x_4);
		v->push_back(y_4);
		v->push_back((float)m_depth);
		v->push_back(color.rn());
		v->push_back(color.gn());
		v->push_back(color.bn());
	}

	void DrawQueue::drawText(const Vec2<float>& p, const unsigned int& size, const char* text, const Color& color)
	{
		//TODO(currently just test code, does not handle different size fonts)
		CmdDataVector* v = &m_cmdTriangles[m_currentTextureId][(int64_t)&m_currentClip];

		float x = (float)p.x();
		float y = (float)p.y();

		while (*text) {
			
			if (*text >= 32 && *text <= 128) {

				auto q = m_font->quadAsciiCharacter(&x, &y, *text);

				//first half rects
				v->push_back(q.x0);
				v->push_back(q.y0);
				v->push_back((float)m_depth);
				v->push_back(q.s0);
				v->push_back(q.t0);

				v->push_back(q.x0);
				v->push_back(q.y1);
				v->push_back((float)m_depth);
				v->push_back(q.s0);
				v->push_back(q.t1);

				v->push_back(q.x1);
				v->push_back(q.y1);
				v->push_back((float)m_depth);
				v->push_back(q.s1);
				v->push_back(q.t1);

				//second

				v->push_back(q.x0);
				v->push_back(q.y0);
				v->push_back((float)m_depth);
				v->push_back(q.s0);
				v->push_back(q.t0);

				v->push_back(q.x1);
				v->push_back(q.y1);
				v->push_back((float)m_depth);
				v->push_back(q.s1);
				v->push_back(q.t1);

				v->push_back(q.x1);
				v->push_back(q.y0);
				v->push_back((float)m_depth);
				v->push_back(q.s1);
				v->push_back(q.t0);
			}

			++text;
		}
	}

	void DrawQueue::setClipRect(const Clip& clip)
	{
		m_currentClip = clip;
	}

	void DrawQueue::setDepth(const unsigned int & depth)
	{
		m_depth = depth;
	}

	void DrawQueue::setFont(Font* font)
	{
		m_font = font;
	}

	void DrawQueue::setTextureId(const unsigned int & idx)
	{
		m_currentTextureId = idx;
	}


}
