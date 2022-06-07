#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "vector.hpp"

using namespace sf;

extern const int WINDOW_X, WINDOW_Y;

struct Star
{
    Sprite              star;
    int                 radius;
    int                 mass;
    Vector              acceleration;
    Vector              velocity;
    
    Star(Texture* image, float scale = 1)
    {
        star = Sprite(*image);
        Vector2f size(image->getSize());
        star.setTextureRect(IntRect(0, 0, size.x, size.y));
        star.setOrigin(size.x/2, size.y/2);
        star.setPosition(WINDOW_X/2, WINDOW_Y/2);
        radius = round((size.x * scale)/2);
        star.setScale(radius*2/size.x, radius*2/size.x);
        mass = radius;
    }

    void move(float x, float y)
    {
        // star.move(x, y);
        star.setPosition(getPosition().x + x, getPosition().y + y);
    }

    void move(Vector v)
    {
        move(v.x, v.y);
    }

    void impulse(Vector v)
    {
        velocity += v;
    }

    void accelerate(Vector a)
    {
        acceleration += a;
    }

    void setPosition(Vector v)
    {
        star.setPosition(v);
    }

    void setColor(sf::Color color)
    {
        star.setColor(color);
    }

    sf::Color getColor()
    {
        return star.getColor();
    }

    Vector2f getPosition()
    {
        return star.getPosition();
    }

    FloatRect getHitbox()
    {
        return star.getGlobalBounds();
    }

    float getLeft()
    {
        return getPosition().x - radius;
    }

    float getRight()
    {
        return getPosition().x + radius;
    }

    float getUp()
    {
        return getPosition().y - radius;
    }

    float getDown()
    {
        return getPosition().y + radius;
    }

    Vector getMove(float dt)
    {
        return velocity * dt;
    }

    void update(float dt)
    {
        move(getMove(dt));

        velocity += acceleration * dt;
    }

    Star* point() { return this; }

    bool operator==(Star other) {
        return point() == other.point();
    }

    operator Sprite() {
        return star;
    }
};