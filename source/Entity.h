#ifndef INCLUDE_ENTITY
#define INCLUDE_ENTITY

class Entity
{
    public:

        Entity(const int, const int, const int, const int);
        Entity(const Entity&) = delete;
        Entity(Entity&&) = delete;

        Entity& operator=(const Entity&) = delete;
        Entity& operator=(Entity&&) = delete;

        virtual void create(const int) = 0;

        void step();

        virtual void render();

        static void setSpeed(const int);

        const J_Point& getPosition() const;
        const J_Collider& getCollider() const;

        void destroy();
        ~Entity();

    protected:

        static int speed;

        J_Point pos;
        const int SPRITE_WIDTH, SPRITE_HEIGHT;
        const int COLLIDER_X_OFFSET, COLLIDER_Y_OFFSET;
        J_Image image;
        J_Animation animation;
        J_Sound sound;
        J_Collider collider;
        bool dead;
};



class End: public Entity
{
    public:

        End();
        End(const End&) = delete;
        End(End&&) = delete;

        End& operator=(const End&) = delete;
        End& operator=(End&&) = delete;

        void create(const int);

        void render();

        ~End() = default;
};



class Collectible: public Entity
{
    public:

        Collectible();
        Collectible(const Collectible&) = delete;
        Collectible(Collectible&&) = delete;

        Collectible& operator=(const Collectible&) = delete;
        Collectible& operator=(Collectible&&) = delete;

        void create(const int);

        void kill();

        int getScore();
        bool isDead() const;

        ~Collectible() = default;

    private:

        int score;
};



class Obstacle: public Entity
{
    public:

        Obstacle(const int, const int, const int, const int);
        Obstacle(const Obstacle&) = delete;
        Obstacle(Obstacle&&) = delete;

        Obstacle& operator=(const Obstacle&) = delete;
        Obstacle& operator=(Obstacle&&) = delete;

        void create(const int) = 0;

        ~Obstacle() = default;
};



class Pole: public Obstacle
{
    public:

        Pole();
        Pole(const Pole&) = delete;
        Pole(Pole&&) = delete;

        Pole& operator=(const Pole&) = delete;
        Pole& operator=(Pole&&) = delete;

        void create(const int);

        ~Pole() = default;
};



class Spike: public Obstacle
{
    public:

        Spike();
        Spike(const Spike&) = delete;
        Spike(Spike&&) = delete;

        Spike& operator=(const Spike&) = delete;
        Spike& operator=(Spike&&) = delete;

        void create(const int);

        ~Spike() = default;
};



class Wall: public Obstacle
{
    public:

        Wall();
        Wall(const Wall&) = delete;
        Wall(Wall&&) = delete;

        Wall& operator=(const Wall&) = delete;
        Wall& operator=(Wall&&) = delete;

        void create(const int);

        ~Wall() = default;
};

#endif // INCLUDE_ENTITY
