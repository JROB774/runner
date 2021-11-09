/// ENTITY ////////////////////////////////////////////////////////////////////

bool Entity::halloween = false;
int Entity::speed = 0;



Entity::Entity (const int a_spr_w, const int a_spr_h, const int a_off_x, const int a_off_y):
                pos({ { 0, 0 } }),
                SPRITE_WIDTH(a_spr_w), SPRITE_HEIGHT(a_spr_h),
                COLLIDER_X_OFFSET(a_off_x), COLLIDER_Y_OFFSET(a_off_y),
                dead(true) {}



void Entity::initialise (const bool a_halloween)
{
    halloween = a_halloween;
}



void Entity::step ()
{
    if (!dead)
    {
        pos.point.x -= speed;
        collider.quad.quad.x = (pos.point.x + COLLIDER_X_OFFSET);
    }
}



void Entity::render ()
{
    if (!dead)
    {
        image.render(pos.point.x, pos.point.y, animation.play());
        collider.render();
    }
}



void Entity::setSpeed (const int a_speed)
{
    speed = a_speed;
}



const J_Point& Entity::getPosition () const
{
    return pos;
}

const J_Collider& Entity::getCollider () const
{
    return collider;
}



void Entity::destroy ()
{
    pos = { { 0, 0 } };
    image.destroy();
    animation.destroy();
    sound.destroy();
    collider.quad.quad = { 0, 0, 0, 0 };
    dead = true;
}

Entity::~Entity ()
{
    destroy();
}

/// ENTITY ////////////////////////////////////////////////////////////////////



/// END ///////////////////////////////////////////////////////////////////////

End::End (): Entity(0, 0, 0, 0) {}



void End::create (const int a_x)
{
    pos.point = { a_x, 104 };

    collider.quad.quad = { pos.point.x, pos.point.y, 16, 16 };
    collider.quad.colour = { 255, 0, 220, 127, SDL_BLENDMODE_BLEND };

    dead = false;
}



void End::render ()
{
    if (!dead) { collider.render(); }
}

/// END ///////////////////////////////////////////////////////////////////////



/// COLLECTIBLE ///////////////////////////////////////////////////////////////

Collectible::Collectible (): Entity(16, 16, 3, 3) {}



void Collectible::create (const int a_x)
{
    pos.point = { a_x, 104 };

    image.create((halloween) ? "Halloween/Collectible" : "Collectible");
    animation.create("Collectible");
    sound.create("Collectible", 1);

    collider.quad.quad = { pos.point.x + COLLIDER_X_OFFSET, pos.point.y + COLLIDER_Y_OFFSET, 9, 10 };
    collider.quad.colour = { 0, 255, 0, 127, SDL_BLENDMODE_BLEND };

    score = 10;

    dead = false;
}



void Collectible::kill ()
{
    if (!dead)
    {
        sound.play(0);
        collider.quad.quad = { 0, 0, 0, 0 };
        dead = true;
    }
}



int Collectible::getScore ()
{
    if (score == 10) { score = 0; return 10; }
    else { return score; }
}



bool Collectible::isDead () const
{
    return dead;
}

/// COLLECTIBLE ///////////////////////////////////////////////////////////////



/// OBSTACLE /////////////////////////////////////////////////////////////////

Obstacle::Obstacle (const int a_spr_w, const int a_spr_h, const int a_off_x, const int a_off_y):
                    Entity(a_spr_w, a_spr_h, a_off_x, a_off_y) {}

/// OBSTACLE //////////////////////////////////////////////////////////////////




/// POLE //////////////////////////////////////////////////////////////////////

Pole::Pole (): Obstacle(24, 40, 8, 8) {}



void Pole::create (const int a_x)
{
    pos.point = { a_x, 84 };

    image.create((halloween) ? "Halloween/Pole" : "Pole");
    animation.create("Pole");

    collider.quad.quad = { (pos.point.x + COLLIDER_X_OFFSET), (pos.point.y + COLLIDER_Y_OFFSET), 8, 28 };
    collider.quad.colour = { 255, 0, 0, 127, SDL_BLENDMODE_BLEND };

    dead = false;
}

/// POLE //////////////////////////////////////////////////////////////////////



/// SPIKE /////////////////////////////////////////////////////////////////////

Spike::Spike (): Obstacle(37, 24, 10, 8) {}



void Spike::create (const int a_x)
{
    pos.point = { a_x, 100 };

    image.create((halloween) ? "Halloween/Spike" : "Spike");
    animation.create("Spike");

    collider.quad.quad = { (pos.point.x + COLLIDER_X_OFFSET), (pos.point.y + COLLIDER_Y_OFFSET), 12, 12 };
    collider.quad.colour = { 255, 0, 0, 127, SDL_BLENDMODE_BLEND };

    dead = false;
}

/// SPIKE /////////////////////////////////////////////////////////////////////



/// WALL //////////////////////////////////////////////////////////////////////

Wall::Wall (): Obstacle(40, 64, 13, 7) {}



void Wall::create (const int a_x)
{
    pos.point = { a_x, 60 };

    image.create((halloween) ? "Halloween/Wall" : "Wall");
    animation.create("Wall");

    collider.quad.quad = { (pos.point.x + COLLIDER_X_OFFSET), (pos.point.y + COLLIDER_Y_OFFSET), 14, 41 };
    collider.quad.colour = { 255, 0, 0, 127, SDL_BLENDMODE_BLEND };

    dead = false;
}

/// WALL //////////////////////////////////////////////////////////////////////
