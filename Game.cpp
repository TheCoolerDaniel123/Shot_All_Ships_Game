#include "Files_n_libs.h"

GameObject *_map = NULL, *menu = NULL, *Loading = NULL;
BigBang* destruction[2] = {NULL, NULL};
Submarine* submarine = NULL;
LineManager* LineMaster = NULL;
RecordManager* RecordList = NULL;
NameInputManager* NameInput = NULL;
ShipTypeManager* ShipList = NULL;
ShipInfo* CurStats = NULL;
Button *Play = NULL, *Info = NULL, *Records = NULL, *Change_name = NULL, *Exit = NULL;
//Mix_Music *ExpSound = NULL;

const int _Lines = 3; //сколько будет линий, на которых находятся корабли
int Ships_left, Ships_to_add, Missiles, Current_ship, Current_lvl, k;

SDL_Event Game::event;
SDL_Renderer* Game::renderer = NULL;

SDL_Texture *Logo;
SDL_Rect LogoR;

UIText* Load;

GameData Result;

Game::Game(string filename)
{
    ifstream stream;
    int i, j;
    //queue<int> Queue; Queue.push(1); cout << Queue.front() << endl;
    stream.open(filename); //файл с описанием кораблей на уровнях
    if(!stream.is_open()) cout << "[Game constructor] Troubles with opening file (" << filename << ")" << endl;
    stream >> Lvl_amount;
    stream >> Ships_on_lvl;
    srand(time(NULL));
    All_levels = new int*[Lvl_amount];
    for (i = 0; i < Lvl_amount; i++) All_levels[i] = new int[Ships_on_lvl];
    for (i = 0; i < Lvl_amount; i++) //заполнение матрицы типов кораблей для уровней
    {
        for (j = 0; j < Ships_on_lvl; j++)
        {
            stream >> All_levels[i][j]; //считываем тип корабля из файла
            All_levels[i][j] *= 10; //преобразовываем его в ключ для словаря
            //All_levels[i][j] += rand() % 3 + 1; //if(All_levels[i][j] > 21) All_levels[i][j] = 21;
        }
        random_shuffle(All_levels[i], All_levels[i] + Ships_on_lvl); //случайный порядок кораблей
    }
    stream.close();

    Ship_spawn_delay = 1000; //задержка появления кораблей
    Player_points = 0;
    Pause_time = 0;
    Mouse.w = Mouse.h = 1;

    GameState = LOADING;
}

Game::~Game()
{
    int i;
    for (i = 0; i < Lvl_amount; i++) if(All_levels[i]) delete[] All_levels[i];
    if(All_levels) delete[] All_levels;
    for (i = 0; i < 4; i++)
    {
        if(Credits[i])
            delete Credits[i];
        if(PauseDisplay[i])
            delete PauseDisplay[i];
        if(i < 2)
            if(EndGame[i])
                delete EndGame[i];
    }
    if(TorpedoImage)
        SDL_DestroyTexture(TorpedoImage);
    if(TorpedoCount)
        delete TorpedoCount;
    delete PauseDisplay;
    delete Credits;
    delete EndGame;
}

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen)
{
    int flag = 0, i;
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    //Mix_Init(MIX_INIT_MP3);
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // -lSDL2_mixer

    Width = width;
    Height = height;

    if (fullscreen)
        flag = SDL_WINDOW_FULLSCREEN;
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "Initialisation done" << endl << endl;
        window = SDL_CreateWindow(title, x, y, width, height, flag);
        if (window) cout << "Window created" << endl << endl;
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            cout << "Renderer created" << endl << endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        Running = true;
    }
    else Running = false;

    Loading = new Background("images/Loading.jpeg", 1080, 1920, 0, 0);
    Load = new UIText("fonts/Prototype.ttf", 80, {0, 0, 0}, "LOADING...", {20, 5, 1, 1});
    render();

    Ships_left = Ships_to_add = Missiles = Ships_on_lvl;
    Current_ship = Current_lvl = k = 0;

    LineMaster = new LineManager(_Lines);

    Ships_on_map = new Ship*[_Lines];
    for(i = 0; i < _Lines; i++)
        Ships_on_map[i] = NULL;

    Logo = IMG_LoadTexture(Game::renderer, "images/logo.webp");
    LogoR.h = Height / 3; LogoR.w = Width / 1.5; LogoR.x = Width / 2 - LogoR.w / 2; LogoR.y = Height - LogoR.h * 1.5 + 20;

    _map = new Background("images/water.png", 498, 920, 0, 24);
    submarine = new Submarine(Width / 2 - Width / 12, Height - 100);

    RecordList = new RecordManager("files/Records.bin");//RecordList->StoreCurRecs();
    RecordList->LoadCurRecs();

    NameInput = new NameInputManager;

    menu = new Background("images/menu.jpeg", 1080, 1920, 0, 0);

    Ship::Waves = IMG_LoadTexture(renderer, "images/waves6.png");
    Ship::rWaves = IMG_LoadTexture(renderer, "images/wavesRev.png");

    BigBang::BangTexes[0] = IMG_LoadTexture(renderer, "images/exp1.png");
    BigBang::BangTexes[1] = IMG_LoadTexture(renderer, "images/exp2.png");

    Button::Default_look = IMG_LoadTexture(renderer, "images/Buttons.png");
    Button::Lighted_look = IMG_LoadTexture(renderer, "images/ButtonsSelected.png");
    Play = new Button(Button::PLAY, 65, 202, 6, 8);
    Info = new Button(Button::INF, 66, 66, 560, 152);
    Records = new Button(Button::RECS, 65, 202, 215, 8);
    Change_name = new Button(Button::NAME, 65, 202, 6, 296);
    Exit = new Button(Button::EXIT, 65, 202, 6, 367);

    ShipList = new ShipTypeManager("files/TexNames.txt");
    ShipList->LoadAll();

    TorpedoImage = IMG_LoadTexture(renderer, "images/Torpedo.png");
    TorpedoImageRect.h = Height / (Height / 40);
    TorpedoImageRect.w = Width / (Width / 40);
    TorpedoImageRect.x = Width - TorpedoImageRect.w + 10;
    TorpedoImageRect.y = Height - 150;

    LoadTTF("files/Credits.txt");

    //ExpSound = Mix_LoadMUS("explosion.mp3"); // -lSDL2_mixer

    GameState = MENU;
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) //окно закрыли
    {
        RecordList->StoreCurRecs(); //сохранение рекордов в файл
        Running = false;
    }
    switch(GameState)
    {
        case MENU: if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                    {
                        if(MouseOn(Play->GetRect())) Play->Press();
                        if(MouseOn(Info->GetRect())) Info->Press();
                        if(MouseOn(Records->GetRect())) Records->Press();
                        if(MouseOn(Change_name->GetRect())) Change_name->Press();
                        if(MouseOn(Exit->GetRect())) Exit->Press();
                    }
                    break;
        case INFO:
        case RECORDS: if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
                         if(MouseOn(Exit->GetRect())) Exit->Press();
                      break;
        case ENTERING_NAME: NameInput->HandleInput(&event);
                            if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && NameInput->GetNameLength())
                                if(MouseOn(Exit->GetRect())) Exit->Press();
                            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && NameInput->GetNameLength())
                                Exit->Press();
                            break;
        case GAMEPLAY: switch(event.type)
                       {
                            case SDL_KEYDOWN: switch(event.key.keysym.sym)
                                              {
                                                 case SDLK_SPACE: Pause_start = SDL_GetTicks();
                                                                  GameState = PAUSE;
                                                                  submarine->SetMove('l', 0);
                                                                  break;
                                                 case SDLK_RIGHT: submarine->SetMove('r', 1); break;
                                                 case SDLK_LEFT: submarine->SetMove('l', 1); break;
                                                 case SDLK_UP: if(Missiles && submarine->Shoot())
                                                               {
                                                                   Missiles--;
                                                                   TorpedoCount->Update(&to_string(Missiles)[0], -1);
                                                               }
                                                               break;
                                                 case SDLK_F7: for(int i = 0; i < _Lines; i++) //скип уровня для дебага
                                                                   if(Ships_on_map[i])
                                                                   {
                                                                       delete Ships_on_map[i];
                                                                       Ships_on_map[i] = NULL;
                                                                   }
                                                               LineMaster->MakeAllAvailable();
                                                               rewind(0);
                                                               break;
                                              }
                                              break;
                            case SDL_KEYUP: switch(event.key.keysym.sym)
                                            {
                                                 case SDLK_RIGHT: submarine->SetMove('r', 0); break;
                                                 case SDLK_LEFT: submarine->SetMove('l', 0); break;
                                            }
                                            break;
                        }
                        break;
        case PAUSE: if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                    {
                        Pause_time = SDL_GetTicks() - Pause_start;
                        GameState = GAMEPLAY;
                    }
                    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) rewind(1);
                    break;
    }
}

void Game::update()
{
    switch(GameState)
    {
        case MENU: UpdateMouse();
                   Play->Update();
                   Info->Update();
                   Records->Update();
                   Change_name->Update();
                   Exit->Update();
                   break;
        case INFO:
        case RECORDS:
        case ENTERING_NAME: UpdateMouse();
                            Exit->Update();
                            break;
        case GAMEPLAY: int i;
                       bool destr_flag = false;
                       if(Pause_time)
                       { //если игра была на паузе, время учитывается, чтобы продолжить отображать тонущие корабли с того же момента времени
                           for(i = 0; i < 2; i++)
                                if(destruction[i])
                                    destruction[i]->SetStart(Pause_time + destruction[i]->GetStart());
                           Ship_start += Pause_time;
                           Pause_time = 0;
                       }
                       for (i = 0; i < 2; i++)
                            if(destruction[i])
                                destr_flag = true; //проверка, есть ли на карте летящие снаряды
                       if(!Missiles && !submarine->GetShot() && !destr_flag) //если нет снарядов в запасе, снарядов на карте, тонущих
                           rewind(Ships_left); //кораблей - обработать информацию о текущем состоянии игры и обновить данные
                       if (Ships_to_add) //если еще остались корабли для добавления на уровне
                       {
                           for (i = 0; i < _Lines; i++) if(!Ships_on_map[i]) break; //поиск свободной линии
                           if ((i < _Lines) && (Ship_spawn_delay < SDL_GetTicks() - Ship_start))
                           {  //если прошло достаточно времени с появления последнего корабля и линия найдена - добавление
                               CurStats = ShipList->GetShipInfo(All_levels[Current_lvl][Current_ship++]);//CurStats = ShipList->GetShipInfo(13);
                               Ships_on_map[i] = LineMaster->Add_ship(LineMaster->FindFreeLine(), CurStats, rand() % 2);
                               Ship_start = SDL_GetTicks(); //обновление времени последнего добавления
                               Ships_to_add--; //обновление количества кораблей для добавления
                           }
                       }
                       submarine->Update();
                       for (i = 0; i < _Lines; i++)
                       {
                            if (Ships_on_map[i]) //обновление объектов кораблей
                            {
                                Ships_on_map[i]->Update();
                                if (Ships_on_map[i]->IsDestroyed()) //если объект был уничтожен
                                {
                                    LineMaster->UpdateLine(Ships_on_map[i]->GetLine(), true); //обновление инфы о свободном месте
                                    if(destruction[k]) k++; //если уже есть объект взрыва
                                    destruction[k] = new BigBang(Ships_on_map[i]->GetInfo(), Ships_on_map[i]->GetX(),
                                    Ships_on_map[i]->GetY(), Ships_on_map[i]->GetRev(), submarine->GetHitbox());
                                    k = 0; //создание взрыва на месте уничтоженного корабля
                                    Ship_start = SDL_GetTicks(); //задержка появления нового корабля
                                    Player_points += Ships_on_map[i]->GetPoints(); //начисление очков за подбитый корабль
                                    delete Ships_on_map[i]; //удаление объекта
                                    Ships_on_map[i] = NULL;
                                    submarine->SetShot(false); //снаряд попал в корабль, снаряда больше нет, инфа об этом
                                    Ships_left--; //уменьшение счетчика оставшихся неподбитыми кораблей
                                }
                            }
                       }
                       for (i = 0; i < 2; i++)
                           if (destruction[i])
                           {
                               if(destruction[i]->IsDeleted()) //если взрыв закончился - удаление объекта
                               {
                                   delete destruction[i];
                                   destruction[i] = NULL;
                               }
                               else destruction[i]->Update(); //иначе обновить положение
                           }
                       break;
    }
}

void Game::render()
{
    int i;
    SDL_RenderClear(renderer);
    switch(GameState)
    {
        case LOADING: Loading->Render();
                      SDL_RenderCopy(renderer, Load->GetText(), NULL, Load->GetPlaceAdress());
                      break;
        case MENU: menu->Render();
                   SDL_RenderCopy(renderer, Logo, NULL, &LogoR);
                   Play->Render();
                   Info->Render();
                   Records->Render();
                   Change_name->Render();
                   Exit->Render();
                   break;
        case INFO: menu->Render();
                   Exit->Render();
                   for(i = 0; i < 4; i++) SDL_RenderCopy(renderer, Credits[i]->GetText(), NULL, Credits[i]->GetPlaceAdress());
                   break;
        case RECORDS: menu->Render();
                      RecordList->RenderRecs();
                      Exit->Render();
                      break;
        case ENTERING_NAME: menu->Render();
                            NameInput->Render();
                            Exit->Render();
                            break;
        case SHOW_LVL: _map->Render();
                       submarine->Render();
                       SDL_RenderCopy(renderer, PauseDisplay[0]->GetText(), NULL, &ShowLvlRect);
                       break;
        case ENDED: _map->Render();
                    submarine->Render();
                    for(i = 0; i < 2; i++)
                        SDL_RenderCopy(renderer, EndGame[i]->GetText(), NULL, EndGame[i]->GetPlaceAdress());
                    break;
        case PAUSE:
        case GAMEPLAY: _map->Render();
                       for(i = 0; i < 2; i++)
                            if(destruction[i])
                                destruction[i]->Render();
                       submarine->Render();
                       for (i = 0; i < _Lines; i++)
                            if(Ships_on_map[i])
                                Ships_on_map[i]->Render();
                       SDL_RenderCopy(renderer, TorpedoCount->GetText(), NULL, TorpedoCount->GetPlaceAdress());
                       SDL_RenderCopy(renderer, TorpedoImage, NULL, &TorpedoImageRect);
                       if (GameState == PAUSE) for (i = 0; i < 4; i++)
                            SDL_RenderCopy(renderer, PauseDisplay[i]->GetText(), NULL, PauseDisplay[i]->GetPlaceAdress());
                       break;
    }
    SDL_RenderPresent(renderer);
    if(GameState == SHOW_LVL || GameState == ENDED)
    {
        SDL_Delay(3000);
        if(GameState == SHOW_LVL)
            GameState = GAMEPLAY;
        if(GameState == ENDED)
            GameState = MENU;
    }
}

void Game::rewind(int Ships_amount) //переход на следующий уровень или окончание игры, когда снаряды закончились
{
    int i;
    string lvlDisplay, endDisplay;
    Missiles = Ships_left = Ships_to_add = Ships_on_lvl; //возврат к начальным значениям
    submarine->SetX(Width / 2 - Width / 12); //перенос подлодки в начальное место
    submarine->SetY(Height - 100);
    submarine->SetMove('l', 0);
    Current_ship = 0;
    Pause_time = 0;
    TorpedoCount->Update("10", -1);
    if(!Ships_amount) //установка вывода надписей на экран
    {
        if (Current_lvl == Lvl_amount - 1)
        {
            cout << "Game completed" << endl << "Player points: " << Player_points << endl;
            endDisplay = "Game completed";
            GameState = ENDED;
        }
        else
        {
            cout << endl << "Level up" << endl;
            Current_lvl++;
            lvlDisplay = "Level " + to_string(Current_lvl + 1);
            ShowLvlRect.h = PauseDisplay[0]->GetPlacement().h * 2;
            ShowLvlRect.w = PauseDisplay[0]->GetPlacement().w * 2;
            ShowLvlRect.x = (Width - ShowLvlRect.w) / 2;
            GameState = SHOW_LVL;
        }
    }
    else
    {
        endDisplay = "You lost at lvl " + to_string(Current_lvl + 1);
        for (i = 0; i < _Lines; i++)
        {
            if(Ships_on_map[i]) {LineMaster->UpdateLine(Ships_on_map[i]->GetLine(), true); delete Ships_on_map[i];}
            Ships_on_map[i] = NULL;
        }
        GameState = ENDED;
    }
    if(GameState == ENDED)
    {
        Current_lvl = k = 0;
        if(Player_points) //сохранение нового результата, если очков больше 0
        {
            Result.Name = NameInput->GetName();
            Result.Points = Player_points;
            RecordList->AddData(Result);
            RecordList->DeleteRecTexes();
        }
        lvlDisplay = "Level 1";
        SDL_ShowCursor(true);
        for(i = 0; i < 2; i++)
        {
            EndGame[i]->Update(endDisplay, -1);
            EndGame[i]->SetX((Width - EndGame[i]->GetPlacement().w) / 2);
            if(i)
                EndGame[i]->SetY(EndGame[i - 1]->GetPlacement().y + EndGame[i - 1]->GetPlacement().h + 5);
            else
                EndGame[i]->SetY(Height * 0.35);
            endDisplay = "Points: " + to_string(Player_points);
        }
        Player_points = 0;
    }
    PauseDisplay[0]->Update(lvlDisplay, 25);
}

void Game::LoadTTF(string filename) //загрузка инфы, статичного текста, текста для отображения в начале игры
{
    ifstream stream;
    string info, pause_features[4] = {"Level 1", "Move: left/right arrows", "Shoot: upper arrow", "Press ESC to quit"};
    int i;
    SDL_Rect TempRect;
    stream.open(filename);
    if(!stream) cout << "Troubles with opening file files/Credits.txt" << endl;
    Credits = new UIText*[4];
    for(i = 0; i < 4; i++)
    {
        getline(stream, info);
        TempRect.x = 10;
        switch(i)
        {
            case 0: TempRect.y = 5;
                    break;
            case 1:
            case 2: TempRect.y = TempRect.y + Credits[i - 1]->GetPlacement().h + 10;
                    break;
            case 3: TempRect.y = Height - Credits[i - 1]->GetPlacement().h - 5;
                    break;
        }
        Credits[i] = new UIText("fonts/Prototype.ttf", 30, {0, 0, 0}, info, TempRect);
    }
    stream.close();
    cout << "Credits converted by TTF succesfully" << endl;

    PauseDisplay = new UIText*[4];
    for(i = 0; i < 4; i++)
    {
        PauseDisplay[i] = new UIText("fonts/Prototype.ttf", 25, {0, 0, 0}, pause_features[i], TempRect);
        PauseDisplay[i]->SetX((Width - PauseDisplay[i]->GetPlacement().w) / 2);
        if(i) PauseDisplay[i]->SetY(PauseDisplay[i - 1]->GetPlacement().y + PauseDisplay[i - 1]->GetPlacement().h + 5);
        else
        {
            PauseDisplay[i]->SetY(Height / 2 - 2 * PauseDisplay[i]->GetPlacement().h);
            ShowLvlRect.h = PauseDisplay[i]->GetPlacement().h * 2;
            ShowLvlRect.w = PauseDisplay[i]->GetPlacement().w * 2;
            ShowLvlRect.x = (Width - ShowLvlRect.w) / 2;
        }
    }
    TorpedoCount = new UIText("fonts/Prototype.ttf", 40, {0, 0, 0}, "10", TempRect);
    TorpedoCount->SetX(Width - TorpedoCount->GetPlacement().w - 5);
    TorpedoCount->SetY(TorpedoImageRect.y - TorpedoCount->GetPlacement().h);
    ShowLvlRect.y = Height * 0.4;
    EndGame = new UIText*[2];
    for(i = 0; i < 2; i++)
        EndGame[i] = new UIText("fonts/Prototype.ttf", 40, {0, 0, 0}, "2", TempRect);
}

void Game::clean()
{
    int i;
    if(LineMaster)
        delete LineMaster;
    for(i = 0; i < _Lines; i++)
        if(Ships_on_map[i])
            delete Ships_on_map[i];
    for (i = 0; i < 2; i++)
        if(BigBang::BangTexes[i])
            SDL_DestroyTexture(BigBang::BangTexes[i]);
    if(Ships_on_map) delete Ships_on_map;
    if(submarine) delete submarine;
    if(_map) delete _map;
    if(menu) delete menu;
    if(ShipList) delete ShipList;
    if(RecordList) delete RecordList;
    if(Play) delete Play;
    if(Info) delete Info;
    if(Records) delete Records;
    if(Exit) delete Exit;
    if(Load) delete Load;
    SDL_DestroyTexture(Ship::Waves);
    SDL_DestroyTexture(Ship::rWaves);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    cout << endl << "Cleaned" << endl;
}
