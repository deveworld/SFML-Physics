#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

extern const int WINDOW_X, WINDOW_Y;

struct Star
{
    sf::CircleShape     starShape;
    int                 radius;
    int                 mass;
    sf::Vector2f        acceleration;
    sf::Vector2f        velocity;
    
    Star(int _radius = 2, int _mass = 1)
    : radius(_radius)
    , mass(_mass)
    {
        starShape = sf::CircleShape(radius);
        
        starShape.setPointCount(64);
        starShape.setFillColor(sf::Color(255, 255, 255));
        starShape.setOrigin(radius, radius);
        starShape.setPosition(WINDOW_X/2, WINDOW_Y/2);
        starShape.setScale(radius*2, radius*2);
    }
};