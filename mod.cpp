#include "mod.h"

using namespace std;

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
    poly.color=c;
}

void draw(Entity &f, string fparam, string fcolor, string fpict) {

    fstream fin;
    short i=0;
    PolyType poly;
    short int width, heigh, numcol;

    fin.open(fparam, ios::in);
    if (!fin.is_open()) cout<<"err open: "<<fparam<<endl;
        fin>>width;
        fin>>heigh;
        fin>>numcol;
    fin.close();

    Color color[numcol+1];

    fin.open(fcolor, ios::in);
    if (!fin.is_open()) cout<<"err open: "<<fcolor<<endl;
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
    if (!fin.is_open()) cout<<"err open: "<<fpict<<endl;
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


