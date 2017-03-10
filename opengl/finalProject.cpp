
#include "stdafx.h"
#include <STDLIB.H>   /* 이부분을 첨가해 준다. */
#include <time.h>
#include <GL\glut.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glaux.h>
#include <stdio.h>
#include <math.h>

#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma warning( disable : 996)

int RandomTime[5] = {0,100,200,300,400};
bool FlowerFlag[5];				// 불꽃의 상태
int FlowerLife[5] = {0};		// 생명주기
float x[5], y[5], z[5];			// 초기값
float FireCoord[5][200][3];		// 파티클 좌표
float FireVector[5][200][3];	// 파티클 벡터값
float FlowerColor[5][4] = {		// 색상
	{ 1.0, 0.8, 0.0, 1.0 },
	{ 1.0, 1.0, 0.0, 1.0 },
	{ 1.0, 0.4, 0.4, 1.0 },
	{ 0.7, 0.9, 0.2, 1.0 },
	{ 0.6, 0.6, 1.0, 1.0 }
};
int FirePower[5][200];			// 폭발의 힘

static float GRAVITY = 0.0001;		// 중력가속도
static float FRICTION = 0.00005;	// 공기저항
float g[] = {0};

GLfloat LightPosition[] = { 0.0, 2.0, 2.0, 2.0 };

/*
unsigned int MyTextureObject[1];
AUX_RGBImageRec* pTextureImage[1];


AUX_RGBImageRec* LoadBMP(char* szFilename) {
	FILE* pFile = NULL;
	if (!szFilename) {
		return NULL;
	}
	pFile = fopen(szFilename, "r");
	if (pFile) {
		wchar_t filename[MAX_PATH];
		fclose(pFile);
		mbstowcs(filename, szFilename, MAX_PATH);
		return auxDIBImageLoad(filename);     //파일로부터 메모리로
	}
	return NULL;
}

int LoadGLTextures(char* szFilePath) {       //파일을 로드하고 텍스쳐로 변환
	int Status = FALSE;
	glClearColor(0.0, 0.0, 0.0, 0.5);
	memset(pTextureImage, 0, sizeof(void*) * 1);    //포인터를 널로

	if (pTextureImage[0] = LoadBMP(szFilePath)) {   //비트맵을 로드하고 오류확인
		Status = TRUE;                              //상태 플랙을 True로
		glGenTextures(1, &MyTextureObject[0]);      //텍스쳐 생성
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3,
			pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}
	if (pTextureImage[0]) {                 //텍스쳐가 존재하면
		if (pTextureImage[0]->data) {       //텍스쳐 영상이 존재하면
			free(pTextureImage[0]->data);   //텍스쳐 영상공간 반납
		}
		free(pTextureImage[0]);             //텍스쳐 반납
	}
	return Status;
}
*/

void InitLight() {
	GLfloat light0_ambient[] = { 0.5, 0.4, 0.3, 0.5 };     //조명 특성
	GLfloat light0_specular[] = { 0.5, 0.5, 0.5, 1.0 };

	GLfloat material_ambient[] = { 0.4, 0.4, 0.4, 1.0 };  //물체 특성
	GLfloat material_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_SMOOTH);    //구로 셰이딩
	glEnable(GL_DEPTH_TEST);    //깊이 버퍼 활성화
	glEnable(GL_LIGHTING);      //조명 활성화
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT4, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light0_specular);

}

// 불꽃 그리기
void FireFlower() {

	glTranslatef(0, -1, -1);

	for (int f = 0; f < 5; f++) {
		glPushMatrix();
		glRotatef(270, 1.0, 0.0, 0.0);
		glTranslatef(x[f], y[f]-2+((float)FlowerLife[f]/50), z[f]+1-((float)FlowerLife[f] / 100));
		switch (f) {				// 시간이 지날수록 조명이 멀어짐
		case 0:
			glLightfv(GL_LIGHT0, GL_DIFFUSE, FlowerColor[f]);
			glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_DIFFUSE, FlowerColor[f]);
			glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
			break;
		case 2:
			glLightfv(GL_LIGHT2, GL_DIFFUSE, FlowerColor[f]);
			glLightfv(GL_LIGHT2, GL_POSITION, LightPosition);
			break;
		case 3:
			glLightfv(GL_LIGHT3, GL_DIFFUSE, FlowerColor[f]);
			glLightfv(GL_LIGHT3, GL_POSITION, LightPosition);
			break;
		case 4:
			glLightfv(GL_LIGHT4, GL_DIFFUSE, FlowerColor[f]);
			glLightfv(GL_LIGHT4, GL_POSITION, LightPosition);
			break;
		}
		glPopMatrix();

		for (int i = 0; i < 200; i++) {
			glPushMatrix();
			glTranslatef(FireCoord[f][i][0], FireCoord[f][i][1], FireCoord[f][i][2]);
			glColor3f(FlowerColor[f][0], FlowerColor[f][1], FlowerColor[f][2]);
			glutSolidCube(0.02-((float)FlowerLife[f]/12000));
			glPopMatrix();				// 시간이 지날수록 파티클이 작아짐
		}
	}
	
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -5.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	// fire flower
	glPushMatrix();
	FireFlower();
	glPopMatrix();
	// earth
	glEnable(GL_LIGHTING);
	glTranslatef(0.0, -9.5, 0.0);
	//glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glutSolidSphere(9.0, 400, 400);
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
	
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// 새로운 불꽃 생성
void CreateFlower(int f) {
	switch (f) {
	case 0: glEnable(GL_LIGHT0); break;
	case 1: glEnable(GL_LIGHT1); break;
	case 2: glEnable(GL_LIGHT2); break;
	case 3: glEnable(GL_LIGHT3); break;
	case 4: glEnable(GL_LIGHT4); break;
	}

	FlowerFlag[f] = true;
	FlowerLife[f] = 0;
	g[f] = 0;

	x[f] = (float)(rand() % 100) / 25 - 2;	// 폭발좌표
	y[f] = (float)(rand() % 200) / 200 +1.5;
	z[f] = (float)(rand() % 100) / 20 - 2;

	for (int i = 0; i < 200; i++) {
		FireCoord[f][i][0] = x[f];
		FireCoord[f][i][1] = y[f];
		FireCoord[f][i][2] = z[f];

		int diameter = rand() % 360;				// 파티클 방향
		FirePower[f][i] = rand() % 50+10;			// 파티클 속도
		float x = FirePower[f][i] * cos(diameter);
		float y = FirePower[f][i] * sin(diameter);
			
		FireVector[f][i][0] = x / 4000.0;
		FireVector[f][i][1] = y / 4000.0;
		FireVector[f][i][2] = FirePower[f][i] / 4000.0 - 0.001;
	}

	
}

void MyTimer(int Value) {
	// 불꽃 생성시간
	for (int i = 0; i < 5; i++) {
		RandomTime[i] += (rand() % 4);
		if (RandomTime[i]>400) {
			RandomTime[i] = 0;
			CreateFlower(i);
		}
	}

	// 좌표 계산
	for (int f = 0; f < 5; f++) {
		for (int i = 0; i < 200; i++) {
			for (int j = 0; j < 3; j++) {			// 공기마찰력 적용
				if (FireVector[f][i][j]>0)
					FireVector[f][i][j] -= FRICTION*(FirePower[f][i]/30);
				else if (FireVector[f][i][j]<0)
					FireVector[f][i][j] += FRICTION*(FirePower[f][i]/30);
			}
			FireCoord[f][i][0] += FireVector[f][i][0];
			FireCoord[f][i][1] += FireVector[f][i][1] - g[f];	// 중력적용
			FireCoord[f][i][2] += FireVector[f][i][2];
		}
		if (FlowerFlag[f]) {
			FlowerLife[f]++;
			g[f] += GRAVITY;
		}
	}




	glutPostRedisplay();

	glutTimerFunc(10, MyTimer, 1);
}

int main(int argc, char** argv) {

	srand(time(NULL));
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("FireFlower");

	/*
	LoadGLTextures(argv[1]);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	*/

	InitLight();
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(40, MyTimer, 1);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
	
}
