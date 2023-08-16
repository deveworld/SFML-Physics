#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

struct Vector
{
    float       x;
    float       y;

    Vector()
    {
        x = 0;
        y = 0;
    }

    Vector(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

    Vector(sf::Vector2f v)
    {
        x = v.x;
        y = v.y;
    }

    float getMagnitude()
    {
        return sqrt(x * x + y * y);
    }

    Vector normalize() 
    {
        float m = getMagnitude();
        if (m > 0) 
        {
            x /= m;
            y /= m;
        }
        return *this;
    }

    float dot(Vector o)
    {
        return x * o.x + y * o.y;
    }

    Vector operator*(float o) 
    {
        return Vector(x*o, y*o);
    }

    Vector operator*(Vector o) 
    {
        return Vector(x*o.x, y*o.y);
    }

    Vector operator/(float o) 
    {
        return Vector(x/o, y/o);
    }

    Vector operator/(Vector o) 
    {
        return Vector(x/o.x, y/o.y);
    }

    Vector operator+(float o) 
    {
        return Vector(x+o, y+o);
    }

    Vector operator+(Vector o) 
    {
        return Vector(x+o.x, y+o.y);
    }

    Vector operator-(float o) 
    {
        return Vector(x-o, y-o);
    }

    Vector operator-(Vector o) 
    {
        return Vector(x-o.x, y-o.y);
    }

    void operator+=(Vector o) 
    {
        x += o.x;
        y += o.y;
    }

    void operator-=(Vector o) 
    {
        x -= o.x;
        y -= o.y;
    }

    void operator*=(float o) 
    {
        x *= o;
        y *= o;
    }

    operator sf::Vector2f() 
    {
        return sf::Vector2f(x, y);
    }

    // operator std::string() 
    // {
    //     return std::to_string(x) +", "+std::to_string(y);
    // }
};