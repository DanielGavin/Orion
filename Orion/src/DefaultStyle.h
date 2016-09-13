#ifndef DEFAULTSTYLE_H
#define DEFaULTSTYLE_H

#include "Style.h"
#include "DataTypes.h"

namespace Orion {

	class DefaultStyle : public Style {
		void drawLabel(DrawQueue* queue, const Vec2<float>& p, const unsigned int& size, const char* text, const Color& color) override;
	};

}


#endif