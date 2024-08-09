#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

class Button: public GameObject
{
    public:
        Button(int type, int SourceH, int SourceW, int SourceX, int SourceY);
        ~Button();
        void Update() override;
        void Press();
        SDL_Rect* GetRect(){return &destR;}

        static SDL_Texture* Default_look;
        static SDL_Texture* Lighted_look;

        enum
        {
            PLAY, INF, RECS, NAME, EXIT
        }ButtonType;
    private:
        int Type;
};

#endif // BUTTON_H_INCLUDED
