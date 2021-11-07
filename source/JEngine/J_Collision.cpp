/// J_COLLIDER ////////////////////////////////////////////////////////////////

void J_Collider::render ()
{
    if (J_Collision::shouldRenderColliders())
    {
        if (J_Collision::shouldRenderFilled()) { J_Renderer::drawQuadFilled(quad); }
        else { J_Renderer::drawQuadOutline(quad); }
    }
}



inline bool operator== (const J_Collider& a_lhs, const J_Collider& a_rhs)
{
    return a_lhs.quad == a_rhs.quad;
}

inline bool operator!= (const J_Collider& a_lhs, const J_Collider& a_rhs)
{
    return !operator==(a_lhs, a_rhs);
}

/// J_COLLIDER ////////////////////////////////////////////////////////////////



/// J_COLLISION ///////////////////////////////////////////////////////////////

bool J_Collision::renderColliders = false;
bool J_Collision::renderCollidersFilled = false;



void J_Collision::handle (const SDL_Event& a_event)
{
    if (a_event.type == SDL_KEYDOWN)
    {
        switch (a_event.key.keysym.sym)
        {
            case (SDLK_1): { renderColliders = !renderColliders; break; }

            case (SDLK_2): { renderCollidersFilled = !renderCollidersFilled; break; }
        }
    }
}



bool J_Collision::collision (const J_Collider& a_first, const J_Collider& a_second)
{
    return ((a_first.quad.quad.x < (a_second.quad.quad.x + a_second.quad.quad.w)) &&
            ((a_first.quad.quad.x + a_first.quad.quad.w) > a_second.quad.quad.x) &&
            (a_first.quad.quad.y < (a_second.quad.quad.y + a_second.quad.quad.h)) &&
            ((a_first.quad.quad.y + a_first.quad.quad.h) > a_second.quad.quad.y)) ? true : false;
}

bool J_Collision::collision (const int a_x1, const int a_y1, const int a_x2, const int a_y2)
{
    return ((a_x1 == a_x2) && (a_y1 == a_y2)) ? true : false;
}

bool J_Collision::collision (const int a_x, const int a_y, const J_Collider& a_collider)
{
    return ((a_x >= a_collider.quad.quad.x) &&
            (a_x < (a_collider.quad.quad.x + a_collider.quad.quad.w)) &&
            (a_y >= a_collider.quad.quad.y) &&
            (a_y < (a_collider.quad.quad.y + a_collider.quad.quad.h))) ? true : false;
}



bool J_Collision::shouldRenderColliders ()
{
    return renderColliders;
}

bool J_Collision::shouldRenderFilled ()
{
    return renderCollidersFilled;
}

/// J_COLLISION ///////////////////////////////////////////////////////////////
