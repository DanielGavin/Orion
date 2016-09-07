#ifndef GUI_H
#define GUI_H

#include <stack>


namespace Orion {

	class Style;
	class Layout;
	class CommandQueue;

	class Gui {
	public:
		Gui(Style* style);
		~Gui();

		//set what the metric of the font that is used - this is used to make the dynamic layout scale widgets correctly with text.
		void setFontMetric(const unsigned int& w, const unsigned int& h);

		//dynamic layout functions
		void beginLayout(Layout* layout);
		void endLayout();

		//window frame functions 
		void beginWindow(Layout* layout, bool dragable);
		void endWindow();


		//clipping
		void beginClipRect(...);
		void endClipRect(...);

		//fixed layout functions
		bool button(const unsigned int& x, const unsigned int& y,
			const unsigned int& w, const unsigned int& h, char* text);
	private:
		std::stack<Layout*> m_layout;
		CommandQueue* m_queue;
		Style* m_style;
	}

};



#endif