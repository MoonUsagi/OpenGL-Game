#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2\gpu\gpu.hpp>
#include <math.h>
#include "glut.h"
#include <iostream>
#include <array>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

int WinNumber = NULL;                    //用來放置視窗代碼
#define MapNumber 26

bool Ismousedown = false;
bool ItemsA = false;
bool ItemsAO = false;
bool ItemsB = false;
bool ItemsC = false;
bool ItemsD = false;
bool ItemsDO = false;

GLuint texName[MapNumber];
cv::Mat texture[MapNumber];
GLUquadric * quad;

int old_rot_x = 0;                       //剛按下滑鼠時的視窗座標
int old_rot_y = 0;

int rot_x = 0;                           //拖曳後的相對座標，用這決定要旋轉幾度
int rot_y = 0;

int record_x = 0;                        //紀錄上一次旋轉的角度
int record_y = 0;

float distanceX = 0;                      //在平移矩陣(glTranslatef();)中使用
float distanceY = 0;                      //在平移矩陣(glTranslatef();)中使用
float distanceZ = 0;                      //在平移矩陣(glTranslatef();)中使用
//float light_position[] = { -20, 20, 0 };  //光源的位置
float light_position[] = { 0, 30, 500 };  //光源的位置



//void WindowSize(int, int);              //負責視窗及繪圖內容的比例
//void Keyboard(unsigned char, int, int); //獲取鍵盤輸入
//void Mouse(int, int, int, int);         //獲取滑鼠按下和放開時的訊息
//void MotionMouse(int, int);             //獲取滑鼠按下期間的訊息
//void Display(void);                     //描繪
//
//void SetLightSource(void);               //設定光源屬性
//void SetMaterial(void);                  //設定材質屬性

void DrawRoom(void)
{
	GLfloat vertexes[8][3] = {
		{-0.5, -0.5, 0.5},
		{0.5, -0.5, 0.5},
		{0.5, -0.5, -0.5},
		{-0.5,-0.5,-0.5},
		{-0.5, 0.5, 0.5},
		{0.5, 0.5, 0.5},
		{0.5, 0.5, -0.5},
		{-0.5, 0.5, -0.5},
	};
	GLfloat texVertexes[4][2] = {
		{0, 1},
		{1, 1},
		{1, 0},
		{0, 0}
	};
	enum PicName
	{
		DESK = 0,
		DOOR,
		SOFA,
		BOOK,
		FLOOR,
		CEIL
	};
	//前

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[PicName::DOOR]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[4]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[5]);
	}
	glEnd();

	//右
	glBindTexture(GL_TEXTURE_2D, texName[PicName::SOFA]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[5]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[6]);
	
	}
	glEnd();

	//後
	glBindTexture(GL_TEXTURE_2D, texName[PicName::BOOK]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[3]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[6]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[7]);
	}
	glEnd();

	//左
	glBindTexture(GL_TEXTURE_2D, texName[PicName::DESK]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[3]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[7]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[4]);
	}
	glEnd();
	//上
	glBindTexture(GL_TEXTURE_2D, texName[PicName::CEIL]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[7]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[6]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[5]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[4]);
	}
	glEnd();
	//下
	glBindTexture(GL_TEXTURE_2D, texName[PicName::FLOOR]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[3]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawCloister(void)
{
	GLfloat vertexes[8][3] = {
		{ -0.5, -0.5, 0.5 },
		{ 0.5, -0.5, 0.5 },
		{ 0.5, -0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, 0.5, 0.5 },
		{ 0.5, 0.5, 0.5 },
		{ 0.5, 0.5, -0.5 },
		{ -0.5, 0.5, -0.5 },
	};
	GLfloat texVertexes[4][2] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
	};
	enum PicName
	{
		DESK = 0,
		DOOR,
		SOFA,
		BOOK,
		FLOOR,
		CEIL
	};
	//前

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[PicName::DOOR]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[4]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[5]);
	}
	glEnd();

	//右
	glBindTexture(GL_TEXTURE_2D, texName[PicName::SOFA]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[5]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[6]);

	}
	glEnd();

	//後
	glBindTexture(GL_TEXTURE_2D, texName[PicName::BOOK]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[3]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[6]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[7]);
	}
	glEnd();

	//左
	glBindTexture(GL_TEXTURE_2D, texName[PicName::DESK]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[3]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[7]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[4]);
	}
	glEnd();
	//上
	glBindTexture(GL_TEXTURE_2D, texName[PicName::CEIL]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[7]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[6]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[5]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[4]);
	}
	glEnd();
	//下
	glBindTexture(GL_TEXTURE_2D, texName[PicName::FLOOR]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[3]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawRoomB(void)
{
	GLfloat vertexes[8][3] = {
		{ -0.5, -0.5, 0.5 },
		{ 0.5, -0.5, 0.5 },
		{ 0.5, -0.5, -0.5 },
		{ -0.5, -0.5, -0.5 },
		{ -0.5, 0.5, 0.5 },
		{ 0.5, 0.5, 0.5 },
		{ 0.5, 0.5, -0.5 },
		{ -0.5, 0.5, -0.5 },
	};
	GLfloat texVertexes[4][2] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
	};
	enum PicName
	{
		DESK = 0,
		DOOR,
		SOFA,
		BOOK,
		FLOOR,
		CEIL,
		Items1,
		Items2,
		Items3,
		Items4,
		ROOM2BK,
		ROOM2DN,
		ROOM2FT,
		ROOM2LF,
		ROOM2RF,
		ROOM2UP
	};
	//前

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[PicName::ROOM2FT]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[4]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[5]);
	}
	glEnd();

	//右
	glBindTexture(GL_TEXTURE_2D, texName[PicName::ROOM2RF]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[5]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[6]);

	}
	glEnd();

	//後
	glBindTexture(GL_TEXTURE_2D, texName[PicName::ROOM2BK]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[3]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[6]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[7]);
	}
	glEnd();

	//左
	glBindTexture(GL_TEXTURE_2D, texName[PicName::ROOM2LF]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[3]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[7]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[4]);
	}
	glEnd();
	//上
	glBindTexture(GL_TEXTURE_2D, texName[PicName::ROOM2UP]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[7]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[6]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[5]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[4]);
	}
	glEnd();
	//下
	glBindTexture(GL_TEXTURE_2D, texName[PicName::ROOM2DN]);
	glBegin(GL_QUADS);
	{
		glTexCoord2fv(texVertexes[0]); glVertex3fv(vertexes[0]);
		glTexCoord2fv(texVertexes[1]); glVertex3fv(vertexes[1]);
		glTexCoord2fv(texVertexes[2]); glVertex3fv(vertexes[2]);
		glTexCoord2fv(texVertexes[3]); glVertex3fv(vertexes[3]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Display(void)
{
	glEnable(GL_DEPTH_TEST);                                    //啟動深度測試
	glClearColor(0.0, 0.0, 0.0, 0.0);                           //用白色塗背景
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         //清除所有像素
	glPolygonMode(GL_BACK, GL_LINE);                            //設定面的背面用線條顯示
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

		//gluLookAt(0, -5, 10, 0, 0, 0, 0, 1, 0);                    //視線的座標及方向
		gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
		//gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1, 0);                    //視線的座標及方向
		glRotatef((float)rot_y + (float)record_y, 1.0, 0.0, 0.0);   //以x軸當旋轉軸
		glRotatef((float)rot_x + (float)record_x, 0.0, 1.0, 0.0);   //以y軸當旋轉軸
		glTranslatef(distanceX, 0, 0);                               //沿著x軸平移
		glTranslatef(0, distanceY, 0);                               //沿著x軸平移
		glTranslatef(0, 0, distanceZ);                               //沿著z軸平移

    //太陽
	glEnable(GL_TEXTURE_2D);                        
	glBindTexture(GL_TEXTURE_2D, texName[17]);
	glPushMatrix();
	glTranslatef(0, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 65, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//水星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[18]);
	glPushMatrix();
	glTranslatef(110, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 5, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//金星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[19]);
	glPushMatrix();
	glTranslatef(140, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 5, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//地球
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[20]);
	glPushMatrix();
	glTranslatef(200, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 10, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//火星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[21]);
	glPushMatrix();
	glTranslatef(250, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 10, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//木星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[22]);
	glPushMatrix();
	glTranslatef(350, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 20, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//土星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[23]);
	glPushMatrix();
	glTranslatef(450, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 20, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//天王星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[24]);
	glPushMatrix();
	glTranslatef(600, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 15, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//海王星
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName[25]);
	glPushMatrix();
	glTranslatef(700, -30, 0);
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 15, 100, 60);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
		
	////房間 測試球
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(0, 0, -10);
	//glutSolidSphere(0.5, 30, 30);
	//glPopMatrix();

	//房間A
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(40, 30, 40);
	DrawRoom();
	glPopMatrix();

	//門
	//glPushMatrix();
	//glBindTexture(GL_TEXTURE_2D, texName[2]);
	//glEnable(GL_TEXTURE_2D);
	//glBegin(GL_POLYGON);
	//glTexCoord2f(0.0, 1.0); glVertex3f(3, -10, 19.8);
	//glTexCoord2f(1.0, 1.0); glVertex3f(-3, -10, 19.8);
	//glTexCoord2f(1.0, 0.0); glVertex3f(-3, 0, 19.8);
	//glTexCoord2f(0.0, 0.0); glVertex3f(3, 0, 19.8);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//glPopMatrix();

	////迴廊
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(0, 0, 60);
	//glScalef(30, 20, 80);
	////DrawCloister();
	//glutSolidCube(1.0);
	//glPopMatrix();

	//房間B
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0, -40, 0);
	glScalef(80, 40, 80);
	DrawRoomB();
	glPopMatrix();

	//// 房間A
	////glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(0, 0, 10);
	//glScalef(20, 20, 1);
	//DrawCube();
	//glutSolidCube(1.0);
	//glPopMatrix();
	
	//glPushMatrix();
	//glTranslatef(0, 0, -10);
	//glScalef(20, 10, 0);
	////glutSolidCube(1.0);
	//DrawCube();
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(-10, 0, 0);
	//glScalef(0, 10, 20);
	////DrawCube();
	//glutSolidCube(1.0);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(10, 0, 0);
	//glScalef(0, 10, 20);
	////DrawCube();
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間A地板
	//glPushMatrix();
	//glTranslatef(0, -5, 0);
	//glScalef(20, 0, 20); 
	////DrawCube();
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間A屋頂
	//glPushMatrix();
	//glTranslatef(0, -2, 0);
	//glScalef(20, 10, 20);
	//glutSolidCube(1.0);
	//glPopMatrix();
	////房間A右門
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(1.5, 0, 9.5);
	//glScalef(3, 8, 0.5);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間A左門
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(-1.5, 0, 9.5);
	//glScalef(3, 8, 0.5);
	//glutSolidCube(1.0);
	//glPopMatrix();
	
	////迴廊出右門
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(1.5, 0, 10.5);
	//glScalef(3, 8, 0.5);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊出左門
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(-1.5, 0, 10.5);
	//glScalef(3, 8, 0.5);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊右壁
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(5, 0, 40);
	//glScalef(0, 10, 60);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊左壁
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(-5, 0, 40);
	//glScalef(0, 10, 60);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊底面
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(0, -5, 40);
	//glScalef(10, 0, 60);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊進右門
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(1.5, 0, 69.5);
	//glScalef(3, 8, 0.5);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊進左門
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(-1.5, 0, 69.5);
	//glScalef(3, 8, 0.5);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////迴廊進門壁
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(0, 0, 70);
	//glScalef(10, 10, 0);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間B左壁
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(15, 0, 85);
	//glScalef(0, 10, 30);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間B右壁
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(-15, 0, 85);
	//glScalef(0, 10, 30);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間B後壁
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(0, 0, 70);
	//glScalef(30, 10, 0);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間B前壁
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(0, 0, 100);
	//glScalef(30, 10, 0);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////房間B底部
	//glColor3f(0.0, 0.0, 0.0);
	//glPushMatrix();
	//glTranslatef(0, -5, 85);
	//glScalef(30, 0, 30);
	//glutSolidCube(1.0);
	//glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void Display2(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);                           //用白色塗背景
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         //清除所有像素
	glPolygonMode(GL_BACK, GL_LINE);                            //設定面的背面用線條顯示
	glMatrixMode(GL_MODELVIEW);
	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
	glLoadIdentity();

	////gluLookAt(0, 0, 300.0, 0, 0, 0, 0, 1, 0);                    //視線的座標及方向
	//gluLookAt(0, 0, 2.0, 0, 0, 0, 0, 1, 0);                    //視線的座標及方向
	//glTranslatef(distanceX, 0, 0);                               //沿著x軸平移
	//glTranslatef(0, distanceY, 0);                               //沿著x軸平移
	//glTranslatef(0, 0, distanceZ);                               //沿著z軸平移
	//glRotatef((float)rot_y + (float)record_y, 1.0, 0.0, 0.0);   //以x軸當旋轉軸
	//glRotatef((float)rot_x + (float)record_x, 0.0, 1.0, 0.0);   //以y軸當旋轉軸

	//中垂線
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(0, 2, 0);
	glutSolidCube(1.0);
	glPopMatrix();

	//區域中線
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(2, 0, 0);
	glutSolidCube(1.0);
	glPopMatrix();

	////區域上中線
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(0, 0.5, 0);
	//glScalef(2, 0, 0);
	//glutSolidCube(1.0);
	//glPopMatrix();

	////區域下中線
	//glColor3f(1.0, 1.0, 1.0);
	//glPushMatrix();
	//glTranslatef(0, -0.5, 0);
	//glScalef(2, 0, 0);
	//glutSolidCube(1.0);
	//glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	if (ItemsA == true)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName[6]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.75, 0.25, 0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.25, 0.25, 0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.25, 0.75, 0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.75, 0.75, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (ItemsB == true)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName[7]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.25, 0.25, 0);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.75, 0.25, 0);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.75, 0.75, 0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.25, 0.75, 0);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	if (ItemsC == true)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName[8]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.75, -0.75, 0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.25, -0.75, 0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.25, -0.25, 0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.75, -0.25, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}


	if (ItemsD == true)
	{
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName[9]);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.25, -0.75, 0);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.75, -0.75, 0);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.75, -0.25, 0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.25, -0.25, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	////球
	//if (ItemsA == true)
	//{
	//	glColor3f(1.0, 1.0, 1.0);
	//	glPushMatrix();
	//	glTranslatef(-0.5, 0.5, 0);
	//	glutSolidSphere(0.25, 30, 30);
	//	glPopMatrix();
	//}

	glFlush();
	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		distanceY -= 1;
		//std::cout << "座標X:" << distanceX << " Y:" << distanceY << " Z:" << distanceZ << std::endl;
		break;
	case 's':
		distanceY += 1;
		//std::cout << "座標X:" << distanceX << " Y:" << distanceY << " Z:" << distanceZ << std::endl;
		break;
	case 'd':
		distanceX -= 1;
		//std::cout << "座標X:" << distanceX << " Y:" << distanceY << " Z:" << distanceZ << std::endl;
		break;
	case 'a':
		distanceX += 1;
		//std::cout << "座標X:" << distanceX << " Y:" << distanceY << " Z:" << distanceZ << std::endl;
		break;
	case 'q':
		distanceZ += 1;
		//std::cout << "座標X:" << distanceX << " Y:" << distanceY << " Z:" << distanceZ << std::endl;
		break;
	case 'e':
		distanceZ -= 1;
		//std::cout << "座標X:" << distanceX << " Y:" << distanceY << " Z:" << distanceZ << std::endl;
		break;
	case 27:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glutDestroyWindow(WinNumber);
		exit(0);
		break;
	}
	glutPostRedisplay();            //令視窗重繪
}

void WindowSize(int w, int h)
{
	float rate;
	if (h == 0) h = 1;                        //阻止h為零，分母可不能為零啊
	glViewport(0, 0, w, h);                 //當視窗長寬改變時，畫面也跟著變
	rate = (float)w / (float)h;                //畫面視野變了，但內容不變形

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, rate, 1.0, 1500.0);   //透視投影

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP)
		{
			Ismousedown = !Ismousedown;
			record_x += x - old_rot_x;
			record_y += y - old_rot_y;

			rot_x = 0;   //沒有歸零會有不理想的結果
			rot_y = 0;
			if (record_x < 0)
			{
				record_x = record_x + 360;
			}
			else
			{
			}
		}
		else
		{
			Ismousedown = !Ismousedown;
			old_rot_x = x;
			old_rot_y = y;
		}
		break;


	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)            //揮手動作
		{
			//std::cout << "場景座標 X:" << x << " Y:" << y << std::endl;
			//std::cout << "滑鼠座標 X:" << record_x << " Y:" << record_y << std::endl << std::endl;

				if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 300 && y >= 300
					&& x <= 450 && record_y >= -20 && record_y <= 20 && (record_x % 360) <= 280 && (record_x % 360) >= 260)
				{
					ItemsA = true;
					std::cout << "系統:在垃圾桶裡找到了一件神秘的物品，請查看包包\n" << std::endl;
					std::cout << "場景座標 X:" << x << " Y:" << y << std::endl;
					std::cout << "滑鼠座標 X:" << record_x << " Y:" << record_y << std::endl;

				}
				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 && x <= 500 && ItemsA == false
					&& record_y >= -20 && record_y <= 20 && (record_x % 360) <= 200 && (record_x % 360) >= 160)
				{
					std::cout << "這門好牢靠，難道我要一輩子在這裡活下去了嗎\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 && x <= 500 && 
					     record_y >= -20 && record_y <= 20 && (record_x % 360) <= 50 && (record_x % 360) >= 0)
				{
					std::cout << "你覺得書櫃上會有東西嗎? 怎麼可能呢\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 150 && x <= 500 && y>=300
					&& record_y >= -20 && record_y <= 20 && (record_x % 360) <= 120 && (record_x % 360) >= 60)
				{
					std::cout << "沙發真的好舒服哦\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 190 && x <= 500 && y <= 300
					&& record_y >= -20 && record_y <= 20 && (record_x % 360) <= 120 && (record_x % 360) >= 60)
				{
					std::cout << "櫃子上什麼也沒有，但是有個箭頭朝向對面耶？\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x <= 189 &&  y<= 250 &&
					record_y >= -20 && record_y <= 20 && (record_x % 360) <= 120 && (record_x % 360) >= 60)
				{
					std::cout << "這幅畫彷彿藏著什麼玄機\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 &&  x<= 500 && y <= 300
					&& x <= 450 && record_y >= -20 && record_y <= 20 && (record_x % 360) <= 290 && (record_x % 360) >= 250)
				{
					std::cout << "這幅畫應該是我脫離這裡的關鍵\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 && x <= 400 && y >= 300
					&& x <= 450 && record_y >= -20 && record_y <= 20 && (record_x % 360) <= 290 && (record_x % 360) >= 250)
				{
					std::cout << "書桌上寫著，關鍵點在於ZAN，哦不對是在圓柱體\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 && record_y <= -40 )
				{
					std::cout << "這星野好美，希望有天也能去宇宙旅行\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 && record_y >= 40)
				{
					std::cout << "地板有個模糊的粉筆字(越貼近地板越靠近真相??)\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 0 && distanceZ == 0 && x >= 100 && x <= 500 && ItemsA == true
					    && record_y >= -20 && record_y <= 20 && (record_x % 360) <= 200 && (record_x % 360) >= 160)
				{
					ItemsA = false;
					ItemsAO = true;
					distanceY = distanceY + 40;
					std::cout << "門被打開了" << std::endl;
					std::cout << "沒想到竟然是在太空站上" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 40 && distanceZ == 0 && x >= 100 && x <= 500 &&
					record_y >= -20 && record_y <= 20 && (record_x % 360) <= 200 && (record_x % 360) >= 160)
				{
					std::cout << "有艘太空小艇呢，看來缺少一些重要的配備\n" << std::endl;
					std::cout << "聽說在小行星上有我們要的東西\n" << std::endl;
				}

				else if (distanceX == 0 && distanceY == 40 && distanceZ == 0 && x >= 100 && x <= 500 &&
					record_y >= -40 && record_y <= -20 && (record_x % 360) <= 300 && (record_x % 360) >= 200)
				{
					std::cout << "有顆小行星呢？去探險一下吧！\n" << std::endl;
					std::cout << "系統：找到魔水晶！請查看包包！\n" << std::endl;
					std::cout << "聽說底端的星際空間有我們要的油桶！\n" << std::endl;
					ItemsD = true;
				}

				else if (distanceX == 0 && distanceY == 40 && distanceZ == 0 && x >= 100 && x <= 500 &&
					record_y >= 35 && record_y <= 50 && (record_x % 360) <= 210 && (record_x % 360) >= 130)
				{
					std::cout << "這裡空無一物?\n";
					std::cout << "咦！這個是?\n";
				    std::cout << "竟然有一桶油呢\n" << std::endl;
					std::cout << "系統：找到一桶太空燃料！請查看包包！\n" << std::endl;
					std::cout << "前方的衛星可能有些什麼\n" << std::endl;
					ItemsC = true;
				}

				else if (distanceX == 0 && distanceY == 40 && distanceZ == 0 && x >= 100 && x <= 400 &&
					record_y >= -20 && record_y <= 20 && (record_x % 360) <= 50 && (record_x % 360) >= 0)
				{
					std::cout << "感覺上面也很多秘密\n";
					std::cout << "應該有我們需要的東西\n" << std::endl;;
					std::cout << "系統：找到ID晶片卡！請查看包包！\n" << std::endl;
					ItemsB = true;
				}

				else if (ItemsB == true && ItemsC == true && ItemsD == true)
				{
					ItemsB = false;
					ItemsC = false;
					ItemsD =false;
					ItemsDO = true;
					distanceY = distanceY + 800;
				}
		}
		else
		{
		}
		break;

	default:
		break;
	}
	glutPostRedisplay();
	//Display2();
}

void MotionMouse(int x, int y)
{
	if (!Ismousedown) return;
	rot_x = x - old_rot_x;
	rot_y = y - old_rot_y;
	glutPostRedisplay();
}

void Mouse2(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP)
		{
			std::cout << "道具座標 X:" << x << " Y:" << y << std::endl;
		}
		else
		{
		}
		break;


	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP)            //揮手動作
		{
			if (x <= 125 && y <= 125)
			{
				ItemsA = !ItemsA;
				std::cout << "道具A" << ItemsA << std::endl;
			}
			else if (x > 125 && y <= 125)
			{
				ItemsB = !ItemsB;
				std::cout << "道具B" << ItemsB << std::endl;
			}
			else if (x <= 125 && y > 125)
			{
				ItemsC = !ItemsC;
				std::cout << "道具C" << ItemsC << std::endl;
			}
			else
			{
				ItemsD = !ItemsD;
				std::cout << "道具D" << ItemsD << std::endl;
			}

		}
		else
		{
		}
		break;

	default:
		break;
	}
	glutPostRedisplay();

}

void MotionMouse2(int x, int y)
{
}

void SetLightSource()
{
	float light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };



	// 設定發光體的光源的特性
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);      //環境光(Ambient Light)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);      //散射光(Diffuse Light)
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);     //反射光(Specular Light)

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);     //光的座標

	glEnable(GL_LIGHTING);                                 //開燈
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);                               //啟動深度測試
}

void SetMaterial()
{
	float material_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float material_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
}

void myInit()
{

	glGenTextures(MapNumber, texName); //產生Generate 貼圖ID
	//讀圖
	quad = gluNewQuadric();

	PlaySound(TEXT("text.wav"), NULL, SND_ASYNC);

	for (int i = 0, num = 0; i < MapNumber; i++)
	{
		char Path[256] = {};
		sprintf_s(Path, sizeof(Path), "texture/%02d.png", num);
		cv::Mat textureInput = cv::imread(Path, cv::IMREAD_COLOR);

		if (textureInput.empty())
		{
			std::cout << "image empty" << std::endl;
			continue;
		}
		//cv::flip(textureInput, textureInput, 0);
		//cv::cvtColor(textureInput, textureInput, CV_BGR2RGB); ///OpenCV轉色彩 (需要cv.h)
		if (textureInput.channels() == 3)
		{
			cv::cvtColor(textureInput, textureInput, CV_BGR2RGBA);
		}
		else
		{
			cv::cvtColor(textureInput, textureInput, CV_BGRA2RGBA);
		}

		/*cv::imshow("d",textureInput);
		cv::waitKey(0);*/
		//丟進陣列
		texture[i] = textureInput;

		//for (int i = 0; i < texture.cols; i++)
		//{
		//	for (int j = 0; j < texture.rows; j++)
		//	{
		//		int c = 0;
		//		c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
		//		texture[i][j][0] = (GLubyte)c;
		//		texture[i][j][1] = (GLubyte)c;
		//		texture[i][j][2] = (GLubyte)c;
		//		texture[i][j][3] = (GLubyte)255;
		//		
		//	}
	
		//bind 
		//GLuint texName[10];  //準備一個 unsigned int 整數, 叫 貼圖ID
		glBindTexture(GL_TEXTURE_2D, texName[num]); //綁定bind 貼圖ID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //貼圖參數, 超過包裝的範圖T, 就重覆貼圖
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /// 貼圖參數, 超過包裝的範圖S, 就重覆貼圖
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /// 貼圖參數, 放大時的內插, 用最近點
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /// 貼圖參數, 縮小時的內插, 用最近點
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture[i].cols, texture[i].rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture[i].data);
		num++;
	}

}

void myInit2()
{

	glGenTextures(MapNumber, texName); //產生Generate 貼圖ID
	//讀圖
	for (int i = 0, num = 0; i < MapNumber; i++)
	{
		char Path[256] = {};
		sprintf_s(Path, sizeof(Path), "texture/%02d.png", num);
		cv::Mat textureInput = cv::imread(Path, cv::IMREAD_COLOR);

		if (textureInput.empty())
		{
			std::cout << "image empty" << std::endl;
			continue;
		}
		//cv::flip(textureInput, textureInput, 0);
		//cv::cvtColor(textureInput, textureInput, CV_BGR2RGB); ///OpenCV轉色彩 (需要cv.h)
		if (textureInput.channels() == 3)
		{
			cv::cvtColor(textureInput, textureInput, CV_BGR2RGBA);
		}
		else
		{
			cv::cvtColor(textureInput, textureInput, CV_BGRA2RGBA);
		}

		/*cv::imshow("d",textureInput);
		cv::waitKey(0);*/
		//丟進陣列
		texture[i] = textureInput;

		//for (int i = 0; i < texture.cols; i++)
		//{
		//	for (int j = 0; j < texture.rows; j++)
		//	{
		//		int c = 0;
		//		c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
		//		texture[i][j][0] = (GLubyte)c;
		//		texture[i][j][1] = (GLubyte)c;
		//		texture[i][j][2] = (GLubyte)c;
		//		texture[i][j][3] = (GLubyte)255;
		//		
		//	}

		//bind 
		//GLuint texName[10];  //準備一個 unsigned int 整數, 叫 貼圖ID
		glBindTexture(GL_TEXTURE_2D, texName[num]); //綁定bind 貼圖ID
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //貼圖參數, 超過包裝的範圖T, 就重覆貼圖
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /// 貼圖參數, 超過包裝的範圖S, 就重覆貼圖
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /// 貼圖參數, 放大時的內插, 用最近點
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /// 貼圖參數, 縮小時的內插, 用最近點
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture[i].cols, texture[i].rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture[i].data);
		num++;
	}

}

int main()
{
	std::cout << "逃離密室\n\nQ,E遠近\nW,S上下\nA,D左右\nEsc關閉\n\n在場景視窗中\n滑鼠右鍵轉移視角\n滑鼠左鍵取得物品\n在道具視窗中左鍵點擊物品\n";
	std::cout << std::endl <<  "頭好暈唷...這裡是...哪裡呢?\n";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);                     //視窗長寬
	glutInitWindowPosition(600, 100);                  //視窗左上角的位置
	WinNumber = glutCreateWindow("密室逃脫-孔明的陷阱");   //建立視窗

	//下面五個是用來指定Callback函數
	glutReshapeFunc(WindowSize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MotionMouse);
	glutDisplayFunc(Display);
	SetLightSource();
	myInit();
	SetMaterial();


	glutInitWindowPosition(350, 100);
	glutInitWindowSize(250, 250);
	glutCreateWindow("道具視窗");
	glutMouseFunc(Mouse2);
	glutMotionFunc(MotionMouse);
	glutDisplayFunc(Display2);
	myInit2();


	glutMainLoop();
	return 0;
}

