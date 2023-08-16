#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <list>
#include <star.hpp>

extern const int WINDOW_X, WINDOW_Y, MAX_FPS, MAX_SPEED;

extern int SPEED;

struct Renderer
{
    public:
        sf::RenderWindow        window;

        std::vector<Star*>      stars;

        bool                    running;
        bool                    playing;

        Renderer(std::string title_);
    
        void clear(sf::Color color, bool drawWall);
        void drawDebug(int32_t dt, int32_t physicsDt, int32_t renderDt);
        void drawStars();
        void display();

        void handleEventLoop();
    private:
        std::string             title;
        sf::Font                font;
        sf::RectangleShape      wall;
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

    wall = sf::RectangleShape(sf::Vector2f(WINDOW_X, WINDOW_Y));
    wall.setFillColor(sf::Color(0, 0, 0, 0));
    wall.setOutlineColor(sf::Color(0, 255, 0));
    wall.setOutlineThickness(7);

    font.loadFromFile("../resources/tnr.ttf");

    running = true;
    playing = true;
}

void Renderer::clear(sf::Color color = sf::Color(0, 0, 0), bool drawWall = true)
{
    window.clear(color);
    if (drawWall)
        window.draw(wall);
}

void Renderer::drawDebug(int32_t dt, int32_t physicsDt, int32_t renderDt)
{
    std::list<sf::Text*> debug;

    sf::View view = window.getView();
    sf::Vector2f topLeft(view.getCenter() - view.getSize() / 2.1f);

    float size = sqrt(view.getSize().x * view.getSize().x + view.getSize().y * view.getSize().y) / 4500;

    if (dt == 0)
        dt = 1000000;
    std::string nowFps = std::to_string(1000000/dt);
    sf::Text fps("FPS : "+std::to_string(1000000/dt), font, 50);
    fps.setScale(size, size);
    fps.setPosition(topLeft);
    fps.setOutlineColor(sf::Color(255, 255, 255, 150));
    fps.setOutlineThickness(2);
    debug.push_back(&fps);

    float interval = fps.getGlobalBounds().height + fps.getGlobalBounds().height/6;

    sf::Text nowSpeed("speed : "+std::to_string(SPEED), font, 50);
    nowSpeed.setScale(size, size);
    nowSpeed.setPosition(topLeft.x, topLeft.y+interval);
    nowSpeed.setOutlineColor(sf::Color(255, 255, 255, 150));
    nowSpeed.setOutlineThickness(2);
    debug.push_back(&nowSpeed);

    sf::Text physicsTime("physics : "+std::to_string(physicsDt)+"ms", font, 50);
    physicsTime.setScale(size, size);
    physicsTime.setPosition(topLeft.x, topLeft.y+interval*2);
    physicsTime.setOutlineColor(sf::Color(255, 255, 255, 150));
    physicsTime.setOutlineThickness(2);
    debug.push_back(&physicsTime);

    sf::Text renderTime("render : "+std::to_string(renderDt)+"ms", font, 50);
    renderTime.setScale(size, size);
    renderTime.setPosition(topLeft.x, topLeft.y+interval*3);
    renderTime.setOutlineColor(sf::Color(255, 255, 255, 150));
    renderTime.setOutlineThickness(2);
    debug.push_back(&renderTime);

    for (sf::Text* text : debug)
        window.draw(*text);
}

void Renderer::drawStars()
{
    for (Star* star : stars)
        window.draw(star->starShape);
}

void Renderer::display()
{
    window.display();
}

void Renderer::handleEventLoop()
{
    sf::Vector2f oldPos;
    bool moving = false;
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
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    moving = true;
                    oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                    moving = false;
                break;
            case sf::Event::MouseMoved:
            {
                if (!moving)
                    break;

                const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                const sf::Vector2f deltaPos = oldPos - newPos;

                sf::View newView = window.getView();
                newView.setCenter(newView.getCenter() + deltaPos);
                window.setView(newView);
                oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                break;
            }
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
            case sf::Event::KeyPressed:
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    (SPEED == 0) ? (SPEED = 1) : (SPEED = 0);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    if (SPEED < MAX_SPEED)
                        SPEED += 1;

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    if (SPEED > 1)
                        SPEED -= 1;
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    SPEED = 1;
                    window.setView(window.getDefaultView());
                    window.setSize(sf::Vector2u(WINDOW_X, WINDOW_Y));
                    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
                    window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));
                }
            }
            default:
                break;
            }
        }
    }
}