#include <GL/glut.h>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

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

struct Character{
    short type;
    Position pos;
    bool isselect;
    bool tomove;
    bool toattack;

    short cd;
    short hp;
    short atk;
    short range;
};

Character
    background={0, {0, 0}, false, false, false, 0, 0, 0, 0},
    hexagone={0, {0, 0}, false, false, false, 0, 0, 0, 0},

    warrior={1, {0, 0}, false, true, false, 0, 3, 2, 1},
    enemy={2, {0, 0}, false, true, false, 0, 2, 2, 1};

clock_t time_appStart, time_anim;



void RenderScene();
void quad(PolyType poly);
void setpolystat(PolyType& poly, char type, float sx, float sy, float dx, float dy, Color c);

void draw(Character &f, string a, string b, string c);
void drawChar();
void drawBG();
void drawMenu();

void animate(int value);
void animofattack();

void button(int button,int state,int x,int y);



int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(700, 70);
    glutInitWindowSize(900, 900);
    glutCreateWindow("Game");

    time_appStart=clock();
    time_anim=clock();

    glutTimerFunc(1000/60, animate, 1);

    glutMouseFunc(button);
    glutDisplayFunc(RenderScene);
    glutMainLoop();
    return 0;
}

void RenderScene() {

    glClearColor(0.8, 0.8, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glScalef(0.005, 0.005, 0);

    drawBG();
    drawChar();

    glutSwapBuffers();
}



void quad(PolyType poly) {
    glPushMatrix();
    glBegin(GL_QUADS);
        glColor3f(poly.color.r, poly.color.g, poly.color.b);

        glVertex2f(poly.sx, poly.sy);
        poly.sx += poly.dx;
        glVertex2f(poly.sx, poly.sy);
        poly.sy += poly.dy;
        glVertex2f(poly.sx, poly.sy);
        poly.sx -= poly.dx;
        glVertex2f(poly.sx, poly.sy);
    glEnd();
    glPopMatrix();
}

void setpolystat(PolyType& poly, char type, float sx, float sy, float dx, float dy, Color c) {
    poly.type=type;
    poly.sx=sx;
    poly.sy=sy;
    poly.dx=dx;
    poly.dy=dy;
    poly.color = c;
}



void draw(Character &f, string fparam, string fcolor, string fpict) {

    fstream fin;
    short i=0;
    PolyType poly;
    short int width, heigh, numcol;

    fin.open(fparam, ios::in);
    if (!fin.is_open()) cout<<"err open char1/param";
        fin>>width;
        fin>>heigh;
        fin>>numcol;
    fin.close();

    Color color[numcol+1];

    fin.open(fcolor, ios::in);
    if (!fin.is_open()) cout<<"err open char1/color";
    i=1;
    while (i<numcol+1){
        fin>>color[i].r;
        fin>>color[i].g;
        fin>>color[i].b;
        i++;
    }
    fin.close();

    int pict[heigh][width];

    fin.open(fpict, ios::in);
    if (!fin.is_open()) cout<<"err open char1/pict";
    i=0;
    while (i<heigh){
        int j=0;
        while (j<width){
            fin>>pict[i][j];
            j++;
        }
        i++;
    }
    fin.close();


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < heigh; j++) {
            if (pict[j][i] != 0) {
                glPushMatrix();
                glTranslatef(i-(heigh/2)+f.pos.x, j-(width/2)+f.pos.y, 0);
                setpolystat(poly, 0, 0, 0, 1, 1, color[pict[j][i]]);
                quad(poly);
                glPopMatrix();
            }
        }
    }
}

void drawChar(){
    animofattack();

    draw(warrior, "sprites/char1/param.txt", "sprites/char1/color.txt", "sprites/char1/pict.txt");
}

void drawBG(){
    int x=-34, y=4, width=5, heigh=3;

    glPushMatrix();
    glScalef(4, 4, 1);
    draw(background, "sprites/bg/main/param.txt", "sprites/bg/main/color.txt", "sprites/bg/main/pict.txt");
    glPopMatrix();

    Character hexagone[100][100];

    for (int i=0; i<width*2; i+=2){
        for (int j=0; j<heigh*2; j+=2){
            glPushMatrix();
            glScalef(5, 5, 1);
            glTranslatef(x+i*8, y+j*-5, 0);
            draw(hexagone[i][j], "sprites/hex/empty/param.txt", "sprites/hex/empty/color.txt", "sprites/hex/empty/pict.txt");
            hexagone[i][j].pos.x=x;
            hexagone[i][j].pos.y=y;
            glPopMatrix();
        }
    }

    width-=1;
    heigh-=1;

    for (int i=1; i<width*2; i+=2){
        for (int j=1; j<heigh*2; j+=2){
            glPushMatrix();
            glScalef(5, 5, 1);
            glTranslatef(x+i*8, y+j*-5, 0);
            draw(hexagone[i][j], "sprites/hex/empty/param.txt", "sprites/hex/empty/color.txt", "sprites/hex/empty/pict.txt");
            glPopMatrix();
        }
    }
}

void drawMenu(){

}



void animate(int value){
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, value);
}

void animofattack(){
    clock_t cT=clock()-time_appStart-time_anim;
    float x, y;

    if(warrior.toattack){
        if (cT<1000){
            y=0;
            x=0+(40.0-0)/(1000-0)*cT;
        }else{
            if(cT<3000){
                x=0+(40.0-0)/(1000-0)*1000;
                y=0;
            }else{
                    time_anim=clock();
                    warrior.toattack=false;
                }
            }
        }
        glTranslatef(x, y, 0);
}



void button(int button, int state, int x, int y){
    if (button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN) {
            warrior.toattack=true;
        }
    }
}
