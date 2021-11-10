/// J_SOUND ///////////////////////////////////////////////////////////////////

J_Sound::J_Sound ()
{
    channel = 0;
    chunk = nullptr;
}



void J_Sound::create (const std::string a_file, const int a_channel)
{
    if (chunk != nullptr) { destroy(); }

    std::string directory = RES_DIR_SOUNDS + a_file + ".wav";
    chunk = Mix_LoadWAV(directory.c_str());
    if (chunk == nullptr) { J_Error::log("J_ERROR_SOUND_CREATE"); }

    channel = a_channel;
    if (channel > J_Mixer::getChannels()) { J_Mixer::setChannels(channel); }
}



void J_Sound::play (const int a_loops)
{
    Mix_PlayChannel(channel, chunk, a_loops);
}



void J_Sound::destroy ()
{
    if (chunk != nullptr)
    {
        channel = 0;
        Mix_FreeChunk(chunk);
        chunk = nullptr;
    }
}

J_Sound::~J_Sound ()
{
    destroy();
}

/// J_SOUND ///////////////////////////////////////////////////////////////////



/// J_MIXER ///////////////////////////////////////////////////////////////////

const std::string J_Mixer::AUDIO_FILE = RES_DIR_DATA "Audio.dat";
int J_Mixer::maxChannels = 0;
float J_Mixer::soundVolume = 0.0;
bool J_Mixer::muted = false;



void J_Mixer::initialise ()
{
    std::ifstream audioFile(AUDIO_FILE, std::ifstream::in);

    if (audioFile.is_open())
    {
        std::string rawData = "\0";
        std::istringstream data;

        std::getline(audioFile, rawData);
        data.str(rawData);
        data >> maxChannels;

        audioFile.close();
    }
    else { J_Error::log("J_ERROR_MIXER_FILE_READ"); }

    Mix_AllocateChannels(maxChannels);
}



void J_Mixer::setChannels (const int a_channels)
{
    Mix_AllocateChannels(a_channels);
}

void J_Mixer::setSoundVolume (const float a_volume)
{
    soundVolume = a_volume;

    if(soundVolume > 1.0f) { soundVolume = 1.0f; }
    if(soundVolume < 0.0f) { soundVolume = 0.0f; }

    int volume = (int)((float)MIX_MAX_VOLUME * soundVolume);

    Mix_Volume(-1, volume);
}

void J_Mixer::setMute (const bool a_mute)
{
    int volume = (int)((float)MIX_MAX_VOLUME * soundVolume);

    muted = a_mute;

    if (muted) { Mix_Volume(-1, 0); }
    else { Mix_Volume(-1, volume); }
}

void J_Mixer::toggleMute ()
{
    setMute(!muted);
}



int J_Mixer::getChannels ()
{
    return Mix_AllocateChannels(-1);
}

float J_Mixer::getSoundVolume ()
{
    return soundVolume;
}

bool J_Mixer::isMuted ()
{
    return muted;
}



void J_Mixer::terminate ()
{
    maxChannels = 0;
    soundVolume = 0.0;
    muted = false;
}

/// J_MIXER ///////////////////////////////////////////////////////////////////
