#include "Files_n_libs.h"

extern Game* game;

UIText::UIText(string font, int size, SDL_Color color, string text, SDL_Rect placement)
{
    Text = NULL;
    Font = font;
    Size = size;
    Color = color;
    dRect = placement;
    Update(text, -1);
}

UIText::~UIText()
{
    if(Text)
        SDL_DestroyTexture(Text);
}

void UIText::Update(string text, int size)
{
    if(Text)
        SDL_DestroyTexture(Text);
    if(size == -1)
        size = Size;
    SDL_Surface* TextSurface;
    TTF_Font* font = TTF_OpenFont(&Font[0], size);
    TextSurface = TTF_RenderText_Solid(font, &text[0], Color);
    Text = SDL_CreateTextureFromSurface(Game::renderer, TextSurface);
    dRect.w = TextSurface->w;
    dRect.h = TextSurface->h;
    SDL_FreeSurface(TextSurface);
    TTF_CloseFont(font);
}

