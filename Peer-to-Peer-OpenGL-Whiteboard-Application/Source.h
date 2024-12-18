#pragma once

// visual studio config
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define GLEW_STATIC
// standard includess
#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <atomic>
#include <cmath>
#include <fstream>

// library includes
#include <boost/asio.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <QtCore/QString>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>


// type definitions
typedef u_short PORT;
typedef std::string IP;
struct FormData;

// macro constants
#define LOCALHOST "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

// local includes
#include "ConcurrentContainer.h"
#include "Connection.h"
#include "Server.h"
#include "ConnectionManager.h"
#include "DrawAction.h"
#include "Whiteboard.h"
#include "SessionCode.h"
#include "UI_Interface.h"
#include "Interface.h"
