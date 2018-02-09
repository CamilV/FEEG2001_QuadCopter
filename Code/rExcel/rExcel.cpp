
#include "rExcel.h"

// The message formats to Excel (this app client role) are:
//   XLS,Command,Sheet,Range,Value
//		or
//   XLS,Command,Sheet,Row,Colunm,Value
// where:
//  'XLS'         is a constant keyword for messages to Excel
//  'Command'     specifies the action (write, writeCells, get, getCells, clear, save, ...)
//  'Worksheet'   specifies the sheet where the action is applied
//  'Range'       specifies a range of cells where the action is applied
//		or
//  'Row'         specifies the cell row where the action is applied
//  'Column'      specifies the cell column where the action is applied
//  'Value'       is the written value that can be:
//      integer
//      float
//		long
//      string
//      '%now%'   keyword that will be substituted with current date time
//      '%date%'  keyword that will be substituted with current date
//      '%time%'  keyword that will be substituted with current time
// messages are \n terminated
// after each message Excel answers with ACK or NAK

// The message formats from Excel (this app server role) are:
//   VAL,Sheet,Range,Value
//		or
//   VAL,Sheet,Row,Colunm,Value
// where:
//  'VAL'         is a constant keyword for messages from Excel
//  'Worksheet'   specifies the sheet where data are from
//  'Range'       specifies a range of cells where data are from
//		or
//  'Row'         specifies the cell row where data are from
//  'Column'      specifies the cell column where data are from
//  'Value'       is the returned value as string
// messages are \n terminated
// after each message ACK or NAK are sent to Excel


rExcel::rExcel(void)
{
}
bool rExcel::newss(char* worksheet)
{
	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send save message
		Serial.print("XLS,newss,");
		Serial.print(worksheet);
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				return true;
			}
		}
		clearInput();
		i++;
	}

	return false;
}

// write the value in the specified range
bool rExcel::write(char* worksheet, char* range, int value) {
	sprintf(_buffer, "%d", value);
	return write(worksheet, range, _buffer);
}
bool rExcel::write(char* worksheet, char* range, long value) {
	sprintf(_buffer,"%ld",value);
	return write(worksheet, range, _buffer);
}
bool rExcel::write(char* worksheet, char* range, float value, int decimals) {
#ifdef ATMEL_COMPATIBLE
	dtostrf(value, 4, decimals, _buffer);					// http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__stdlib_1ga060c998e77fb5fc0d3168b3ce8771d42.html
#else
	sprintf(_str, "%%.%df", decimals);
	sprintf(_buffer, _str, value);
#endif
	return write(worksheet, range, _buffer);
}
bool rExcel::write(char* worksheet, char* range, char* value) {

	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send write message
		Serial.print("XLS,write,");
		Serial.print(worksheet);
		Serial.print(",");
		Serial.print(range);
		Serial.print(",");
		Serial.print(value);
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				return true;
			}
		}
		clearInput();
		i++;
	}

	return false;
}


// write the value in the specified worksheet, row, colunm
bool rExcel::writeIndexed(char* worksheet, unsigned int row, unsigned int column, int value) {
	sprintf(_buffer, "%d", value);
	return writeIndexed(worksheet, row, column, _buffer);
}
bool rExcel::writeIndexed(char* worksheet, unsigned int row, unsigned int column, long value) {
	sprintf(_buffer, "%ld", value);
	return writeIndexed(worksheet, row, column, _buffer);
}
bool rExcel::writeIndexed(char* worksheet, unsigned int row, unsigned int column, float value, int decimals) {
#ifdef ATMEL_COMPATIBLE
	dtostrf(value, 4, decimals, _buffer);					// http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__stdlib_1ga060c998e77fb5fc0d3168b3ce8771d42.html
#else
	sprintf(_str, "%%.%df", decimals);
	sprintf(_buffer, _str, value);
#endif
	return writeIndexed(worksheet, row, column, _buffer);
}
bool rExcel::writeIndexed(char* worksheet, unsigned int row, unsigned int column, char* value) {

	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send writeCells message
		Serial.print("XLS,writeCells,");
		Serial.print(worksheet);
		Serial.print(",");
		Serial.print(row);
		Serial.print(",");
		Serial.print(column);
		Serial.print(",");
		Serial.print(value);
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				return true;
			}
		}
		clearInput();
		i++;
	}

	return false;
}


// get the value from specified range
bool rExcel::get(char* worksheet, char* range, char* value) {

	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send get message
		Serial.print("XLS,get,");
		Serial.print(worksheet);
		Serial.print(",");
		Serial.print(range);
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				// get value
				unsigned int row = 0;
				unsigned int column = 0;
				if ((l = read(worksheet, range, &row, &column, value, 'R')) > 0) {
					return true;
				}
			}
		}
		clearInput();
		i++;
	}

	return false;
}


// get the value from specified worksheet, row, colunm
bool rExcel::getIndexed(char* worksheet, unsigned int row, unsigned int column, char* value) {

	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send get message
		Serial.print("XLS,getCells,");
		Serial.print(worksheet);
		Serial.print(",");
		Serial.print(row);
		Serial.print(",");
		Serial.print(column);
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				// get value
				char range = '\0';
				if ((l = read(worksheet, &range, &row, &column, value, 'I')) > 0) {
					return true;
				}
			}
		}
		clearInput();
		i++;
	}

	return false;
}


// clear the range
bool rExcel::clear(char* worksheet, char* range) {

	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send clear message
		Serial.print("XLS,clear,");
		Serial.print(worksheet);
		Serial.print(",");
		Serial.print(range);
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				return true;
			}
		}
		clearInput();
		i++;
	}

	return false;
}


// save the workbook
bool rExcel::save() {

	int i = 0;
	int l;
	char c[2];

	while (i < XLS_MAX_TRIES) {

		// send save message
		Serial.print("XLS,save");
		Serial.print("\n");

		// get acknowlegment
		if ((l = readMessage((byte*)c, 2, XLS_TIMEOUT, true)) > 0) {
			if (c[0] == XLS_ACK) {
				return true;
			}
		}
		clearInput();
		i++;
	}

	return false;
}


// read a message and decode it
int rExcel::read(char* worksheet, char* range, unsigned int* row, unsigned int* column, char* value, char mode) {
	
	// clear buffer
	clearBuffer((byte*)_serialBuffer, XLS_BUFFER_SIZE);

	// wait a message
	int l;
	if ((l = readMessage((byte*)_serialBuffer, XLS_BUFFER_SIZE, XLS_TIMEOUT, true)) > 0) {

		// if not buffer overflow
		if (l < XLS_BUFFER_SIZE) {

			// map CSV arguements
			// at least tree arguements for VAL message are expected
			if (mapArgs(_serialBuffer, l, _serialArgs, XLS_MAX_ARGS) > 2) {

				// messages must have VAL at beginning
				if (strcmp(_serialArgs[0], "VAL") == 0) {

					// no check digit in this version to ensure the value correctness

					// references can be useful in server role
					strcpy(worksheet, _serialArgs[1]);
					if (mode == 'R') {
						strcpy(range, _serialArgs[2]);
						strcpy(value, _serialArgs[3]);				// _serialArgs[3] contains the current received value
					}
					else if (mode == 'I') {
						*row = atoi(_serialArgs[2]);
						*column = atoi(_serialArgs[3]);
						strcpy(value, _serialArgs[4]);				// _serialArgs[4] contains the current received value
					}
					else {
						range[0] = '\0';
						*row = 0;
						*column = 0;
						*value = '\0';
					}

					// get the value length
					l = 0;
					while (value[l] != '\0') {
						l++;
					}

					return (l);
				}
			}
		}
	}
	
	return 0;
}


// input clearing
void rExcel::clearInput() {

	delay(1);

	while (Serial.available()) {

		char c = Serial.read();
	}
}

// private functions

// read a message from serial
// trasporre il codice in rTcpLink
int rExcel::readMessage(byte* startAddr, int nBytes, int timeout, bool lineBreak) {

	int		wait = 0;
	byte*	p = startAddr;
	bool	finish = false;
	int		receivedCount = 0;

	//Serial.println(F("readMessage"));

	while ((wait < timeout) && !finish) {

		// buffer compiling cycle
		while ((Serial.available() > 0) && !finish) {

			*p = Serial.read();
			receivedCount++;

			if (lineBreak && *p == '\n') {
				//Serial.println(F("\tline break"));
				finish = true;
			}
			else if (receivedCount == nBytes) {
				//Serial.println(F("\tnBytes break"));
				finish = true;
			}

			p++;
		}

		// wait 1ms
		delay(1);
		wait++;
	}

	if (finish == false) {
		//Serial.println(F("\ttimeout"));
	}

	return receivedCount;
}


// CSV arguements mapping
int rExcel::mapArgs(char* startAddr, int nBytes, char** argPointer, int maxArgs) {

	int		argCount;
	char*	p;

	*argPointer = startAddr;
	argCount = 1;

	for (p = startAddr; p < (startAddr + nBytes); p++) {

		if ((*p == ',') || (*p == '\n')) {		// comma separate arguements, \n terminates the last arguement

			*p = '\0';							// comma is substituted with \0 so the buffer contains more arguements readable as string

			if (argCount <= maxArgs) {
				argCount++,
				argPointer++;
				*argPointer = p + 1;			// store arg address
			}
			else {
				//Serial.println(F("args overflow"));
				return 0;
			}
		}
	}

	// at least return 1
	return argCount;
}


// buffer clearing
void rExcel::clearBuffer(byte* startAddr, int nBytes) {

	byte*   p;

	for (p = startAddr; p < (startAddr + nBytes); p++) {
		*p = '\0';
	}
}

