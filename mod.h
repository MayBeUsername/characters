#ifndef MOD_H
#define MOD_H

#include <GL/glut.h>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <array>

using namespace std;

struct Color {
    GLfloat r, g, b;
};

struct PolyType {
    short type;
    float sx, sy, dx, dy;
    Color color;
};

struct Position{
    GLfloat x, y;
};

struct Stats{
    short cd;
    short hp;
    short atk;
    short range;
};

struct Character{
    Stats stats;
    bool toattack;
    bool tomove;
};

struct Card{
    bool inuse;
};

struct Hexagone{
    short type;
    bool inuse;
    bool tomove;
};

struct Entity{
    short type;
    Position pos;

    bool isselect;

    Character character;
    Hexagone hexagone;
    Card card;
};



void quad(PolyType poly);
void setpolystat(PolyType& poly, char type, float sx, float sy, float dx, float dy, Color c);
void draw(Entity &f, string a, string b, string c);
void fillfunc();



#endif // MOD_H
