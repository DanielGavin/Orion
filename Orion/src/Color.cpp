#include "Color.h"

namespace Orion {

	Color::Color(unsigned int r, unsigned int g, unsigned int b)
		: m_r(r), m_g(g), m_b(b)
	{
	}

	unsigned int Color::r() 
	{
		return m_r;
	}

	unsigned int Color::g() 
	{
		return m_g;
	}

	unsigned int Color::b() 
	{
		return m_b;
	}

}