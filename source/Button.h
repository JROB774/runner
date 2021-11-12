#ifndef INCLUDE_BUTTON
#define INCLUDE_BUTTON

class Button;

typedef void(*ButtonCallback)(Button*, const int);

class Button
{
    public:

        enum Type
        {
            TYPE_PRESS,
            TYPE_SLIDER,
            TYTPE_TOTAL
        };

        Button() = default;
        Button(const Button&) = delete;
        Button(Button&&) = delete;

        Button& operator=(const Button&) = delete;
        Button& operator=(Button&&) = delete;

        void create(const Type, const int, const int, const std::string, J_Font*, ButtonCallback);

        void render();

        void press();

        void increment();
        void decrement();

        void select();
        void deselect();

        void updatePosition(const int, const int);
        void updateText(const std::string);

        Type getType() const;

        void destroy();
        ~Button();

    private:

        enum Sprite
        {
            SPRITE_LEFT,
            SPRITE_MIDDLE,
            SPRITE_RIGHT,
            SPRITE_LARROW,
            SPRITE_RARROW,
            SPRITE_TOTAL
        };

        static const int SPRITE_WIDTH, SPRITE_HEIGHT;
        static const int ARROW_WIDTH, ARROW_HEIGHT;

        Type type;
        J_Point pos;
        std::string text;
        J_Image image;
        J_Quad sprite[SPRITE_TOTAL];
        J_Font* font;
        ButtonCallback process;
        bool selected;
};



class ButtonList
{
    public:

        ButtonList() = default;
        ButtonList(const ButtonList&) = delete;
        ButtonList(ButtonList&&) = delete;

        ButtonList& operator=(const ButtonList&) = delete;
        ButtonList& operator=(ButtonList&&) = delete;

        static void initialise();
        static void terminate();

        void create(const Button::Type, const int, const int, const std::string, J_Font*, ButtonCallback);

        void render();

        void forward();
        void back();

        void press();

        void increment();
        void decrement();

        void destroy();
        ~ButtonList();

    private:

        int position;
        std::vector <Button*> button;
        static J_Sound sound;

};

#endif // INCLUDE_BUTTON
