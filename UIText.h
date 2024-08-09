#ifndef UITEXT_H_INCLUDED
#define UITEXT_H_INCLUDED

class UIText
{
    public:
        UIText(string font, int size, SDL_Color color, string text, SDL_Rect placement);
        ~UIText();

        void Update(string text, int size);

        void SetX (int x) {dRect.x = x;}
        void SetY (int y) {dRect.y = y;}

        SDL_Texture* GetText(){return Text;}
        SDL_Rect GetPlacement(){return dRect;}

        SDL_Rect* GetPlaceAdress(){return &dRect;}
    private:
        SDL_Texture* Text;
        string Font;
        int Size;
        SDL_Color Color;
        SDL_Rect dRect;
};

#endif // UITEXT_H_INCLUDED
