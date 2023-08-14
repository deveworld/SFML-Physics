#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <star.hpp>

extern const int WINDOW_X, WINDOW_Y, MAX_FPS, MAX_SPEED;

extern int SPEED;

struct Renderer
{
    public:
        std::vector<Star*>      stars;
        sf::RectangleShape      wall;
        sf::RenderWindow        window;
        bool                    running;
        bool                    playing;

        Renderer(std::string title_);
    
        void draw(sf::Color color);
        void handleEventLoop();
    private:
        std::string             title;
};

Renderer::Renderer(std::string title_)
: title(title_) 
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    window.create(sf::VideoMode(WINDOW_X, WINDOW_Y), title, sf::Style::Close, settings);
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));
    window.setFramerateLimit(MAX_FPS);
    // window.setVerticalSyncEnabled(true);
    window.setActive(false);
    running = true;
    playing = true;
}

void Renderer::draw(sf::Color color = sf::Color(0, 0, 0)) {
    window.clear(color);
    for (Star* star : stars)
    {
        window.draw(star->starShape);
    }
    window.draw(wall);
    window.display();
}

void Renderer::handleEventLoop() {
    while (running)
    {
        sf::Event event;
        if (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::Closed:
                running = false;
                while (playing) {} // Waiting for OpenGL be de-activated in another thread
                window.close();
                break;
            case sf::Event::MouseWheelScrolled:
            {
                sf::View newView = window.getView();
                sf::Vector2f size = newView.getSize();
                float delta = event.mouseWheelScroll.delta
                    * (0.1f - (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 0.05f : 0.f));
                float newSizeFactor = 1.f - delta;
                newView.setSize(size.x*newSizeFactor, size.y*newSizeFactor);
                window.setView(newView);
                break;
            }
            default:
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            window.setView(window.getDefaultView());
            window.setSize(sf::Vector2u(WINDOW_X, WINDOW_Y));
            sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
            window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));
        }        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            if (SPEED < MAX_SPEED)
                SPEED += 1;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            if (SPEED > 1)
                SPEED -= 1;
    }
}