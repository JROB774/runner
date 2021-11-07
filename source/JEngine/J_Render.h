#ifndef INCLUDE_J_RENDER
#define INCLUDE_J_RENDER

#include <sstream>
#include <vector>
#include "J_Error.h"

struct J_Colour
{
    public:

        Uint8 red, green, blue, alpha;
        SDL_BlendMode blendmode;
};

inline bool operator==(const J_Colour&, const J_Colour&);
inline bool operator!=(const J_Colour&, const J_Colour&);



struct J_Point
{
    public:

        SDL_Point point;
        J_Colour colour;
};

inline bool operator==(const J_Point&, const J_Point&);
inline bool operator!=(const J_Point&, const J_Point&);



struct J_Line
{
    public:

        SDL_Point start;
        SDL_Point end;
        J_Colour colour;
};

inline bool operator==(const J_Line&, const J_Line&);
inline bool operator!=(const J_Line&, const J_Line&);



struct J_Quad
{
    public:

        SDL_Rect quad;
        J_Colour colour;
};

inline bool operator==(const J_Quad&, const J_Quad&);
inline bool operator!=(const J_Quad&, const J_Quad&);



class J_Image
{
    public:

        J_Image();
        J_Image(const J_Image&) = delete;
        J_Image(J_Image&&) = delete;

        J_Image& operator= (const J_Image&) = delete;
        J_Image& operator= (J_Image&&) = delete;

        void create(const std::string);

        void render(const int, const int, J_Quad*);
        void render(const int, const int, const double, const J_Point*, const SDL_RendererFlip, J_Quad*);

        void setColour(const J_Colour);

        int getWidth() const;
        int getHeight() const;
        J_Point getCentre() const;
        void getCentre(int*, int*) const;
        J_Colour getColour() const;

        void destroy();
        ~J_Image();

    private:

        int width, height;
        J_Colour colour;
        SDL_Texture* texture;
};



class J_Animation
{
    public:

        enum Play
        {
            PLAY_BACKWARD,
            PLAY_FORWARD
        };

        J_Animation();
        J_Animation(const J_Animation&) = delete;
        J_Animation(J_Animation&&) = delete;

        J_Animation& operator=(const J_Animation&) = delete;
        J_Animation& operator=(J_Animation&&) = delete;

        void create(const std::string);

        J_Quad* play();
        void restart();

        void setDirection(const int);

        bool isDone() const;
        int getDirection() const;
        int getLength() const;

        void destroy();
        ~J_Animation();

    private:

        int spriteTotal;
        std::vector <J_Quad> sprite;
        std::vector <int> delay;
        int loops;
        int currentLoop;
        bool done;
        int direction;
        int currentSprite;
        int delayCounter;
};



class J_Background
{
    public:

        enum Move
        {
            MOVE_STATIONARY,
            MOVE_NORTH,
            MOVE_EAST,
            MOVE_SOUTH,
            MOVE_WEST
        };

        J_Background();
        J_Background(const J_Background&) = delete;
        J_Background(J_Background&&) = delete;

        J_Background& operator=(const J_Background&) = delete;
        J_Background& operator=(J_Background&&) = delete;

        void create(const std::string);

        void render();

        void restart();
        void stop();

        void setColour(const J_Colour&);

        int getSpeed() const;
        int getDirection() const;
        J_Colour getColour() const;

        void destroy();
        ~J_Background();

    private:

        int x, y;
        int speed;
        int offset;
        int prevDirection;
        int direction;
        J_Image image;
};



class J_Font
{
    public:

        static const int CHARACTER_TOTAL;

        J_Font();
        J_Font(const J_Font&) = delete;
        J_Font(J_Font&&) = delete;

        J_Font& operator=(const J_Font&) = delete;
        J_Font& operator=(J_Font&&) = delete;

        void create(const std::string);

        void render(const int, const int, const std::string, const J_Colour& = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND });

        int getCharWidth() const;
        int getCharHeight() const;

        void destroy();
        ~J_Font();

    private:

        int characterWidth, characterHeight;
        J_Quad* character;
        J_Image image;
};



class J_Renderer
{
    friend class J_Image;

    public:

        static const J_Colour DEFAULT_COLOUR;

        static void initialise(SDL_Window*);

        static void drawPoint(const J_Point&);
        static void drawLine(const J_Line&);
        static void drawQuadOutline(const J_Quad&);
        static void drawQuadFilled(const J_Quad&);

        static void clear();
        static void update();

        static void setViewport(const J_Quad);
        static void setScale(const float, const float);
        static void setColour(const J_Colour&);

        static void getViewport(J_Quad*);
        static void getScale(float*, float*);
        static J_Colour getColour();

        static void terminate();

    private:

        static SDL_Renderer* renderer;
};

#endif // INCLUDE_J_RENDER
