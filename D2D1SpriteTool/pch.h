#pragma once
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\D2D1Library.lib")
#else 
#pragma comment(lib, "Release\\D2D1Library.lib")
#endif


#include "CorePch.h"