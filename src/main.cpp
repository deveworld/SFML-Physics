#include <SFML/Graphics.hpp>
#include <iostream>
#include <physics.hpp>
#include <renderer.hpp>
#include <chrono>
#include <thread>

const int WINDOW_X      = 1500;
const int WINDOW_Y      = 1000;
const int MAX_FPS       = 120;

int SPEED               = 1;
const int MAX_SPEED     = 5;

void renderThread(Renderer* renderer, PhysicsSolver* physicsSolver) {
    renderer->window.setActive(true);

    int32_t sink = 0;
    sf::Clock clock;
    while (renderer->running)
    {
        int32_t dt = clock.restart().asMilliseconds();
        
        for (int i = 0; i < SPEED; i++)
        {
            physicsSolver->solve(dt);
        }
        renderer->draw();
        sink = (1000/MAX_FPS) - dt;
        if (sink < 0) sink = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(sink));
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