#pragma once
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "star.hpp"

extern const int WINDOW_X, WINDOW_Y;
extern const float ballRetention, wallRetention, groundFriction, airFriction;
extern float speed;

struct PhysicsSolver
{
    std::vector<Star*>           stars;
    uint32_t                     sub_steps;
    float                        frame_dt;
    Vector gravity =             Vector(0, 10);

    PhysicsSolver()
    {
        sub_steps = 2;
    }

    void addStar(Star* star)
    {
        stars.push_back(star);
    }

    float max(float x, float y)
    {
        return (x > y) ? x : y;
    }

    void solve(float dt)
    {
        sub_steps = max(ceil(2 * speed), 2);
        frame_dt = dt / float(sub_steps);
        for (int i(sub_steps); i--;) 
        {
            wallSolve();
            collisionsSolve();
            updateStar();
        }
    }

    std::pair<bool, bool> isWallCollide(Star* star)
    {
        Vector pos = star->getPosition();
        const int radius = star->radius;
        return std::pair<bool, bool>(pos.x - radius < 0 || pos.x + radius > WINDOW_X, pos.y - radius < 0 || pos.y + radius > WINDOW_Y);
    }

    bool onGround(Star* star)
    {
        return (round(star->getPosition().y + star->radius) == WINDOW_Y);
    }

    void wallSolve()
    {
        for (Star* star : stars) 
        {
            if ((isWallCollide(star).first || isWallCollide(star).second))
            {
                Vector pos = star->getPosition();
                const int radius = star->radius;

                if (isWallCollide(star).first)
                {
                    star->velocity.x = -star->velocity.x * wallRetention;
                    if (pos.x - radius < 0)
                    {
                        star->move(Vector(-2 * (pos.x - radius), 0));
                    }
                    else {
                        star->move(Vector(-2 * (pos.x + radius - WINDOW_X), 0));
                    }
                }
                if (isWallCollide(star).second) 
                {
                    star->velocity.y = -star->velocity.y * wallRetention;
                    if (pos.y - radius < 0)
                    {
                        star->move(Vector(0, -2 * (pos.y - radius)));
                    }
                    else {
                        star->move(Vector(0, -2 * (pos.y + radius - WINDOW_Y)));
                    }
                }
            }
        }
    }

    void collisionsSolve()
    {
        std::list<std::pair<Star*, Star*>> possibleStars = findPossiblePairs();
        
        for (std::pair<Star*, Star*> pair : possibleStars)
        {
            if (isCollide(pair.first, pair.second))
            {
                solveCollision(pair.first, pair.second);
            }
        }
        
    }

    std::list<std::pair<Star*, Star*>> findPossiblePairs()
    {
        std::list<Star*> axis_list;
        std::list<Star*> activeList;
        std::list<std::pair<Star*, Star*>> possibleStars;
        for (Star* star : stars)
        {
            axis_list.push_back(star);
        }
        axis_list.sort([](Star* a, Star* b) { return a->getLeft() < b->getLeft(); });
        for (Star* star : axis_list)
        {
            std::list<Star*> toRemove;
            for (Star* p : activeList)
            {
                if (star->getLeft() > p->getRight())// && star->getUp() > p->getDown())
                {
                    toRemove.push_back(p);
                }
            }
            for (Star* r : toRemove)
            {
                activeList.remove(r);
            }
            for (Star* o : activeList)
            {
                possibleStars.push_back(std::pair<Star*, Star*>(star, o));
            }
            activeList.push_back(star);
        }

        return possibleStars;
    }

    void solveCollision(Star* star1, Star* star2)
    {        
        Vector delta = star1->getPosition() - star2->getPosition();
        float length = delta.getMagnitude();
        Vector mtd = delta * (((star1->radius + star2->radius)-length)/length); 

        float im1 = 1.0 / star1->mass; 
        float im2 = 1.0 / star2->mass;

        star1->move(mtd * (im1 / (im1 + im2)));
        star2->move(mtd * -1 * (im2 / (im1 + im2)));

        Vector v = star1->velocity - star2->velocity;
        float vn = v.dot(mtd.normalize());

        if (vn > 0) return;

        float i = (-1.f * (1.0f + ballRetention) * vn) / (im1 + im2);
        Vector impulse = mtd.normalize() * i;

        star1->velocity += impulse * im1;
        star2->velocity -= impulse * im2;
    }

    bool isCollide(Star* star)
    {
        for (Star* otherStar : stars)
        {
            if (star->getLeft() < otherStar->getRight() && star->getUp() < otherStar->getDown())
            {
                if (isCollide(star, otherStar))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool isCollide(Star* star1, Star* star2)
    {
        if (star1 == star2)
        {
            return false;
        }

        float xd = star1->getPosition().x - star2->getPosition().x;
        float yd = star1->getPosition().y - star2->getPosition().y;

        int sumRadius = star1->radius + star2->radius;
        int sqrRadius = sumRadius * sumRadius;
        float distSqr = (xd * xd) + (yd * yd);
        if (distSqr < sqrRadius)
        {
            return true;
        }
        else {
            return false;
        }
    }

    void updateStar() 
    {
        int i = stars.size();
        for (Star* star : stars) 
        {
            star->velocity -= star->velocity * (1 - airFriction) * frame_dt;
            if (!onGround(star))
            {
                star->acceleration = gravity * frame_dt * 1000;
            }
            else {
                star->acceleration = Vector(0, 0);
                star->velocity -= star->velocity * (1 - groundFriction) * frame_dt;
            }
			star->update(frame_dt);
            i--;
		}
    }
};