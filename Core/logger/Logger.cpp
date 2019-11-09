/*
 * Logger.cpp
 *
 *  Created on: Oct 19, 2019
 *      Author: Mati
 */

/***** Includes section *****/

#include "Logger.h"
#include <cstdio>
#include <cstdarg>
#include <cstring>
/****************************/


namespace SDL {

/***** Static Consts initialization *****/

const char Logger::colorCodes[5][9] = {

		{"\u001b[0m"},	// WHITE COLOR CODE
		{"\u001b[37m"},	// GREY COLOR CODE
		{"\u001b[31m"},	// RED COLOR CODE
		{"\u001b[32m"},	// GREEN COLOR CODE
		{"\u001b[34m"}	// BLUE COLOR CODE
};

const char* Logger::logLevelStrings[3] = {

		"DEBUG::",
		"INFO::",
		"ERROR::"
};

/***************************************/

/***** Public methods definitions ****/

Logger& Logger::getInstance() {

	static Logger singleton;
	return singleton;
}


Logger::~Logger() {
	// nothing to do here
}

void Logger::print(const char* format, ...) {

	va_list args;
	va_start(args, format);

	uint8_t size = std::vsprintf(message, format, args);

	uint8_t bytesSent = 0;
	uint8_t shift = 0;

	do{

		bytesSent = uart_put(message + shift, size);
		shift+= bytesSent;
		size -= bytesSent;
	}while(size);

	va_end(args);
}

void Logger::print(LogLevel logLevel, const char* format, ...) {

	printLogLevel(logLevel);

	va_list args;
	va_start(args, format);

	uint8_t size = std::vsprintf(message, format, args);

	uint8_t bytesSent = 0;
	uint8_t shift = 0;

	do{

		bytesSent = uart_put(message + shift, size);
		shift+= bytesSent;
		size -= bytesSent;
	}while(size);

	va_end(args);
}

void Logger::setColor(Color color) {

	fontColor = color;
	print(colorCodes[fontColor]);
}



/***** Private methods definitions ****/
void Logger::printLogLevel(LogLevel logLevel){

	switch(logLevel){

		case LogLevel::DEBUG:{

			setColor(Color::grey);
			print(logLevelStrings[logLevel]);
			setColor(Color::white);
			break;
		}

		case LogLevel::ERROR:{

			setColor(Color::red);
			print(logLevelStrings[logLevel]);
			setColor(Color::white);
			break;
		}

		case LogLevel::INFO:{

			print(logLevelStrings[logLevel]);
			break;
		}

	}
}

Logger::Logger() :
		fontColor(white)
{

	std::memset(message, 0x00, MAX_MESSAGE_SIZE);
}

Logger::Logger(const Logger &other) {}

/**********************************************************/


} /* namespace SDL */


#define UNUSED(X) (void)(X)     /* To avoid gcc/g++ warnings */
__weak uint8_t uart_put(const char* msg, size_t size){

	UNUSED(msg);
	UNUSED(size);

	return 0;
}
