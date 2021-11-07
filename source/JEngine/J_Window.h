#ifndef INCLUDE_J_WINDOW
#define INCLUDE_J_WINDOW

class J_Window
{
    public:

        static void initialise();

        static void handle(const SDL_Event&, const bool);

        static void setIcon(const std::string);
        static void setScreenScale(const int);
        static void toggleFullscreen();
        static void updateViewport();

        static int getWidth();
        static int getHeight();
        static int getScreenWidth();
        static int getScreenHeight();
        static int getDesktopWidth();
        static int getDesktopHeight();
        static int getScreenScale();
        static bool getFullscreen();
        static SDL_Window* getWindow();

        static void terminate();

    private:

        static const std::string WINDOW_FILE;
        static std::string title;
        static int width, height;
        static int screenWidth, screenHeight;
        static int desktopWidth, desktopHeight;
        static int screenScale;
        static bool fullscreen;
        static SDL_Window* window;
};

#endif // INCLUDE_J_WINDOW
