#ifndef INCLUDE_PLAYER
#define INCLUDE_PLAYER

class Player
{
    public:

        enum Key
        {
            KEY_JUMP,
            KEY_SLIDE,
            KEY_PAUSE,
            KEY_TOTAL
        };

        static void initialise(const bool);

        static void handle();

        static void step(const J_Collider&);

        static void render();

        static void kill();

        static J_Collider getCollider();
        static bool isDead();
        static int getKey(const int);

        static void terminate();

    private:

        enum State
        {
            STATE_RUN,
            STATE_JUMP,
            STATE_SLIDE,
            STATE_DEAD
        };

        static const std::string KEY_FILE;
        static const std::string SAVE_FILE;
        static const J_Point STARTING_POS;
        static J_Point pos;
        static J_Vector vel;
        static const int FLOAT_LENGTH;
        static const int GRAVITY;
        static const int MAX_JUMP_POWER;
        static int jumpPower;
        static J_Image image;
        static J_Animation runA, jumpA, slideA, deadA;
        static J_Sound runS, jumpS, deadS;
        static int runCounter;
        static J_Collider collider[3];
        static int key[KEY_TOTAL];
        static bool slide;
        static int state;
};

#endif // INCLUDE_PLAYER
