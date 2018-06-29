#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "stb_image.h"

using namespace std;

//����model��������ȡ
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);



//��ͨ�������ȡ
int GenerateTexture(const char* image_path);


//��ȡ����������������պУ�
unsigned int loadCubemap(vector<std::string> faces);

