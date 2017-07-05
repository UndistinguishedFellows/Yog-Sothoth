#pragma once
#include <string>

namespace tools
{
	static char** ParseCommand(const char* string, int* argc)
	{
		bool lastCharSpace = true;
		int _argc = 0;
		char buffer[100][500];
		int bi = 0;
		char **args = nullptr;

		for (int i = 0; i < strlen(string)+1; ++i)
		{
			if (string[i] == ' ')
			{
				if (!lastCharSpace)
				{
					buffer[_argc][bi] = '\0';	
					_argc++;
					lastCharSpace = true;
					bi = 0;
				}
			}
			else
			{
				if (lastCharSpace)
					lastCharSpace = false;

				buffer[_argc][bi++] = string[i];
			}
		}
		_argc++;
		(*argc) = _argc;
		args = new char*[_argc];
		for (int i = 0; i < _argc; i++)
		{
			args[i] = new char[(strlen(buffer[i]) + 1)];
			strcpy(args[i], buffer[i]);
		}
		return args;
	}
	//static class ConsoleCommandReader
	//{
	//	ConsoleCommandReader(){}
	//	virtual ~ConsoleCommandReader(){}

	//	

	//};
}
