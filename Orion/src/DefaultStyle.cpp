#include "DefaultStyle.h"
#include "DrawQueue.h"

namespace Orion {

    void DefaultStyle::drawLabel(DrawQueue* queue, const Vec2<float>& p,
        const unsigned int& size, const char* text, const Color& color)
    {
        queue->drawText(p, size, text, color);
    }

    void DefaultStyle::drawButton(DrawQueue * queue, const Vec2<float>& p1, 
        const unsigned int& width, const unsigned int& height, const char * text)
    {
		int fontSize = 16;

        queue->drawPrimRect(p1, (float)width, (float)height, Color(100, 100, 100));

        auto p2 = Vec2<float>(p1.x(), p1.y() + fontSize);


        queue->drawText(p2, fontSize, text, Color(0.0, 0.0, 0.0));
    }

}