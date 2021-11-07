#ifndef INCLUDE_J_COLLISION
#define INCLUDE_J_COLLISION

#include "J_Render.h"

struct J_Collider
{
    public:

        J_Quad quad;

        void render();
};

inline bool operator==(const J_Collider&, const J_Collider&);
inline bool operator!=(const J_Collider&, const J_Collider&);



class J_Collision
{
    public:

        static void handle(const SDL_Event&);

        static bool collision(const J_Collider&, const J_Collider&);
        static bool collision(const int, const int, const int, const int);
        static bool collision(const int, const int, const J_Collider&);

        static bool shouldRenderColliders();
        static bool shouldRenderFilled();

    private:

        static bool renderColliders;
        static bool renderCollidersFilled;
};

#endif // INCLUDE_J_COLLISION
