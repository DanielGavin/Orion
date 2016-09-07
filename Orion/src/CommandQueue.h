#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include <vector>

#include "Math.h"
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


	//records the different commands, and can convert those commands into triangle data for use in opengl, directx, etc
	class CommandQueue {
	public:
		void clear();
	private:
		//keep the different commands type sorted
		std::vector<CmdDrawRect> m_cmdRect;
		std::vector<CmdDrawLine> m_cmdLine;
		std::vector<CmdDrawTriangle> m_cmdTriangle;
		std::vector<CmdDrawText> m_cmdText;

	};

}


#endif