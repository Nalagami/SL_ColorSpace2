#pragma once
#include <iostream>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

GLuint vertexshader, fragmentshader;

//shader file��ǂݍ��݃R���p�C������
void readShaderCompile(GLuint shader, const char* file)
{
	FILE* fp;
	char* buf;
	GLsizei size, len;
	GLint compiled;

	//�t�@�C�����J��
	//fp = fopen(file, "rb");
	if ((fopen_s(&fp, file, "rb"))) std::cout << "�t�@�C�����J�����Ƃ��ł��܂���" << file << std::endl;

	//�t�@�C���̖����Ɉړ����A���݈ʒu�𓾂�
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	//�t�@�C���T�C�Y�̃��������m��
	buf = (GLchar*)malloc(size);
	if (buf == NULL) std::cout << "���������m�ۂł��܂���ł���" << std::endl;

	//�t�@�C����擪����ǂݍ���
	fseek(fp, 0, SEEK_SET);
	fread(buf, 1, size, fp);

	//�V�F�[�_�E�I�u�W�F�N�g�Ƀv���O�������Z�b�g
	glShaderSource(shader, 1, (const GLchar**)&buf, &size);

	//�V�F�[�_�̓ǂݍ��ݗ̈�̊J��
	free(buf);
	fclose(fp);

	//�V�F�[�_�̃R���p�C��
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		std::cout << "�R���p�C���ł��܂���ł���!!:" << file << std::endl;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (char*)malloc(size);
			glGetShaderInfoLog(shader, size, &len, buf);
			std::cout << buf << std::endl;
			free(buf);
		}
	}
}

//�����N����
void link(GLuint prog)
{
	GLsizei size, len;
	GLint	linked;
	char* infoLog;

	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE)
	{
		std::cout << "�����N�ł��܂���ł���!" << std::endl;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &size);
		if (size > 0)
		{
			infoLog = (char*)malloc(size);
			glGetProgramInfoLog(prog, size, &len, infoLog);
			std::cout << infoLog << std::endl;
			free(infoLog);
		}
	}
}

//���_�V�F�[�_�݂̂̏ꍇ
void initGlsl(GLuint* program, const char* vertexFile)
{
	//glew�̏�����
	GLenum err = glewInit();
	if (err != GLEW_OK) std::cout << "Error:" << glewGetErrorString(err) << std::endl;

	//GPU�AOpenGL���
	std::cout << "VENDOR=" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GPU=" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL=" << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL=" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//�V�F�[�_�I�u�W�F�N�g�̍쐬
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//�V�F�[�_�[�̓ǂݍ��݂�R���p�C��
	readShaderCompile(vertexshader, vertexFile);

	//�V�F�[�_�v���O�����̍쐬
	*program = glCreateProgram();

	//�V�F�[�_���W�F�N�g���V�F�[�_�v���O�����Ɋ֘A�t����
	glAttachShader(*program, vertexshader);

	//�V�F�[�_�I�u�W�F�N�g�̍폜
	glDeleteShader(vertexshader);

	//�V�F�[�_�v���O�����̃����N
	link(*program);
}

//�t���O�����g�V�F�[�_������ꍇ
void initGlsl(GLuint* program, const char* vertexFile, const char* fragmentFile)
{
	//glew�̏�����
	GLenum err = glewInit();
	if (err != GLEW_OK) std::cout << "Error:" << glewGetErrorString(err) << std::endl;

	//GPU�AOpenGL���
	std::cout << "VENDOR=" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GPU=" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL=" << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL=" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//�V�F�[�_�I�u�W�F�N�g�̍쐬
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//�V�F�[�_�[�̓ǂݍ��݂�R���p�C��
	readShaderCompile(vertexshader, vertexFile);
	readShaderCompile(fragmentshader, fragmentFile);

	//�V�F�[�_�v���O�����̍쐬
	*program = glCreateProgram();

	//�V�F�[�_���W�F�N�g���V�F�[�_�v���O�����Ɋ֘A�t����
	glAttachShader(*program, vertexshader);
	glAttachShader(*program, fragmentshader);

	//�V�F�[�_�I�u�W�F�N�g�̍폜
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//�V�F�[�_�v���O�����̃����N
	link(*program);
}