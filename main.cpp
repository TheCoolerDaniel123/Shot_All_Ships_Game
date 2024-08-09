#include "Files_n_libs.h"

Game* game = NULL;

int main (int argc, char* argv[])
{
    const int FPS = 60, FrameDelay = 1000 / FPS;
    Uint32 FrameStart;
    int FrameTime;
    game = new Game("files/LvlDescription.txt");
    game->init("Shot_all_ships_game :)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    while(game->IsRunning()) //���� ���� �� ���� �� �����
    {
        FrameStart = SDL_GetTicks();
        game->handleEvents(); //��������� �������� ������������
        game->update(); //���������� ��������� ��������
        game->render(); //��������� ��������
        FrameTime = SDL_GetTicks() - FrameStart;
        if (FrameDelay > FrameTime) SDL_Delay(FrameDelay - FrameTime); //������������� ���������� �������� �����
    }
    game->clean();
    return 0;
}

