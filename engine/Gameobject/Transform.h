//
// Created by erhan güven on 17.09.2023.
//

#ifndef GROWTOPIA_CLONE_TRANSFORM_H
#define GROWTOPIA_CLONE_TRANSFORM_H

#include <glm/glm.hpp>

namespace Engine
{
    class RigidBody
    {
        friend class Transform;
        friend class Scene;
    public:
        inline void addForce(const glm::vec2& direction)
        {
            velocity += direction;
        }
        inline void setVelocity(const glm::vec2& v)
        {
            velocity = v;
        }
    private:
        glm::vec2 velocity = glm::vec2(0,0);

        bool m_collidingLeft = false;
        bool m_collidingRight = false;
        bool m_collidingDown = false;
        bool m_collidingUp = false;
    };

    class Transform
    {
    public:
        void update(float dt);
        inline void addToPosition(glm::vec2& delta)
        {
            m_position += delta;
        }
        inline void setPositionX(float x)
        {
            m_position.x = x;
        }
        inline void setPositionY(float y)
        {
            m_position.y = y;
        }
        inline glm::vec2& getPosition(){ return m_position; }
        inline void setScale(glm::vec2& scale)
        {
            m_scale = scale;
        }
        inline void setScaleX(float x)
        {
            m_scale.x = x;
        }
        inline void setScaleY(float y)
        {
            m_scale.y = y;
        }
        inline glm::vec2& getScale(){ return m_scale; }

        inline void setIsRigidBody(bool val)
        {
            m_isRigidBody = val;
        }
        inline bool getIsRigidBody()
        {
            return m_isRigidBody;
        }
        inline RigidBody& getRigidBody(){return rigidBody;}
        bool hasCollider = false;
    private:
        glm::vec2 m_position = glm::vec2(0.0f);
        glm::vec2 m_scale = glm::vec2(1.0f);
        float m_rotation = 0.0f;
        bool m_isRigidBody = false;
        RigidBody rigidBody;
    };

} // Engine

#endif //GROWTOPIA_CLONE_TRANSFORM_H
