#pragma once

class Type;

class Object
{
public:
    virtual const Type *GetType() = 0;
};
