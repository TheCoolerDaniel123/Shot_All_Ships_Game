#ifndef TORPEDO_H_INCLUDED
#define TORPEDO_H_INCLUDED

class Torpedo: public GameObject
{
    public:
        Torpedo(const char* texture);
        ~Torpedo();
        void Update() override; //void Render() override; //проверка хитбоксов
        SDL_Rect* GetHitbox(){return &Hitbox;}
    private:
        SDL_Rect Hitbox;
};

#endif // TORPEDO_H_INCLUDED
