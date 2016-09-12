#include "Color.h"

namespace Orion {

	Color::Color()
		: m_r(0), m_g(0), m_b(0)
	{
	}

	Color::Color(unsigned int r, unsigned int g, unsigned int b)
		: m_r(r), m_g(g), m_b(b)
	{
	}

	unsigned int Color::r() const
	{
		return m_r;
	}

	unsigned int Color::g() const
	{
		return m_g;
	}

	unsigned int Color::b() const
	{
		return m_b;
	}

	float Color::rn() const
	{
		return (float)m_r / 255.0;
	}

	float Color::gn() const
	{
		return (float)m_g / 255.0;
	}

	float Color::bn() const
	{
		return (float)m_b / 255.0;
	}

}