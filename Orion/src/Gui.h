#ifndef GUI_H
#define GUI_H

#include <stack>
#include <map>
#include <memory>

#include "DataTypes.h"
#include "Color.h"
#include "Input.h"


namespace Orion {

    class Style;
    class Layout;
    class DrawQueue;
    class Font;

    //todo(should replace all the handles to font and texture with their own handle class)
    class Gui {
    public:
        //test constructor, will most likely be replaced with a struct containing settings
        Gui(const unsigned int& width, const unsigned int& height, Style* style);
        ~Gui();

        DrawQueue* queue();

        void clear();

        //creates baked font. parameter expects .ttf file raw data. Will allocate memory to bake the font chars.
        //returned handle can also be used to retrieve the baked texture and font structure.
        unsigned int createFont(void* data);

        //create texture - returns handle to the texture.
        unsigned int createTexture(const unsigned int& width, const unsigned int& height,
            const unsigned int& components, void* data);

        //set the current font
        void setFont(const unsigned int& i);

        //retrieve the texture that maps to the handle.
        Texture texture(unsigned int idx);

        //dynamic layout functions
        void beginLayout(Layout* layout);
        void endLayout();

        //window frame functions 
        void beginWindow(Layout* layout, bool dragable);
        void endWindow();

        void beginTextureId(const unsigned int& id);
        void endTextureId();

        //clipping
        void beginClipRect(const unsigned int& x, const unsigned int& y,
            const unsigned int& width, const unsigned int& height);
        void endClipRect();

        //dynamic layout widget functions
        bool button(const char* text);
        bool label(const char* text, const Color& color);

        //fixed layout widget functions
        bool button(const unsigned int& x, const unsigned int& y,
            const unsigned int& w, const unsigned int& h, const char* text);

        void label(const unsigned int& x, const unsigned int& y,
            const unsigned int& size, const char* text, const Color& color);

        //input interface
        void buttonDown(const KEY_INPUT& input);
        void buttonUp(const KEY_INPUT& input);
        void mouseMovement(const unsigned int& x, const unsigned int& y);
        void mouseDown(const MOUSE_INPUT& input);
        void mouseUp(const MOUSE_INPUT& input);
    private:
        std::stack<Layout*> m_layout;
        std::stack<Clip> m_clips;
        std::stack<unsigned int> m_textureId;

        std::map<unsigned int, Texture> m_textures;
        std::map<unsigned int, std::unique_ptr<Font>> m_fonts;
        std::map<KEY_INPUT, bool> m_keys;
        std::map<MOUSE_INPUT, bool> m_mouseButtons;
        Vec2<unsigned int> m_mousePos;


        std::unique_ptr<DrawQueue> m_queue;

        Style* m_style;

        //used to get an unique id for fonts and textures
        unsigned int m_counter;
        unsigned int m_currentAtlasId;

        uintptr_t m_hot;
        uintptr_t m_active;
    };

}



#endif