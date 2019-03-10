#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>
#include <unistd.h>

const double PI = 3.14;
const int SPACEBAR = 32;

double angle = 0;
double bx = 0.75, dx = -0.001, by;
double ballx, bally;
double xO, yO;
double vx, vy, a = -0.3, t;
int timerToDisapear = 0;
double beta;
double deltaX = -0.25, deltaY = 0.16;
double gama;
bool isShoot = false;

void init()
{
    glClearColor(0.004, 0.031, 0.392, 0); // set background color
    glOrtho(-1, 1, -1, 1, 1, -1); // set coordinate system
}

void DrawGround()
{
    double x, y;
    glColor3d(1, 1, 1);
    glBegin(GL_LINE_STRIP);
    for (x = -1; x<=1; x+=0.01)
    {
        y = 0.1*sin(x*6);
        glVertex2d(x, y);
    }
    glEnd();
}

void DrawWheel()
{
    double alpha, teta = PI/60;
    double x, y;
    
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    
    for (alpha = 0; alpha <= 2 * PI; alpha+=teta)
    {
        x= cos(alpha);
        y= sin(alpha);
        glVertex2d(x, y);
    }
    
    glEnd();
    
    glLineWidth(1);

    glBegin(GL_LINES);
    teta = PI / 10;
    for (alpha = 0; alpha <= 2 * PI; alpha+=teta)
    {
        x = cos(alpha);
        y = sin(alpha);
        glVertex2d(0, 0);
        glVertex2d(x, y);
    }
    
    glEnd();
}

void DrawCircle()
{
    double alpha, teta = PI/60;
    double x, y;
    
    glBegin(GL_POLYGON);
    
    for (alpha = 0; alpha <= 2 * PI; alpha+=teta)
    {
        x= cos(alpha);
        y= sin(alpha);
        glVertex2d(x, y);
    }
    
    glEnd();
}

void DrawTriangular()
{
    glBegin(GL_POLYGON);
    glVertex2d(-1, 1);
    glVertex2d(1, 1);
    glVertex2d(0, -1);
    glEnd();
}

void DrawTank()
{
    // front wheel
    glPushMatrix();
    glTranslated(0.06, 0, 0);
    glScaled(0.022, 0.022, 1);
    glRotated(600 * angle, 0, 0, 1);
    DrawWheel();
    glPopMatrix();
    
    // center whell
    glPushMatrix();
    glTranslated(0.01, 0, 0);
    glScaled(0.022, 0.022, 1);
    glRotated(600 * angle, 0, 0, 1);
    DrawWheel();
    glPopMatrix();
    
    //center whell
    glPushMatrix();
    glTranslated(-0.04, 0, 0);
    glScaled(0.022, 0.022, 1);
    glRotated(600 * angle, 0, 0, 1);
    DrawWheel();
    glPopMatrix();
    
    //rear whell
    glPushMatrix();
    glTranslated(-0.09, 0, 0);
    glScaled(0.022, 0.022, 1);
    glRotated(600 * angle, 0, 0, 1);
    DrawWheel();
    glPopMatrix();
    
    glLineWidth(3);

    // bottom body
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.17, 0.06);
    glVertex2d(0.07, -0.02);
    glVertex2d(-0.1, -0.02);
    glVertex2d(-0.2, 0.06);
    glEnd();
    
    glPushMatrix();
    glTranslated(0.14, 0.05, 0);
    glScaled(0.015, 0.015, 1);
    DrawCircle();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.17, 0.05, 0);
    glScaled(0.015, 0.015, 1);
    DrawCircle();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(0.038, 0.05, 0);
    glScaled(0.015, 0.015, 1);
    DrawTriangular();
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-0.012, 0.05, 0);
    glScaled(0.015, 0.015, 1);
    DrawTriangular();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.062, 0.05, 0);
    glScaled(0.015, 0.015, 1);
    DrawTriangular();
    glPopMatrix();

    glLineWidth(1);

    // center body
    glBegin(GL_LINE_LOOP);
    glVertex2d(-0.15, 0.06);
    glVertex2d(-0.12, 0.09);
    glVertex2d(0.12, 0.09);
    glVertex2d(0.15, 0.06);
    glEnd();
    
    // top body
    glBegin(GL_LINE_LOOP);
    glVertex2d(-0.10, 0.09);
    glVertex2d(-0.07, 0.12);
    glVertex2d(0.07, 0.12);
    glVertex2d(0.10, 0.09);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glVertex2d(-0.09, 0.1);
    glVertex2d(-0.08, 0.11);
    glVertex2d(-0.26, 0.16);
    glVertex2d(-0.25, 0.15);
    glEnd();
}


void DrawHit(double initialX, double initialY, double div)
{
    glBegin(GL_POLYGON);
    glVertex2d(initialX, initialY);
    glVertex2d(initialX + 0.15/div, initialY + 0.1/div);
    glVertex2d(initialX + 0.12/div, initialY + 0.05/div);
    
    glVertex2d(initialX + 0.2/div, initialY + 0.07/div);
    glVertex2d(initialX + 0.17/div, initialY + 0.02/div);
    
    glVertex2d(initialX + 0.23/div, initialY + 0.02/div);
    glVertex2d(initialX + 0.19/div, initialY - 0.03/div);
    
    glVertex2d(initialX + 0.25/div, initialY - 0.08/div);
    glVertex2d(initialX + 0.16/div, initialY - 0.05/div);
    
    glVertex2d(initialX + 0.22/div, initialY - 0.13/div);
    glVertex2d(initialX + 0.13/div, initialY - 0.06/div);
    
    glVertex2d(initialX + 0.08/div, initialY - 0.16/div);
    glVertex2d(initialX + 0.05/div, initialY - 0.06/div);
    
    glVertex2d(initialX - 0.01/div, initialY - 0.19/div);
    glVertex2d(initialX - 0.05/div, initialY - 0.06/div);
    
    glVertex2d(initialX - 0.07/div, initialY - 0.15/div);
    glVertex2d(initialX - 0.06/div, initialY - 0.05/div);
    
    glVertex2d(initialX - 0.17/div, initialY - 0.07/div);
    glVertex2d(initialX - 0.1/div, initialY - 0.01/div);
    
    glVertex2d(initialX - 0.15/div, initialY + 0.01/div);
    glVertex2d(initialX - 0.08/div, initialY + 0.04/div);
    
    glVertex2d(initialX - 0.19/div, initialY + 0.09/div);
    glVertex2d(initialX - 0.06/div, initialY + 0.06/div);
    
    glVertex2d(initialX - 0.04/div, initialY + 0.15/div);
    glVertex2d(initialX + 0, initialY + 0.1/div);
    
    glVertex2d(initialX + 0.04/div, initialY + 0.17/div);
    glVertex2d(initialX + 0.08/div, initialY + 0.08/div);
    
    glVertex2d(initialX + 0.15/div, initialY + 0.1/div);
    glEnd();
}

void DrawHits()
{
    glColor3d(0.8, 0, 0);
    DrawHit(ballx, bally,1.5);
    glColor3d(1, 0.992, 0.219);
    DrawHit(ballx, bally,2);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
    glLoadIdentity(); //start the transformations from the beginning
    
    DrawGround();

    glPushMatrix();

    glTranslated(bx, by, 0);
    glRotated(beta * 180 / PI , 0, 0, 1);
    if (dx>0)
        glScaled(-1, 1, 1);
    glColor3d(1, 1, 1);
    DrawTank();
    glPopMatrix();
    

    if (isShoot) {
        glPushMatrix();
        glTranslated(ballx, bally, 0);
        glScaled(0.01, 0.01, 1);
        glColor3d(0.3 , 0.3, 0.3);
        DrawCircle();
        glPopMatrix();
    }

    if (0.1*sin(ballx * 6) > bally)
    {
        isShoot = false;
        DrawHits();
    }
    
    glutSwapBuffers();
}

void idle()
{
    
    by = 0.03 + 0.095 * sin(bx*6);
    beta = atan(0.7 * cos (bx * 6)); // beta is in radian

    if (isShoot)
    {
        t+=0.0008;
        ballx = xO + vx * t;
        bally = yO + vy * t + a*t*t;
    }
    else
    {
        timerToDisapear += 1;
        angle+=0.001;
        bx += 0.1*dx;
        
        if (bx < -0.75 || bx > 0.75)
        {
            dx = -dx;
        }
        
        if (timerToDisapear>50)
        {
            ballx=0;
            bally=0;
        }
    }
    glutPostRedisplay(); // indirect call to display
}

void keyboard(unsigned char button, int x, int y)
{
    
    if (button == SPACEBAR)
    {
        if (!isShoot)
        {
            if (dx>0)
            {
                deltaX = 0.25;
            }
            else
            {
                deltaX = -0.25;
            }
            
            gama = acos(deltaX / sqrt(deltaX*deltaX + deltaY*deltaY));
            
            vx = 0.8*cos(gama+beta);
            vy = 0.8*sin(gama+beta);
            
            xO = bx + ((deltaX)*cos(beta) - (deltaY)*(sin(beta)));
            yO = by + ((deltaX)*sin(beta) + (deltaY)*(cos(beta)));

            t = 0;
            isShoot = true;
            timerToDisapear = 0;
        }
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("HW3_Tank");
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);

    init();
    
    glutMainLoop();
    return 0;
}
