#pragma once

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

class SessionCode {
public:
	SessionCode(Host host);
	SessionCode(IP ip, PORT port);
	SessionCode(std::string encryptedCode);
	SessionCode();
	string generateCode();    //For the server
	string decodeSession(string encryptedCode);   //For the client

	Host host;
	string code;
};