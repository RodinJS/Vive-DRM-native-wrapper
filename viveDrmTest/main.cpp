#pragma once
#include <iostream>
#include <string>
#include "htc_vita_api_wrapper.h"

using namespace std;

int main()
{

	htc_vita_api_wrapper vitaApi{ "bd67b286-aafc-449d-8896-bb7e9b351876" };
	vitaApi.init();
	vitaApi.checkDRM();

	int t;
	cin >> t;
	return 0;
}