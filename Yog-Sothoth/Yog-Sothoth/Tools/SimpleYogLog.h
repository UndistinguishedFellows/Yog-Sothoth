#ifndef SIMPLEYOGLOG_H_INCLUDED
#define SIMPLEYOGLOG_H_INCLUDED

#include "BaseLog.h"
#include <ostream>
#include <iostream>

///the SimpleLog is just a Virtuoso::BaseLog that uses a single ostream for all write operations.
///the default c-tor uses cout by default, but can be changed later.
class SimpleYogLog :public Virtuoso::BaseLog
{

	std::ostream* os;
public:
	//default to cout
	SimpleYogLog()
		:os(&std::cout)
	{
	}

	//construct with a particular stream
	SimpleYogLog(std::ostream& str) :os(&str)
	{
	}

	///overwrite the stream
	void  setStream(std::ostream& str)
	{
		os = &str;
	}


	inline virtual std::ostream& status()
	{
		return *os;
	}

	inline virtual std::ostream& error()
	{
		return *os;
	}

	inline virtual std::ostream& echo()
	{
		return *os;
	}

	inline virtual std::ostream& warning()
	{
		return *os;
	}

};

#endif