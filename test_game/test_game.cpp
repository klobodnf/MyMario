// test_game.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <time.h>


int _tmain(int argc, _TCHAR* argv[])
{
	char str1[] = "Sample string";
	char str2[40];

	strcpy(str2, str1);
	strcpy(str2, "td");
	printf("%s", str2); 
	system("pause");
	return 0;
}

