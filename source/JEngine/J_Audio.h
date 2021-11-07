#ifndef INCLUDE_J_AUDIO
#define INCLUDE_J_AUDIO

class J_Sound
{
    public:

        J_Sound();
        J_Sound(const J_Sound&) = delete;
        J_Sound(J_Sound&&) = delete;

        J_Sound& operator=(const J_Sound&) = delete;
        J_Sound& operator=(J_Sound&&) = delete;

        void create(const std::string, const int = -1);

        void play(const int);

        void destroy();
        ~J_Sound();

    private:

        int channel;
        Mix_Chunk* chunk;
};



class J_Mixer
{
    public:

        static void initialise();

        static void handle(const SDL_Event&);

        static void setChannels(const int);
        static void setSoundVolume(const float);
        static void toggleMute();

        static int getChannels();
        static float getSoundVolume();
        static float getVolumePiece();
        static bool isMuted();

        static void terminate();

    private:

        static const std::string AUDIO_FILE;
        static int maxChannels;
        static float soundVolume;
        static float volumePiece;
        static bool muted;
};

#endif // INCLUDE_J_AUDIO
