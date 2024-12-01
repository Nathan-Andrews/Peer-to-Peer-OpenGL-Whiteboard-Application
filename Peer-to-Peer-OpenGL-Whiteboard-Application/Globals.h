#include "Source.h"

#ifndef GLOBALS_H
#define GLOBALS_H

// global connection manager
extern ConnectionManager* manager;

// thread variables for form managment
extern std::mutex form_mtx;
extern std::condition_variable wait_form;
extern FormData* formData;

#endif