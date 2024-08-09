#include "Files_n_libs.h"

extern Game* game;

RecordManager::RecordManager(string filename)
{
    int i;
    Converted = false;
    Filename = filename;
    SDL_Rect temp;
    CurRecords = new UIText*[10];
    for(i = 0; i < 10; i++)
    {
        CurRecords[i] = new UIText("fonts/Prototype.ttf", 45, {0, 0, 0}, "randTextToCreate", temp);
        CurRecords[i]->SetX(5);
    }

}

RecordManager::~RecordManager()
{
    int i;
    for(i = 0; i < 10; i++)
            delete CurRecords[i];
}

void RecordManager::LoadCurRecs() //�������� ������ �������� �� �����
{
    ifstream stream;
    GameData CurData;
    int CurNameSize, i;
    char c;
    stream.open(Filename, ios_base::binary);
    if(!stream) cout << "Troubles with opening file " << Filename << endl;
    while(stream.read((char*)&CurNameSize, sizeof(int))) //���� �������� ���� �� �����
    {
        for(i = 0; i < CurNameSize; i++) //����������� ������ ��� ������
        {
            stream.read((char*)&c, sizeof(char));
            CurData.Name += c;
        }
        stream.read((char*)&CurData.Points, sizeof(int)); //������� ���� ������
        CurRecData.push_back(CurData); //��������� � ������ �������� ���������
        CurData.Name = "";
    }
    stream.close();
}

void RecordManager::ConvertCurRecs() //�������������� � �������� ��� �����������
{
    if(Converted)
        return;
    vector<GameData>::iterator itr;
    string All_together;
    int n = 0, y = 0, i;
    for (itr = CurRecData.begin(); itr != CurRecData.end(); advance(itr, 1))
    {
        All_together = "["; All_together += to_string(n + 1); //������������ ������
        if (!n) All_together += " ";
        All_together += "] ";
        All_together += itr->Name; All_together += ": ";
        for(i = 0; i < 12 - itr->Name.length(); i++) All_together += " ";
        All_together += to_string(itr->Points);

        CurRecords[n++]->Update(All_together, -1);

        if(n - 1)
            CurRecords[n - 1]->SetY(y += 5 + CurRecords[n - 2]->GetPlacement().h);
        else
            CurRecords[n - 1]->SetY(y += 5);
    }
    if (CurRecData.empty()) //���� �������� ���, ���������� ���������� �� ����
    {
        CurRecords[0]->Update("No games palyed yet :/", -1);
        CurRecords[0]->SetY(5);
    }
    Converted = true; //��������� ����� ��������������
}

void RecordManager::StoreCurRecs() //������ �������� � ����
{
    if(!CurRecData.size())
        return;
    vector<GameData>::iterator itr;
    ofstream stream;
    int temp, i;
    char c;
    stream.open(Filename, ios_base::binary);
    if(!stream) cout << "Troubles with opening file " << Filename << endl;
    for (itr = CurRecData.begin(); itr != CurRecData.end(); advance(itr, 1)) //������ ����� ������� �������� � ����
    {
        temp = itr->Name.length();
        stream.write((char*)&temp, sizeof(int));
        for(i = 0; i < temp; i++)
        {
            c = itr->Name[i];
            stream.write((char*)&c, sizeof(char));
        }
        temp = itr->Points;
        stream.write((char*)&temp, sizeof(int));
    }
    stream.close();
    cout << endl << "Records stored successfully" << endl;
}

void RecordManager::DeleteRecTexes() //�������� ��������� ������� ��������
{
    Converted = false; //������������ ����� ��������������
}

void RecordManager::AddData(GameData New_info) //���������� � ������ ������ ����������
{
    CurRecData.push_back(New_info);
    sort(CurRecData.begin(), CurRecData.end(), [](const GameData &a, const GameData &b)
    {
        return a.Points > b.Points;
    }); //�������������� ������� �������� �� �������� �����
    if(CurRecData.size() > 10) CurRecData.erase(CurRecData.end() - 1); //�������� ������� �� 11�� �����, ���� �� ����
}

void RecordManager::RenderRecs()
{
    int i, k;
    if(CurRecData.size())
        k = CurRecData.size();
    else
        k = 1;
    for(i = 0; i < k; i++)
        SDL_RenderCopy(Game::renderer, CurRecords[i]->GetText(), NULL, CurRecords[i]->GetPlaceAdress());
}
