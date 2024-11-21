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

// local includes
#include "ConcurrentQueue.h"
#include "Connection.h"
#include "Server.h"
#include "ConnectionManager.h"