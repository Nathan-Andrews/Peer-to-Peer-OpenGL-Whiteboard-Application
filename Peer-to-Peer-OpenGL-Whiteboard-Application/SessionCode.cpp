#include "Source.h"

using namespace std;

SessionCode::SessionCode() {
	this->host.ip = "";
	this->host.port = 0;
}

SessionCode::SessionCode(Host host) {
	this->host = host;
}

SessionCode::SessionCode(IP ip, PORT port) {
	this->host.ip = ip;
	this->host.port = port;
}

SessionCode::SessionCode(std::string encryptedCode) {
	decodeSession(encryptedCode);
}

string SessionCode::generateCode() {
	string combine = host.ip + ":" + to_string(host.port);

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

	this->host.ip = encryptedCode.substr(0, parse);
	this->host.port = stoi(encryptedCode.substr(parse+1));

	return encryptedCode;
}