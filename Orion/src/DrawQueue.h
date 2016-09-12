#ifndef DRAWQUEUE_H
#define DRAWQUEUE_H

#include <vector>
#include <map>

#include "Datatypes.h"
#include "Color.h"

namespace Orion {

	typedef std::vector<float> CmdDataVector;

	class DrawQueue {
	public:
		DrawQueue();

		//clear the commands
		void clear();

		//get the next untextured prim triangle
		std::pair<Clip, CmdDataVector*> nextTriangleCmd();
		unsigned int TrianglesCount();

		void beginTriangle(const unsigned int& idx);
		void endTriangle();

		//primtive draw commands that directly map to commands for gpu commands
		void drawPrimTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Color& color);
		void drawPrimRect(const Vec2<float>& p, const float& width, const float& height, const Color& color);

		void drawText(const Vec2<float>& p, const unsigned int& size, const char* text);

		//geometry with thickness made with lines
		void drawTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const float& thickness, const Color& color);

		//Lets the DrawQueue know which clipping rect is currently used. 
		void setClipRect(const Clip& clip);

		//set the depth of the draw commands
		void setDepth(const unsigned int& depth);

		//set the current atlas and font
		void setFontAtlas(FontAtlas* atlas);
		void setFontIndex(const unsigned int& idx);
	private:
		//current clip region
		Clip m_currentClip;

		//current id texture
		int m_currentTextureId;

		//current z depth
		unsigned int m_depth;

		//baked atlas info
		FontAtlas* m_fontAtlas;

		//keep the triangles sorted by atlas texture id and clip(0 is untextured)
		std::map<int, std::map<int64_t, CmdDataVector>> m_cmdTriangles;

		//keep the lines sorted by clip
		std::map<int, CmdDataVector> m_cmdLines;

		//used for iterating through cmds.
		std::map<int64_t, CmdDataVector>::iterator m_it;

	};

}


#endif