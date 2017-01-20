#include "htc_vita_api_wrapper.h"

const LPCSTR htc_vita_api_wrapper::VitaApiDLLName = "vita_api";
const LPCSTR htc_vita_api_wrapper::SignatureVerifierDLLName = "signatureVerifier";
htc_vita_api_wrapper *htc_vita_api_wrapper::instnace;

htc_vita_api_wrapper::htc_vita_api_wrapper(std::string appID, std::string publicKey) : 
	appID{appID}, 
	publicKey{publicKey},
	VIVEDLLHANDLE{ nullptr },
	CSHARPDLLHANDLE{ nullptr }
{
	if (htc_vita_api_wrapper::instnace != NULL)
		throw new std::exception("htc_vita_api_wrapper is a singletone class");
		
	this->VIVEDLLHANDLE.reset(LoadLibrary(this->VitaApiDLLName));

	if (this->VIVEDLLHANDLE.get() == nullptr)
		throw new std::exception("Can not load dll");
		

	this->vive_init = (VIVE_INIT_FUNC)GetProcAddress(VIVEDLLHANDLE.get(), "init");
	if (!vive_init)
	{
		//todo: check if interfeers with the destructor
		FreeLibrary(this->VIVEDLLHANDLE.get());
		throw new std::exception("Can not find init function in the dll");
	}

	htc_vita_api_wrapper::instnace = this;
}

bool htc_vita_api_wrapper::verifySignature(std::string message, std::string signature, std::string publicKey)
{

	htc_vita_api_wrapper::instnace->CSHARPDLLHANDLE.reset(LoadLibrary(htc_vita_api_wrapper::SignatureVerifierDLLName));
	if (htc_vita_api_wrapper::instnace->CSHARPDLLHANDLE.get() == NULL)
		throw new std::exception("Can not load signatureVerifier dll");

	csharp_verifyPublicKey verifyPublicKey = (csharp_verifyPublicKey)GetProcAddress(htc_vita_api_wrapper::instnace->CSHARPDLLHANDLE.get(), "verifyPublicKey");

	bool res = false;

	if (!verifyPublicKey)
	{
		//todo: check if interfeers with the constructor
		FreeLibrary(htc_vita_api_wrapper::instnace->VIVEDLLHANDLE.get());
		throw new std::exception("Can not find verifyPublicKey function in the dll");
	}

	try {
		res = verifyPublicKey(message.c_str(), signature.c_str(), publicKey.c_str());
	}
	catch (std::exception e)
	{		
		FreeLibrary(htc_vita_api_wrapper::instnace->CSHARPDLLHANDLE.get());
		return res;
	}

	FreeLibrary(htc_vita_api_wrapper::instnace->CSHARPDLLHANDLE.get());
	return res;
}

int htc_vita_api_wrapper::vive_callback(char *message, char *signature)
{
	using namespace std;

	htc_vita_api_wrapper *_this = htc_vita_api_wrapper::getInstance();	
	string Message(message);
	string Signature(signature);

	_this->responseMessage = base64_decode(Message);

	if (Signature.size() == 0)
	{
		_this->isError = true;
		try 
		{			
			_this->responseJSON = json::parse(Message.c_str());

			_this->error = _this->responseJSON["message"].get<string>();
			_this->errorCode = _this->responseJSON["code"].get<string>();
		}
		catch (exception e)
		{
			cout << e.what() << endl;
		}

		_this->recievedResoponse = true;
		return 1;
	}

	bool signatureVerification = verifySignature(_this->appID + "\n" + Message, Signature, _this->publicKey);
	if (!signatureVerification)
	{
		_this->isError = true;
		_this->error = "Signature verification error";

		_this->recievedResoponse = true;
		return 1;
	}

	_this->responseMessage = base64_decode(Message.substr(Message.find("\n") + 1));

	try
	{
		_this->responseJSON = json::parse(_this->responseMessage.c_str());
	}
	catch (exception e)
	{
		_this->isError = true;
		_this->error = "JSON parse error";

		_this->recievedResoponse = true;
		return 1;
	}
	

	try 
	{
		_this->issueTime = _this->responseJSON["issueTime"].get<long long>();
	}
	catch (exception e)
	{}

	try
	{
		_this->expirationTime = _this->responseJSON["expirationTime"].get<long long>();
	}
	catch (exception e)
	{}

	_this->recievedResoponse = true;
	return 0;
}


htc_vita_api_wrapper* htc_vita_api_wrapper::getInstance()
{
	return htc_vita_api_wrapper::instnace;
}


bool htc_vita_api_wrapper::checkDRM()
{
	this->vive_init(this->appID.c_str(), &this->vive_callback);

	while (!this->recievedResoponse)
		Sleep(50);

	if (this->isError)
	{
		std::cout << "ERROR : " << this->errorCode << " " << this->error << std::endl;

		return false;
	}
		

	std::cout << "Successfully verified " << this->issueTime << " " << this->expirationTime << std::endl;

	return true;
}


std::string base64_decode(const std::string &in) {

	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

	int val = 0, valb = -8;
	for (char c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}
