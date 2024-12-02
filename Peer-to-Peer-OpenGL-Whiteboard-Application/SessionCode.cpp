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
	vector<unsigned char> encoded;
	size_t start = 0, end;

	while ((end = host.ip.find('.', start)) != string::npos) {
		encoded.push_back(stoi(host.ip.substr(start, end - start)));
		start = end + 1;
	}
	encoded.push_back(stoi(host.ip.substr(start)));

	//Port needs 2 bytes due to its size:
	encoded.push_back((host.port >> 8) & 0xFF);
	encoded.push_back(host.port & 0xFF);

	return base64Encoder(encoded);
}

void SessionCode::decodeSession(string encryptedCode) {
	vector<unsigned char> message = base64Decoder(encryptedCode);

	if (message.size() != 6) {
		throw std::invalid_argument("Incorrect parsing! Likely caused due to incorrect code.");
	}

	this->host.ip = to_string(message[0]) + "." + to_string(message[1]) + "." + to_string(message[2]) + "." + to_string(message[3]);
	this->host.port = (message[4] << 8) | message[5];
}

string SessionCode::base64Encoder(vector<unsigned char> encoded) {
	string message;
	int val = 0;
	int interval = -6;

	for (unsigned char c : encoded) {
		val = (val << 8) + c;
		interval += 8;
		while (interval >= 0) {
			message += base64[(val >> interval) & 0x3F]; //6 bit size
			interval -= 6;
		}
	}
	if (interval > -6) { //extra
		message += base64[((val << 8) >> (interval + 8)) & 0x3F];
	}
	while (message.size() % 4) { //padding
		message += '=';
	}
	return message;
}

//Helpers
vector<unsigned char> SessionCode::base64Decoder(string encryptedCode) {
	vector<unsigned char> message;
	vector<int> decode(256, -1);

	//Error checking
	if (encryptedCode.size() % 4 != 0 || encryptedCode.size() < 1) {
		throw std::invalid_argument("Incorrect code length, must be a multiple of 4!");
	}

	for (int i = 0; i < encryptedCode.size(); ++i) {
		if (isalnum(encryptedCode[i]) == false) {
			if (encryptedCode[i] != '+' || encryptedCode[i] != '/' || encryptedCode[i] != '=') {
				throw std::invalid_argument("Code contains incorrect character type!");
			}
		}
	}

	size_t findEqual = encryptedCode.find('=');
	if (findEqual != string::npos) {
		if (findEqual != (encryptedCode.size() - 1)) {
			if (encryptedCode.back() != '=') {
				throw std::invalid_argument("Invalid padding!");
			}
		}
	}

	for (int i = 0; i < 64; ++i) {
		decode[base64[i]] = i;
	}
	int val = 0;
	int interval = -8;
	for (unsigned char c : encryptedCode) {
		if (decode[c] == -1) {
			break;
		}
		val = (val << 6) + decode[c];
		interval += 6;
		if (interval >= 0) {
			message.push_back((val >> interval) & 0xFF); //8 bit size
			interval -= 8;
		}
	}
	return message;
}