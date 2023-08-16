#pragma once
#include <SFML/Graphics.hpp>
#include <random>

struct RNG
{
    public:
        RNG(const int _x, const int _y);

        sf::Color randomColor();
        sf::Vector2f randomPosition();
        sf::Vector2f randomVelocity();
        int randomMass();
    private:
        const int x, y;
        std::random_device dev;
        std::mt19937 rng;
};

RNG::RNG(const int _x, const int _y)
: x(_x), y(_y)
{
    rng = std::mt19937(dev());
}

sf::Vector2f RNG::randomPosition()
{
    std::uniform_real_distribution<> random_x(50, x-50);
    std::uniform_real_distribution<> random_y(50, y-50);

    return sf::Vector2f(random_x(rng), random_y(rng));
}

sf::Color RNG::randomColor()
{
    std::uniform_int_distribution<> color(100, 255);
    return sf::Color(color(rng), color(rng), color(rng));
}

sf::Vector2f RNG::randomVelocity()
{
    std::uniform_real_distribution<> velocity(-5.f, 5.f);
    return sf::Vector2f(velocity(rng), velocity(rng));
}

int RNG::randomMass()
{
    std::uniform_int_distribution<> mass(30, 100);
    return mass(rng);
}