//
// Created by erhan güven on 3.09.2023.
//

#ifndef GROWTOPIA_CLONE_ENGINEMACROS_H
#define GROWTOPIA_CLONE_ENGINEMACROS_H

namespace Engine
{
#define CREATE_ABSTRACT_START_UPDATE \
public:                              \
    virtual void start() = 0;        \
    virtual void update(float) = 0   \

#define CREATE_VIRTUAL_START_UPDATE \
public:                              \
    virtual void start();        \
    virtual void update(float);


#define AUTO_GETTER_SETTER(type, varName, defaultValue) \
private: \
    type varName;                         \
public: \
    void set##varName(type val) { varName = val; } \
    type get##varName() { return varName; }
}

#endif //GROWTOPIA_CLONE_ENGINEMACROS_H
