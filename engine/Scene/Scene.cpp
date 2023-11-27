//
// Created by erhan güven on 3.09.2023.
//

#include "Scene.h"
#include "Networking/Client.h"
#include "Window/Window.h"

namespace Engine
{

    void Scene::start()
    {
        for (const auto& pair : gameObjects)
        {
            for(const auto& gameObject : pair.second)
            {
                gameObject->start();
            }
        }
    }

    void Scene::update(float dt)
    {
        if(!newlyCreatedGameObjects.empty())
        {
            for(const auto& gameObject : newlyCreatedGameObjects)
            {
                gameObject->start();
                gameObjects[gameObject->layer].push_back(gameObject);
            }

            newlyCreatedGameObjects.clear();
        }

        for (auto& pair : gameObjects)
        {
            int i = 0;
            for(auto gameObject : pair.second)
            {
                if(gameObject == nullptr)
                {
                    continue;
                }
                if(gameObject->isDead)
                {
                    pair.second.erase(pair.second.begin() + i);
                    gameObject = nullptr;
                    continue;
                }
                gameObject->update(dt);
                if(gameObject->getTransform()->getIsRigidBody())
                {
                    checkCollisionsWithGameObject(*gameObject);
                }
                gameObject->tryRender(dt);
                i++;
            }
        }
    }

    void Scene::checkCollisionsWithGameObject(Gameobject& gameObject)
    {
        for (const auto& pair : gameObjects)
        {
            for(auto& go : pair.second)
            {
                if(go != &gameObject && go->getTransform()->hasCollider)
                {
                    auto* transformA = gameObject.getTransform();
                    auto* transformB = go->getTransform();

                    auto& positionA = transformA->getPosition();
                    auto& positionB = transformB->getPosition();

                    auto scaleA = transformA->getScale();
                    scaleA.x *= 800.0f / Engine::Window::windowSize.x;
                    scaleA.y *= 600.0f / Engine::Window::windowSize.y;

                    auto scaleB = transformB->getScale();
                    scaleB.x *= 800.0f / Engine::Window::windowSize.x;
                    scaleB.y *= 600.0f / Engine::Window::windowSize.y;

                    float leftA = positionA.x - scaleA.x * .5f;
                    float rightA = positionA.x + scaleA.x * .5f;

                    float leftB = positionB.x - scaleB.x * .5f;
                    float rightB = positionB.x + scaleB.x * .5f;

                    float upA = positionA.y + scaleA.y * .5f;
                    float downA = positionA.y - scaleA.y * .5f;

                    float upB = positionB.y + scaleB.y * .5f;
                    float downB = positionB.y - scaleB.y * .5f;

                    if((leftA < rightB &&
                            rightA > leftB &&
                            downA < upB &&
                            upA > downB))
                    {
                        float overlapX = std::min(positionA.x + scaleA.x * .5f - positionB.x + scaleB.x *.5f, positionB.x + scaleB.x * .5f - positionA.x + scaleA.x * .5f);
                        float overlapY = std::min(positionA.y + scaleA.y * .5f - positionB.y + scaleB.y *.5f, positionB.y + scaleB.y * .5f - positionA.y + scaleA.y * .5f);

                        if (overlapX < overlapY)
                        {
                            // Adjust horizontally
                            if (positionA.x + scaleA.x/2.0f < positionB.x + scaleB.x/2.0f)
                            {
                                // Player is to the left of the obstacle
                                positionA.x -= overlapX;
                            } else
                            {
                                // Player is to the right of the obstacle
                                positionA.x += overlapX;
                            }
                        } else
                        {
                            // Adjust vertically
                            if (positionA.y + scaleA.y / 2 < positionB.y + scaleB.y / 2)
                            {
                                positionA.y -= overlapY;
                                if(transformA->getIsRigidBody() && transformA->getRigidBody()->velocity.y < 0)
                                {
                                    transformA->getRigidBody()->velocity.y = 0;
                                }
                            } else
                            {
                                // Player is below the obstacle
                                positionA.y += overlapY;
                                if(transformA->getIsRigidBody() && transformA->getRigidBody()->velocity.y < 0)
                                {
                                    transformA->getRigidBody()->velocity.y = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void Scene::clear()
    {

    }

} // Engine