#pragma once



#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <chrono>
#include <time.h>
#include <map>
#include <unordered_map>
#include <mutex>
#include <thread>

#include "framework.h"
#include "logger.h"
#include "macros.h"
#include "result.h"


typedef unsigned long long DELuint64;
typedef unsigned int DELuint32;

typedef long long DELint64;
typedef int DELint32;

typedef unsigned int DELHandle;
typedef unsigned char DELubyte;
typedef unsigned char DELuchar;



typedef glm::mat3x3 Matrix3;
typedef glm::mat4x4 Matrix4;
typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;
typedef glm::quat Quaternion;
