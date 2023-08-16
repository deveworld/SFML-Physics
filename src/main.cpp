#include <SFML/Graphics.hpp>
#include <iostream>
#include <physics.hpp>
#include <renderer.hpp>
#include <chrono>
#include <thread>

const int WINDOW_X      = 1500;
const int WINDOW_Y      = 1000;
const int MAX_FPS       = 144;
const int POLYGON       = 64;

int SPEED               = 1;
const int MAX_SPEED     = 1000;

void renderThread(Renderer* renderer, PhysicsSolver* physicsSolver) {
    renderer->window.setActive(true);

    sf::Clock clock;
    sf::Clock physicsClock;
    sf::Clock renderClock;

    int32_t renderDt;
    while (renderer->running)
    {
        int32_t dt = clock.restart().asMicroseconds();
        
        physicsClock.restart();
        for (int i = 0; i < SPEED; i++)
        {
            physicsSolver->solve(dt);
        }
        int32_t physicsDt = physicsClock.restart().asMilliseconds();

        renderClock.restart();
        renderer->clear();
        renderer->drawStars();
        renderer->drawDebug(dt, physicsDt, renderDt);
        renderer->display();
        renderDt = renderClock.restart().asMilliseconds();
    }
    renderer->window.setActive(false);
    renderer->playing = false;
}

int main()
{
    Renderer renderer = Renderer("SFML Physics v.Alpha");
    PhysicsSolver physicsSolver = PhysicsSolver(&renderer);

    std::thread render_thr(&renderThread, &renderer, &physicsSolver);

    renderer.handleEventLoop();

    render_thr.join(); // Waiting for render_thr be stop in another thread
    
    return 0;
}