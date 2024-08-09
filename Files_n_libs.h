#pragma once
#ifndef FILES_N_LIBS_H_INCLUDED
#define FILES_N_LIBS_H_INCLUDED

#include <iostream>

using namespace std;

#include "SDL2\include\SDL2\SDL.h"
#include "SDL2\include\SDL2\SDL_image.h"
#include "SDL2\include\SDL2\SDL_ttf.h"


#include <map>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>

#include "GameObject.h"
#include "UIText.h"
#include "Button.h"
#include "NameInputManager.h"
#include "ShipTypeManager.h"
#include "BigBang.h"
#include "Ship.h"
#include "Background.h"
#include "Submarine.h"
#include "Torpedo.h"
#include "LineManager.h"
#include "RecordManager.h"
#include "Game.h"

#endif // FILES_N_LIBS_H_INCLUDED

/**
C:\SDL2\include\SDL2 <linker settings search directions compiler>
C:\SDL2\lib <linker settings search directions linker>
-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image <linker settings other_linker_opitons>
**/
