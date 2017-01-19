#pragma once
#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class htc_vita_api_wrapper
{
private:

	static const LPCSTR dllName;
	static htc_vita_api_wrapper *instnace;

	typedef void(CALLBACK* VIVE_INIT_FUNC)(const char *, int(*callback)(char *message, char *signature));
	typedef bool(CALLBACK* csharp_verifyPublicKey)(const char *message, const char *signature);
	HINSTANCE VIVEDLLHANDLE;
	HINSTANCE csharpdllhandle;

	VIVE_INIT_FUNC vive_init;

	static int vive_callback(char *message, char *signature);
	
	std::string appID;

	bool recievedResoponse = false;
	std::string responseMessage;
	json responseJSON;

	bool isError = false;
	std::string error;
	std::string errorCode;

	long long issueTime;
	long long expirationTime;
public:
	
	htc_vita_api_wrapper(std::string appID);
	int init();
	static htc_vita_api_wrapper* getInstance();
	static bool verifySignature(std::string message, std::string signature);

	std::string& getLastMessage();
	std::string& getLastSignature();

	bool checkDRM();

	~htc_vita_api_wrapper();
};

std::string base64_decode(const std::string &in);