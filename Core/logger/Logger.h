/*
 * Logger.h
 *
 *  Created on: Oct 19, 2019
 *      Author: Mati
 */

#ifndef LOGGER_H_
#define LOGGER_H_

/***** Includes section *****/

#include <cstdint>
#include <cstddef>

/****************************/


namespace SDL {


/*** Color enum ***/
enum Color{

	white = 0x00,
	grey,
	red,
	green,
	blue
};

/*** Log Level enum ***/
enum LogLevel{

	DEBUG = 0x00,
	INFO,
	ERROR
};

class Logger {

/*** public methods ***/
public:
	static Logger& getInstance();

	void print(const char* format, ...);
	void print(LogLevel logLevel, const char* format, ...);
	void setColor(Color color);

/*** public fields ***/
public:
	static const uint8_t MAX_MESSAGE_SIZE = 100;

/*** private methods ***/
private:
	void printLogLevel(LogLevel logLevel);
	Logger();
	Logger(const Logger& other);

	~Logger();
/*** priavte fields ***/
private:
	Color fontColor;

	/*Static array that contains colors codes*/
	static const char colorCodes[5][9];
	static const char* logLevelStrings[3];

	/*Message buffer*/;
	char message[MAX_MESSAGE_SIZE];

};

} /* namespace SDL */

#ifdef __cplusplus
extern "C" {
#endif

/**** Uart send function has to be defined by user ****/
__weak uint8_t uart_put(const char* msg, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* LOGGER_H_ */
