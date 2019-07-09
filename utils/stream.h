/**
 * @code	#include "utils/stream.h" @endcode
 * @file 	libs/utils/stream.h
 * @brief 	Stream handling library.
 */
//###############################################
#ifndef _UTILS_STREAM_H_
#define _UTILS_STREAM_H_
//###############################################
#include <utils/stream_defs.h>
//###############################################
/**
 * @class	Stream
 * @brief	Implements a bunch of functionalities to make easier the processing
 * 			of data for any serial interface.
 */
class Stream {
public:
	//===========================================
	/**
	 * @brief	Stream class constructor.
	 */
	Stream(void);

	//===========================================
	/**
	 * @brief	Return the number of bytes available to be written in the
	 * 			Tx buffer.
	 * @return	bytes available to write on the TX buffer.
	 */
	virtual uint8_t tx_available(void) = 0;

	/**
	 * @brief	Flush the remaining bytes of the TX buffer.
	 */
	virtual void flush(void) = 0;

	/**
	 * @brief	Send a single byte.
	 * @param	byte		Byte to be transmitted.
	 * @return 	Number of bytes sent.
	 */
	virtual size_t write(uint8_t byte) = 0;

	/**
	 * @brief	Send a bunch of characters from a given buffer.
	 * @param	cstr		Pointer to character array (string).
	 * @return 	Number of bytes sent.
	 */
	size_t write(const char* cstr);

	/**
	 * @brief	Send a bunch of characters from a given buffer.
	 * @param	cstr		Pointer to character array.
	 * @param	size		Number of bytes to be sent.
	 * @return 	Number of bytes sent.
	 */
	size_t write(const char* cstr, size_t size);

	/**
	 * @brief	Send a bunch of bytes from a given buffer.
	 * @param	buff		Pointer to byte array.
	 * @param	size		Number of bytes to be sent.
	 * @return 	Number of bytes sent.
	 */
	size_t write(const uint8_t* buff, size_t size);

	//======================================
	size_t print(char c);

	size_t print(const char* cstr);

	size_t print(const String& str);

	size_t print(const FlashString* pstr);

	size_t print(uint32_t num, num_base_t base=NUM_BASE_DEC);

	size_t print(uint16_t num, num_base_t base=NUM_BASE_DEC);

	size_t print(uint8_t num, num_base_t base=NUM_BASE_DEC);

	size_t print(int32_t num, num_base_t base=NUM_BASE_DEC);

	size_t print(int16_t num, num_base_t base=NUM_BASE_DEC);

	size_t print(int8_t num, num_base_t base=NUM_BASE_DEC);

	size_t print(double num, uint8_t p=3);

	//===========================================
	/**
	 * @brief	Return the number of bytes waiting to be read.
	 * @return	bytes waiting in the RX buffer.
	 */
	virtual int16_t rx_available(void) = 0;

	/**
	 * @brief	Peek the next byte available on the RX buffer.
	 * @return	Next byte available on the RX buffer.
	 */
	virtual int16_t peek(void) = 0;

	/**
	 * @brief	Get a single byte from the RX buffer.
	 * @return	Next byte available on the RX buffer.
	 */
	virtual int16_t read(void) = 0;

	//======================================
	/**
	 * @brief	Sets the timeout used to get bytes from the input stream.
	 * @param	timeout		Time for byte timeout in [ms].
	 */
	void set_timeout(uint32_t timeout);

	/**
	 * @brief	Get the actual timeout.
	 * @return	Stream reading timeout in [ms].
	 */
	uint32_t get_timeout(void);

	//======================================
	/**
	 * @brief	Parse a signed integer number from the input stream.
	 * @param	ignore		Characters to be ignored on the number search.
	 * @param	skip_mode	Look ahead modes. Checkout stream_look_mode_t for more info.
	 * @return	Integer number.
	 */
	int32_t parse_int(char ignore='\x01', stream_look_mode_t skip_mode=SKIP_ALL);

	/**
	 * @brief	Parse a signed double number from the input stream.
	 * @param	ignore		Characters to be ignored on the number search.
	 * @param	skip_mode	Look ahead modes. Checkout stream_look_mode_t for more info.
	 * @return	Double number.
	 */
	double parse_double(char ignore='\x01', stream_look_mode_t skip_mode=SKIP_ALL);

	//======================================
	/**
	 * @brief	Read a 'size' number of bytes from the input stream. It stops
	 * 			when the buffer is full or the timeout is reached.
	 * @param	buff		Pointer to a byte array.
	 * @param	size		Number of bytes to be read.
	 * @return	Number of bytes actually read.
	 */
	size_t read_bytes(uint8_t* buff, size_t size);

	/**
	 * @brief	Variant of Stream::read_bytes(uint8_t*, size_t) for characters.
	 * @param	cstr		Pointer to a character array.
	 * @param	size		Number of characters to read.
	 * @return	Number of characters actually read.
	 */
	size_t read_bytes(char* cstr, size_t size);

	/**
	 * @brief	Read a string from the input stream until any more bytes
	 * 			are available.
	 * @return	Input stream string.
	 */
	String read_string(void);

	/**
	 * @brief	Similar to Stream::read_bytes(uint8_t*, size_t) but this method
	 * 			also stops when the terminator character is detected.
	 * @param	terminator	Character used to terminate the reading.
	 * @param	buff		Pointer to a byte array.
	 * @param	size		Number of bytes to be read.
	 * @return	Number of bytes actually read.
	 */
	size_t read_bytes_until(char terminator, uint8_t* buff, size_t size);

	/**
	 * @brief	Variant of Stream::read_bytes_until(char, uint8_t*, size_t) for characters.
	 * @param	terminator	Character used to terminate the reading.
	 * @param	cstr		Pointer to a character array.
	 * @param	size		Number of bytes to be read.
	 * @return	Number of characters actually read.
	 */
	size_t read_bytes_until(char terminator, char* cstr, size_t size);

	/**
	 * @brief	Read a string from the input stream until any more bytes
	 * 			are available or the terminator character is reached.
	 * @param 	terminator	Character used to terminate the reading.
	 * @return	Input stream string.
	 */
	String read_string_until(char terminator='\n');

	/**
	 * @brief	Ignore the input stream until a terminator is detected.
	 * @param	terminator 	Character used to terminate the ignoring.
	 * @return	True if the next byte is the terminator, False if empty or timed out.
	 */
	bool ignore_bytes_until(char terminator);

protected:
	//===========================================
	uint32_t _rx_time_start;			//!< Timeout measurement start.
	uint32_t _rx_timeout;				//!< Timeout for "reading" functions.

	//===========================================
	/**
	 * @brief	Converts a unsigned integer into a string and writes the
	 * 			characters on the TX buffer.
	 * @param	num			Number to be converted.
	 * @param	base		Number desired base: BIN, OCT, DEC, HEX.
	 * @return	Number of characters written.
	 */
	size_t print_uint(uint32_t num, num_base_t base);

	/**
	 * @brief	Extends Stream::print_uint(uint32_t, num_base_t) for signed
	 * 			integers.
	 * @copydetails Stream::print_uint(uint32_t, num_base_t)
	 */
	size_t print_int(int32_t num, num_base_t base);

	/**
	 * @brief	Converts a double into a string and writes the characters
	 * 			on the TX buffer.
	 * @param	num			Number to be converted.
	 * @param	p			Number of decimal positions.
	 * @return 	Number of characters written.
	 */
	size_t print_double(double num, uint8_t p);

	//===========================================
	/** @brief	Peeks the input stream using a timeout. */
	int16_t timed_peek(void);

	/** @brief	Reads the input stream using a timeout. */
	int16_t timed_read(void);

	//======================================
	/**
	 * @brief	Peek the input stream with a timeout. It search if the
	 * 			peeked byte is part of a digit.
	 * @param	look_mode	Look ahead modes. Checkout stream_look_mode_t for more info.
	 * @param	is_decimal	Accept the detection of decimal points.
	 * @return	Return the next numeric byte on the stream or -1 if timeout.
	 */
	int16_t peek_next_digit(stream_look_mode_t look_mode, bool is_decimal);

};

//###############################################
#endif /* _UTILS_STREAM_H_ */
//###############################################
