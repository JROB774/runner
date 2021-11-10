/// J_PLAYER //////////////////////////////////////////////////////////////////

const J_Point Player::STARTING_POS = { { 10, 100 } };
J_Point Player::pos = { { 0, 0 } };
J_Vector Player::vel = { 0, 0 };
const int Player::FLOAT_LENGTH = 8;
const int Player::GRAVITY = 1;
const int Player::MAX_JUMP_POWER = 7;
int Player::jumpPower = 0;
J_Image Player::image;
J_Animation Player::runA, Player::jumpA, Player::slideA, Player::deadA;
J_Sound Player::runS, Player::jumpS, Player::deadS;
int Player::runCounter = 0;
J_Collider Player::collider[3];
int Player::key[KEY_TOTAL];
bool Player::slide = false;
int Player::state = -1;
int Player::currentCharacter = 0;

void Player::initialise(const bool a_halloween)
{
    pos = STARTING_POS;

    std::string player = "Player" + std::to_string(currentCharacter);
    image.create(player);

    runA.create("Run");
    jumpA.create("Jump");
    slideA.create("Slide");
    deadA.create("Dead");

    runS.create("Run", 0);
    jumpS.create("Jump", 0);
    deadS.create("Dead", 0);

    runCounter = runA.getLength();

    collider[STATE_RUN].quad.quad = { pos.point.x + 6, pos.point.y + 6, 12, 14 };
    collider[STATE_JUMP].quad.quad = { pos.point.x + 6, pos.point.y + 5, 12, 12 };
    collider[STATE_SLIDE].quad.quad = { pos.point.x + 6, pos.point.y + 10, 12, 10 };

    for (int i = 0; i < 3; ++i) { collider[i].quad.colour = { 0, 0, 255, 127, SDL_BLENDMODE_BLEND }; }

    state = STATE_RUN;
}



void Player::handle ()
{
    if ((state == STATE_RUN) || (state == STATE_SLIDE))
    {
        if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
        {
            if (J_Input::getInput().key.keysym.sym == key[KEY_SLIDE]) { state = STATE_SLIDE; }
            else if (J_Input::getInput().key.keysym.sym == key[KEY_JUMP])
            {
                jumpS.play(0);
                jumpPower = MAX_JUMP_POWER;
                state = STATE_JUMP;
            }
        }
        else if ((J_Input::getInput().type == SDL_KEYUP) && (J_Input::getInput().key.repeat == 0))
        {
            if (J_Input::getInput().key.keysym.sym == key[KEY_SLIDE])
            {
                state = STATE_RUN;
            }
        }
    }

    if (state != STATE_DEAD)
    {
        if (J_System::isDebug())
        {
            if ((J_Input::getInput().type == SDL_KEYDOWN) && (J_Input::getInput().key.repeat == 0))
            {
                if (J_Input::getInput().key.keysym.sym == SDLK_x)
                {
                    kill();
                }
            }
        }
    }
}



void Player::step (const J_Collider& a_ground)
{
    if (state == STATE_RUN)
    {
        if (runCounter <= 0)
        {
            runS.play(0);
            runCounter = runA.getLength();
        }
        else { --runCounter; }
    }
    else if (state == STATE_JUMP)
    {
        if (jumpPower >= 0) { vel.y = (float)(-jumpPower); --jumpPower; }
        else if (jumpPower < -FLOAT_LENGTH) { vel.y += GRAVITY; }
        else { --jumpPower; }

        pos.point.y += (int)vel.y;
        collider[state].quad.quad.y = (pos.point.y + 5);
    }

    if (state != STATE_DEAD)
    {
        while (J_Collision::collision(collider[state], a_ground))
        {
            if (state == STATE_JUMP) { state = STATE_RUN; }

            --pos.point.y;

            switch (state)
            {
                case (STATE_RUN): { collider[state].quad.quad.y = (pos.point.y + 6); break; }
                case (STATE_JUMP): { collider[state].quad.quad.y = (pos.point.y + 5); break; }
                case (STATE_SLIDE): { collider[state].quad.quad.y = (pos.point.y + 10); break; }
            }
        }
    }
}



void Player::render ()
{
    switch (state)
    {
        case (STATE_RUN): { image.render(pos.point.x, pos.point.y, runA.play()); break; }
        case (STATE_JUMP): { image.render(pos.point.x, pos.point.y, jumpA.play()); break; }
        case (STATE_SLIDE): { image.render(pos.point.x, pos.point.y, slideA.play()); break; }
        case (STATE_DEAD): { image.render(pos.point.x, pos.point.y, deadA.play()); break; }
    }

     if (state != STATE_DEAD) { collider[state].render(); }
}



void Player::kill()
{
    deadS.play(0);
    state = STATE_DEAD;
}



void Player::setKey (int a_key, int a_value)
{
    key[a_key] = a_value;
}

void Player::setCharacter (int a_character)
{
    currentCharacter = a_character;
}



J_Collider Player::getCollider ()
{
    J_Collider temp;

    if (state != STATE_DEAD) { temp = collider[state]; }
    else { temp.quad.quad = { 0, 0, 0, 0 }; }

    return temp;
}

bool Player::isDead ()
{
    return (state == STATE_DEAD) ? true : false;
}

int Player::getKey (int a_key)
{
    return key[a_key];
}

int Player::getCharacter ()
{
    return currentCharacter;
}



void Player::terminate ()
{
    pos = { { 0, 0 } };

    vel = { 0, 0 };
    jumpPower = 0;

    image.destroy();

    runA.destroy();
    jumpA.destroy();
    slideA.destroy();
    deadA.destroy();

    runS.destroy();
    jumpS.destroy();
    deadS.destroy();

    runCounter = 0;

    state = -1;
}

/// J_PLAYER //////////////////////////////////////////////////////////////////
