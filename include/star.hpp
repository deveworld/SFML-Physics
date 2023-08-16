#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector.hpp>

extern const int WINDOW_X, WINDOW_Y, POLYGON, MAX_FPS;

struct Star
{
    public:
        sf::CircleShape     starShape;
        
        Star(int _radius, int _mass);

        void update(int32_t dt_);
        void wallSolve();

        bool isCollision(Star* otherStar);
        void solveCollision(Star* otherStar);

        void setColor(sf::Color color);
        void setPosition(sf::Vector2f position_);
        void setVelocity(sf::Vector2f velocity_);
        void setAcceleration(sf::Vector2f acceleration_);

        sf::Vector2f getPosition();
        sf::Vector2f getVelocity();
        int getRadius();
        int getMass();
    private:
        int                 radius;
        int                 mass;

        sf::Vector2f        velocity;
        sf::Vector2f        acceleration;

        void move(sf::Vector2f vector);
};

Star::Star(int _radius = 6, int _mass = 1)
: radius(_radius)
, mass(_mass)
{
    starShape = sf::CircleShape(radius);

    starShape.setPointCount(POLYGON);
    starShape.setFillColor(sf::Color(255, 255, 255));
    starShape.setOrigin(radius, radius);
    starShape.setPosition(WINDOW_X/2, WINDOW_Y/2);
}

void Star::update(int32_t dt)
{
    float delta = dt / (1000000/MAX_FPS);
    velocity += acceleration * delta;
    move(velocity * delta);
}

void Star::wallSolve()
{
    sf::Vector2f position = getPosition();
    if (position.x - radius < 0 || position.x + radius > WINDOW_X)
    {
        velocity.x = -velocity.x;
        if (position.x - radius < 0)
            move(sf::Vector2f(-2 * (position.x - radius), 0));
        else
            move(sf::Vector2f(-2 * (position.x + radius - WINDOW_X), 0));
    }
    else if (position.y - radius < 0 || position.y + radius > WINDOW_Y)
    {
        velocity.y = -velocity.y;
        if (position.y - radius < 0)
            move(sf::Vector2f(0, -2 * (position.y - radius)));
        else
            move(sf::Vector2f(0, -2 * (position.y + radius - WINDOW_Y)));
    }
}

void Star::move(sf::Vector2f vector)
{
    setPosition(getPosition() + vector);
}

bool Star::isCollision(Star* otherStar)
{
    if (this == otherStar) return false;

    float xd = getPosition().x - otherStar->getPosition().x;
    float yd = getPosition().y - otherStar->getPosition().y;

    int sumRadius = getRadius() + otherStar->getRadius();
    int sqrRadius = sumRadius * sumRadius;
    float distSqr = (xd * xd) + (yd * yd);
    
    return (distSqr < sqrRadius) ? true : false;
}

void Star::solveCollision(Star* otherStar)
{        
    Vector delta = getPosition() - otherStar->getPosition();
    float length = delta.getMagnitude();
    Vector mtd = delta * (((getRadius() + otherStar->getRadius())-length)/length); 

    float im1 = 1.0 / getMass(); 
    float im2 = 1.0 / otherStar->getMass();

    move(mtd * (im1 / (im1 + im2)));
    otherStar->move(mtd * -1 * (im2 / (im1 + im2)));

    Vector v = getVelocity() - otherStar->getVelocity();
    float vn = v.dot(mtd.normalize());

    if (vn > 0) return;

    float i = (-1.f * (1.0f + 1.0f) * vn) / (im1 + im2);
    Vector impulse = mtd.normalize() * i;

    setVelocity(                   (impulse * im1)    + getVelocity());
    otherStar->setVelocity(        (impulse * im2)*-1 + otherStar->getVelocity());
}

void Star::setColor(sf::Color color)
{
    starShape.setFillColor(color);
}

void Star::setAcceleration(sf::Vector2f acceleration_)
{
    acceleration = acceleration_;
}

void Star::setVelocity(sf::Vector2f velocity_)
{
    velocity = velocity_;
}

void Star::setPosition(sf::Vector2f position_)
{
    starShape.setPosition(position_.x, position_.y);
}

sf::Vector2f Star::getPosition()
{
    return starShape.getPosition();
}

sf::Vector2f Star::getVelocity()
{
    return velocity;
}

int Star::getRadius()
{
    return radius;
}

int Star::getMass()
{
    return mass;
}