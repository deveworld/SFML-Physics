#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdint.h>
#include <renderer.hpp>
#include <star.hpp>
#include <random.hpp>

extern const int WINDOW_X, WINDOW_Y;

struct PhysicsSolver
{
    public:
        PhysicsSolver(Renderer* renderer_);

        void solve(int32_t dt_);
    private:
        int32_t dt;
        Renderer* renderer;
};

PhysicsSolver::PhysicsSolver(Renderer* renderer_)
: renderer(renderer_)
{
    RNG rng(WINDOW_X, WINDOW_Y);
    for (size_t i = 0; i < 50; i++)
    {
        Star* star = new Star(24);
        star->setColor(rng.randomColor());
        star->setPosition(rng.randomPosition());
        star->setVelocity(rng.randomVelocity());
        renderer->stars.push_back(star);
    }
}

void PhysicsSolver::solve(int32_t dt_)
{
    dt = dt_;
    for (auto &&star : renderer->stars)
    {
        star->update(dt);
        for (auto &&star2 : renderer->stars)
        {
            if (star->isCollision(star2)) star->solveCollision(star2);
        }
        star->wallSolve();
    }
}