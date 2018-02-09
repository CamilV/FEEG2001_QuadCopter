
/*
rExcel.h - Library for Arduino Excel data exchange 
Created by Roberto Valgolio, 20/06/2016

Released into the public domain

History

14/07/2016	added 1ms delay in clearInput()
21/07/2016	ACK / NAK protocol that makes all very stable

*/

#ifndef rExcel_h
#define rExcel_h

#include "Arduino.h"								// without it: 'Serial' was not declared in this scope

#define ATMEL_COMPATIBLE							// comment this define if your Arduino isn't Atmel compatible like Arduino Due or Intel Edison

const int	XLS_BUFFER_SIZE = 32;					// buffer size for input
const int	XLS_MAX_ARGS = 5;						// max CSV message args for input
const int	XLS_TIMEOUT = 2000;						// reading timeout ms
const int	XLS_MAX_TRIES = 5;						// protocol tries
const char	XLS_ACK = 0x06;							// positive acknowledgement
const char	XLS_NAK = 0x15;							// negative acknowledgement


class rExcel
{

public:

	rExcel();
	bool    newss(char* worksheet);
	bool	write(char* worksheet, char* range, int   value);
	bool	write(char* worksheet, char* range, long  value);
	bool	write(char* worksheet, char* range, float value, int decimals);
	bool	write(char* worksheet, char* range, char* value);
	bool	writeIndexed(char* worksheet, unsigned int row, unsigned int column, int   value);
	bool	writeIndexed(char* worksheet, unsigned int row, unsigned int column, long  value);
	bool	writeIndexed(char* worksheet, unsigned int row, unsigned int column, float value, int decimals);
	bool	writeIndexed(char* worksheet, unsigned int row, unsigned int column, char* value);
	bool	get(char* worksheet, char* range, char* buffer);
	bool	getIndexed(char* worksheet, unsigned int row, unsigned int column, char* buffer);
	bool	clear(char* worksheet, char* range);
	bool	save();
	int		read(char* worksheet, char* range, unsigned int* row, unsigned int* column, char* buffer, char mode);
	void	clearInput();

private:

	int		readMessage(byte* startAddr, int nBytes, int timeout, bool lineBreak);
	int		mapArgs(char* startAddr, int nBytes, char** argPointer, int maxArgs);
	void	clearBuffer(byte* startAddr, int nBytes);

	char	_buffer[16];
	char	_str[6];
	char	_serialBuffer[XLS_BUFFER_SIZE];		// buffer for input
	char*	_serialArgs[XLS_MAX_ARGS];			// args pointers

};

#endif