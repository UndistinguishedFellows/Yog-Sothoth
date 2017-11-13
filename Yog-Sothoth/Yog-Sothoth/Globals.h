#pragma once

#define yogLog(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define yogConsole(type, format, ...) log(type, format, __VA_ARGS__);

enum ConsoleType
{
	CONSOLE_WARNING,
	CONSOLE_ERROR,
	CONSOLE_MESSAGE,
	CONSOLE_INFO
};

void log(const char file[], int line, const char* format, ...);
void log(ConsoleType type, const char* format, ...);

#define ROOT "root/"
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum game_status
{
	PLAY = 1,
	PAUSE,
	STOP,
	TO_PLAY,
	TO_STOP
};

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;
typedef unsigned __int32 UID;