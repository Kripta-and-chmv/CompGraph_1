
#include "stdafx.h"
#include <vector>
#include <iterator> 
using namespace std;

GLint Width = 512, Height = 512;
int menuFlag = 0;
int triangleMenu, mainMenu, colorMenu;

enum color {RED=1, GREEN, BLUE, WHITE};
int activeTraingle;

//кол-во треугольников
int k=1;

struct type_point
{
	GLint x, y;
	type_point(GLint _x, GLint _y) { x = _x; y = _y; }
};
struct type_triangle
{
	vector <type_point> points;
	int  order;
	GLubyte ColorR = 255, ColorG = 255, ColorB = 255;
};
vector <type_triangle> Triangles;


/* Функция изменения размеров окна */
void Reshape(GLint w, GLint h)
{
	Width = w;    Height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
/* Функция обработки сообщений от клавиатуры */
void Keyboard(unsigned char key, int x, int y)
{
	int rnd = 5;
	int i, n = Triangles.size()-1;

	

	/* Изменение XY-кординат точек */
	if (key == 'w') for (i = 0; i<3; i++)
		Triangles[activeTraingle].points[i].y += rnd;
	if (key == 's') for (i = 0; i<3; i++)
		Triangles[activeTraingle].points[i].y -= rnd;
	if (key == 'a') for (i = 0; i<3; i++)
		Triangles[activeTraingle].points[i].x -= rnd;
	if (key == 'd') for (i = 0; i<3; i++)
		Triangles[activeTraingle].points[i].x += rnd;

	if ((key == ' ') && (activeTraingle >= 0))
	{
		Triangles.erase(Triangles.begin() + activeTraingle);
		activeTraingle = Triangles.size() - 1;
	}
	if (Triangles.size() == 0)
		k = 0;
	glutPostRedisplay();
}

/* Функция обработки сообщения от мыши */
void Mouse(int button, int state, int x, int y)
{
	/* клавиша была нажата, но не отпущена */
	if (state != GLUT_DOWN) return;
	int n = Triangles.size()-1;
	/* новая точка по левому клику */
	if (button == GLUT_LEFT_BUTTON)
	{
		type_point p(x, Height - y);
		if (Triangles.size() != 0)
		{
			if (Triangles[n].points.size() < 3)
				Triangles[n].points.push_back(p);
			else
			{
				type_triangle newTriangle;
				newTriangle.order = k++;
				Triangles.push_back(newTriangle);
				Triangles[n+1].points.push_back(p);
				activeTraingle = Triangles.size() - 1;
			}
		}
		else
		{
			type_triangle newTriangle;
			newTriangle.order = k++;
			Triangles.push_back(newTriangle);
			Triangles[0].points.push_back(p);
			activeTraingle = 0;
		}
		
	}

	glutPostRedisplay();
}

///////Меню
void processColorMenu(int option)
{
	if (Triangles.size() == 0)
		return;
	switch (option)
	{
	case RED:
		Triangles[activeTraingle].ColorR = 255;
		Triangles[activeTraingle].ColorG = 0;
		Triangles[activeTraingle].ColorB = 0;
		break;
	case GREEN:
		Triangles[activeTraingle].ColorR = 0;
		Triangles[activeTraingle].ColorG = 255;
		Triangles[activeTraingle].ColorB = 0;
		break;
	case BLUE:
		Triangles[activeTraingle].ColorR = 0;
		Triangles[activeTraingle].ColorG = 0;
		Triangles[activeTraingle].ColorB = 255;
		break;
	case WHITE:
		Triangles[activeTraingle].ColorR = 255;
		Triangles[activeTraingle].ColorG = 255;
		Triangles[activeTraingle].ColorB = 255;
		break;
	}
	// Almost any menu selection requires a redraw
	glutPostRedisplay();
}

void processMainMenu(int option) { if (option == 3) exit(0); };

void processTriangleMenu(int option)
{
	activeTraingle = option-1;
	glutPostRedisplay();
}

void BuildPopupMenu()
{
	triangleMenu = glutCreateMenu(processTriangleMenu);
	int i;
	for (i = 0; i < Triangles.size(); i++)
	{
		char buffer[3];
		itoa(Triangles[i].order, buffer, 10);
		glutAddMenuEntry(buffer, i+1);
	}
	colorMenu = glutCreateMenu(processColorMenu);
	glutAddMenuEntry("Make triangle red", 1);
	glutAddMenuEntry("Make triangle green", 2);
	glutAddMenuEntry("Make triangle blue", 3);
	glutAddMenuEntry("Make triangle white", 4);

	mainMenu = glutCreateMenu(processMainMenu);
	glutAddSubMenu("Color", colorMenu);
	glutAddSubMenu("Triangles", triangleMenu);
	glutAddMenuEntry("Exit\tEsc", 3);

}

/* Функция вывода на экран */
void Display(void)
{

	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(6); glEnable(GL_POINT_SMOOTH);
	glBegin(GL_TRIANGLES);
	
	for (int i = 0; i < Triangles.size(); i++)
	{
		glColor3ub(Triangles[i].ColorR, Triangles[i].ColorG, Triangles[i].ColorB);
		
		for (int j = 0; j < Triangles[i].points.size(); j++)
		{
			glVertex2i(Triangles[i].points[j].x, Triangles[i].points[j].y);
		}
	}
	glEnd();
	glBegin(GL_POINTS);
	
	if (Triangles.size() != 0)
	{
		glColor3ub(0, 0, 0);
		for (int j = 0; j <Triangles[activeTraingle].points.size(); j++)
			glVertex2i(Triangles[activeTraingle].points[j].x, Triangles[activeTraingle].points[j].y);
	}
	glEnd();
	glFinish();
	//создание меню
	BuildPopupMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
/* Головная программа */
void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Laba 1");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

