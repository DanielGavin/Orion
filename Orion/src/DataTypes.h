#ifndef MATH_H
#define MATH_H

#include <stdint.h>

#include "stb_truetype.h"

//might move some classes over to their seperate file, if they become large enough to warrant it.

namespace Orion {

    template<class T>
    class Vec2 {
    public:
        Vec2()
        {
        }

        Vec2(T x, T y)
            : m_x(x), m_y(y)
        {
        }

        Vec2<T> operator+(const Vec2<T>& b)
        {
            return Vec2(this->m_x + b.m_x, this->m_y + b.m_y);
        }

        Vec2<T> operator-(const Vec2<T>& b)
        {
            return Vec2(this->m_x - b.m_x, this->m_y - b.m_y);
        }

        void operator=(const T& b)
        {
            this->m_x = b.m_x;
            this->m_y = b.m_y;
        }

        T x() const
        {
            return m_x;
        }

        T y() const
        {
            return m_y;
        }
    private:
        T m_x;
        T m_y;
    };


    //pod structure for sorting draw commands.
    //exactly 64 bit, so it can be used as a int64_t key.
    struct Clip {
        int16_t x;
        int16_t y;
        int16_t width;
        int16_t height;
    };

    struct Texture {
        unsigned char* data;
        unsigned int components;
        unsigned int width;
        unsigned int height;
    };
}


#endif