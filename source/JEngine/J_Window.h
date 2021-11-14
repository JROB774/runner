#ifndef INCLUDE_J_WINDOW
#define INCLUDE_J_WINDOW

class J_Window
{
    public:

        static void initialise();

        static void handle(const SDL_Event&, const bool);

        static void show();
        static void hide();

        static void setFullscreen(const bool);
        static void toggleFullscreen();

        static void setWidth(int);
        static void setHeight(int);

        static void updateScale(const bool);
        static void updateViewport(const int);

        static int getScreenWidthScaled();
        static int getScreenHeightScaled();
        static int getScreenWidth();
        static int getScreenHeight();
        static int getWidth(); // When in fullscreen mode this returns the width if the window was in windowed mode.
        static int getHeight(); // When in fullscreen mode this returns the height if the window was in windowed mode.
        static bool getFullscreen();
        static SDL_Window* getWindow();

        static void terminate();

    private:

        static const std::string WINDOW_FILE;
        static std::string title;
        static int startWidth, startHeight;
        static int cachedWidth, cachedHeight;
        static int screenWidthScaled, screenHeightScaled;;
        static int screenWidth, screenHeight;
        static bool fullscreen;
        static SDL_Window* window;
};

#endif // INCLUDE_J_WINDOW
