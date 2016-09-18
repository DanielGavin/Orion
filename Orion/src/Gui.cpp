#include "Gui.h"

#include <memory>
#include <assert.h>

#include "Style.h"
#include "DrawQueue.h"
#include "Font.h"

namespace Orion {

    Gui::Gui(const unsigned int& width, const unsigned int& height, Style * style)
        : m_style(style), m_counter(1), m_queue(new DrawQueue), m_hot(0), m_active(0), m_currentAtlasId(0)
    {
        Clip clip;
        clip.width = (int16_t)width;
        clip.height = (int16_t)height;
        clip.x = 0;
        clip.y = 0;
        m_clips.push(clip);

        //set the clip to be the size of the window.
        m_queue->setClipRect(clip);
    }

    Gui::~Gui()
    {
    }

    DrawQueue* Gui::queue()
    {
        return m_queue.get();
    }

    void Gui::clear()
    {
        m_queue->clear();
    }

    unsigned int Gui::createFont(void* data)
    {
        Font* font = new Font(data);

        Texture texture;
        texture.components = 1;
        texture.data = font->bitmap();
        texture.width = font->width();
        texture.height = font->height();

        m_fonts[m_counter] = std::unique_ptr<Font>(font);
        m_textures[m_counter] = texture;

        return m_counter++;
    }

    unsigned int Gui::createTexture(const unsigned int& width, const unsigned int& height, const unsigned int& components, void * data)
    {
        return m_counter++;
    }

    void Gui::setFont(const unsigned int& i)
    {
        m_currentAtlasId = i;
        m_queue->setFont(m_fonts[i].get());
    }

    Texture Gui::texture(unsigned int idx)
    {
        return m_textures[idx];
    }

    bool Gui::button(const char* text)
    {
        return false;
    }

    bool Gui::label(const char* text, const Color& color = Color(255, 255, 255))
    {
        return false;
    }

    bool Gui::button(const unsigned int& x, const unsigned int& y, const unsigned int& w, const unsigned int& h, const char* text)
    {
        Vec2<float> p((float)x, (float)y);
        bool ret = false;

        //first check to see if the button is hovered over
        if (m_mousePos.x() >= x && m_mousePos.x() <= x + w &&m_mousePos.y() >= y && m_mousePos.y() <= y + h) {
            m_hot = (uintptr_t)text;
        }

        else if (m_hot == (uintptr_t)text) {
            m_hot = 0;
        }

        //if active that means that the button has been down pressed, but we now have to see if it's released while hovering over the button
        if (m_active == (uintptr_t)text) {
            if (!m_mouseButtons[MOUSE_BUTTON_LEFT]) {

                if (m_hot == (uintptr_t)text) {
                    ret = true;
                }

                m_active = 0;
            }
        }

        else if (m_hot == (uintptr_t)text) {

            if (m_mouseButtons[MOUSE_BUTTON_LEFT]) {
                m_active = (uintptr_t)text;
            }
            
        }

        m_style->drawButton(m_queue.get(), p, w, h, text);

        return ret;
    }

    void Gui::label(const unsigned int& x, const unsigned int& y,
        const unsigned int& size, const char * text, const Color& color)
    {
        Vec2<float> p((float)x, (float)y);
        beginTextureId(m_currentAtlasId);
        m_style->drawLabel(m_queue.get(), p, size, text, color);
        endTextureId();
    }

    void Gui::buttonDown(const KEY_INPUT& input)
    {
        m_keys[input] = true;
    }

    void Gui::buttonUp(const KEY_INPUT& input)
    {
        m_keys[input] = false;
    }

    void Gui::mouseMovement(const unsigned int& x, const unsigned int& y)
    {
        m_mousePos = Vec2<unsigned int>(x, y);
    }

    void Gui::mouseDown(const MOUSE_INPUT& input)
    {
        m_mouseButtons[input] = true;
    }

    void Gui::mouseUp(const MOUSE_INPUT& input)
    {
        m_mouseButtons[input] = false;
    }

    void Gui::beginTextureId(const unsigned int& id)
    {
        m_textureId.push(id);
        m_queue->setTextureId(m_textureId.top());
    }

    void Gui::endTextureId()
    {
        m_textureId.pop();

        if (!m_textureId.empty()) {
            m_queue->setTextureId(m_textureId.top());
        }
    }

    void Gui::beginClipRect(const unsigned int& x, const unsigned int& y, const unsigned int& width, const unsigned int& height)
    {
        Clip clip;
        clip.x = (int16_t)x;
        clip.y = (int16_t)y;
        clip.width = (int16_t)width;
        clip.height = (int16_t)height;

        m_clips.push(clip);
        m_queue->setClipRect(clip);
    }

    void Gui::endClipRect()
    {
        m_clips.pop();
        assert(!m_clips.empty());
        m_queue->setClipRect(m_clips.top());
    }

}
