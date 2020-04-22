#include "myGlsl.h"

//�֐��̃v���g�^�C�v�錾
//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void draw();
void drawColorSpace();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

//afine�ϊ��p�ϐ�
float pos[] = { 0.0, 0.0, 0.0 };	//�}�`���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };	//�傫���i�{���j
float angle[] = { 0.0, -30.0, 0.0 };//��]�p�x
//�����l
float pos0[] = { 0.0, 0.0, 0.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, -30.0, 0.0 };

//�J�����Ǝ��̐�
struct View
{
	//�J����
	float pos[3];	//�ʒu�i���_�j
	float cnt[3];	//�����_
	float dist;		//�����_���王�_�܂ł̋���
	float theta;	//�p�i�����ʂƂ̕Ίp�j
	float phi;		//���ʊp
	//���̐�
	float fovY;		//����p
	float nearZ;	//�O���N���b�v�ʁi�ߕ��ʁj
	float farZ;		//�L��N���b�v�ʁi�����ʁj
};
//�����l
View view = {
	0.0, 2.0, 4.0,		//pos
	0.0, 0.0, 0.0,		//cnt
	4.0, 30.0, 20.0,	//dist, theta, phi
	30.0, 1.0, 100.0	//fovY, nearZ, farZ
};

//Window�̃T�C�Y
int width = 500;
int height = 500;
//�A�t�B���ϊ�
enum SELECT_KEY { ROTATE, SCALE, TRANSLATE };
SELECT_KEY sKey = TRANSLATE;

//�F�̔z�����i���j�t�H�[���ϐ��̗�j
float factor0 = 0.5;
float delta = 0.01;

//�V�F�[�_�v���O������
GLuint shaderProg;
//Help�L�[�t���O
bool fragHelp = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	//�\�����[�h
	glutInitWindowSize(width, height);				//�\���E�B���h�E�̃T�C�Y
	glutInitWindowPosition(100, 100);				//����̈ʒu
	glutCreateWindow("�F���(GL_ColorSpace)");		//�E�B���h�E�쐬
	glutReshapeFunc(resize);						//�E�B���h�E�̃T�C�Y�ύX
	glutDisplayFunc(display);						//�\��
	glutKeyboardFunc(keyboard);						//�L�[�{�[�h�̗��p
	glutSpecialFunc(special);						//���L�[�Ȃǂ̓���L�[���p
	glutIdleFunc(idle);								//�ĕ`��
	init();											//�����ݒ�
	initGlsl(&shaderProg,"ColorSpace2.vert");
	glutMainLoop();									//�C�x���g�������[�v�ɓ���
	glDeleteProgram(shaderProg);
	return 0;
}

void idle()
{
	//�ĕ`��
	glutPostRedisplay();
}

void init()
{
	//�w�i�F
	glClearColor(0.2, 0.2, 0.3, 1.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	//�J���[�o�b�t�@�[�̃N���A
	if (factor0 <= 0.0) delta =  0.01;
	if (factor0 >= 1.0) delta = -0.01;
	factor0 += delta;
	//�`��
	glUseProgram(shaderProg);		//�V�F�[�_�v���O�����̓K�p
	GLuint factorLoc = glGetUniformLocation(shaderProg, "factor");
	glUniform1f(factorLoc, factor0);
	draw();							//�}�`�`��
	glUseProgram(0);				//�V�F�[�_�v���O�����̓K�p������
	glutSwapBuffers();				//�I��
}

void draw()
{
	glPolygonMode(GL_FRONT, GL_FILL);		//���[�h
	glPolygonMode(GL_BACK, GL_POINT);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(angle[2], 0.0, 0.0, 1.0);		//Z����]
	glRotatef(angle[1], 0.0, 1.0, 0.0);		//Y����]
	glRotatef(angle[0], 1.0, 0.0, 0.0);		//X����]
	glScalef(scale[0], scale[1], scale[2]);
	drawColorSpace();						//�`��I�u�W�F�N�g
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
	glViewport(0, 0, w, h);																					//�r���[�|�[�g�ϊ�
	glMatrixMode(GL_PROJECTION);																			//���e�s��̎w��
	glLoadIdentity();																						//���e�s��̏�����
	gluPerspective(view.fovY, (double)w / (double)h, view.nearZ, view.farZ);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();																						//���f���r���[�s��̏�����
	gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);	//���_��ݒ�i����ϊ��j
	//�\���E�B���h�E�̃T�C�Y
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