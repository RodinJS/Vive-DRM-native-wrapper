#include "htc_vita_api_wrapper.h"
#include <string>

extern "C"
{
	__declspec(dllexport) void *newVitaApiWrapper(std::string appID, std::string publicKey)
	{
		//todo: try catch
		return new htc_vita_api_wrapper{ appID, publicKey };
	}
	
	__declspec(dllexport) bool checkDRM(void *VitaApiWrapper)
	{
		//todo: try catch
		return ((htc_vita_api_wrapper*)VitaApiWrapper)->checkDRM();
	}
}