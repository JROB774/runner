#include "J_Render.h"

/// J_COLOUR //////////////////////////////////////////////////////////////////

inline bool operator== (const J_Colour& a_lhs, const J_Colour& a_rhs)
{
    return ((a_lhs.red == a_rhs.red) &&
            (a_lhs.green == a_rhs.green) &&
            (a_lhs.blue == a_rhs.blue) &&
            (a_lhs.alpha == a_rhs.alpha) &&
            (a_lhs.blendmode == a_rhs.blendmode)) ? true : false;
}

inline bool operator!= (const J_Colour& a_lhs, const J_Colour& a_rhs)
{
    return !operator==(a_lhs, a_rhs);
}

/// J_COLOUR //////////////////////////////////////////////////////////////////



/// J_POINT ///////////////////////////////////////////////////////////////////

inline bool operator== (const J_Point& a_lhs, const J_Point& a_rhs)
{
    return ((a_lhs.point.x == a_rhs.point.x) && (a_lhs.point.y == a_rhs.point.y)) ? true : false;
}

inline bool operator!= (const J_Point& a_lhs, const J_Point& a_rhs)
{
    return !operator==(a_lhs, a_rhs);
}

/// J_POINT ///////////////////////////////////////////////////////////////////



/// J_LINE ////////////////////////////////////////////////////////////////////

inline bool operator== (const J_Line& a_lhs, const J_Line& a_rhs)
{
    return ((a_lhs.start.x == a_rhs.start.x) &&
            (a_lhs.start.y == a_rhs.start.y) &&
            (a_lhs.end.x == a_rhs.end.x) &&
            (a_lhs.end.y == a_rhs.end.y)) ? true : false;
}

inline bool operator!= (const J_Line& a_lhs, const J_Line& a_rhs)
{
    return !operator==(a_lhs, a_rhs);
}

/// J_LINE ////////////////////////////////////////////////////////////////////



/// J_QUAD ////////////////////////////////////////////////////////////////////

inline bool operator== (const J_Quad& a_lhs, const J_Quad& a_rhs)
{
    return (SDL_RectEquals(&a_lhs.quad, &a_rhs.quad) == SDL_TRUE) ? true : false;
}

inline bool operator!= (const J_Quad& a_lhs, const J_Quad& a_rhs)
{
    return !operator==(a_lhs, a_rhs);
}

/// J_QUAD ////////////////////////////////////////////////////////////////////



/// J_IMAGE ///////////////////////////////////////////////////////////////////

J_Image::J_Image ()
{
    width = 0, height = 0;
    colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };
    texture = nullptr;
}



void J_Image::create (const std::string a_file)
{
    if (texture != nullptr) { destroy(); }

    std::string directory = "Resources\\Images\\" + a_file + ".png";

    SDL_Surface* surface = IMG_Load(directory.c_str());
    if (surface == nullptr) { J_Error::log("J_ERROR_IMAGE_SURFACE_LOAD"); }

    texture = SDL_CreateTextureFromSurface(J_Renderer::renderer, surface);
    if (texture == nullptr) { J_Error::log("J_ERROR_IMAGE_TEXTURE_CREATE"); }

    width = surface->w, height = surface->h;

    SDL_FreeSurface(surface);
    surface = nullptr;
}



void J_Image::render (const int a_x, const int a_y, J_Quad* a_source)
{
    J_Quad destination;

    destination.quad = { a_x, a_y, width, height };
    if (a_source != nullptr) { destination.quad.w = a_source->quad.w, destination.quad.h = a_source->quad.h; }

    if (SDL_RenderCopy(J_Renderer::renderer, texture, &a_source->quad, &destination.quad) != 0) { J_Error::log("J_ERROR_IMAGE_RENDER"); }
}

void J_Image::render (const int a_x, const int a_y, const double a_angle, const J_Point* a_centre, const SDL_RendererFlip a_flip, J_Quad* a_source)
{
    J_Quad destination;

    destination.quad = { a_x, a_y, width, height };
    if (a_source != nullptr) { destination.quad.w = a_source->quad.w, destination.quad.h = a_source->quad.h; }

    if (SDL_RenderCopyEx(J_Renderer::renderer, texture, &a_source->quad, &destination.quad, a_angle, &a_centre->point, a_flip) != 0) { J_Error::log("J_ERROR_IMAGE_RENDER_EXTRA"); }
}



void J_Image::setColour (const J_Colour a_colour)
{
    colour = a_colour;

    if (SDL_SetTextureColorMod(texture, colour.red, colour.green, colour.blue) != 0) { J_Error::log("J_ERROR_IMAGE_SET_COLOUR"); }
    if (SDL_SetTextureAlphaMod(texture, colour.alpha) != 0) { J_Error::log("J_ERROR_IMAGE_SET_ALPHA"); }
    if (SDL_SetTextureBlendMode(texture, colour.blendmode) != 0) { J_Error::log("J_ERROR_IMAGE_SET_BLENDMODE"); }
}



int J_Image::getWidth () const
{
    return width;
}

int J_Image::getHeight () const
{
    return height;
}

J_Point J_Image::getCentre () const
{
    return J_Point { { width / 2, height / 2 } };
}

void J_Image::getCentre (int* a_x, int* a_y) const
{
    *a_x = width / 2, *a_y = height / 2;
}

J_Colour J_Image::getColour () const
{
    return colour;
}



void J_Image::destroy ()
{
    if (texture != nullptr)
    {
        width = 0, height = 0;
        colour = { 255, 255, 255, 255, SDL_BLENDMODE_BLEND };
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

J_Image::~J_Image ()
{
    destroy();
}

/// J_IMAGE ///////////////////////////////////////////////////////////////////



/// J_ANIMATION ///////////////////////////////////////////////////////////////

J_Animation::J_Animation ()
{
    spriteTotal = 0;
    loops = 0;
    currentLoop = 0;
    done = false;
    direction = PLAY_FORWARD;
    currentSprite = 0;
    delayCounter = 0;
}



void J_Animation::create (const std::string a_file)
{
    if (!sprite.empty() && (!delay.empty())) { destroy(); }

    std::string directory = "Resources\\Animations\\" + a_file + ".anim";
    std::ifstream animationFile(directory, std::ifstream::in);
    std::string rawData = "\0";

    if (animationFile.is_open())
    {
        std::getline(animationFile, rawData);
        animationFile.close();
    }
    else { J_Error::log("J_ERROR_ANIMATION_CREATE"); }

    std::istringstream data;
    data.str(rawData);

    J_Quad _sprite;
    std::string rawDelay = "\0";
    data >> spriteTotal >> loops >> direction >> _sprite.quad.x >> _sprite.quad.y >> _sprite.quad.w >> _sprite.quad.h >> rawDelay;
    done = false;

    for (int i = 0, ix = _sprite.quad.x; i < spriteTotal; ++i, ix += _sprite.quad.w)
    {
        J_Quad tempSprite;
        tempSprite.quad = { ix, _sprite.quad.y, _sprite.quad.w, _sprite.quad.h };
        sprite.push_back(tempSprite);
    }

    char* strdelay = new(std::nothrow) char[rawDelay.length() + 1];
    if (strdelay == nullptr) { J_Error::log("J_ERROR_ANIMATION_CREATE"); }
    strcpy(strdelay, rawDelay.c_str());
    delay.push_back(atoi(strtok(strdelay, ":")));
    for (int i = 0; i < (spriteTotal - 1); ++i) { delay.push_back(atoi(strtok(nullptr, ":"))); }

    delete[] strdelay;
    strdelay = nullptr;

    if (direction == PLAY_FORWARD)
    {
        currentSprite = 0;
        delayCounter = delay.front();
    }
    else
    {
        currentSprite = spriteTotal - 1;
        delayCounter = delay.back();
    }
}



J_Quad* J_Animation::play ()
{
    if (loops <= -1)
    {
        if (delayCounter <= 0)
        {
            if (direction == PLAY_FORWARD) { ++currentSprite; }
            else { --currentSprite; }

            if (currentSprite >= spriteTotal) { currentSprite = 0; }
            else if (currentSprite < 0) { currentSprite = spriteTotal - 1; }

            delayCounter = delay.at(currentSprite);
        }
        else { --delayCounter; }
    }
    else
    {
        if (currentLoop > -1)
        {
            if (delayCounter <= 0)
            {
                if (direction == PLAY_FORWARD) { ++currentSprite; }
                else { --currentSprite; }

                if (currentSprite >= spriteTotal)
                {
                    --currentLoop;

                    if (currentLoop > -1) { currentSprite = 0; }
                    else
                    {
                        currentSprite = spriteTotal - 1;
                        done = true;
                    }
                }
                else if (currentSprite < 0)
                {
                    currentSprite = spriteTotal - 1;
                    --currentLoop;

                    if (currentLoop > -1) { currentSprite = spriteTotal -1; }
                    else
                    {
                        currentSprite = 0;
                        done = true;
                    }
                }

                delayCounter = delay.at(currentSprite);
            }
            else { --delayCounter; }
        }
    }

    return &sprite.at(currentSprite);
}

void J_Animation::restart ()
{
    if (loops > -1)
    {
        if (direction == PLAY_FORWARD) { currentSprite = 0; }
        else { currentSprite = spriteTotal - 1; }

        delayCounter = delay.at(currentSprite);

        currentLoop = loops;
        done = false;
    }
}



void J_Animation::setDirection (const int a_direction)
{
    if (!done) { direction = a_direction; }
}



bool J_Animation::isDone () const
{
    return done;
}

int J_Animation::getDirection () const
{
    return direction;
}

int J_Animation::getLength () const
{
    int length = 0;
    for (int i = 0; i < spriteTotal; ++i) { length += delay.at(i); }

    return length;
}



void J_Animation::destroy ()
{
    if (!sprite.empty() && !delay.empty())
    {
        spriteTotal = 0;
        sprite.clear();
        delay.clear();
        loops = 0;
        currentLoop = 0;
        done = false;
        direction = PLAY_FORWARD;
        currentSprite = 0;
        delayCounter = 0;
    }
}

J_Animation::~J_Animation ()
{
    destroy();
}

/// J_ANIMATION ///////////////////////////////////////////////////////////////



/// J_BACKGROUND //////////////////////////////////////////////////////////////

J_Background::J_Background ()
{
    x = 0, y = 0;
    speed = 0;
    offset = 0;
    prevDirection = MOVE_STATIONARY;
    direction = MOVE_STATIONARY;
}



void J_Background::create (const std::string a_file)
{
    std::string directory = "Resources\\Backgrounds\\" + a_file + ".bg";
    std::ifstream backgroundFile(directory, std::ifstream::in);
    std::string rawData = "\0";

    if (backgroundFile.is_open())
    {
        std::getline(backgroundFile, rawData);
        backgroundFile.close();
    }
    else { J_Error::log("J_ERROR_BACKGROUND_CREATE"); }

    std::istringstream data;
    data.str(rawData);
    std::string imageFile = "\0";

    data >> x >> y >> speed >> direction >> imageFile;
    prevDirection = direction;

    image.create(imageFile);
}



void J_Background::render ()
{
    switch (direction)
    {
        case (MOVE_NORTH) :
        {
            offset -= speed;
            if (offset <= -image.getHeight()) { offset += image.getHeight(); }

            image.render(x, y + offset, nullptr);
            image.render(x, y + (offset + image.getHeight()), nullptr);

            break;
        }

        case (MOVE_EAST) :
        {
            offset += speed;
            if (offset >= image.getWidth()) { offset -= image.getWidth(); }

            image.render(x + offset, y, nullptr);
            image.render(x + (offset - image.getWidth()), y, nullptr);

            break;
        }

        case (MOVE_SOUTH) :
        {
            offset += speed;
            if (offset >= image.getHeight()) { offset -= image.getHeight(); }

            image.render(x, y + offset, nullptr);
            image.render(x, y + (offset - image.getHeight()), nullptr);

            break;
        }

        case (MOVE_WEST) :
        {
            offset -= speed;
            if (offset <= -image.getWidth()) { offset += image.getWidth(); }

            image.render(x + offset, y, nullptr);
            image.render(x + (offset + image.getWidth()), y, nullptr);

            break;
        }

        default :
        {
            switch (prevDirection)
            {
                case (MOVE_NORTH) :
                {
                    image.render(x, y + offset, nullptr);
                    image.render(x, y + (offset + image.getHeight()), nullptr);

                    break;
                }

                case (MOVE_EAST) :
                {
                    image.render(x + offset, y, nullptr);
                    image.render(x + (offset - image.getWidth()), y, nullptr);

                    break;
                }

                case (MOVE_SOUTH) :
                {
                    image.render(x, y + offset, nullptr);
                    image.render(x, y + (offset - image.getHeight()), nullptr);

                    break;
                }

                case (MOVE_WEST) :
                {
                    image.render(x + offset, y, nullptr);
                    image.render(x + (offset + image.getWidth()), y, nullptr);

                    break;
                }

                default : { image.render(x, y, nullptr); }
            }

            break;
        }
    }
}



void J_Background::restart ()
{
    if (direction == MOVE_STATIONARY) { direction = prevDirection; }
}

void J_Background::stop ()
{
    if (direction != MOVE_STATIONARY)
    {
        prevDirection = direction;
        direction = MOVE_STATIONARY;
    }
}



void J_Background::setColour (const J_Colour& a_colour)
{
    image.setColour(a_colour);
}



int J_Background::getSpeed () const
{
    return (direction != MOVE_STATIONARY) ? speed : 0;
}

int J_Background::getDirection () const
{
    return direction;
}

J_Colour J_Background::getColour () const
{
    return image.getColour();
}



void J_Background::destroy ()
{
    x = 0, y = 0;
    speed = 0;
    offset = 0;
    prevDirection = MOVE_STATIONARY;
    direction = MOVE_STATIONARY;
    image.destroy();
}

J_Background::~J_Background ()
{
    destroy();
}

/// J_BACKGROUND //////////////////////////////////////////////////////////////



/// J_FONT ////////////////////////////////////////////////////////////////////

const int J_Font::CHARACTER_TOTAL = 128;



J_Font::J_Font ()
{
    characterWidth = 0, characterHeight = 0;
    character = nullptr;
}



void J_Font::create (const std::string a_file)
{
    if (character != nullptr) { destroy(); }

    std::string directory = "Resources\\Fonts\\" + a_file + ".font";
    std::ifstream fontFile(directory, std::ifstream::in);
    std::string rawData = "\0";

    if (fontFile.is_open())
    {
        std::getline(fontFile, rawData);
        fontFile.close();
    }
    else { J_Error::log("J_ERROR_FONT_LOAD"); }

    std::istringstream data;
    data.str(rawData);
    std::string imageFile = "\0";

    data >> characterWidth >> characterHeight >> imageFile;
    image.create(imageFile);

    character = new(std::nothrow) J_Quad[CHARACTER_TOTAL];
    if (character == nullptr) { J_Error::log("J_ERROR_FONT_CREATE"); }

    for (int i = 0, ix = 0, iy = 0; i < CHARACTER_TOTAL; ++i, ix += characterWidth)
    {
        character[i].quad = { ix, iy, characterWidth, characterHeight };
        if (ix >= image.getWidth()) { ix = 0, iy += characterHeight; }
    }
}



void J_Font::render (const int a_x, const int a_y, const std::string a_text, const J_Colour& a_colour)
{
    if (character != nullptr)
    {
        int characterValue = 0;

        image.setColour(a_colour);

        for (unsigned int i = 0, ix = a_x, iy = a_y; i < a_text.length(); ++i)
        {
            switch (a_text.at(i))
            {
                case ('\n') : { ix = a_x, iy += characterHeight; break; }
                case ('\t') : { ix += (characterWidth * 4); break; }

                default :
                {
                    characterValue = static_cast <unsigned char> (a_text.at(i));
                    image.render(ix, iy, &character[characterValue]);

                    ix += characterWidth;

                    break;
                }
            }
        }

        image.setColour(J_Colour({ 255, 255, 255, 255, SDL_BLENDMODE_BLEND }));
    }
}



int J_Font::getCharWidth () const
{
    return characterWidth;
}

int J_Font::getCharHeight () const
{
    return characterHeight;
}



void J_Font::destroy ()
{
    if (character != nullptr)
    {
        characterWidth = 0, characterHeight = 0;
        delete[] character;
        character = nullptr;
        image.destroy();
    }
}

J_Font::~J_Font ()
{
    destroy();
}

/// J_FONT ////////////////////////////////////////////////////////////////////



/// J_RENDERER ////////////////////////////////////////////////////////////////

const J_Colour J_Renderer::DEFAULT_COLOUR = { 0, 0, 0, 255, SDL_BLENDMODE_BLEND };
SDL_Renderer* J_Renderer::renderer = nullptr;



void J_Renderer::initialise (SDL_Window* a_window)
{
    if (renderer != nullptr) { return; }

    renderer = SDL_CreateRenderer(a_window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) { J_Error::log("J_ERROR_RENDERER_CREATE"); }

    setColour(DEFAULT_COLOUR);
}



void J_Renderer::drawPoint (const J_Point& a_point)
{
    setColour(a_point.colour);
    if (SDL_RenderDrawPoint(renderer, a_point.point.x, a_point.point.y) != 0) { J_Error::log("J_ERROR_RENDERER_DRAW_POINT"); }

    setColour(DEFAULT_COLOUR);
}

void J_Renderer::drawLine (const J_Line& a_line)
{
    setColour(a_line.colour);
    if (SDL_RenderDrawLine(renderer, a_line.start.x, a_line.start.y, a_line.end.x, a_line.end.y) != 0) { J_Error::log("J_ERROR_RENDERER_DRAW_LINE"); }

    setColour(DEFAULT_COLOUR);
}

void J_Renderer::drawQuadOutline (const J_Quad& a_quad)
{
    setColour(a_quad.colour);
    if (SDL_RenderDrawRect(renderer, &a_quad.quad) != 0) { J_Error::log("J_ERROR_RENDERER_DRAW_QUAD_OUTLINE"); }

    setColour(DEFAULT_COLOUR);
}

void J_Renderer::drawQuadFilled (const J_Quad& a_quad)
{
    setColour(a_quad.colour);
    if (SDL_RenderFillRect(renderer, &a_quad.quad) != 0) { J_Error::log("J_ERROR_RENDERER_DRAW_QUAD_FILLED"); }

    setColour(DEFAULT_COLOUR);
}



void J_Renderer::clear ()
{
    setColour(DEFAULT_COLOUR);
    if (SDL_RenderClear(renderer) != 0) { J_Error::log("J_ERROR_RENDERER_CLEAR"); }
}

void J_Renderer::update ()
{
    SDL_RenderPresent(renderer);
}



void J_Renderer::setViewport (const J_Quad a_viewport)
{
    if (SDL_RenderSetViewport(renderer, &a_viewport.quad) != 0) { J_Error::log("J_ERROR_RENDERER_SET_VIEWPORT"); }
}

void J_Renderer::setScale (const float a_scale_x, const float a_scale_y)
{
    if ((a_scale_x < 0.0) || (a_scale_y < 0.0)) { J_Error::log("J_ERROR_RENDERER_SCALE_RANGE"); }
    if (SDL_RenderSetScale(renderer, a_scale_x, a_scale_y) != 0) { J_Error::log("J_ERROR_RENDERER_SET_SCALE"); }
}

void J_Renderer::setColour (const J_Colour& a_colour)
{
    if (SDL_SetRenderDrawColor(renderer, a_colour.red, a_colour.green, a_colour.blue, a_colour.alpha) != 0) { J_Error::log("J_ERROR_RENDERER_SET_COLOUR"); }
    if (SDL_SetRenderDrawBlendMode(renderer, a_colour.blendmode) != 0) { J_Error::log("J_ERROR_RENDERER_SET_BLENDMODE"); }
}



void J_Renderer::getViewport (J_Quad* a_quad)
{
    SDL_RenderGetViewport(renderer, &a_quad->quad);
}

void J_Renderer::getScale (float* a_xScale, float* a_yScale)
{
    SDL_RenderGetScale(renderer, a_xScale, a_yScale);
}

J_Colour J_Renderer::getColour ()
{
    J_Colour colour;

    SDL_GetRenderDrawColor(renderer, &colour.red, &colour.green, &colour.blue, &colour.alpha);
    SDL_GetRenderDrawBlendMode(renderer, &colour.blendmode);

    return colour;
}



void J_Renderer::terminate ()
{
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}

/// J_RENDERER ////////////////////////////////////////////////////////////////
