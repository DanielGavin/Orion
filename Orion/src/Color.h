#ifndef COLOR_H
#define COLOR_H


namespace Orion {

	class Color {
	public:
		Color(unsigned int r, unsigned int, g, unsigned int b);
		unsigned int r();
		unsigned int g();
		unsigned int b();
	private:
		unsigned int m_r;
		unsigned int m_g;
		unsigned int m_b;
	};

}

#endif