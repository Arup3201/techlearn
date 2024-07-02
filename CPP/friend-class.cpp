#include <bits/stdc++.h>
using namespace std;

class Point
{
private:
    int x, y;

public:
    Point()
    {
        this->x = 0;
        this->y = 0;
    }

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    pair<int, int> getCoords()
    {
        return {x, y};
    }

    friend class Player;
};

class Player
{
private:
    string name = "Arup";
    Point p;

public:
    Player() {}
    Player(Point p)
    {
        this->p = p;
    }

    void goUp()
    {
        p.x++;
        cout << "Player went up by 1 and currently at " << p.getCoords().first << "\n";
    }
};

int main()
{
    Player player;
    player.goUp();
    return 0;
}