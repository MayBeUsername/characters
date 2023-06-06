#include "mod.h"

using namespace std;

clock_t time_appStart, time_anim;

short count, chance;

float lmouse_x=0, lmouse_y=0,
      rmouse_x=0, rmouse_y=0;

const short numofcards=1,
            xsizehexnet=9,
            ysizehexnet=5;

Entity
    background={0, {0, 0}, false, {{0, 0, 0, 0}, false, false}, {0, false, false}, {false}},
    desk={1, {0, 0}, false, {{0, 0, 0, 0}, false, false}, {0, false, false}, {false}},

    hexagone[xsizehexnet][ysizehexnet]={50, {0, 0}, false, {{0, 0, 0, 0}, false, false}, {0, false, false}, {false}},
    card[numofcards]={51, {0, 0}, false, {{0, 0, 0, 0}, false, false}, {0, false, false}, {false}},

    warrior={150, {0, 0}, false, {{0, 3, 2, 1}, false, false}, {0, false, false}, {false}},
    enemy={200, {0, 0}, false, {{0, 2, 2, 1}, false, false}, {0, false, false}, {false}};


void fillfunc();

//////////////////

void RenderScene();

void drawWarrior();
void drawBG();
void drawHexNet();
void drawDesk();
void drawCard();

void animate(int value);
void animofattack();

void button(int button,int state,int x,int y);



int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(700, 70);
    glutInitWindowSize(900, 900);
    glutCreateWindow("Game");

    time_appStart=clock();
    time_anim=clock();

    glutTimerFunc(1000/60, animate, 1);
    fillfunc();

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
    drawDesk();

    glutSwapBuffers();
}

//////////////////


void fillfunc(){
    while (count<1){
        for (int i=0; i<xsizehexnet; i++)
            for (int j=0; j<ysizehexnet; j++){
                hexagone[i][j].isselect=false;
                hexagone[i][j].hexagone.inuse=false;
                hexagone[i][j].hexagone.tomove=false;
                chance=rand()%100;
                if (chance>80) hexagone[i][j].hexagone.type=1;
            }

        for (int i=0; i<numofcards; i++)
            card[i].card.inuse=false;
    count++;
    }
}

//////////////////

void drawWarrior(){
    glPushMatrix();
    animofattack();

    draw(warrior, "sprites/char1/param.txt", "sprites/char1/color.txt", "sprites/char1/pict.txt");
    glPopMatrix();
}

void drawBG(){
    glPushMatrix();
    glScalef(4, 4, 1);
    draw(background, "sprites/bg/main/param.txt", "sprites/bg/main/color.txt", "sprites/bg/main/pict.txt");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 20, 0);
    drawHexNet();
    glPopMatrix();
}

void drawHexNet(){
    int x=-34, y=4;
    float X=0, Y=0;

    for (int i=0; i<xsizehexnet; i++){
        int j=0;

        if (i%2==0) j=0;
        else j=1;

        for (j; j<ysizehexnet; j+=2){

            bool lmousehit{
                (lmouse_x>hexagone[i][j].pos.x-1)&&
                (lmouse_x<hexagone[i][j].pos.x+3)&&
                (lmouse_y>hexagone[i][j].pos.y-1)&&
                (lmouse_y<hexagone[i][j].pos.y+3)
            },
            rmousehit{
                (rmouse_x>hexagone[i][j].pos.x-1)&&
                (rmouse_x<hexagone[i][j].pos.x+3)&&
                (rmouse_y>hexagone[i][j].pos.y-1)&&
                (rmouse_y<hexagone[i][j].pos.y+3)
            };

            glPushMatrix();
            glScalef(5, 5, 1);
            X=(x+i*4)+17;
            Y=(y+j*-2.5)-4;
            glTranslatef(X, Y, 0);

            if (lmousehit) hexagone[i][j].isselect=true;
            else hexagone[i][j].isselect=false;

            if ((card->isselect) &&
                (!card->card.inuse) &&
                (hexagone[i][j].isselect) &&
                (rmousehit)){
                    hexagone[i][j].hexagone.inuse=true;
                    card->card.inuse=true;
                }

            if ((card->card.inuse) &&
                (hexagone[i][j].hexagone.type!=1) &&
                (hexagone[i][j].isselect) &&
                (rmousehit)){
                    hexagone[i][j].hexagone.inuse=true;
                }

            if ((hexagone[i][j].hexagone.inuse) && (!rmousehit)) hexagone[i][j].hexagone.inuse=false;

            switch (hexagone[i][j].hexagone.type){
                case 0:{draw(hexagone[i][j], "sprites/hex/empty/param.txt", "sprites/hex/empty/color.txt", "sprites/hex/empty/pict.txt"); break;}
                case 1:{draw(hexagone[i][j], "sprites/hex/pool/param.txt", "sprites/hex/pool/color.txt", "sprites/hex/pool/pict.txt"); break;}
                default:{};
            }

            if ((hexagone[i][j].isselect) && (hexagone[i][j].hexagone.type!=1)) draw(hexagone[i][j], "sprites/hex/selected_empty/param.txt", "sprites/hex/selected_empty/color.txt", "sprites/hex/selected_empty/pict.txt");

            hexagone[i][j].pos.x=X;
            hexagone[i][j].pos.y=Y;

            if ((hexagone[i][j].hexagone.inuse) && (card->card.inuse)) {
                glPushMatrix();
                glTranslatef(1, 2, 0);
                glScalef(0.3, 0.3 ,1);
                drawWarrior();
                warrior.pos.x=hexagone[i][j].pos.x*3.3;
                warrior.pos.y=hexagone[i][j].pos.y*3.3;
                glPopMatrix();
            }

            glPopMatrix();
        }
    }
}

void drawDesk(){
    glPushMatrix();
    glScalef(4, 4, 1);
    glTranslatef(-35, -7, 0);
    draw(desk, "sprites/bar/param.txt", "sprites/bar/color.txt", "sprites/bar/pict.txt");
    glPopMatrix();

    drawCard();
}

void drawCard(){
    short character=0;

    glPushMatrix();
    glScalef(4.5, 4.5, 1);
    glTranslatef(3, -17, 0);

    for (int i=0; i<numofcards; i++){

        if(!card->card.inuse){
            card[i].pos.x=20/numofcards-20;
            card[i].pos.y=-19;

            if((lmouse_x>card[i].pos.x-2)&&(lmouse_x<card[i].pos.x+3)&&(lmouse_y>card[i].pos.y-1)&&(lmouse_y<card[i].pos.y+8))
                         card[i].isselect=true;
            else card[i].isselect=false;

            if (card[i].isselect) draw(card[i], "sprites/card/selected/param.txt", "sprites/card/selected/color.txt", "sprites/card/selected/pict.txt");
            else draw(card[i], "sprites/card/norm/param.txt", "sprites/card/norm/color.txt", "sprites/card/norm/pict.txt");

            character=150;
            switch(character){
                case 150:{
                    glPushMatrix();
                        glScalef(0.3, 0.3, 1);
                        glTranslatef(-7, -50, 0);
                        drawWarrior();
                        glScalef(0.3, 0.3, 1);
                    glPopMatrix();
                    break;
                    }
                default:{cout<<"error of choosing character in drawCard";}
                }
            }
        }
    glPopMatrix();
}

////////////////////

void animate(int value){
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, value);
}

void animofattack(){
    clock_t cT=clock()-time_appStart-time_anim;
    float animx, animy;

    if(warrior.character.toattack){
        if (cT<1000){
            animy=0;
            animx=0+(40.0-0)/(1000-0)*cT;
        }else{
            if(cT<3000){
                animx=0+(40.0-0)/(1000-0)*1000;
                animy=0;
            }else{
                    time_anim=clock();
                    warrior.character.toattack=false;
                }
            }
        }
        glTranslatef(animx, animy, 0);
}

///////////////////

void button(int button, int state, int x, int y){
    if (button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN) {
            rmouse_x=(x-450)/22;
            rmouse_y=(y-450)/22*-1;
        }
    }

    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN){
            lmouse_x=(x-450)/22;
            lmouse_y=(y-450)/22*-1;
        }
}
