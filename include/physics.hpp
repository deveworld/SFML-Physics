#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdint.h>
#include <renderer.hpp>
#include <star.hpp>

extern const int WINDOW_X, WINDOW_Y;

struct PhysicsSolver
{
    public:
        PhysicsSolver(Renderer* renderer_);

        void solve(int32_t dt_);
    private:
        int32_t dt;
        Renderer* renderer;
        // void calculate();
};

PhysicsSolver::PhysicsSolver(Renderer* renderer_)
: renderer(renderer_)
{
    Star* star = new Star();
    renderer->stars.push_back(star);

    sf::RectangleShape wallShape(sf::Vector2f(WINDOW_X, WINDOW_Y));
    wallShape.setFillColor(sf::Color(0, 0, 0, 0));
    wallShape.setOutlineColor(sf::Color(0, 255, 0));
    wallShape.setOutlineThickness(7);

    renderer->wall = wallShape;
}

void PhysicsSolver::solve(int32_t dt_)
{
    dt = dt_;
}