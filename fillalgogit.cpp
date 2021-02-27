#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define xpix 500
#include <cstring>
using namespace std;

float r, g, b, x, y;
bool flag = true;
int x2,y2,x3,y3,x4,y4,x5,y5;
int a1,b1;
int a2,b2;
int a3,b3;
int a4,b4;


int counter=1;
char buffer[20];
void drawBitmapText(char *string, float x, float y, float z)
{
	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void delay(float ms){
    clock_t goal = ms + clock();
    while(goal>clock());
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600);
}

void bound_it(int x, int y, float* fillColor, float* bc){
    float color[3];
    glReadPixels(x,y,1.0,1.0,GL_RGB,GL_FLOAT,color);
    if((color[0]!=bc[0] || color[1]!=bc[1] || color[2]!=bc[2])&&(
     color[0]!=fillColor[0] || color[1]!=fillColor[1] || color[2]!=fillColor[2])){
        glColor3f(fillColor[0],fillColor[1],fillColor[2]);
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();
        glFlush();
        bound_it(x+1,y,fillColor,bc);
        bound_it(x-2,y,fillColor,bc);
        bound_it(x,y+2,fillColor,bc);
        bound_it(x,y-2,fillColor,bc);
    }
}

void mouseboundaryfill(int btn, int state, int x, int y){
    y = 600-y;
    if(btn==GLUT_RIGHT_BUTTON)
    {
        if(state==GLUT_DOWN)
        {
            float bCol[] = {1,0,0};
            float color[] = {0,0,1};
            bound_it(x,y,color,bCol);
        }
    }
    if(btn==GLUT_LEFT_BUTTON)
    {

        if(state==GLUT_DOWN)
        {
            if(counter==1)
            {
                a1=x;
                b1=y;
                counter++;
            }
            else if(counter==2)
            {

                a2=x;
                b2=y;
                counter++;
            }
            else if(counter==3)
            {
                a3=x;
                b3=y;
                counter++;
            }
            else if(counter==4)
            {
                a4=x;
                b4=y;
                counter=0;
            }
        }
    }
}

void world(){
    glLineWidth(3);
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(a1,b1);
        glVertex2f(a2,b2);
        glVertex2f(a3,b3);
        glVertex2f(a4,b4);
    glEnd();
    glColor3f(0, 0, 0);
    	drawBitmapText("BOUNDARY FILL ALGORITHM", 50,25,0);
    glFlush();
}







void init2(){
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,400,0,600);
}
struct Point {
	GLint x;
	GLint y;
};

struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
};
Color getPixelColor(GLint x, GLint y) {
	Color color;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
	return color;
}
void setPixelColor(GLint x, GLint y, Color color) {
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void floodFill(GLint x, GLint y, Color oldColor, Color newColor) {
	Color color;
	color = getPixelColor(x, y);

	if(color.r == oldColor.r && color.g == oldColor.g && color.b == oldColor.b)
	{
		setPixelColor(x, y, newColor);
		floodFill(x+1, y, oldColor, newColor);
		floodFill(x, y+1, oldColor, newColor);
		floodFill(x-1, y, oldColor, newColor);
		floodFill(x, y-1, oldColor, newColor);
	}
	return;
}
void mousefloodfill(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON
        && state == GLUT_DOWN) {
        flag = true;
        x2 = x;
        y2 = 600 - y;

    }
    else if (button == GLUT_RIGHT_BUTTON
             && state == GLUT_DOWN)
             {
                 Color newColor = {1.0f, 0.0f, 0.0f};
                 Color oldColor = {1.0f, 1.0f, 1.0f};

                 floodFill(x2, y2, oldColor, newColor);
             }
             glutPostRedisplay();

}

void draw_circle(Point pC, GLfloat radius) {
    if(flag)
    {


	GLfloat step = 1/radius;
	GLfloat x, y;

	for(GLfloat theta = 0; theta <= 360; theta += step) {
		x = pC.x + (radius * cos(theta));
		y = pC.y + (radius * sin(theta));
		glVertex2i(x, y);
	}
	}
}

void display(void) {
	Point pt = {x2,y2};
	GLfloat radius = 50;

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
		draw_circle(pt, radius);
	glEnd();
	glColor3f(0, 0, 0);
	
    	
	glFlush();
}


int main(int argc,char** argv)
{
	glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    int mainWindow=glutCreateWindow("Boundary fill Algorithm on Left and Flood fill Algorithm on Right");
    init();
	glutDisplayFunc(display);
	glutMouseFunc(mousefloodfill);
    int subWindow1=glutCreateSubWindow(mainWindow ,  0, 0 , 400 , 600);
    glutInitWindowPosition(100,100);
    glutDisplayFunc(world);
    glutMouseFunc(mouseboundaryfill);
    init2();

    glutMainLoop();
    return 0;

}
