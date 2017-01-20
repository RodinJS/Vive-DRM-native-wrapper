#pragma once
#include <iostream>
#include <string>
#include "htc_vita_api_wrapper.h"

using namespace std;

string APP_KEY = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDFypCg0OHfBC+VZLSWPbNSgDo9qg/yQORDwGy1rKIboMj3IXn4Zy6h6bgn8kiMY7VI0lPwIj9lijT3ZxkzuTsI5GsK//Y1bqeTol4OUFR+47gj+TUuekAS2WMtglKox+/7mO6CA1gV+jZrAKo6YSVmPd+oFsgisRcqEgNh5MIURQIDAQAB";

int main()
{
	try {
		htc_vita_api_wrapper vitaApi{ "bd67b286-aafc-449d-8896-bb7e9b351876", APP_KEY };		
		vitaApi.checkDRM();
	}
	catch (exception *e)
	{
		cout << e->what() << endl;
	}

	int t;
	cin >> t;
	return 0;
}