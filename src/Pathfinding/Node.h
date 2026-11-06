#pragma once
#include <iostream>

class Node
{
private:

    std::pair<int, int> m_pos;

    int m_level = 1;

    bool IsVisited = false;
    bool IsWall = false;
    bool IsBlocked = false; //taken

public:


    //Get
    int GetX();
    int GetY();

    int GetLevel();

    bool GetIsVisited();
    bool GetIsWall();
    bool GetIsBlocked();

    //Set
    void SetX(int x);
    void SetY(int y);

    void SetLevel(int value);

    void SetIsVisited(bool visited);
    void SetIsWall(bool wall);
    void SetIsBlocked(bool blocked);

};
