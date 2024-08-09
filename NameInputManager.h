#pragma once
#ifndef NAMEINPUTMANAGER_H_INCLUDED
#define NAMEINPUTMANAGER_H_INCLUDED

class NameInputManager
{
    public:
        NameInputManager();
        ~NameInputManager();
        void HandleInput(SDL_Event* event);
        void Render();

        void ChangeWarning(bool NameExists);

        int GetNameLength(){return Name.length();}
        string GetName(){return Name;}
    private:
        string Name;
        UIText* NameDisplay;
        UIText** Warnings;
        int NameSize;
        bool WarningType;
};

#endif // NAMEINPUTMANAGER_H_INCLUDED
