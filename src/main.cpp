#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.hpp"

using namespace sf;

const int WINDOW_X = 1500;
const int WINDOW_Y = 1000;
const float wallRetention = 0.98;
const float ballRetention = 0.95;
const float groundFriction = 0.7;
const float airFriction = 1;

PhysicSolver physicSolver;
bool focusing = false;
Star* focus;
Color oldColor;
double zoom = 1;
float speed = 1;

int main()
{
    RenderWindow window;
    Thread gameThread(&GameThread, &window);
    ContextSettings settings;
    settings.antialiasingLevel = 16;
    window.create(VideoMode(WINDOW_X, WINDOW_Y), "SFML Physics", Style::Default, settings);
    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(144);
    window.setActive(false);

    gameThread.launch();

	while (window.isOpen())
	{
        Event event;
        if (window.pollEvent(event))
        {
            switch (event.type) {
            case Event::Closed:
                window.close();
                break;
            case Event::Resized:
                window.setView(View(window.getView().getCenter(), Vector2f(event.size.width, event.size.height)));
                break;
            case Event::MouseButtonPressed:{
                if (event.mouseButton.button == Mouse::Left)
                {
                    for (Star* star : physicSolver.stars)
                    {
                        if (star->getHitbox().contains(window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y))))
                        {
                            if (focus == star && focusing) {
                                star->setColor(oldColor);
                                focusing = !focusing;
                            } 
                            else {
                                focusing = true;
                                if (focus != nullptr)
                                {
                                    focus->setColor(oldColor);
                                }
                                focus = star;
                                oldColor = star->getColor();
                                star->setColor(Color(255, 255, 255));
                            }
                            break;
                        }
                    }
                }
                break;
            }
            case Event::KeyPressed:{
                if (event.key.code == Keyboard::Up)
                {
                    if (speed <= 2)
                    {
                        speed *= 2;
                    }
                }
                if (event.key.code == Keyboard::Down)
                {
                    if (speed >= 0.5)
                    {
                        speed /= 2;
                    }
                }
                if (event.key.code == Keyboard::Space)
                {
                    if (speed == 0)
                    {
                        speed = 1;
                    }
                    else {
                        speed = 0;
                    }
                }
            }
            default:
                break;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::R))
        {
            View view = window.getDefaultView();
            window.setView(view);
            window.setSize(Vector2u(WINDOW_X, WINDOW_Y));
            VideoMode desktop = VideoMode::getDesktopMode();
            window.setPosition(Vector2i(desktop.width/2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));
            zoom = 1;
            speed = 1;
            if (focusing)
            {
                focus->setColor(oldColor);
                focusing = false;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            if (focusing)
            {
                focus->setColor(oldColor);
                focusing = false;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            View view = window.getView();
            view.zoom(0.99999f);
            zoom *= 0.9999945f;
            window.setView(view);
        }
        if (Keyboard::isKeyPressed(Keyboard::Q))
        {
            View view = window.getView();
            view.zoom(1.00001f);
            zoom *= 1.000005f;
            window.setView(view);
        }
        if (!focusing)
        {
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                View view = window.getView();
                view.move(0.f, -0.001f*zoom);
                window.setView(view);
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                View view = window.getView();
                view.move(-0.001f*zoom, 0.f);
                window.setView(view);
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                View view = window.getView();
                view.move(0.f, 0.001f*zoom);
                window.setView(view);
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                View view = window.getView();
                view.move(0.001f*zoom, 0.f);
                window.setView(view);
            }
        }
	}

    gameThread.wait();

    return 0;
}