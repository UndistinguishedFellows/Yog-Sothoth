#pragma once

#define yogLog(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define yogConsole(format, ...) log(format, __VA_ARGS__);
void log(const char file[], int line, const char* format, ...);
void log(const char* format, ...);

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

typedef unsigned int uint;