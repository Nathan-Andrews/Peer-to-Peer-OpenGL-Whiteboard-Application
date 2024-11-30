#pragma once

// visual studio config
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

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

// library includes
#include <boost/asio.hpp>
#include <GLFW/glfw3.h>
#include <QtCore/QString>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>

// type definitions
typedef u_short PORT;
typedef std::string IP;

// macro constants
#define LOCALHOST "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

// local includes
#include "ConcurrentContainer.h"
#include "Connection.h"
#include "Server.h"
#include "ConnectionManager.h"
#include "ui_clientinterface.h"
#include "clientinterface.h"