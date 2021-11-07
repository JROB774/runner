#ifndef INCLUDE_BUTTON
#define INCLUDE_BUTTON

class Button
{
    public:

        Button() = default;
        Button(const Button&) = delete;
        Button(Button&&) = delete;

        Button& operator=(const Button&) = delete;
        Button& operator=(Button&&) = delete;

        void create(const int, const int, const std::string, J_Font*, void (*)());

        void render();

        void press();

        void select();
        void deselect();

        void destroy();
        ~Button();

    private:

        enum Sprite
        {
            SPRITE_LEFT,
            SPRITE_MIDDLE,
            SPRITE_RIGHT,
            SPRITE_TOTAL
        };

        static const int SPRITE_WIDTH, SPRITE_HEIGHT;

        J_Point pos;
        std::string text;
        J_Image image;
        J_Quad sprite[SPRITE_TOTAL];
        J_Font* font;
        void (*process)();
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

        void create(const int, const int, const std::string, J_Font*, void (*)());

        void render();

        void forward();
        void back();

        void press();

        static void terminate();

        void destroy();
        ~ButtonList();

    private:

        int position;
        std::vector <Button*> button;
        static J_Sound sound;

};

#endif // INCLUDE_BUTTON
