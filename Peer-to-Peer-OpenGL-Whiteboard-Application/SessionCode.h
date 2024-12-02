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
	void decodeSession(string encryptedCode);   //For the client

	Host host;
	string code;

private:
	string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	string base64Encoder(vector<unsigned char> encoded); //generateCode helper
	vector<unsigned char> base64Decoder(string encryptedCode); //decodeSession helper

};