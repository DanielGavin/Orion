#include "DefaultStyle.h"
#include "DrawQueue.h"

namespace Orion {

	void DefaultStyle::drawLabel(DrawQueue* queue, const Vec2<float>& p, const unsigned int& size, const char* text)
	{
		queue->drawText(p, size, text);
	}

}