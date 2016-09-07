#ifndef MATH_H
#define MATH_H

namespace Orion {

	template<class T>
	class Vec2 {
	public:
		Vec2(T x, T y)
			: m_x(x), m_y(y)
		{
		}

		Vec2<T> operator+(const Vector2D<T>& b)
		{
			return Vec2(this->m_x + b.m_x, this->m_y + b.m_y);
		}

		Vec2<T> operator-(const Vector2D<T>& b)
		{
			return Vec2(this->m_x - b.m_x, this->m_y - b.m_y);
		}

		void operator=(const T& b)
		{
			this->m_x = b.m_x;
			this->m_y = b.m_y;
		}

		T x()
		{
			return m_x;
		}
		
		T y()
		{
			return m_y;
		}
	private:
		T m_x;
		T m_y;
	};

		
	class Rect {

		Rect()
		{
		}

		Rect(const unsigned int& x, const unsigned int& y, const unsigned int& w, const unsigned int& h)
			: m_x(x), m_y(y), m_width(w), m_height(h)
		{
		}

		void operator=(const Rect& b)
		{
			this->m_x = b.m_x;
			this->m_y = b.m_y;
			this->m_width = b.m_width;
			this->m_height = b.m_height;
		}

		unsigned int m_x;
		unsigned int m_y;
		unsigned int m_width;
		unsigned m_height;
	};

}


#endif