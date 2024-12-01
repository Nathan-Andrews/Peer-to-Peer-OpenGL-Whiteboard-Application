#include "Globals.h"

// global connection manager
ConnectionManager* manager = nullptr;

// thread variables for form managment
std::mutex form_mtx;
std::condition_variable wait_form;
FormData* formData = new FormData();