#ifndef LAYOUT_H
#define LAYOUT_H

#include <vector>

namespace Orion {

	//Data for each widget item
	struct LayoutItem {
		unsigned int width;//prefered width of the widget
		unsigned int height;//prefered height of the height
		void* identifier;//some memory address to identify the widget, such as string address for editbox.
	};

	//Return result for the widget dimensions
	struct LayoutPosition {
		unsigned int x;
		unsigned int y;
		unsigned int width;
		unsigned int height;
	};


	//Idea is to handle layout based on what happend last frame, since we cannot know what happens this frame.
	class Layout {
	public:
		void addItem(LayoutItem item);
		virtual LayoutPosition item(void* identifier) = 0;
	protected:
		std::vector<LayoutItem> m_lastItems;//stores the last frames items
		unsigned int m_width; //max width of the layout
		unsigned int m_height; //max height of the layout
		unsigned int m_x;
		unsigned int m_y;
	};

}


#endif