/// J_WINDOW //////////////////////////////////////////////////////////////////

const std::string J_Window::WINDOW_FILE = RES_DIR_DATA "Window.dat";
std::string J_Window::title = "\0";
int J_Window::startWidth = 0, J_Window::startHeight = 0;
int J_Window::cachedWidth = 0, J_Window::cachedHeight = 0;
int J_Window::screenWidthScaled = 0, J_Window::screenHeightScaled = 0;
int J_Window::screenWidth = 0, J_Window::screenHeight = 0;
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
        data >> title >> screenWidth >> screenHeight >> startWidth >> startHeight;

        windowFile.close();
    }
    else { J_Error::log("J_ERROR_WINDOW_FILE_READ"); }

    // The window itself is created using the informaation gathered from the data file.
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, startWidth, startHeight, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) { J_Error::log("J_ERROR_WINDOW_CREATE"); }

    // Set the window's scale.
    updateScale(false);

    // Set the window's minimum size to the screen dimensions.
    SDL_SetWindowMinimumSize(window, screenWidth, screenHeight);
}



void J_Window::handle (const SDL_Event& a_event, const bool a_debug)
{
    // Handle all the window related events.
    if (a_event.type == SDL_WINDOWEVENT)
    {
        if (a_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            updateScale(true);
        }
    }

    // Handle all window debug events.
    #ifndef PLATFORM_WEB
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
            }
        }
    }
    #endif
}



void J_Window::step ()
{
    // Monitor if the fullscreen state has changed and update the flag accordingly.
    // This is needed on the web where forces external to the game can change the
    // fullscreen state. If we don't update this then the flag will be out of sync.
    bool isFullscreen = SDL_GetWindowFlags(window) & (SDL_WINDOW_FULLSCREEN|SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (isFullscreen != fullscreen) { fullscreen = isFullscreen;  }
}



void J_Window::show ()
{
    SDL_ShowWindow(window);
}

void J_Window::hide ()
{
    SDL_HideWindow(window);
}



void J_Window::setFullscreen (const bool a_fullscreen)
{
    fullscreen = a_fullscreen;

    SDL_ShowCursor((fullscreen) ? SDL_DISABLE : SDL_ENABLE);

    if(fullscreen)
    {
        SDL_GetWindowSize(window, &cachedWidth, &cachedHeight); // Cache the current width and height in windowed mode.
    }

    if (fullscreen) { if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) { J_Error::log("J_ERROR_WINDOW_FULLSCREEN_ON"); } }
    else { if (SDL_SetWindowFullscreen(window, 0) != 0) { J_Error::log("J_ERROR_WINDOW_FULLSCREEN_OFF"); } }
}

void J_Window::toggleFullscreen ()
{
    setFullscreen(!fullscreen);
}



void J_Window::setWidth (int a_width)
{
    if (fullscreen) { return; }
    int height;
    if (a_width <= 0) { a_width = startWidth; }
    SDL_GetWindowSize(window, NULL, &height);
    SDL_SetWindowSize(window, a_width, height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void J_Window::setHeight (int a_height)
{
    if (fullscreen) { return; }
    if (a_height <= 0) { a_height = startHeight; }
    int width;
    SDL_GetWindowSize(window, &width, NULL);
    SDL_SetWindowSize(window, width, a_height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}



void J_Window::updateScale (const bool a_updateRenderScaleAndViewport)
{
    // Calculate the new screen scale based on the current window size.
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int screenScale = 1;
    while(((screenWidth * (screenScale+1)) <= windowWidth) && ((screenHeight * (screenScale+1)) <= windowHeight))
    {
        screenScale++;
    }

    screenWidthScaled = screenWidth * screenScale;
    screenHeightScaled = screenHeight * screenScale;

    // Set the renderer scale and viewport.
    if(a_updateRenderScaleAndViewport)
    {
        J_Renderer::setScale(static_cast <float> (screenScale), static_cast <float> (screenScale));
        updateViewport(screenScale);
    }
}

void J_Window::updateViewport (const int a_screenScale)
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    J_Quad viewport;
    viewport.quad.x = ((windowWidth - screenWidthScaled) / 2) / a_screenScale;
    viewport.quad.y = ((windowHeight - screenHeightScaled) / 2) / a_screenScale;
    viewport.quad.w = screenWidth;
    viewport.quad.h = screenHeight;

    J_Renderer::setViewport(viewport);
}



int J_Window::getScreenWidthScaled ()
{
    return screenWidthScaled;
}

int J_Window::getScreenHeightScaled ()
{
    return screenHeightScaled;
}

int J_Window::getScreenWidth ()
{
    return screenWidth;
}

int J_Window::getScreenHeight ()
{
    return screenHeight;
}

int J_Window::getWidth ()
{
    int width;
    if (fullscreen) { width = cachedWidth; }
    else { SDL_GetWindowSize(window, &width, NULL); }
    return (width == startWidth) ? 0 : width;
}

int J_Window::getHeight ()
{
    int height;
    if (fullscreen) { height = cachedHeight; }
    else { SDL_GetWindowSize(window, NULL, &height); }
    return (height == startHeight) ? 0 : height;
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
        startWidth = 0, startHeight = 0;
        cachedWidth = 0, cachedHeight = 0;
        screenWidthScaled = 0, screenHeightScaled = 0;
        screenWidth = 0, screenHeight = 0;
        fullscreen = false;
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

/// J_WINDOW //////////////////////////////////////////////////////////////////
