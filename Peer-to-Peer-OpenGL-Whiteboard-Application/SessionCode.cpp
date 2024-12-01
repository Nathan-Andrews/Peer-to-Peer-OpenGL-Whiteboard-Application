#include "Source.h"

using namespace std;

SessionCode::SessionCode() {
	this->ip = "";
	this->port = 0;
}

SessionCode::SessionCode(string ip, int port) {
	this->ip = ip;
	this->port = port;
}

string SessionCode::generateCode() {
	string combine = ip + ":" + to_string(port);

	for (int i = 0; i < combine.size(); ++i) {
		combine[i] = combine[i] + 19;
	}

	return combine;
}

string SessionCode::decodeSession(string encryptedCode) {
	for (int i = 0; i < encryptedCode.size(); ++i) {
		encryptedCode[i] = encryptedCode[i] - 19;
	}
	int parse = encryptedCode.find(':');

	this->ip = encryptedCode.substr(0, parse);
	this->port = stoi(encryptedCode.substr(parse+1));

	return encryptedCode;
}