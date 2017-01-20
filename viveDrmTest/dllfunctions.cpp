#include "htc_vita_api_wrapper.h"
#include <string>

extern "C"
{
	__declspec(dllexport) void *newVitaApiWrapper(std::string appID, std::string publicKey)
	{
		return new htc_vita_api_wrapper{ appID, publicKey };
	}
	
	__declspec(dllexport) bool checkDRM(void *VitaApiWrapper)
	{
		return ((htc_vita_api_wrapper*)VitaApiWrapper)->checkDRM();
	}
}