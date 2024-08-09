#pragma once
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

class GameObject
{
    public:
        virtual ~GameObject(){};
        virtual void Update() = 0;
        virtual void Render();

        int GetX (void) {return x;}
        int GetY (void) {return y;}

        void SetX (int value) {x = value;}
        void SetY (int value) {y = value;}
    protected:
       int x, y;
       SDL_Texture* ObjTex;
       SDL_Rect srcR, destR;
};

#endif // GAMEOBJECT_H_INCLUDED
