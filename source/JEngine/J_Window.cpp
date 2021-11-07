/// J_WINDOW //////////////////////////////////////////////////////////////////

const std::string J_Window::WINDOW_FILE = "Resources\\Data\\Window.dat";
std::string J_Window::title = "\0";
int J_Window::width = 0, J_Window::height = 0;
int J_Window::screenWidth = 0, J_Window::screenHeight = 0;
int J_Window::desktopWidth = 0, J_Window::desktopHeight = 0;
int J_Window::screenScale = 0;
bool J_Window::fullscreen = false;
SDL_Window* J_Window::window = nullptr;



void J_Window::initialise ()
{
    // If the window is already initialised, end the process.
    if (window != nullptr) { return; }

    // Open the window data file to extract data.
    std::ifstream windowFile(WINDOW_FILE, std::ifstream::in);

    if (windowFile.is_open())
    {
        std::string rawData = "\0";
        std::istringstream data;

        std::getline(windowFile, rawData);
        data.str(rawData);
        data >> title >> screenWidth >> screenHeight;

        windowFile.close();
    }
    else { J_Error::log("J_ERROR_WINDOW_FILE_READ"); }


    // Find out the display information of the user's desktop and store the resolution.
    SDL_DisplayMode display = {};
    int numberOfDisplays = SDL_GetNumVideoDisplays();

    for (int i = 0; i < numberOfDisplays; ++i)
    {
        if (SDL_GetDesktopDisplayMode(i, &display) != 0) { continue; }
        else { break; }
    }

    desktopWidth = display.w, desktopHeight = display.h;


    // Set the window's starting dimensions and scale.
    screenScale = 1;
    width = screenWidth * screenScale, height = screenHeight * screenScale;

    // The window itself is created using the informaation gathered from the data file.
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) { J_Error::log("J_ERROR_WINDOW_CREATE"); }


    // Set the window's minimum size to the screen dimensions and its maximum size to the desktop's dimensions.
    SDL_SetWindowMinimumSize(window, screenWidth, screenHeight);
    SDL_SetWindowMaximumSize(window, desktopWidth, desktopHeight);
}



void J_Window::handle (const SDL_Event& a_event, const bool a_debug)
{
    // Handle all the window related events.
    if (a_event.type == SDL_WINDOWEVENT)
    {
        if (a_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            width = a_event.window.data1, height = a_event.window.data2;

            if (((screenWidth * screenScale) > width) || ((screenHeight * screenScale) > height))
            {
                width = screenWidth * screenScale, height = screenHeight * screenScale;
                SDL_SetWindowSize(window, width, height);
            }

            updateViewport();
        }
    }

    // Handle all window debug events.
    if (a_debug)
    {
        if (a_event.type == SDL_KEYDOWN)
        {
            switch (a_event.key.keysym.sym)
            {
                case (SDLK_f) :
                {
                    toggleFullscreen();
                    break;
                }

                case (SDLK_0) :
                {
                    setScreenScale(getScreenScale() + 1);
                    break;
                }

                case (SDLK_9) :
                {
                    setScreenScale(getScreenScale() - 1);
                    break;
                }
            }
        }
    }
}



void J_Window::setScreenScale (const int a_scale)
{
    // Set the new screen scale.
    screenScale = a_scale;

    // Check to see if the new scale is out of bounds.
    while (((screenWidth * screenScale) > desktopWidth) || ((screenHeight * screenScale) > desktopHeight)) { --screenScale; }
    if (screenScale < 1) { screenScale = 1; }

    // If the new scale is bigger than the current window size, increase it.
    if (!fullscreen)
    {
        // if (((screenWidth * screenScale) > width) || ((screenHeight * screenScale) > height))
        // {
        width = screenWidth * screenScale, height = screenHeight * screenScale;
        SDL_SetWindowSize(window, width, height);
        // }
    }

    // Set the renderer scale.
    J_Renderer::setScale(static_cast <float> (screenScale), static_cast <float> (screenScale));

    // Update the viewport.
    updateViewport();
}



void J_Window::toggleFullscreen ()
{
    // Toggle the fullscreen value then set the window fullscreen based on what it now is.
    fullscreen = !fullscreen;

    if (fullscreen) { if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) { J_Error::log("J_ERROR_WINDOW_FULLSCREEN_ON"); } }
    else
    {
        if (SDL_SetWindowFullscreen(window, 0) != 0) { J_Error::log("J_ERROR_WINDOW_FULLSCREEN_OFF"); }

        width = screenWidth * screenScale, height = screenHeight * screenScale;
        SDL_SetWindowSize(window, width, height);
    }
}



void J_Window::updateViewport ()
{
    J_Quad viewport;
    viewport.quad = { ((J_Window::getWidth() / 2) - ((J_Window::getScreenWidth() * screenScale) / 2)) / screenScale,
                      ((J_Window::getHeight() / 2) - ((J_Window::getScreenHeight() * screenScale) / 2)) / screenScale,
                      J_Window::getScreenWidth(),
                      J_Window::getScreenHeight() };

    J_Renderer::setViewport(viewport);

}



int J_Window::getWidth ()
{
    return width;
}

int J_Window::getHeight ()
{
    return height;
}

int J_Window::getScreenWidth ()
{
    return screenWidth;
}

int J_Window::getScreenHeight ()
{
    return screenHeight;
}

int J_Window::getDesktopWidth ()
{
    return desktopWidth;
}

int J_Window::getDesktopHeight ()
{
    return desktopHeight;
}

int J_Window::getScreenScale ()
{
    return screenScale;
}

bool J_Window::getFullscreen ()
{
    return fullscreen;
}

SDL_Window* J_Window::getWindow ()
{
    return window;
}



void J_Window::terminate ()
{
    if (window != nullptr)
    {
        title = "\0";
        width = 0, height = 0;
        screenWidth = 0, screenHeight = 0;
        desktopWidth = 0, desktopHeight = 0;
        screenScale = 0;
        fullscreen = false;
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

/// J_WINDOW //////////////////////////////////////////////////////////////////
