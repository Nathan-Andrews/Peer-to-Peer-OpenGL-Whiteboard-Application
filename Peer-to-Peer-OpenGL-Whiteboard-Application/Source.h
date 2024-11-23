#pragma once

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

// typedefs
typedef u_short PORT;
typedef std::string HOST;

// macro constants
#define LOCALHOST "127.0.0.1"
#define MAX_BUFFER_SIZE 1024

// local includes
#include "ConcurrentContainer.h"
#include "Connection.h"
#include "Server.h"
#include "ConnectionManager.h"
