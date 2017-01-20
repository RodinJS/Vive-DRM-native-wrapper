#pragma once
#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "json.hpp"

using json = nlohmann::json;

struct dllDeleter {
	void operator()(HMODULE dll) { FreeLibrary(dll); }
};

class htc_vita_api_wrapper
{
	template<typename T>
	using dll_unique_ptr = std::unique_ptr<T, dllDeleter>;

private:

	static const LPCSTR dllName;
	static htc_vita_api_wrapper *instnace;

	typedef void(CALLBACK* VIVE_INIT_FUNC)(const char *, int(*callback)(char *message, char *signature));
	typedef bool(CALLBACK* csharp_verifyPublicKey)(const char *message, const char *signature, const char *publicKey);

	dll_unique_ptr<std::remove_pointer_t<HMODULE>> VIVEDLLHANDLE = nullptr;
	dll_unique_ptr<std::remove_pointer_t<HMODULE>> csharpdllhandle = nullptr;

	VIVE_INIT_FUNC vive_init;

	static int vive_callback(char *message, char *signature);
	
	std::string appID;
	std::string publicKey;

	bool recievedResoponse = false;
	std::string responseMessage;
	json responseJSON;

	bool isError = false;
	std::string error;
	std::string errorCode;
	
	long long issueTime;
	long long expirationTime;

public:
	
	htc_vita_api_wrapper(std::string appID, std::string publicKey);
	int init();
	static htc_vita_api_wrapper* getInstance();
	static bool verifySignature(std::string message, std::string signature, std::string publicKey);

	bool checkDRM();
};

std::string base64_decode(const std::string &in);