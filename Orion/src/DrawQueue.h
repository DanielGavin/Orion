#ifndef DRAWQUEUE_H
#define DRAWQUEUE_H

#include <vector>
#include <map>

#include "Datatypes.h"
#include "Color.h"

namespace Orion {

    class Font;

    typedef std::vector<float> CmdDataVector;

    struct CmdInfo {
        unsigned int offset;
        unsigned int size;
        unsigned int textureId;
        Clip clip;
    };

    class DrawQueue {
    public:
        DrawQueue();

        //converts all the commands to one large buffer
        std::pair<std::vector<float>&, std::vector<CmdInfo>&> createScene();

        //clear the commands
        void clear();

        void drawPrimTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Color& color);

        void drawPrimRect(const Vec2<float>& p, const float& width, const float& height, const Color& color);

        void drawText(const Vec2<float>& p, const unsigned int& size, const char* text, const Color& color);

        //geometry with thickness made with lines
        void drawTriangle(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const float& thickness, const Color& color);

        //Lets the DrawQueue know which clipping rect is currently used. 
        void setClipRect(const Clip& clip);

        //set the depth of the draw commands
        void setDepth(const unsigned int& depth);

        //set the current atlas and font
        void setFont(Font* font);

        //set the texture id for textured rects and fonts
        void setTextureId(const unsigned int& idx);
    private:
        //current clip region
        Clip m_currentClip;

        //current id texture
        int m_currentTextureId;

        //current z depth
        unsigned int m_depth;

        //cuurent baked atlas info
        Font* m_font;

        //final version of the data to be rendered
        std::vector<float> m_buffer;
        std::vector<CmdInfo> m_commands;

        //keep the triangles sorted by atlas texture id and clip(0 is untextured)
        std::map<int, std::map<int64_t, CmdDataVector>> m_cmdTriangles;

        //keep the lines sorted by clip
        std::map<int, CmdDataVector> m_cmdLines;
    };

}


#endif