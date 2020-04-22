#include "myGlsl.h"

//関数のプロトタイプ宣言
//関数のプロトタイプ宣言
void init();
void idle();
void display();
void draw();
void drawColorSpace();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

//afine変換用変数
float pos[] = { 0.0, 0.0, 0.0 };	//図形中心のxyz座標
float scale[] = { 1.0, 1.0, 1.0 };	//大きさ（倍率）
float angle[] = { 0.0, -30.0, 0.0 };//回転角度
//初期値
float pos0[] = { 0.0, 0.0, 0.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, -30.0, 0.0 };

//カメラと視体積
struct View
{
	//カメラ
	float pos[3];	//位置（視点）
	float cnt[3];	//注視点
	float dist;		//注視点から視点までの距離
	float theta;	//仰角（水平面との偏角）
	float phi;		//方位角
	//視体積
	float fovY;		//視野角
	float nearZ;	//前方クリップ面（近平面）
	float farZ;		//広報クリップ面（遠平面）
};
//初期値
View view = {
	0.0, 2.0, 4.0,		//pos
	0.0, 0.0, 0.0,		//cnt
	4.0, 30.0, 20.0,	//dist, theta, phi
	30.0, 1.0, 100.0	//fovY, nearZ, farZ
};

//Windowのサイズ
int width = 500;
int height = 500;
//アフィン変換
enum SELECT_KEY { ROTATE, SCALE, TRANSLATE };
SELECT_KEY sKey = TRANSLATE;

//色の配分率（ユニフォーム変数の例）
float factor0 = 0.5;
float delta = 0.01;

//シェーダプログラム名
GLuint shaderProg;
//Helpキーフラグ
bool fragHelp = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	//表示モード
	glutInitWindowSize(width, height);				//表示ウィンドウのサイズ
	glutInitWindowPosition(100, 100);				//左上の位置
	glutCreateWindow("色空間(GL_ColorSpace)");		//ウィンドウ作成
	glutReshapeFunc(resize);						//ウィンドウのサイズ変更
	glutDisplayFunc(display);						//表示
	glutKeyboardFunc(keyboard);						//キーボードの利用
	glutSpecialFunc(special);						//矢印キーなどの特殊キー利用
	glutIdleFunc(idle);								//再描画
	init();											//初期設定
	initGlsl(&shaderProg,"ColorSpace2.vert");
	glutMainLoop();									//イベント処理ループに入る
	glDeleteProgram(shaderProg);
	return 0;
}

void idle()
{
	//再描画
	glutPostRedisplay();
}

void init()
{
	//背景色
	glClearColor(0.2, 0.2, 0.3, 1.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	//カラーバッファーのクリア
	if (factor0 <= 0.0) delta =  0.01;
	if (factor0 >= 1.0) delta = -0.01;
	factor0 += delta;
	//描画
	glUseProgram(shaderProg);		//シェーダプログラムの適用
	GLuint factorLoc = glGetUniformLocation(shaderProg, "factor");
	glUniform1f(factorLoc, factor0);
	draw();							//図形描画
	glUseProgram(0);				//シェーダプログラムの適用を解除
	glutSwapBuffers();				//終了
}

void draw()
{
	glPolygonMode(GL_FRONT, GL_FILL);		//モード
	glPolygonMode(GL_BACK, GL_POINT);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(angle[2], 0.0, 0.0, 1.0);		//Z軸回転
	glRotatef(angle[1], 0.0, 1.0, 0.0);		//Y軸回転
	glRotatef(angle[0], 1.0, 0.0, 0.0);		//X軸回転
	glScalef(scale[0], scale[1], scale[2]);
	drawColorSpace();						//描画オブジェクト
	glPopMatrix();
}

void drawColorSpace()
{
	float p[8][3] =
	{
		{0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
		{0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}
	};
	float c[8][3] =
	{
		{1.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0},
		{0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}
	};

	glBegin(GL_QUADS);
	glColor3fv(c[0]); glVertex3fv(p[0]);
	glColor3fv(c[1]); glVertex3fv(p[1]);
	glColor3fv(c[2]); glVertex3fv(p[2]);
	glColor3fv(c[3]); glVertex3fv(p[3]);

	glColor3fv(c[0]); glVertex3fv(p[0]);
	glColor3fv(c[3]); glVertex3fv(p[3]);
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[4]); glVertex3fv(p[4]);

	glColor3fv(c[0]); glVertex3fv(p[0]);
	glColor3fv(c[4]); glVertex3fv(p[4]);
	glColor3fv(c[5]); glVertex3fv(p[5]);
	glColor3fv(c[1]); glVertex3fv(p[1]);

	glColor3fv(c[1]); glVertex3fv(p[1]);
	glColor3fv(c[5]); glVertex3fv(p[5]);
	glColor3fv(c[6]); glVertex3fv(p[6]);
	glColor3fv(c[2]); glVertex3fv(p[2]);

	glColor3fv(c[2]); glVertex3fv(p[2]);
	glColor3fv(c[6]); glVertex3fv(p[6]);
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[3]); glVertex3fv(p[3]);

	glColor3fv(c[4]); glVertex3fv(p[4]);
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[6]); glVertex3fv(p[6]);
	glColor3fv(c[5]); glVertex3fv(p[5]);
	glEnd();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);																					//ビューポート変換
	glMatrixMode(GL_PROJECTION);																			//投影行列の指定
	glLoadIdentity();																						//投影行列の初期化
	gluPerspective(view.fovY, (double)w / (double)h, view.nearZ, view.farZ);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();																						//モデルビュー行列の初期化
	gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);	//視点を設定（視野変換）
	//表示ウィンドウのサイズ
	width = w;
	height = h;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27://Esc
		exit(0);
		break;
	case 'r':
		sKey = ROTATE;
		break;
	case 's':
		sKey = SCALE;
		break;
	case 't':
		sKey = TRANSLATE;
		break;
	case 'R':
		for (int i = 0; i < 3; i++)
		{
			pos[i] = pos0[i];
			scale[i] = scale0[i];
			angle[i] = angle0[i];
		}
		break;
	default:
		break;
	}
}

void special(int key, int x, int y)
{
	if (sKey == TRANSLATE)
	{
		switch (key) {
		case GLUT_KEY_RIGHT:
			pos[0] += 0.1;
			break;
		case GLUT_KEY_LEFT:
			pos[0] -= 0.1;
			break;
		case GLUT_KEY_UP:
			if (glutGetModifiers() == 0) pos[1] += 0.1;
			else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[2] -= 0.1;
			break;
		case GLUT_KEY_DOWN:
			if (glutGetModifiers() == 0) pos[1] -= 0.1;
			else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[2] += 0.1;
			break;
		default:
			break;
		}
	}
	if (sKey == ROTATE)
	{
		switch (key) {
		case GLUT_KEY_RIGHT:
			angle[1] += 10.0;
			break;
		case GLUT_KEY_LEFT:
			angle[1] -= 10.0;
			break;
		case GLUT_KEY_UP:
			if (glutGetModifiers() == 0) angle[0] += 10.0;
			else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[2] -= 10;
			break;
		case GLUT_KEY_DOWN:
			if (glutGetModifiers() == 0) angle[0] -= 10.0;
			else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[2] += 10;
			break;
		default:
			break;
		}
	}
	if (sKey == SCALE)
	{
		switch (key) {
		case GLUT_KEY_RIGHT:
			scale[0] += 0.1;
			break;
		case GLUT_KEY_LEFT:
			scale[0] -= 0.1;
			break;
		case GLUT_KEY_UP:
			if (glutGetModifiers() == 0) scale[1] += 0.1;
			else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[2] += 0.1;
			break;
		case GLUT_KEY_DOWN:
			if (glutGetModifiers() == 0) scale[1] -= 0.1;
			else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[2] -= 0.1;
			break;
		default:
			break;
		}
	}
}