#ifndef DRAWQUEUE_H
#define DRAWQUEUE_H

#include <vector>
#include <map>

#include "Datatypes.h"
#include "Color.h"

namespace Orion {

	struct CmdDrawTriangle {
		Vec2<float> p1;
		Vec2<float> p2;
		Vec2<float> p3;
		Color color;
	};

	struct CmdDrawRect {
		Vec2<float> pos;
		float width;
		float height;
		bool filled;
		Color color;
	};

	struct CmdDrawLine {
		Vec2<float> p1;
		Vec2<float> p2;
		float thickness;
		Color color;
	};

	struct CmdDrawText {
		Vec2<float> pos;
		void* font;
		std::string string;
		Color color;
	};

	struct CmdDrawRectTextured {

	};

	struct FontAtlas {

	};

	typedef std::vector<CmdDrawRect> CmdDrawRectVector;
	typedef std::vector<CmdDrawLine> CmdDrawLineVector;
	typedef std::vector<CmdDrawTriangle> CmdDrawTriangleVector;
	typedef std::vector<CmdDrawText> CmdDrawTextVector;

	//records the different commands, and can convert those commands into triangle data for use in opengl, directx, etc
	//command draws are sorted per clipping rect and texture(only for text and RectTextured).
	class DrawQueue {
	public:
		//clear the commands
		void clear();

		//primtive draw commands that directly map to commands for gpu commands
		void drawPrimTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Color& color);
		void drawPrimRect(const Vec2<float>& p, const float& width, const float& height, const Color& color);

		//geometry with thickness made with lines
		void drawTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const float& thickness, const Color& color);

		//Lets the CommandQueue know which clipping rect is currently used. 
		void setClipRect(const Clip& clip);
	private:
		//current clip region
		Clip m_currentClip;

		//texture ids and baked atlas
		FontAtlas m_fontAtlas;

		//keep the different commands type sorted by clip
		std::map<int64_t, CmdDrawRectVector> m_cmdRect;
		std::map<int64_t, CmdDrawLineVector> m_cmdLine;
		std::map<int64_t, CmdDrawTriangleVector> m_cmdTriangle;

		//keep the text sorted by atlas texture id and clip
		std::map<int64_t, std::map<int, CmdDrawTextVector>> m_cmdText;
	};

}


#endif