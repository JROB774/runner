/// J_SYSTEM //////////////////////////////////////////////////////////////////

const std::string J_System::SYSTEM_FILE = "Resources\\Data\\System.dat";
const std::string J_System::CONFIG_FILE = "Resources\\Data\\Config.dat";
SDL_Event J_System::event;
J_Timer J_System::fpsTimer, J_System::capTimer;
int J_System::fps = 0, J_System::tpf = 0;
int J_System::frameCount = 0;
double J_System::averageFps = 0.0;
bool J_System::debug = false;
int J_System::state = -1;



void J_System::initialise ()
{
    // Open the system data file to extract the data.
    std::ifstream systemFile(SYSTEM_FILE, std::ifstream::in);

    if (systemFile.is_open())
    {
        std::string rawData = "\0";
        std::istringstream data;

        std::getline(systemFile, rawData);
        data.str(rawData);
        data >> fps >> debug;

        tpf = 1000 / fps;

        systemFile.close();
    }
    else { J_Error::log("J_ERROR_SYSTEM_FILE_READ"); }


    // If the system is not resetting initialise all the SDL sub-systems.
    if (state != STATE_RESETTING)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { J_Error::log("J_ERROR_SYSTEM_SDL_INIT"); }
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) { J_Error::log("J_ERROR_SYSTEM_MIX_INIT"); }
    }


    // Initialise all the J-Engine sub-systems.
    J_Error::initialise();
    J_Window::initialise();
    J_Renderer::initialise(J_Window::getWindow());
    J_Mixer::initialise();
    J_Input::initialise(&event);


    // Open the config data file to extract the data.
    std::ifstream configFile(CONFIG_FILE, std::ifstream::in);
    std::string rawData = "\0";

    if (configFile.is_open())
    {
        std::getline(configFile, rawData);
        configFile.close();
    }

    std::istringstream data;
    data.str(rawData);

    bool configFullscreen = false;
    int configScale = 0; // This is now unused! (v1.3.0)
    float configSoundVolume = 0.0;
    bool configMuted = false;

    data >> configFullscreen >> configScale >> configSoundVolume >> configMuted;


    // Set some starting values stored in the config file.
    if (configFullscreen && !J_Window::getFullscreen()) { J_Window::toggleFullscreen(); }
    J_Mixer::setSoundVolume(configSoundVolume);
    if (configMuted && !J_Mixer::isMuted()) { J_Mixer::toggleMute(); }

    // Set the system state to active.
    state = STATE_ACTIVE;
}



int J_System::pollEvent ()
{
    // Return the value from SDL's poll event function.
    return SDL_PollEvent(&event);
}

void J_System::handle ()
{
    // Handle the system events.
    switch (event.type)
    {
        case (SDL_QUIT) : { state = STATE_INACTIVE; break; }

        case (SDL_KEYDOWN) :
        {
            if (debug)
            {
                switch (event.key.keysym.sym)
                {
                    case (SDLK_ESCAPE) : { state = STATE_INACTIVE; break; }
                    case (SDLK_r) : { state = STATE_RESETTING; break; }
                }
            }

            break;
        }
    }

    // Handle all sub-system events
    if (debug)
    {
        J_Collision::handle(event);
        J_Mixer::handle(event);
    }

    J_Window::handle(event, debug);
}



void J_System::stepBegin ()
{
    // Calculate the average FPS.
    averageFps = frameCount / (fpsTimer.getTicks() / 1000.0);
    if (averageFps > 2000000.0) { averageFps = 0.0; }

    // if (debug) { printf("FPS: %f\n", averageFps); }

    // Set the renderer colour to default and clear the screen.
    J_Renderer::setColour(J_Renderer::DEFAULT_COLOUR);
    J_Renderer::clear();
}

void J_System::stepEnd ()
{
    // Update the screen with everyting that needs to be rendered.
    J_Renderer::update();

    // Incrment the frame count.
    int frameTicks = capTimer.getTicks();
    if (frameTicks < tpf) { SDL_Delay(tpf - frameTicks); }

    // if (debug) { printf("%d : %d\n", frameTicks, tpf - frameTicks); }

    ++frameCount;
}



void J_System::start ()
{
    state = STATE_ACTIVE;
}

void J_System::restart ()
{
    state = STATE_RESETTING;
}

void J_System::stop ()
{
    state = STATE_INACTIVE;
}



SDL_Event J_System::getEvent ()
{
    return event;
}

J_Timer* J_System::getFpsTimer ()
{
    return &fpsTimer;
}

J_Timer* J_System::getCapTimer ()
{
    return &capTimer;
}

int J_System::getFps ()
{
    return fps;
}

bool J_System::isDebug ()
{
    return debug;
}

int J_System::getState ()
{
    return state;
}



void J_System::terminate ()
{
    // Reset everything.
    fpsTimer.stop(), capTimer.stop();
    frameCount = 0;
    averageFps = 0.0;

    // Terminate all the J-Engine sub-systems.
    J_Input::terminate();
    J_Mixer::terminate();
    J_Renderer::terminate();
    J_Window::terminate();
    J_Error::terminate();


    // If the system is not resetting then terminate all the SDL sub-systems.
    if (state != STATE_RESETTING)
    {
        Mix_CloseAudio();
        SDL_Quit();
    }
}

/// J_SYSTEM //////////////////////////////////////////////////////////////////
