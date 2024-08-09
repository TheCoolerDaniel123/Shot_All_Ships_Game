#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

class Background: public GameObject
{
    public:
        Background(const char* texture, int SourceH, int SourceW, int SourceX, int SourceY);
        ~Background();
        void Update(){};
};

#endif // BACKGROUND_H_INCLUDED
