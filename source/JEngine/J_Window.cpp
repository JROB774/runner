/// J_WINDOW //////////////////////////////////////////////////////////////////

const std::string J_Window::WINDOW_FILE = RES_DIR_DATA "Window.dat";
std::string J_Window::title = "\0";
int J_Window::width = 0, J_Window::height = 0;
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
        data >> title >> screenWidth >> screenHeight;

        windowFile.close();
    }
    else { J_Error::log("J_ERROR_WINDOW_FILE_READ"); }

    // Set the window's starting dimensions and scale.
    updateScale(false);

    // The window itself is created using the informaation gathered from the data file.
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) { J_Error::log("J_ERROR_WINDOW_CREATE"); }

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
}



void J_Window::toggleFullscreen ()
{
    // Toggle the fullscreen value then set the window fullscreen based on what it now is.
    fullscreen = !fullscreen;

    if (fullscreen) { if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) { J_Error::log("J_ERROR_WINDOW_FULLSCREEN_ON"); } }
    else { if (SDL_SetWindowFullscreen(window, 0) != 0) { J_Error::log("J_ERROR_WINDOW_FULLSCREEN_OFF"); } }
}



void J_Window::updateScale (bool updateRenderScaleAndViewport)
{
    // Calculate the new screen scale based on the current window size.
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int screenScale = 1;
    while(((screenWidth * (screenScale+1)) <= windowWidth) && ((screenHeight * (screenScale+1)) <= windowHeight))
    {
        screenScale++;
    }

    width = screenWidth * screenScale;
    height = screenHeight * screenScale;

    // Set the renderer scale and viewport.
    if(updateRenderScaleAndViewport)
    {
        J_Renderer::setScale(static_cast <float> (screenScale), static_cast <float> (screenScale));
        updateViewport(screenScale);
    }
}



void J_Window::updateViewport (int screenScale)
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    J_Quad viewport;
    viewport.quad.x = ((windowWidth - width) / 2) / screenScale;
    viewport.quad.y = ((windowHeight - height) / 2) / screenScale;
    viewport.quad.w = screenWidth;
    viewport.quad.h = screenHeight;

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
        fullscreen = false;
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

/// J_WINDOW //////////////////////////////////////////////////////////////////
