#include "pch.h"
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

int C[6][10]; // 큐브 저장, C[방향][큐브위치]
int xro[28], yro[28], zro[28]; // x,y,z좌표 값을 담을 28개 배열 정의 

int done = 0, di = 1, rot = 0, yes = 1, k = 0;
float ang = 0.0; // ang 값, 큐브를 바라보고 있는 위치
int dragging, unix, uniy, uniz, unif, clicked = 0;
int rotate_x = -25, rotate_y = 35; // 큐브를 바라보고 있는 시점
int pause = -1, pressed = 0, solving = 0; // 초기 pause값, 키보드 프레스여부
int tmi = 0, tm = 0, tse = 0; // 시간 millis, 분 초 단위의 타이머 구동
char st[8] = { '0',':','0','0',':','0','0','\0' }; // 화면에 표시될 시간 타이머를 담을 배열 기본으로 0:00:00 설정

void xrotation(int, int);
void yrotation(int, int);
void rmy(int, int, int, int, int);
void zrotation(int, int);
void rmz(int, int, int, int, int);
void fr(int);
void franti(int);
void yfinal(int);
void xfinal(int);
void zfinal(int);
void wholr(int, int);

void mouse_click(int, int, int, int);

int mpx, mpy, mpz;

int diry, layy, dirx, layx, dirz, layz; // lay = 위치(x,y,z), dir = 방향(x,y,z)


void ChangeSize(int w, int h) // Window 사이즈를 재설정했을때 바라보는중심이 중앙(0,0)으로 오도록 설정
{
	glViewport(0, 0, w, h);
}


void scramble(int x) // 20만큼의 난수로 큐브를 셔플
{
	int i;
	for (i = 0; i < 20; i++)
	{
		switch ((rand() + x) % 20)
		{
			// X축 회전
		case 1:xrotation(3, -1); break;
		case 2:xrotation(2, -1); break;
		case 3:xrotation(1, -1); break;
		case 4:xrotation(1, 1); break;
		case 5:xrotation(3, 1); break;
		case 6:xrotation(2, 1); break;

			// Y축 회전
		case 7:yrotation(3, -1); break;
		case 8:yrotation(2, -1); break;
		case 9:yrotation(1, -1); break;
		case 10:yrotation(1, 1); break;
		case 11:yrotation(3, 1); break;
		case 12:yrotation(2, 1); break;

			// Z축 회전
		case 13:zrotation(3, -1); break;
		case 14:zrotation(2, -1); break;
		case 15:zrotation(1, -1); break;
		case 16:zrotation(1, 1); break;
		case 17:zrotation(3, 1); break;
		case 18:zrotation(2, 1); break;

		default:break;
		}
	}
}


void renderBitmapString(float x, float y, void *font, const char *string) {      // 화면 상단 텍스트 표시 기능
   // 4개의 인자는 world 좌표계의x,y, 폰트는 기본 제공하는 폰트, char string은 스트링할 문자나 기호
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) { // 텍스트를 가로로 일정한 간격으로 출력 
		glutBitmapCharacter(font, *c);
	}
}


//__________________________________________________KEY___
void keyb(unsigned char key, int x, int y)
{
	pressed = 1; // 버튼 클릭여부를 yes에 저장
	if (yes&&pause != pressed) // pause가 안눌러진 상태일때 큐브 회전이 가능하게 함
	{
		switch (key)
		{
			// X축 회전 조작키
		case 'q':xfinal(1); yes = 0; break; // 맨 왼쪽 라인이 x축 기준으로 아래로 회전
		case 'w':xfinal(2); yes = 0; break; // 중간 라인이 x축 기준으로 아래로 회전
		case 'e':xfinal(3); yes = 0; break; // 맨 오른쪽 라인이 x축 기준으로 아래로 회전
		case 'r':xfinal(7); yes = 0; break; // 맨 왼쪽 라인이 x축 기준으로 위로 회전
		case 't':xfinal(8); yes = 0; break; // 중간 라인이 x축 기준으로 위로 회전
		case 'y':xfinal(9); yes = 0; break; // 맨 오른쪽 라인이 x축 기준으로 위로 회전
		// Y축 회전 조작키
		case 'a':yfinal(1); yes = 0; break; // 맨 위쪽 라인이 y축 기준으로 오른쪽으로 회전
		case 's':yfinal(2); yes = 0; break; // 중간 라인이 y축 기준으로 오른쪽으로 회전
		case 'd':yfinal(3); yes = 0; break; // 맨 아래쪽 라인이 y축 기준으로 오른쪽으로 회전
		case 'f':yfinal(7); yes = 0; break; // 맨 위쪽 라인이 y축 기준으로 왼쪽으로 회전
		case 'g':yfinal(8); yes = 0; break; // 중간 라인이 y축 기준으로 왼쪽으로 회전
		case 'h':yfinal(9); yes = 0; break; // 맨 아래쪽 라인이 y축 기준으로 왼쪽으로 회전
		// Z축 회전 조작키
		case 'z':zfinal(1); yes = 0; break; // 맨 앞쪽 라인이 z축 기준으로 오른쪽으로 회전
		case 'x':zfinal(2); yes = 0; break; // 중간 라인이 z축 기준으로 오른쪽으로 회전
		case 'c':zfinal(3); yes = 0; break; // 맨 뒷쪽 라인이 z축 기준으로 오른쪽으로 회전
		case 'v':zfinal(7); yes = 0; break; // 맨 앞쪽 라인이 z축 기준으로 왼쪽으로 회전
		case 'b':zfinal(8); yes = 0; break; // 중간 라인이 z축 기준으로 왼쪽으로 회전
		case 'n':zfinal(9); yes = 0; break; // 맨 뒤쪽 라인이 z축 기준으로 왼쪽으로 회전

		default:break;
		}
	}
}


void specialKeys(int key, int x, int y) // 키보드 방향키를 눌렀을때 불러오는 함수와 그 인자 불러오는 함수는 wholr
{
	if (yes)
	{
		if (key == GLUT_KEY_RIGHT) {
			wholr(0, -1);
		}

		else if (key == GLUT_KEY_LEFT) {
			wholr(0, 1);
		}

		else if (key == GLUT_KEY_UP) {
			wholr(1, 1);
		}

		else if (key == GLUT_KEY_DOWN) {
			wholr(1, -1);
		}
	}
}

//_________________________________ANIMATION___
void wholr(int ax, int dir) // 큐브 전체를 축기준으로 돌리는 것
// xro배열과 yro배열에 0~27까지 넣는다. 
// z축을 넣지 않은 이유는 z축을 기준으로 돌리면 큐브를 정면에서 봤을때 옆으로 돌아가는데
// 그러면 프론트랑 백 부분이 돌아가는데 프론트랑 백 부분은 아무리 돌려봤자 같은색이니까
// 필요가 없다.
{
	int i;
	di = dir;
	for (i = 0; i < 28; i++)
	{
		if (ax)
			xro[i] = 1;
		else
			yro[i] = 1;
	}

	rot = 10 + 5 * ax + dir;
}


void anim(int t) // 타이머를 돌리면서 15millis 타이머가 시간만큼 돌았을때 Redisplay를 해서 애니메이션 효과를 준다.
{
	glutPostRedisplay();
	glutTimerFunc(15, anim, 1); // 15millis로 돌림 매끄러움
}

void yfinal(int com)  // y좌표의 마지막 값 저장
{
	int i;
	diry = di = 2 * (com / 6) - 1;
	layy = (com - 1) % 3 + 1;

	for (i = 1; i < 10; i++)
		yro[(layy - 1) * 9 + i] = 1;
	rot = 1;
}
void xfinal(int com) // x좌표의 마지막 값 저장
{
	dirx = di = 2 * (com / 6) - 1;
	layx = (com - 1) % 3 + 1;
	if (layx == 1)
	{
		xro[7] = 1;
		xro[8] = 1;
		xro[9] = 1;
		xro[16] = 1;
		xro[17] = 1;
		xro[18] = 1;
		xro[25] = 1;
		xro[26] = 1;
		xro[27] = 1;
	}
	if (layx == 2)
	{
		xro[4] = 1;
		xro[5] = 1;
		xro[6] = 1;
		xro[13] = 1;
		xro[14] = 1;
		xro[15] = 1;
		xro[22] = 1;
		xro[23] = 1;
		xro[24] = 1;
	}
	if (layx == 3)
	{
		xro[1] = 1;
		xro[2] = 1;
		xro[3] = 1;
		xro[10] = 1;
		xro[11] = 1;
		xro[12] = 1;
		xro[19] = 1;
		xro[20] = 1;
		xro[21] = 1;
	}

	rot = 2;
}
void zfinal(int com) // z좌표의 마지막 값 저장
{
	dirz = -1 * (di = 2 * (com / 6) - 1);
	layz = (com - 1) % 3 + 1;
	if (layz == 1)
	{
		zro[9] = 1;
		zro[6] = 1;
		zro[3] = 1;
		zro[18] = 1;
		zro[15] = 1;
		zro[12] = 1;
		zro[21] = 1;
		zro[24] = 1;
		zro[27] = 1;
	}
	if (layz == 2)
	{
		zro[2] = 1;
		zro[5] = 1;
		zro[8] = 1;
		zro[11] = 1;
		zro[14] = 1;
		zro[17] = 1;
		zro[20] = 1;
		zro[23] = 1;
		zro[26] = 1;
	}
	if (layz == 3)
	{
		zro[1] = 1;
		zro[4] = 1;
		zro[7] = 1;
		zro[10] = 1;
		zro[13] = 1;
		zro[16] = 1;
		zro[19] = 1;
		zro[22] = 1;
		zro[25] = 1;
	}
	rot = 3;
}
void setcol(int cc) // 각 면에 color값 저장하는 것
{
	switch (cc)
	{
	case 0:glColor3f(1.0, 1.0, 1.0); break;
	case 1:glColor3f(0.0, 0.0, 1.0); break;
	case 2:glColor3f(1.0, 1.0, 0.0); break;
	case 3:glColor3f(0.0, 1.0, 0.0); break;
	case 4:glColor3f(1.0, 0.0, 0.0); break;
	case 5:glColor3f(1.0, 0.5, 0.0); break;
	case 6:glColor3f(0.1, 0.0, 0.0); break; // 큐브들 사이사이의 간격의 색
	default:break;
	}
}
//_______________________________________________________(SINGLE) CUBE
void mycube(int pic, float siz, float x, float y, float z, int d, int f, int u, int b, int r, int l)
{ // GL_QUADS와 비슷한 느낌, 점 4개를 찍어서 그 점을 모두 연결함, 총 6개의 면을 만들고 gltranslatef(x,y,z)값의 
  // 이동으로 한칸씩 이동해서 총 27개의 정육면체를 만듬.
	siz /= 2;
	glPushMatrix();

	if (yro[pic])
		glRotatef(ang*di, 0.0, 1.0, 0.0);
	if (xro[pic])
		glRotatef(ang*di, 1.0, 0.0, 0.0);
	if (zro[pic])
		glRotatef(ang*di, 0.0, 0.0, 1.0);

	glTranslatef(x, y, z);

	glBegin(GL_POLYGON);
	setcol(d);
	glVertex3f(siz, -siz, siz);
	glVertex3f(siz, -siz, -siz);
	glVertex3f(-siz, -siz, -siz);
	glVertex3f(-siz, -siz, siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(b);
	glVertex3f(siz, siz, siz);
	glVertex3f(siz, -siz, siz);
	glVertex3f(-siz, -siz, siz);
	glVertex3f(-siz, siz, siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(u);
	glVertex3f(siz, siz, siz);
	glVertex3f(siz, siz, -siz);
	glVertex3f(-siz, siz, -siz);
	glVertex3f(-siz, siz, siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(f);
	glVertex3f(siz, siz, -siz);
	glVertex3f(siz, -siz, -siz);
	glVertex3f(-siz, -siz, -siz);
	glVertex3f(-siz, siz, -siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(r);
	glVertex3f(siz, siz, siz);
	glVertex3f(siz, -siz, siz);
	glVertex3f(siz, -siz, -siz);
	glVertex3f(siz, siz, -siz);
	glEnd();

	glBegin(GL_POLYGON);
	setcol(l);
	glVertex3f(-siz, siz, siz);
	glVertex3f(-siz, -siz, siz);
	glVertex3f(-siz, -siz, -siz);
	glVertex3f(-siz, siz, -siz);
	glEnd();

	glPopMatrix();
}

void rubic(float siz, float x) // 위에서 설명했던 큐브를 반복문을 사용하지 않고 27번 호출해줌  // 위에서부터 y축 감소하면서 아래로 만들어줌 위 중간 아래
{ // 1번 Down, 2번 front, 3번 up, 4번 back, 5번 right, 6번 left
	mycube(1, siz, x, x, x, 6, 6, C[2][3], C[3][9], C[4][3], 6); // 큐브선택, 위, 뒤, 오, 왼
	mycube(2, siz, x, x, 0, 6, 6, C[2][6], 6, C[4][2], 6);
	mycube(3, siz, x, x, -x, 6, C[1][3], C[2][9], 6, C[4][1], 6);

	mycube(4, siz, 0, x, x, 6, 6, C[2][2], C[3][8], 6, 6);
	mycube(5, siz, 0, x, 0, 6, 6, C[2][5], 6, 6, 6);
	mycube(6, siz, 0, x, -x, 6, C[1][2], C[2][8], 6, 6, 6);

	mycube(7, siz, -x, x, x, 6, 6, C[2][1], C[3][7], 6, C[5][3]);
	mycube(8, siz, -x, x, 0, 6, 6, C[2][4], 6, 6, C[5][2]);
	mycube(9, siz, -x, x, -x, 6, C[1][1], C[2][7], 6, 6, C[5][1]);
	//_____MIDDLE_LAYER
	mycube(10, siz, x, 0, x, 6, 6, 6, C[3][6], C[4][6], 6);
	mycube(11, siz, x, 0, 0, 6, 6, 6, 6, C[4][5], 6);
	mycube(12, siz, x, 0, -x, 6, C[1][6], 6, 6, C[4][4], 6);

	mycube(13, siz, 0, 0, x, 6, 6, 6, C[3][5], 6, 6);
	// 14는 딱 중심이므로 필요없음
	mycube(15, siz, 0, 0, -x, 6, C[1][5], 6, 6, 6, 6);

	mycube(16, siz, -x, 0, x, 6, 6, 6, C[3][4], 6, C[5][6]);
	mycube(17, siz, -x, 0, 0, 6, 6, 6, 6, 6, C[5][5]);
	mycube(18, siz, -x, 0, -x, 6, C[1][4], 6, 6, 6, C[5][4]);
	//_________BOTTOM LATER____
	mycube(19, siz, x, -x, x, C[0][9], 6, 6, C[3][3], C[4][9], 6);
	mycube(20, siz, x, -x, 0, C[0][6], 6, 6, 6, C[4][8], 6);
	mycube(21, siz, x, -x, -x, C[0][3], C[1][9], 6, 6, C[4][7], 6);

	mycube(22, siz, 0, -x, x, C[0][8], 6, 6, C[3][2], 6, 6);
	mycube(23, siz, 0, -x, 0, C[0][5], 6, 6, 6, 6, 6);
	mycube(24, siz, 0, -x, -x, C[0][2], C[1][8], 6, 6, 6, 6);

	mycube(25, siz, -x, -x, x, C[0][7], 6, 6, C[3][1], 6, C[5][9]);
	mycube(26, siz, -x, -x, 0, C[0][4], 6, 6, 6, 6, C[5][8]);
	mycube(27, siz, -x, -x, -x, C[0][1], C[1][7], 6, 6, 6, C[5][7]);
}

//_________________________________________________________DISPLAY
void draw() // 실질적인 큐브의 사이즈를 가지고 있다. 또한 위에 renderbitmapString함수에 들어갈 인자들을 여기에 선언해주었다.
{

	int i, j, flag = 0;
	float siz = 0.2, x, sp = .02;
	x = siz + sp;

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	if (rot)
	{
		ang += 10;
		if (ang > 90)
		{
			ang = 0;
			for (i = 0; i < 28; i++)
			{
				xro[i] = 0;
				yro[i] = 0;
				zro[i] = 0;
			}
			if (rot == 1)
				yrotation(layy, diry);
			if (rot == 2)
				xrotation(layx, dirx);
			if (rot == 3)
				zrotation(layz, dirz);
			if (rot == 9)
			{
				yrotation(1, -1);
				yrotation(2, -1);
				yrotation(3, -1);
			}
			if (rot == 11)
			{
				yrotation(1, 1);
				yrotation(2, 1);
				yrotation(3, 1);
			}
			if (rot == 14)
			{
				xrotation(1, -1);
				xrotation(2, -1);
				xrotation(3, -1);
			}
			if (rot == 16)
			{
				xrotation(1, 1);
				xrotation(2, 1);
				xrotation(3, 1);
			}

			rot = 0;
			yes = 1;
		}
	}
	rubic(siz, x);
	glColor3f(0, 0, 0);
	renderBitmapString(0.6, 1.1, GLUT_BITMAP_TIMES_ROMAN_24, "RESET"); // RESET 버튼 
	renderBitmapString(-0.9, 0.7, GLUT_BITMAP_TIMES_ROMAN_24, "SCRAMBLE"); // SCRAMBLE 버튼

	if (pause == 1)
		renderBitmapString(-0.15, 0.9, GLUT_BITMAP_TIMES_ROMAN_24, "PLAY"); // PLAY 버튼
	  // 클릭했을때 Play로 바뀌고 default 값은 Pause
	else {
		if (pressed&&solving) {
			tmi += 2;
			tse += tmi / 100;
			tm += tse / 60;
			tmi %= 100;
			tse %= 60;
			st[0] = 48 + tm;
			st[2] = 48 + tse / 10;
			st[3] = 48 + tse % 10;
			st[5] = 48 + tmi / 10;
			st[6] = 48 + tmi % 10;
		}
		renderBitmapString(-0.15, 0.9, GLUT_BITMAP_TIMES_ROMAN_24, "PAUSE"); // x,y좌표 폰트이름, 캐릭터값
	}
	renderBitmapString(-0.08, -0.73, GLUT_BITMAP_TIMES_ROMAN_24, st);
	for (i = 0; i < 6; i++)
		for (j = 1; j < 10; j++)
		{
			if (C[i][5] != C[i][j])
			{
				flag = 1;

			}
		}
	if (!flag) {
		pressed = 0;
		solving = 0;
		pause = -1;
		renderBitmapString(-0.2, 0.5, GLUT_BITMAP_TIMES_ROMAN_24, "SOLVED");
	}

	glutSwapBuffers();
}

//_________________________________________________________MOUSE
void mouse_click(int button, int state, int x, int y)
{
	int nf[6], i, j;
	nf[3] = 0;
	nf[4] = 0;
	nf[5] = 0;
	if (button == GLUT_LEFT_BUTTON) // 마우스 왼쪽버튼 클릭했을때
	{
		if (state == GLUT_UP) { // 클릭한 텍스트 반전(UP을 주든 DOWN을 주든 똑같음)
			if (x > 92 && x < 217 && y > 33 && y < 53) // x,y좌표값 조건 걸어줌, 해당조건에 만족하는 좌표 범위를 클릭했을때 각 조건에 해당하는 기능들 실행
			{
				scramble(x); // 큐브 셔플
				pause = -1; // 초기 pause를 클릭하면 play로 전환
				solving = 1; // 타이머 실행
				pressed = 0; // RESET 눌렀을때
				tmi = 0; tm = 0; tse = 0; // 시간 0으로 초기화

			}
			if (x > 314 && x < 387 && y > 33 && y < 53)
			{
				if (solving)
					pause = (-1)*pause; // pause를 눌렀을때 play로 전환해주고 그에 맞는 타이머기능 정지 or 시작
			}
			if (x > 519 && x < 596 && y > 33 && y < 53)
			{
				pressed = 0;
				solving = 0;
				pause = -1;
				for (i = 0; i < 6; i++) // RESET 버튼을 눌렀을 때 큐브를 원위치 시켜주는 반복문
					for (j = 0; j < 10; j++)
						C[i][j] = i;

				tmi = 0; tm = 0; tse = 0; // 시간 0으로 초기화
			}
		}
	}
}

//__________________________ALL ROTATIONS
void xrotation(int lay, int dir)  // 큐브 전체 회전(x축) 큐브 전체를 돌리면서 위치를 계산해서 색깔을 바꿔줌
{
	int i, j, temp, te1, te2, te3;
	if (dir == -1)
	{
		temp = lay;
		te1 = C[0][lay];
		te2 = C[0][lay + 3];
		te3 = C[0][lay + 6];
		for (j = 1; j < 4; j++)
			for (i = 0, lay = temp; i < 3; i++, lay += 3)
				C[j - 1][lay] = C[j][lay];
		lay = temp;
		C[3][lay] = te1;
		C[3][lay + 3] = te2;
		C[3][lay + 6] = te3;
		if (temp == 3)
			franti(4);
		if (temp == 1)
			franti(5);

	}
	else
	{
		temp = lay;
		te1 = C[3][lay];
		te2 = C[3][lay + 3];
		te3 = C[3][lay + 6];
		for (j = 3; j > 0; j--)
			for (i = 0, lay = temp; i < 3; i++, lay += 3)
				C[j][lay] = C[j - 1][lay];
		lay = temp;
		C[0][lay] = te1;
		C[0][lay + 3] = te2;
		C[0][lay + 6] = te3;
		if (temp == 3)
			fr(4);
		if (temp == 1)
			fr(5);
	}
}
void yrotation(int lay, int dir) // rmy로 큐브 각 면 색깔 변경해주면서, 큐브 전체 회전(y축)
{
	if (lay == 1)
	{
		rmy(1, 3, 9, 1, dir);
		rmy(2, 2, 8, 2, dir);
		rmy(3, 1, 7, 3, dir);
		if (dir == 1)
			fr(2);
		if (dir == -1)
			franti(2);
	}
	if (lay == 2)
	{
		rmy(4, 6, 6, 4, dir);
		rmy(5, 5, 5, 5, dir);
		rmy(6, 4, 4, 6, dir);
	}
	if (lay == 3)
	{
		rmy(7, 9, 3, 7, dir);
		rmy(8, 8, 2, 8, dir);
		rmy(9, 7, 1, 9, dir);
		if (dir == 1)
			franti(0);
		else
			fr(0);
	}
}
void rmy(int fr, int le, int ba, int ri, int dir)  // 큐브 color 변경
{
	int te;
	if (dir == 1)
	{
		te = C[4][ri];
		C[4][ri] = C[3][ba];
		C[3][ba] = C[5][le];
		C[5][le] = C[1][fr];
		C[1][fr] = te;
	}
	else
	{
		te = C[4][ri];
		C[4][ri] = C[1][fr];
		C[1][fr] = C[5][le];
		C[5][le] = C[3][ba];
		C[3][ba] = te;

	}
}
void zrotation(int lay, int dir) // rmz로 큐브 각 면 색깔 변경해주면서, 큐브 전체 회전(z축)
{
	if (lay == 1)
	{
		rmz(7, 1, 3, 7, dir);
		rmz(8, 4, 2, 4, dir);
		rmz(9, 7, 1, 1, dir);
		if (dir == 1)
			fr(1);
		else
			franti(1);
	}
	if (lay == 2)
	{
		rmz(4, 2, 6, 8, dir);
		rmz(5, 5, 5, 5, dir);
		rmz(6, 8, 4, 2, dir);
	}
	if (lay == 3)
	{
		rmz(1, 3, 9, 9, dir);
		rmz(2, 6, 8, 6, dir);
		rmz(3, 9, 7, 3, dir);
		if (dir == 1)
			franti(3);
		else
			fr(3);
	}
}

void rmz(int up, int ri, int dw, int le, int dir) // 큐브 color 변경
{
	int te;
	if (dir == 1)
	{
		te = C[4][ri];
		C[4][ri] = C[2][up];
		C[2][up] = C[5][le];
		C[5][le] = C[0][dw];
		C[0][dw] = te;
	}
	else
	{
		te = C[4][ri];
		C[4][ri] = C[0][dw];
		C[0][dw] = C[5][le];
		C[5][le] = C[2][up];
		C[2][up] = te;

	}
}

void fr(int face) // 큐브 color 변경
{
	int temp;
	temp = C[face][2];
	C[face][2] = C[face][4];
	C[face][4] = C[face][8];
	C[face][8] = C[face][6];
	C[face][6] = temp;
	temp = C[face][1];
	C[face][1] = C[face][7];
	C[face][7] = C[face][9];
	C[face][9] = C[face][3];
	C[face][3] = temp;
}

void franti(int face) // 큐브 color 변경
{
	int temp;
	temp = C[face][2];
	C[face][2] = C[face][6];
	C[face][6] = C[face][8];
	C[face][8] = C[face][4];
	C[face][4] = temp;
	temp = C[face][1];
	C[face][1] = C[face][3];
	C[face][3] = C[face][9];
	C[face][9] = C[face][7];
	C[face][7] = temp;
}

//_________________________________________________________MAIN

int main(int argc, char** argv)
{
	int i, j;

	for (i = 0; i < 6; i++)
		for (j = 0; j < 10; j++)
			C[i][j] = i;
	for (i = 0; i < 28; i++)
	{
		xro[i] = 0;
		yro[i] = 0;
		zro[i] = 0;
	}
	printf("MOVES: \n 1. 키조작법 \n\n X축 회전 : keys Q,W,E,R,T,Y\n Y축 회전 : keys A,S,D,F,G,H\n Z축 회전 : keys Z,X,C,V,B,N");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(660, 0);
	glutCreateWindow("RUBIK'S CUBE GAME");

	glClearColor(0.6, 0.6, 0.5, 0.1);

	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(draw);
	glutKeyboardFunc(keyb);
	glutMouseFunc(mouse_click);
	glutTimerFunc(100, anim, 1);
	glutSpecialFunc(specialKeys);
	glutReshapeFunc(ChangeSize);
	glutMainLoop();
	return 0;
}