#pragma once

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class SessionCode {
public:
	SessionCode(string ip, int port);
	SessionCode();
	string generateCode();    //For the server
	string decodeSession(string encryptedCode);   //For the client

	string ip;
	int port;
	string code;
};