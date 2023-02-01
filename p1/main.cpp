#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include<fstream>
#include<vector>
#include<cmath>
using namespace std;
void drawtriangle(int x0, int y0, int x1, int y1, int x2, int y2);
int x0, yy0, x1, yy1, x2, y2, d;
float m = 0.0;
bool is_select = false;
void* object = nullptr;
double sx = 1.1, sy = 1.1;
int x_1, y_1, x_2, y_2;
bool jianqie = false;
bool first = true;
double angle = 15 * 3.1415926 / 180;
void clear(int, int, int, int);
typedef struct
{
	int x, y;
}Point;
class Shape
{
public:
	Shape(vector<Point> vec)
	{
		points.clear();
		unsigned int i;
		for (i = 0; i < vec.size(); i++)
		{
			points.push_back(vec[i]);
		}
		for (i = 0; i < points.size(); i++)
		{
			x += points[i].x;
			y += points[i].y;
		}
		x /= points.size(); y /= points.size();
		cout << x << " " << y << endl;
	}
	void draw()
	{
		unsigned int i;
		for (i = 2; i < points.size(); i++)
		{
			drawtriangle(points[0].x, points[0].y, points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
		}
	}
	vector<Point> points;
	unsigned int x, y;
};
class Area
{
public:
	int x, y, width, height;
	Area(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->height = height;
		this->width = width;
	}
	Area(){}
	void draw()
	{
		clear(0, 0, 500, y);
		clear(0, y, x, 500);
		clear(0, y+height, 500, 500);
		clear(x + width, 0, 500 - x - width, 500);
	}
};
void drawline(int x, int y, int xx, int yy);
class Line
{
public:
	Line(vector<Point> vec)
	{
		unsigned int i = 0;
		points.push_back(vec[0]);
		points.push_back(vec[1]);
	}
	void draw()
	{
		drawline(points[0].x, points[0].y, points[1].x, points[1].y);
	}
	vector<Point> points;
};
Area tmparea;
vector<Point> poi;
vector<Shape> shapes;
vector<Area> areas;
vector<Line> lines;
void drawpoint(int x, int y)		
{
	glBegin(GL_POINTS);
	glVertex2d(x, y);
	glEnd();
}

void drawline1(int x, int y, int d)
{
	y = yy0;
	for (x = x0; x < x1; x++) {
		drawpoint(x, y);
		if (d < 0) {
			y = y + 1;
			d = d + 2 * (x1 - x0) + 2 * (yy0 - yy1);
		}
		else {
			d = d + 2 * (yy0 - yy1);
		}
	}
}

void drawline2(int x, int y, int d)
{
	x = x0;
	for (y = yy0; y < yy1; y++) {
		drawpoint(x, y);

		if (d < 0) {
			x = x + 1;
			d = d + 2 * (yy1 - yy0) + 2 * (x0 - x1);
		}
		else {
			d = d + 2 * (x0 - x1);
		}
	}
}

void drawline3(int x, int y, int d)
{
	y = yy0;
	for (x = x0; x > x1; x--)
	{
		drawpoint(x, y);

		if (d < 0) {
			y = y + 1;
			d = d + 2 * (x0 - x1) + 2 * (yy0 - yy1);
		}
		else {
			d = d + 2 * (yy0 - yy1);
		}
	}
}

void drawline4(int x, int y, int d)
{
	x = x0;
	for (y = yy0; y < yy1; y++) {

		drawpoint(x, y);

		if (d < 0) {
			x = x - 1;
			d = d + 2 * (yy1 - yy0) + 2 * (x1 - x0);
		}
		else {
			d = d + 2 * (x1 - x0);
		}
	}
}

void drawline5(int x, int y, int d)
{
	if (x0 > x1) {
		int temp = x0;
		x0 = x1;
		x1 = temp;
	}
	y = yy0;
	for (x = x0; x < x1; x++)
		drawpoint(x, y);

}

void drawline6(int x, int y, int d)
{
	if (yy0 > yy1) {
		int temp = yy0;
		yy0 = yy1;
		yy1 = temp;
	}
	x = x0;
	for (y = yy0; y < yy1; y++)
		drawpoint(x, y);

}

void drawline(int x, int y, int xx, int yy)
{
	glColor3f(1.0, 1.0, 1.0);
	x0 = x; yy0 = y; x1 = xx; yy1 = yy;
	int temp;
	if (yy0 > yy1) {
		temp = yy0;
		yy0 = yy1;
		yy1 = temp;

		temp = x0;
		x0 = x1;
		x1 = temp;

	}
	d = 2 * (yy1 - yy0 - (x1 - x0));
	m = (float)(yy1 - yy0) / (x1 - x0);
	if (x0 == x1)
		drawline6(x, y, d);
	else if (yy0 == yy1)
		drawline5(x, y, d);
	else
		if (m > 1)
			drawline2(x, y, d);
		else
			if (m > 0)
				drawline1(x, y, d);
			else
				if (m > -1)
					drawline3(x, y, d);
				else
					drawline4(x, y, d);
}
int find(int a[], int count, int b)
{
	int i, flag;
	flag = a[0];
	if (b == 0)
	{
		for (i = 0; i < count; i++)
		{
			if (a[i] < flag)
				flag = a[i];
		}

	}
	if (b == 1)
	{
		for (i = 0; i < count; i++)
		{
			if (a[i] > flag)
				flag = a[i];
		}

	}
	return flag;

}
void drawtriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	int a[3], b[3];
	a[0] = x0;
	a[1] = x1;
	a[2] = x2;
	b[0] = y0;
	b[1] = y1;
	b[2] = y2;
	int xMin, xMax, yMin, yMax;
	xMin = find(a, 3, 0);
	xMax = find(a, 3, 1);
	yMin = find(b, 3, 0);
	yMax = find(b, 3, 1);
	float A, B, C;
	int f01, f02;
	f01 = (y0 - y1) * x2 + (x1 - x0) * y2 + x0 * y1 - x1 * y0;
	f02 = (y2 - y0) * x1 + (x0 - x2) * y1 + x2 * y0 - x0 * y2;
	int x, y;
	glBegin(GL_POINTS);
	for (y = yMin; y < yMax; y++)
	{
		for (x = xMin; x < xMax; x++)
		{
			A = (float)((y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0) / f01;
			B = (float)((y2 - y0) * x + (x0 - x2) * y + x2 * y0 - x0 * y2) / f02;
			C = 1 - A - B;
			if (A > 0 && B > 0 && C > 0)
			{
				glColor3f(A, B, C);
				glVertex2d(x, y);
			}
		}
	}
	glEnd();
}
void draw();
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'c' || key == 'C')
	{
		while (shapes.size())
		{
			shapes.pop_back();
		}
		while (poi.size())
		{
			poi.pop_back();
		}
		while (areas.size())
		{
			areas.pop_back();
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		return;
	}
	else if (key == 'q' || key == 'Q')
	{
		exit(0);
		return;
	}
	else if (key == 'w' || key == 'W')
	{
		is_select = !is_select;
		cout << "select mode" << is_select << endl;
	}
	else if (key == 'M' || key == 'm')
	{
		if (!jianqie && areas.size())
		{
			;
		}
		else
		{
			jianqie = !jianqie;
			cout << "clipping mode" << jianqie << endl;
		}

	}
	else if (key == 'Y' || key == 'y')
	{
		if (areas.size())areas.pop_back();
		areas.push_back(tmparea);
	}
	else if (key == 'T' || key == 't')
	{
		if (poi.size() != 2)
		{
			Shape sh(poi);
			shapes.push_back(sh);
			poi.clear();
		}
		else
		{
			Line l(poi);
			lines.push_back(poi);
			poi.clear();
		}
	}
	else if (key == 'R' || key == 'r')
	{
		if(areas.size())areas.pop_back();
	}
	else
	{
		if (key == 'a' || key == 'A')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					for (unsigned int i = 0; i < ptr->points.size(); i++)
					{
						ptr->points[i].y += 20;
					}
					ptr->y += 20;
				}
			}
		}
		else if (key == 's' || key == 'S')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					for (unsigned int i = 0; i < ptr->points.size(); i++)
					{
						ptr->points[i].y -= 20;
					}
					ptr->y -= 20;
				}
			}
		}
		else if (key == 'd' || key == 'D')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					for (unsigned int i = 0; i < ptr->points.size(); i++)
					{
						ptr->points[i].x -= 20;
					}
					ptr->x -= 20;
				}
			}
		}
		else if (key == 'f' || key == 'F')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					for (unsigned int i = 0; i < ptr->points.size(); i++)
					{
						ptr->points[i].x += 20;
					}
					ptr->x += 20;
				}
			}
		}
		else if (key == 'O'||key=='o')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					unsigned int i;
					for (i = 0; i < ptr->points.size(); i++)
					{
						double a_x = ptr->points[i].x, a_y = ptr->points[i].y;
						ptr->points[i].x = (a_x - ptr->x) * sx + ptr->x;
						ptr->points[i].y = (a_y - ptr->y) * sy + ptr->y;
					}
				}
			}
		}
		else if (key == 'p'||key=='P')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					unsigned int i;
					for (i = 0; i < ptr->points.size(); i++)
					{
						double a_x = ptr->points[i].x, a_y = ptr->points[i].y;
						ptr->points[i].x = (a_x - ptr->x) / sx + ptr->x;
						ptr->points[i].y = (a_y - ptr->y) / sy + ptr->y;
					}
				}
			}
		}
		else if (key == 'k' || key == 'K')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					unsigned int i;
					for (i = 0; i < ptr->points.size(); i++)
					{
						double a_x = ptr->points[i].x, a_y = ptr->points[i].y;
						ptr->points[i].x = (a_x - ptr->x) * cos(angle) - (a_y - ptr->y) * sin(angle) + ptr->x;
						ptr->points[i].y = (a_x - ptr->x) * sin(angle) + (a_y - ptr->y) * cos(angle) + ptr->y;
					}
				}
			}
		}
		else if (key == 'L' || key == 'l')
		{
			if (is_select)
			{
				if (object != nullptr)
				{
					Shape* ptr = (Shape*)object;
					unsigned int i;
					for (i = 0; i < ptr->points.size(); i++)
					{
						double a_x = ptr->points[i].x, a_y = ptr->points[i].y;
						ptr->points[i].x = (a_x - ptr->x) * cos(-angle) - (a_y - ptr->y) * sin(-angle) + ptr->x;
						ptr->points[i].y = (a_x - ptr->x) * sin(-angle) + (a_y - ptr->y) * cos(-angle) + ptr->y;
					}
				}
			}
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	draw();
}
double getDistance(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
double getArea(Point a, Point b, Point c)
{
	double ab = getDistance(a, b);
	double bc = getDistance(b, c);
	double ca = getDistance(c, a);
	double p = (ab + bc + ca) / 2;
	return sqrt(p * (p - ab) * (p - bc) * (p - ca));
}
bool PointInTriangele(Point a,Point b,Point c,Point p)
{
	double areaABC = getArea(a, b, c);
	double areaABP = getArea(a, b, p);
	double areaACP = getArea(a, c, p);
	double areaBCP = getArea(b, c, p);
	return fabs(areaABC - (areaABP + areaACP + areaBCP)) < 1e-6 ? true : false;
}
void* searchObject(int x, int y)
{
	unsigned int i,j;
	Point a[4],P;
	P.x = x; P.y = y;
	for (i = 0; i < shapes.size(); i++)
	{
		a[0].x = shapes[i].points[0].x;
		a[0].y = shapes[i].points[0].y;
		for (j = 2; j < shapes[i].points.size(); j++)
		{
			a[1].x = shapes[i].points[j - 1].x;
			a[1].y = shapes[i].points[j - 1].y;
			a[2].x = shapes[i].points[j].x;
			a[2].y = shapes[i].points[j].y;
			if (PointInTriangele(a[0], a[1], a[2], P))
			{
				return &shapes[i];
			}
		}
	}
	return nullptr;
}
void clear(int x, int y, int width, int height)
{
	int i=x, j=y;
	glBegin(GL_POINTS);
	for (i=x;i<=x+width;i++)
	{
		for (j=y; j <=y+height; j++)
		{
			glColor3f(0, 0, 0);
			glVertex2d(i, j);
		}
	}
	glEnd();
}
void mousefun(int b, int st, int x, int y)
{
	if (!is_select&&!jianqie)
	{
		if (b == GLUT_LEFT_BUTTON && st == GLUT_DOWN)
		{
			Point tmp;
			tmp.x = x;
			tmp.y = 500 - y;
			poi.push_back(tmp);
		}
		if (b == GLUT_RIGHT_BUTTON && st == GLUT_DOWN)
		{

		}
	}
	else if(is_select)
	{
		object = searchObject(x, 500 - y);
	}
	else if (jianqie)
	{
		if (first)
		{
			cout << "choose the leftup corner" << x << " " << y << endl;
			x_1 = x;
			y_1 =  y;
			first = false;
		}
		else
		{
			x_2 = x;
			y_2 = y;
			int scissorX = x_1;
			int scissorY = 500 - (y_1 + y_2-y_1);
			int scissorW = x_2-x_1;
			int scissorH = y_2-y_1;
			cout << x_1 << " " << x_2 << " " << y_1 << " " << y_2 << endl;
			Area a = Area(scissorX, scissorY, scissorW, scissorH);
			if (areas.size())areas.pop_back();
			tmparea = a;
			first = true;
			jianqie = false;
			cout << "clipping mode" << jianqie << endl;
		}
	}
}
void draw()
{
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < shapes.size(); i++)
	{
		shapes[i].draw();
	}
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].draw();
	}
	for (int i = 0; i < areas.size(); i++)
	{
		areas[i].draw();
	}

	//poi.clear();
	glFlush();
}
void display(void)
{
	glColor3f(1.0, 1.0, 1.0);
	draw();
	glFlush();
}
void tishi()
{
	cout << "Hello, welcome, and this program can create graphs, do translate, scale, rotate, and clip." << endl;
	cout << "left click to choose points to make lines or polygons, and then press t to draw.";
	cout << "press w/W to enter or quit select mode" << endl;
	cout << "select graph to click interior area" << endl;
	cout << "you need to select the graph before interaction" << endl;
	cout << "press O/P to scale the graph" << endl;
	cout << "press ASDF to translate the graph" << endl;
	cout << "press K/L to rotate the graph" << endl;
	cout << "Before clipping, press M to enter cut mode, and then choose the area of clipping, and then press y to clip" << endl;
	cout << "After clipping, press r to recover" << endl;
	cout << "select mode1 means enter, select mode0 means quit" << endl;
	cout << "press c to delete all graphs created" << endl;
	cout << "enter q to quit" << endl;
}
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 510, 0, 510);
	tishi();
}
void loadResource(int argc,char **argv)
{
	poi.clear();
	shapes.clear();
	if (argc == 2)
	{
		fstream infile(argv[1], ios::in);
		unsigned int count;
		infile >> count;
		cout << "count:" << count << endl;
		unsigned int i,j;
		for (i = 0; i < count; i++)
		{
			unsigned int points;
			infile >> points;
			cout << "points" << points << endl;
			for (j = 0; j < points; j++)
			{
				Point t;
				infile >> t.x >> t.y;
				cout << "x:" << t.x << "  y:" << t.y << endl;
				poi.push_back(t);
			}
			cout << poi.size() << endl;
			shapes.push_back(poi);
			poi.clear();
		}
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(350, 150);
	loadResource(argc,argv);
	glutCreateWindow("hello");
	init();
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousefun);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}