#include "Files_n_libs.h"

extern Game* game;

NameInputManager::NameInputManager()
{
    Name = "Player_1";
    SDL_Rect temp;
    NameDisplay = new UIText("fonts/Prototype.ttf", 50, {0, 0, 0}, "Player_1", temp);
    NameDisplay->SetX((game->GetWidth() - NameDisplay->GetPlacement().w) / 2);
    NameDisplay->SetY(game->GetHeight() * 0.35);


    int i;
    Warnings = new UIText*[2];
    Warnings[0] = new UIText("fonts/Prototype.ttf", 29, {0, 0, 0}, "You should type at least something", temp);
    Warnings[1] = new UIText("fonts/Prototype.ttf", 29, {0, 0, 0}, "Press Enter or ExitButton to state your nickname", temp);
    for(i = 0; i < 2; i++)
    {
        Warnings[i]->SetX(5);
        Warnings[i]->SetY(5);
    }
    WarningType = 1;

    NameSize = 12;
}

NameInputManager::~NameInputManager()
{
    delete NameDisplay;
    int i;
    for(i = 0; i < 2; i++)
        if(Warnings[i])
            delete Warnings[i];
    delete Warnings;
}

void NameInputManager::HandleInput(SDL_Event* event) //непосредственно ввод имени
{
    if(event->type == SDL_TEXTINPUT || event->type == SDL_KEYDOWN)
    {
        switch(event->type)
        {
            case SDL_TEXTINPUT: if(Name.length() < 12 && (*(event->text.text) != ' ' || Name.length()))
                                    Name += event->text.text;
                                break; //запись символов, если это не пробел первым символом
            case SDL_KEYDOWN: if(event->key.keysym.sym == SDLK_BACKSPACE && Name.length() > 0)
                                  Name.erase(Name.end() - 1);
                              break; //удаление символа бэкспэйсом если есть что удалять
            default:
                return;
        }
        if(Name.length() > 0)
            NameDisplay->Update(Name, -1);
    }
    ChangeWarning(Name.length());//определение, какое предупреждение выводить на экран
}

void NameInputManager::Render()
{
    if(Name.length() > 0)
        SDL_RenderCopy(Game::renderer, NameDisplay->GetText(), NULL, NameDisplay->GetPlaceAdress());
    SDL_RenderCopy(Game::renderer, Warnings[WarningType]->GetText(), NULL, Warnings[WarningType]->GetPlaceAdress());
}

void NameInputManager::ChangeWarning(bool NameExists)
{
    if(NameExists)
        WarningType = 1;
    else
        WarningType = 0;
}
