/// J_ERROR ///////////////////////////////////////////////////////////////////

const std::string J_Error::ERROR_FILE = RES_DIR_DATA "Error.dat";
const std::string J_Error::ERROR_LOG = "Error.log";
int J_Error::total = 0;
std::unordered_map <std::string, std::string> J_Error::error;



void J_Error::initialise ()
{
    #ifndef PLATFORM_WEB

    if (!error.empty()) { return; }

    std::ifstream errorFile(ERROR_FILE);

    if (errorFile.is_open())
    {
        std::string dummy = "\0";
        while (std::getline(errorFile, dummy)) { ++total; }
    }
    else { messageBox("FATAL ERROR\nCould not initialise the error system!"); }

    std::string* line = new(std::nothrow) std::string[total];
    if (line == nullptr) { messageBox("FATAL ERROR\nCould not initialise the error system!"); }

    errorFile.clear();
    errorFile.seekg(0, std::ifstream::beg);

    if (errorFile.is_open())
    {
        for (int i = 0; i < total; ++i) { std::getline(errorFile, line[i]); }
        errorFile.close();
    }
    else { messageBox("FATAL ERROR\nCould not initialise the error system!"); }

    std::string* key = new(std::nothrow) std::string[total];
    if (key == nullptr) { messageBox("FATAL ERROR\nCould not initialise the error system!"); }
    std::string* content = new(std::nothrow) std::string[total];
    if (content == nullptr) { messageBox("FATAL ERROR\nCould not initialise the error system!"); }

    for (int i = 0; i < total; ++i)
    {
        char* data = new(std::nothrow) char[line[i].length() + 1];
        if (data == nullptr) { messageBox("FATAL ERROR\nCould not initialise the error system!"); }
        strcpy(data, line[i].c_str());

        key[i] = strtok(data, "=");
        content[i] = strtok(nullptr, "=");

        delete[] data;
        data = nullptr;
    }

    for (int i = 0; i < total; ++i) { error.insert({ key[i], content[i] }); }

    delete[] line;
    line = nullptr;
    delete[] key;
    key = nullptr;
    delete[] content;
    content = nullptr;

    #endif
}



void J_Error::log (const std::string a_key)
{
    #ifndef PLATFORM_WEB

    if (error.empty()) { messageBox("FATAL ERROR\nCould not write an error to the log!"); }

    std::string key = a_key;
    if (error.count(key) == 0) { key = "J_ERROR_UNKNOWN"; }

    char* data = new(std::nothrow) char[error.at(key).length() + 1];
    if (data == nullptr) { messageBox("FATAL ERROR\nCould not write an error to the log!"); }
    strcpy(data, error.at(key).c_str());

    std::string output = strtok(data, ":");
    int type = atoi(strtok(nullptr, ":"));

    delete[] data;
    data = nullptr;

    std::ofstream errorLog(FILESYS_NAME(ERROR_LOG), std::ofstream::app);

    if (errorLog.is_open())
    {
        errorLog << output;

        switch (type)
        {
            case (TYPE_SDL): { errorLog << " (" << SDL_GetError() << ")"; break; }
            case (TYPE_MIX): { errorLog << " (" << Mix_GetError() << ")"; break; }
        }

        errorLog << "\n";
        errorLog.close();

        FILESYS_SYNC();
    }
    else { messageBox("FATAL ERROR\nCould not write an error to the log!"); }

    messageBox("An error has occured!\nCheck the 'Error.log' for more details.");

    #endif
}



void J_Error::terminate ()
{
    #ifndef PLATFORM_WEB

    if (!error.empty())
    {
        total = 0;
        error.clear();
    }

    #endif
}



void J_Error::messageBox (const std::string a_message)
{
    #ifndef PLATFORM_WEB

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "J-Engine Error", a_message.c_str(), J_Window::getWindow());
    exit(EXIT_FAILURE);

    #endif
}

/// J_ERROR ///////////////////////////////////////////////////////////////////
