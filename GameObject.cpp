#include "Files_n_libs.h"

void GameObject::Render()
{
    SDL_RenderCopy(Game::renderer, ObjTex, &srcR, &destR);
}
