#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <list>
#include "star.hpp"
#include "physics.hpp"

extern PhysicSolver physicSolver;
extern bool focusing;
extern Star* focus;
extern double zoom;
extern float speed;

void GameThread(sf::RenderWindow *window)
{
    Texture circle;
    circle.loadFromFile("../resources/circle.png");

    Font font;
    font.loadFromFile("../resources/tnr.ttf");

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<> x(50, WINDOW_X-50);
    std::uniform_int_distribution<> y(50, WINDOW_Y-50);

    std::uniform_int_distribution<> impulse(10, 50);

    std::uniform_int_distribution<> color(100, 255);

    const int objects_count = 5000;
    for (int i(objects_count); i--;) {
        Star* star = new Star(&circle, 0.02);
        star->impulse(Vector(impulse(rng), impulse(rng)));
        star->setPosition(Vector(x(rng), y(rng)));
        // star->setPosition(Vector(100, 100));
        star->setColor(Color(color(rng), color(rng), color(rng)));
        physicSolver.addStar(star);
    }

    RectangleShape wall(Vector2f(WINDOW_X, WINDOW_Y));
    wall.setFillColor(Color(0, 0, 0, 0));
    wall.setOutlineColor(Color(0, 255, 0));
    wall.setOutlineThickness(7);

    Clock dtClock;
    Clock physicsClock;
    Clock renderClock;

	while (window->isOpen())
	{
        if (focusing)
        {
            Vector2f size = window->getView().getSize();
            window->setView(View(focus->getPosition(), size));
        }

        // Star* star = new Star(&circle, 0.02);
        // star->impulse(Vector(impulse(rng), impulse(rng)));
        // star->setPosition(Vector(100, 100));
        // star->setColor(Color(color(rng), color(rng), color(rng)));
        // physicSolver.addStar(star);

        physicsClock.restart();
        physicSolver.solve(dtClock.restart().asSeconds() * speed);
        // std::cout << "Physics: " << physicsClock.restart().asMilliseconds() << "ms" << std::endl; 

        renderClock.restart();
        window->clear(Color::Black);
        window->draw(wall);
        for (Star* star : physicSolver.stars)
        {
            window->draw(star->star);
        }

        if (focusing)
        {
            std::list<Text*> debug;

            View view = window->getView();
            Vector2f topLeft(view.getCenter() - view.getSize() / 2.f);

            float size = sqrt(view.getSize().x * view.getSize().x + view.getSize().y * view.getSize().y) / 4500;

            // std::pair<bool, bool> collidePair = physicSolver.isWallCollide(focus);//(physicSolver.isWallCollide(focus).first || ) ? "True" : "False");
            // std::string collide((collidePair.first || collidePair.second) ? "True" : "False");

            // std::string collide(physicSolver.isCollide(focus) ? "True" : "False");
            // Text isCollide("IsCollide : " + collide, font, 100);

            std::string collide(physicSolver.onGround(focus) ? "True" : "False");
            Text isCollide("OnGround : " + collide, font, 100);
            isCollide.setScale(size, size);
            isCollide.setPosition(topLeft);
            isCollide.setOutlineColor(Color(255, 255, 255, 150));
            isCollide.setOutlineThickness(2);
            debug.push_back(&isCollide);

            float interval = isCollide.getGlobalBounds().height + isCollide.getGlobalBounds().height/3;

            Text position("Position : " + std::to_string(focus->getPosition().x) + ", " + std::to_string(focus->getPosition().y), font, 100);
            position.setScale(size, size);
            position.setPosition(topLeft.x, topLeft.y+interval);
            position.setOutlineColor(Color(255, 255, 255, 150));
            position.setOutlineThickness(2);
            debug.push_back(&position);

            Text velocity("Velocity : " + std::to_string(focus->velocity.x) + ", " + std::to_string(focus->velocity.y), font, 100);
            velocity.setScale(size, size);
            velocity.setPosition(topLeft.x, topLeft.y+interval*2);
            velocity.setOutlineColor(Color(255, 255, 255, 150));
            velocity.setOutlineThickness(2);
            debug.push_back(&velocity);

            for (Text* text : debug)
            {
                window->draw(*text);
            }
        }
        
        window->display();
        // std::cout << "Render: " << renderClock.restart().asMilliseconds() << "ms" << std::endl;
	}
}