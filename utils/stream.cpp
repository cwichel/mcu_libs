/**
 * @file 	libs/utils/stream.cpp
 * @brief 	Stream handling library.
 */
//###############################################
#include <utils/stream.h>
//###############################################
Stream::Stream(void): _rx_time_start(0), _rx_timeout(500) {
	// Start the timing functions
	timing_enable();
}

//###############################################
size_t Stream::write(const char* cstr) {
	return write(cstr, strlen(cstr));
}

size_t Stream::write(const char* cstr, size_t size) {
	return write((const uint8_t*) cstr, size);
}

size_t Stream::write(const uint8_t* buff, size_t size) {
	// Check if the buffer is available
	if ((buff == NULL) || (size == 0)) return 0;
	// If it is, send the bytes
	size_t txnum = 0;
	while (size--) {
		if (write(*buff++)) txnum++;
		else break;
	}
	return txnum;
}

//===============================================
size_t Stream::print_uint(uint32_t num, num_base_t base) {
	char num_str[12];	// Max U32: 4294967295 => 10 digits
	char *str = &num_str[sizeof(num_str) - 1];
	char tmp;
	*str='\0';
	do {
		tmp = num % base;
		num /= base;
		*--str = (tmp < 10)? tmp + '0' : (tmp - 10) + 'A';
	} while (num);
	return write(str);
}

size_t Stream::print_int(int32_t num, num_base_t base) {
	size_t len = 0;
	// Handle negative numbers
	if (num < 0) {
		len += write('-');
		num = -num;
	}
	// Print integer
	return print_uint(num, base) + len;
}

size_t Stream::print_double(double num, uint8_t p) {
	if (isnan(num)) return print(F("nan"));
	if (isinf(num)) return print(F("inf"));
	if (num > 4294967040.0) return print(F("ovf"));
	if (num <-4294967040.0) return print(F("ovf"));

	size_t len = 0;
	// Handle negative numbers
	if (num < 0.0) {
		len += write('-');
		num = -num;
	}
	// Round
	double round = 0.5;
	for (uint8_t idx=0; idx < p; idx++)
		round /= 10.0;
	num += round;
	// Print the integer part
	uint32_t int_num = (uint32_t) num;
	double 	 dec_num = num - int_num;
	len += print(int_num);
	// Print the decimal point
	if (p > 0) len += write('.');
	// Print the decimal part
	while (p-- > 0) {
		dec_num *= 10.0;
		uint8_t dec = (uint8_t) dec_num;
		len+= print(dec);
		dec_num -= dec;
	}

	// Return the number of printed characters
	return len;
}

//===============================================
size_t Stream::print(char c) {
	return write((uint8_t) c);
}

size_t Stream::print(const char* cstr) {
	return write(cstr);
}

size_t Stream::print(const String& str) {
	return write(str.begin(), str.get_length());
}

size_t Stream::print(const FlashString* pstr) {
	PGM_P str = reinterpret_cast<PGM_P>(pstr);
	size_t len = 0;
	while (true) {
		uint8_t c = pgm_read_byte(str++);
		if (c == 0) break;
		if (write(c)) len++;
		else break;
	}
	return len;
}

size_t Stream::print(uint32_t num, num_base_t base) {
	return print_uint(num, base);
}

size_t Stream::print(uint16_t num, num_base_t base) {
	return print_uint((uint32_t) num, base);
}

size_t Stream::print(uint8_t num, num_base_t base) {
	return print_uint((uint32_t) num, base);
}

size_t Stream::print(int32_t num, num_base_t base) {
	return print_int(num, base);
}

size_t Stream::print(int16_t num, num_base_t base) {
	return print_int((int32_t) num, base);
}

size_t Stream::print(int8_t num, num_base_t base) {
	return print_int((int32_t) num, base);
}

size_t Stream::print(double num, uint8_t p) {
	return print_double(num, p);
}

//###############################################
void Stream::set_timeout(uint32_t timeout) {
	_rx_timeout = timeout;
}

uint32_t Stream::get_timeout(void) {
	return _rx_timeout;
}

//===============================================
int16_t Stream::timed_peek(void) {
	int16_t byte;
	_rx_time_start = get_millis();

	// While time < timeout
	do {
		byte = peek();
		if (byte >= 0) return byte;
	} while ((get_millis() - _rx_time_start) < _rx_timeout);

	// If no bytes where available before the timeout
	return -1;
}

int16_t Stream::timed_read(void) {
	int16_t byte;
	_rx_time_start = get_millis();

	// While time < timeout
	do {
		byte = read();
		if (byte >= 0) return byte;
	} while((get_millis() - _rx_time_start) < _rx_timeout);

	// If no bytes where available before the timeout
	return -1;
}

//===============================================
int16_t Stream::peek_next_digit(stream_look_mode_t look_mode, bool is_decimal) {
	int16_t byte;

	while (true) {
		// Get the next byte (if any)
		byte = timed_peek();

		// If there's no byte available or the byte is numeric return the byte
		if (
			(byte < 0) ||						// No byte or timeout reached (error -1)
			(byte == '-') || 					// Negative value
			((byte >= '0') && (byte <= '9')) || // Numeric char
			(is_decimal && (byte == '.'))		// Decimal point
			)
			return byte;

		// If the method reach this point that means that the byte is non numeric.
		// This checks if the detected byte has to be skipped.
		switch (look_mode) {
			case SKIP_NONE:	return -1;	break;	// Returns that no numeric byte was detected.
			case SKIP_ALL: 				break;	// Just skip the byte (until a numeric byte is detected).
			case SKIP_WHITESPACE: {
				switch (byte) {
					case ' ':
					case '\t':
					case '\r':
					case '\n': 			break;	// If any of this bytes is detected, skip.
					default: return -1;	break;	// Return that no numeric byte was detected.
				}
			} break;
		}

		// Reading the buffer will "skip" the byte.
		read();
	}
}

//===============================================
int32_t Stream::parse_int(char ignore, stream_look_mode_t look_mode) {
	int16_t byte;
	int32_t value = 0;
	bool 	is_negative = false;

	// Get the first byte (ignore non numeric bytes)
	byte = peek_next_digit(look_mode, false);

	// If there's an error or timeout the value is returned as 0
	if (byte < 0) return 0;

	// Process the input
	do {
		// Handle the byte
		if (byte == ignore)
			;
		else if (byte == '-')
			is_negative = true;
		else if ((byte >= '0') && (byte <= '9'))
			value = (10 * value) + (byte - '0');
		// Consume the byte and peek the next one
		read();
		byte = timed_peek();
	} while (										// continue if..
			((byte >= '0') && (byte <= '9')) 	||	// The input is a number
			(byte == ignore)						// The input will be ignored
			);

	// Apply the sign and return
	if (is_negative)
		value = -value;
	return value;


}

double Stream::parse_double(char ignore, stream_look_mode_t look_mode) {
	int16_t byte;
	int32_t value = 0;
	double	fraction = 1.0;
	bool	is_fraction = false;
	bool 	is_negative = false;

	// Get the first byte (ignore non numeric bytes)
	byte = peek_next_digit(look_mode, true);

	// If there's an error or timeout the value is returned as 0
	if (byte < 0) return 0;

	// Process the input
	do {
		// Handle the byte
		if (byte == ignore)
			;
		else if (byte == '-')
			is_negative = true;
		else if (byte == '.')
			is_fraction = true;
		else if ((byte >= '0') && (byte <= '9')) {
			value = (10 * value) + (byte - '0');
			fraction *= 0.1;
		}
		// Consume the byte and peek the next one
		read();
		byte = timed_peek();
	} while (										// Continue if..
			((byte >= '0') && (byte <= '9')) 	||	// The input is a number
			(!is_fraction && (byte == '.')) 	||	// The input is a decimal point (only once)
			(byte == ignore)						// The input is the character to be ignored
			);

	// Apply the sign
	if (is_negative)
		value = -value;
	// Apply the fractionary part
	if (is_fraction)
		return fraction * value;
	return value;
}

//===============================================
size_t Stream::read_bytes(uint8_t* buff, size_t size) {
	int16_t byte;
	size_t 	bytes_read = 0;

	// Check size
	if (size == 0) return 0;

	// Get bytes if there's any error and the buffer has space left
	byte = timed_read();
	while ((bytes_read < size) && (byte >= 0)) {
		*buff++ = (uint8_t) byte;
		bytes_read++;
		byte = timed_read();
	}

	// Return the number of bytes read
	return bytes_read;
}

size_t Stream::read_bytes(char* cstr, size_t size) {
	return read_bytes((uint8_t*)cstr, size);
}

String Stream::read_string(void) {
	int16_t byte;
	String 	str;

	// Get bytes if there's any error
	byte = timed_read();
	while (byte >= 0) {
		str += (char) byte;
		byte = timed_read();
	}

	// Return a string
	return str;

}

size_t Stream::read_bytes_until(char terminator, uint8_t* buff, size_t size) {
	int16_t byte;
	size_t 	bytes_read = 0;

	// Check size
	if (size == 0) return 0;

	// Get bytes while there's any error, the buffer has space available
	// And the incoming byte is different to the terminator
	byte = timed_read();
	while ((bytes_read < size) && (byte >= 0) && (byte != terminator)) {
		*buff++ = (uint8_t) byte;
		bytes_read++;
		byte = timed_read();
	}

	// Return the number of bytes read
	return bytes_read;
}

size_t Stream::read_bytes_until(char terminator, char* cstr, size_t size) {
	return read_bytes_until(terminator, (uint8_t*)cstr, size);
}

String Stream::read_string_until(char terminator) {
	int16_t byte;
	String	str;

	// Get bytes if there's any error and the incoming byte is different
	// to the terminator
	byte = timed_read();
	while ((byte >= 0) && (byte != terminator)) {
		str += (char) byte;
		byte = timed_read();
	}

	// Return a string
	return str;
}

bool Stream::ignore_bytes_until(char terminator) {
	int16_t byte;
	byte = timed_peek();

	// Peek the next byte, if the next byte is the terminator or
	// there's no any more bytes, return
	while ((byte >= 0) && (byte != terminator)) {
		byte = read();
	}

	// If timed out or rx empty, false
	return !(byte < 0);

}

