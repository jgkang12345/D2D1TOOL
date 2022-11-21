#include "pch.h"
#include "CorePch.h"

void GetFileExp(IN TCHAR* _fileName, OUT TCHAR* _ret) 
{
	std::stack<TCHAR> s;
	int pathLen = _tcslen(_fileName);
	for (int i = pathLen - 1; i >= 0; i--)
	{
		if (_fileName[i] == _T('.'))
			break;
		s.push(_fileName[i]);
	}

	int index = 0;

	while (s.empty() == false) 
	{
		TCHAR t =  s.top();
		s.pop();
		_ret[index] = t;
		index++;
	}
	_ret[index] ='\0';
}