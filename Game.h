#pragma once
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

class Game
{
    public:
        Game(string filename);
        ~Game();

        void init(const char* title, int x, int y, int width, int heigth, bool fullscreen);
        void LoadTTF(string filename);
        void handleEvents();
        void update();
        void render();
        void rewind(int Ships_amount);
        void clean();

        bool IsRunning(){return Running;}
        void SetRunning(bool condition){Running = condition;}

        void SetShipStart(Uint32 value){Ship_start = value;}

        void SetState(int state){GameState = state;}
        int GetState(){return GameState;}

        int GetWidth() {return Width;}
        int GetHeight() {return Height;}

        void UpdateMouse(){SDL_GetMouseState(&Mouse.x, &Mouse.y);}
        bool MouseOn(SDL_Rect *Destination){return SDL_HasIntersection(&Mouse, Destination);}

        static SDL_Renderer* renderer;
        static SDL_Event event;

        enum
        {
            LOADING, MENU, INFO, RECORDS, ENTERING_NAME,
            GAMEPLAY, PAUSE, SHOW_LVL, ENDED

        }GameConditions;
    private:
        int GameState;

        bool Running;
        SDL_Window *window;
        int Width, Height;

        Uint32 Pause_time, Pause_start;

        Ship** Ships_on_map;
        Uint32 Ship_spawn_delay, Ship_start;
        int** All_levels;
        int Lvl_amount, Ships_on_lvl;
        int Player_points;

        UIText** Credits; //4

        UIText** PauseDisplay; //4

        UIText** EndGame; //2

        SDL_Texture* TorpedoImage;
        SDL_Rect TorpedoImageRect;
        UIText* TorpedoCount;

        SDL_Rect ShowLvlRect;

        SDL_Rect Mouse;
};

#endif // GAME_H_INCLUDED
