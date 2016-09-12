#ifndef GUI_H
#define GUI_H

#include <stack>

#include "DataTypes.h"
#include <map>


namespace Orion {

	class Style;
	class Layout;
	class DrawQueue;

	class Gui {
	public:
		Gui(const unsigned int& width, const unsigned int& height, Style* style);
		~Gui();

		//font functions
		int createFontAtlas(void* data);
		void setFont(const unsigned int& i);

		//dynamic layout functions
		void beginLayout(Layout* layout);
		void endLayout();

		//window frame functions 
		void beginWindow(Layout* layout, bool dragable);
		void endWindow();


		//clipping
		void beginClipRect(const unsigned int& x, const unsigned int& y, 
						   const unsigned int& width, const unsigned int& height);
		void endClipRect();

		//fixed layout functions
		bool button(const unsigned int& x, const unsigned int& y,
			const unsigned int& w, const unsigned int& h, const char* text);

		void label(const unsigned int& x, const unsigned int& y, 
			const unsigned int& size, const char* text);
	private:
		std::stack<Layout*> m_layout;
		std::stack<Clip> m_clips;
		std::map<unsigned int, const char*> m_textures;
		std::map<unsigned int, FontAtlas> m_fonts;
		DrawQueue* m_queue;
		Style* m_style;
	};

}



#endif