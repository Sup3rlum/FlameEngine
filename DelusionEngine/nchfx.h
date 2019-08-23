#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <map>
#include <unordered_map>
#include <mutex>
#include <thread>

#ifdef DELUSIONENGINE_EXPORTS
#define DELUSION_DLL __declspec(dllexport)
#else
#define DELUSION_DLL __declspec(dllimport)
#endif