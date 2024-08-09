#ifndef LINEMANAGER_H_INCLUDED
#define LINEMANAGER_H_INCLUDED

class LineManager
{
    public:
        LineManager(int lines_amount);
        ~LineManager();
        Ship* Add_ship(int line, ShipInfo* CurStats, int IsRev);
        int FindFreeLine();
        void UpdateLine(int line, bool flag);
        void MakeAllAvailable(void);
    private:
        queue<int>* FreeLines;
        int Lines_amount;
};

#endif // LINEMANAGER_H_INCLUDED
