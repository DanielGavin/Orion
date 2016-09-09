#include "DrawQueue.h"


namespace Orion {

	void DrawQueue::clear()
	{
	}

	void DrawQueue::drawPrimTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Color & color)
	{
		CmdDrawTriangle tri;
		tri.p1 = p1;
		tri.p2 = p2;
		tri.p3 = p3;
		tri.color = color;
		m_cmdTriangle[(int64_t)&m_currentClip].push_back(tri);
	}

	void DrawQueue::drawPrimRect(const Vec2<float>& p, const float & width, const float & height, const Color & color)
	{
		CmdDrawRect rect;
		rect.pos = p;
		rect.width = width;
		rect.height = height;
		rect.color = color;
		m_cmdRect[(int64_t)&m_currentClip].push_back(rect);
	}

}
