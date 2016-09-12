#include "DrawQueue.h"

#define STB_TRUETYPE_IMPLEMENTATION

#include "stb_truetype.h"

namespace Orion {

	DrawQueue::DrawQueue()
		: m_depth(0), m_currentTextureId(0)
	{
	}

	void DrawQueue::clear()
	{
	}

	unsigned int DrawQueue::TrianglesCount()
	{
		return (unsigned int)m_cmdTriangles.size();
	}

	void DrawQueue::beginTriangle(const unsigned int& idx)
	{
		m_it = m_cmdTriangles[idx].begin();
	}

	void DrawQueue::endTriangle()
	{
	}

	std::pair<Clip, CmdDataVector*> DrawQueue::nextTriangleCmd()
	{
		CmdDataVector* v = &m_it->second;
		auto clip = &m_it->first;
		m_it++;
		return std::pair<Clip, CmdDataVector*>(*(Clip*)clip, v);
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

	void DrawQueue::drawText(const Vec2<float>& p, const unsigned int& size, const char* text)
	{
		//TODO(currently just test code, does not handle different size fonts)
		CmdDataVector* v = &m_cmdTriangles[m_currentTextureId][(int64_t)&m_currentClip];

		float x = (float)p.x();
		float y = (float)p.y();

		while (*text) {
			
			if (*text >= 32 && *text <= 128) {
				stbtt_aligned_quad q;
				stbtt_GetBakedQuad(m_fontAtlas->cdata, 512, 512, *text - 32, &x, &y, &q, 1);

				v->push_back(q.x0);
				v->push_back(q.y0);
				v->push_back(q.s0);
				v->push_back(q.t1);

				v->push_back(q.x1);
				v->push_back(q.y0);
				v->push_back(q.s1);
				v->push_back(q.t1);

				v->push_back(q.x1);
				v->push_back(q.y1);
				v->push_back(q.s1);
				v->push_back(q.t0);

				v->push_back(q.x0);
				v->push_back(q.y1);
				v->push_back(q.s0);
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

	void DrawQueue::setFontAtlas(FontAtlas * atlas)
	{
		m_fontAtlas = atlas;
	}

	void DrawQueue::setFontIndex(const unsigned int & idx)
	{
	}

}
