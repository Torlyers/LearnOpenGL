#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "stb_image.h"

using namespace std;

//用于model类的纹理读取
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);



//普通的纹理读取
int GenerateTexture(const char* image_path);


//读取立方体纹理（用于天空盒）
unsigned int loadCubemap(vector<std::string> faces);

