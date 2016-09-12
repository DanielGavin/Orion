#ifndef COLOR_H
#define COLOR_H


namespace Orion {

	class Color {
	public:
		Color();
		Color(unsigned int r, unsigned int g, unsigned int b);
		unsigned int r() const;
		unsigned int g() const;
		unsigned int b() const;
		float rn() const;
		float gn() const;
		float bn() const;
	private:
		unsigned int m_r;
		unsigned int m_g;
		unsigned int m_b;
	};

}

#endif