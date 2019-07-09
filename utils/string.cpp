/**
 * @file 	libs/utils/string.cpp
 * @brief 	Strings handling library.
 */
//###############################################
#include <utils/string.h>
//###############################################
/************************************************
 * Constructors
 ***********************************************/
String::String(const char* cstr) {
    buffer_init();
    if (cstr) copy(cstr, strlen(cstr));
}

String::String(const String& str) {
    buffer_init();
    *this = str;
}

String::String(const FlashString* pstr) {
	buffer_init();
	*this = pstr;
}

/************************************************
 * Memory Management
 ***********************************************/
bool String::reserve(size_t size) {
    // If the buffer is bigger than needed, all OK!
    if (_buff && (_cap >= size)) return true;
    // If requires resize and the operation is succesful, all OK!
    return resize(size);
}

bool String::resize(size_t size) {
	// If the resize fails...
	if (!buffer_change(size)) return false;
	// Otherwise...
	if (_len > _cap) {
		_len = _cap;
		_buff[_len + 1] = '\0';
	} else if (_len == 0) {
		_buff[0] = '\0';
	}
	return true;
}

bool String::buffer_change(size_t size) {
    // Allocate space for the 'size' chars and the terminator
    char* new_buff = (char*)realloc(_buff, size + 1);
    // If the operation goes wrong...
    if (!new_buff) return false;
    // Otherwise...
	_buff = new_buff;
	_cap = size;
	return true;
}

void String::buffer_delete(void) {
    // Free the used memory
    if (_buff) free(_buff);
    // Reset the string
    buffer_init();
}

void String::buffer_init(void) {
    // Set the default contents
    _buff = NULL;		// String empty.
    _cap = 0;			// No space reserved.
    _len = 0;			// No data on string.
}

/************************************************
 * Data Access
 ***********************************************/
void String::set_char(size_t idx, char c) {
    if (idx < _len) _buff[idx] = c;
    if ((idx > _len) && (idx < _cap)) {
        _buff[_len + 1] = c;
        _buff[_len + 2] = '\0';
    }
}

char String::get_char(size_t fromidx) const {
    return operator[](fromidx);
}

void String::get_chars(char* buff, size_t size, size_t fromidx) {
    get_bytes((uint8_t*) buff, size, fromidx);
}

void String::get_bytes(uint8_t* buff, size_t size, size_t fromidx) {
    // Check if the buffer and size are ok...
    if (!buff || (size == 0)) return;
    // If the start point is after the last character...
    if (fromidx > _len) {
        buff[0] = '\0';
        return;
    }
    // If all is OK
    uint16_t num = size - 1;
    if (num > (_len - fromidx))
        num = _len - fromidx;
    strncpy((char*) buff, _buff + fromidx, num);
    buff[num] = '\0';
}

char* String::begin(void) {
    return _buff;
}

char* String::end(void) {
    return _buff + _len;
}

const char* String::begin(void) const {
    return _buff;
}

const char* String::end(void) const {
    return begin() + get_length();
}

/************************************************
 * Concatenate elements
 ***********************************************/
bool String::concat(const char* cstr, size_t size) {
    // If there's nothing to add, end operation with error.
    if (!cstr)          return false;
    // If the size is 0, end operation OK.
    if (size == 0)      return true;
    // If there's problems with the resize, end with error.
    uint16_t new_len = _len + size;
    if (!reserve(new_len)) return false;
    // Concatenate
    strcpy(_buff + _len, cstr);
    _len = new_len;
    return true;
}

bool String::concat(const char* cstr) {
    return concat(cstr, strlen(cstr));
}

bool String::concat(const String& str) {
    return concat(str._buff, str._len);
}

bool String::concat(const FlashString* pstr) {
	uint16_t size = strlen_P((const char*)pstr);
	// If there's nothing to add, end operation with error.
	if (!pstr)          return false;
	// If the size is 0, end operation OK.
	if (size == 0)      return true;
	// If there's problems with the resize, end with error.
	uint16_t new_len = _len + size;
	if (!reserve(new_len)) return false;
	// Concatenate
	strcpy_P(_buff + _len, (const char*)pstr);
	_len = new_len;
	return true;
}

bool String::concat(char c) {
    char tmp[] = {c, '\0'};
    return concat(tmp, 1);
}

bool String::concat(uint32_t num, num_base_t base) {
    // U32 max: 4.294.967.295 => 12 digits (+ null)
    char tmp[12];
    ultoa(num, tmp, base);
    return concat(tmp, strlen(tmp));
}

bool String::concat(uint16_t num, num_base_t base) {
    // U16 max: 65.535 => 8 digits (+ null)
    char tmp[8];
    utoa(num, tmp, base);
    return concat(tmp, strlen(tmp));
}

bool String::concat(uint8_t num, num_base_t base) {
    // U8 max: 255 => 5 digits(+ null)
    char tmp[5];
    utoa(num, tmp, base);
    return concat(tmp, strlen(tmp));
}

bool String::concat(int32_t num, num_base_t base) {
    // I32 max: 4.294.967.295 => 12 digits (+ null + sign)
    char tmp[12];
    ltoa(num, tmp, base);
    return concat(tmp, strlen(tmp));
}

bool String::concat(int16_t num, num_base_t base) {
    // I16 max: 65.535 => 8 digits (+ null + sign)
    char tmp[8];
    itoa(num, tmp, base);
    return concat(tmp, strlen(tmp));
}

bool String::concat(int8_t num, num_base_t base) {
    // I8 max: 255 => 5 digits (+ null + sign)
    char tmp[5];
    itoa(num, tmp, base);
    return concat(tmp, strlen(tmp));
}

bool String::concat(double num, uint8_t p) {
    char tmp[15];
    char* str = dtostrf(num, 4, p, tmp);
    return concat(str, strlen(str));
}

/************************************************
 * Modify string elements
 ***********************************************/
void String::replace(char find, char replace) {
    // If there's not data, return
    if (!_buff) return;
    // Else, replace
    for (char* cp = _buff; *cp; cp++) {
        if (*cp == find) *cp = replace;
    }
}

void String::replace(const String& find, const String& replace) {
    // If the target buffer or the word to find are empty, do nothing
    if ((_len == 0) || (find._len == 0)) return;
    // Otherwise, find and replace
    int16_t str_diff = replace._len - find._len;
    char* source = _buff;
    char* found_at;
    if (str_diff == 0) {
        // If the old and new values have the same size...
        while ((found_at = strstr(source, find._buff)) != NULL) {
            memcpy(found_at, replace._buff, replace._len);
            source = found_at + replace._len;
        }
    } else if (str_diff < 0) {
        // If the new value is smaller than the old one...
        char* output = _buff;
        while ((found_at = strstr(source, find._buff)) != NULL) {
            uint16_t pos = found_at - source;
            memcpy(output, source, pos);
            output += pos;
            memcpy(output, replace._buff, replace._len);
            output += replace._len;
            source += pos + find._len;
            _len += str_diff;
        }
        strcpy(output, source);
    } else {
        // If the new value is bigger than the old one...
        uint16_t new_len = _len;
        while ((found_at = strstr(source, find._buff)) != NULL) {
            source = found_at + find._len;
            new_len += str_diff;
        }
        if (new_len == _len) return;
        if ((new_len > _cap) && !buffer_change(new_len)) return;
        int16_t index = _len - 1;
        while ((index > 0) && ((index = last_index_of(find, index)) >= 0)) {
            source = _buff + index + find._len;
            memmove(source + str_diff, source, _len - (source - _buff));
            memcpy(_buff + index, replace._buff, replace._len);
            _len += str_diff;
            _buff[_len] = '\0';
            index--;
        }
    }
}

void String::remove(size_t idx) {
    remove(idx, (uint16_t)-1);
}

void String::remove(size_t idx, size_t size) {
    // If no data is selected, return
    if (size == 0) return;
    // If the index exceeds the length, return
    if (idx > _len) return;
    // Ensure the size is OK
    if (size > (_len - idx)) {
        _len = idx;
    } else {
        char* output = _buff + idx;
        memcpy(output, output + size, _len - (idx + size));
        _len -= size;
    }
    _buff[_len] = '\0';
}

void String::to_upper_case(void) {
    // If there's not data, return
    if (!_buff) return;
    // Else, replace
    for (char* cp = _buff; *cp; cp++) {
        *cp = toupper(*cp);
    }
}

void String::to_lower_case(void) {
    // If there's not data, return
    if (!_buff) return;
    // Else, replace
    for (char* cp = _buff; *cp; cp++) {
        *cp = tolower(*cp);
    }
}

void String::trim(void) {
    // If the buffer is not available, return
    if (!_buff || (_len == 0)) return;
    // Else, clean all the spaces (in the beginning and end)
    char* begin = _buff;
    while (isspace(*begin)) begin++;
    char* end = _buff + _len - 1;
    while (isspace(*end) && (end >= begin)) end--;
    _len = end + 1 - begin;
    if (begin > _buff) memcpy(_buff, begin, _len);
    _buff[_len] = '\0';
}

/************************************************
 * Numeric parsing/conversions
 ***********************************************/
int32_t String::to_int(void) {
    if (!_buff) return 0;
    return atol(_buff);
}

double String::to_double(void) {
    if (!_buff) return 0;
    return atof(_buff);
}

/************************************************
 * Search substrings
 ***********************************************/
int16_t String::index_of(char c) const {
    return index_of(c, 0);
}

int16_t String::index_of(char c, size_t fromidx) const {
    // Check if the index is OK...
    if (fromidx >= _len) return -1;
    // Search
    const char* tmp = strchr(_buff + fromidx, c);
    if (tmp == NULL) return -1;
    return (tmp - _buff);
}

int16_t String::index_of(const String &str) const {
    return index_of(str, 0);
}

int16_t String::index_of(const String &str, size_t fromidx) const {
    // Check if the index is OK...
    if (fromidx >= _len) return -1;
    // Search
    const char* tmp = strstr(_buff + fromidx, str._buff);
    if (tmp == NULL) return -1;
    return (tmp - _buff);
}

int16_t String::last_index_of(char c) const {
    return last_index_of(c, _len - 1);
}

int16_t String::last_index_of(char c, size_t fromidx) const {
    // Check if index is OK...
    if (fromidx >= _len) return -1;
    // Search
    const char* tmp = strchr(_buff + fromidx, c);
    if (tmp == NULL) return -1;
    return (tmp - _buff);
}

int16_t String::last_index_of(const String &str) const {
    return last_index_of(str, _len - str._len);
}

int16_t String::last_index_of(const String &str, size_t fromidx) const {
    // Check if indexes are OK...
    if ((str._len == 0) || (str._len > _len) || (_len == 0)) return -1;
    // Search
    int16_t found = -1;
    if (fromidx >= _len) fromidx = _len - 1;
    for (char* p = _buff; p <= _buff + fromidx; p++) {
        p = strstr(p, str._buff);
        if (!p) break;
        if ((uint16_t)(p - _buff) <= fromidx) found = (p - _buff);
    }
    return found;
}

String String::get_substring(size_t fromidx) const {
    return get_substring(fromidx, _len);
}

String String::get_substring(size_t fromidx, size_t toidx) const {
    String out;
    // Ensure that from < to
    if (fromidx > toidx) {
        uint16_t tmp = fromidx;
        fromidx = toidx;
        toidx = tmp;
    }
    // Check limits
    if (fromidx > _len) return out;
    if (toidx > _len)   toidx = _len;
    // Get substring
    char tmp = _buff[toidx];
    _buff[toidx] = '\0';
    out = _buff + fromidx;
    _buff[toidx] = tmp;
    return out;
}

/************************************************
 * Comparison
 ***********************************************/
int16_t String::compare_to(const String& str) const {
    // If any of the strings is empty, return the firs
    if (!_buff || !str._buff)   {
        if (str._buff && (str._len > 0)) return 0 - *(uint8_t*)str._buff;
        if (_buff && (_len > 0))         return *(uint8_t*)_buff;
        return 0;
    }
    return strcmp(_buff, str._buff);
}

bool String::is_equal_to(const String& str) const {
    return ((_len == str._len) && (compare_to(str) == 0));
}

bool String::is_equal_to(const char* cstr) const {
    if (_len == 0)      return ((cstr == NULL) || (*cstr == 0));
    if (cstr == NULL)   return (_buff[0] == '\0');
    return (strcmp(_buff, cstr) == 0);
}

bool String::start_with(const String& str) const {
    return start_with(str, 0);
}

bool String::start_with(const String& str, size_t fromidx) const {
    if ((fromidx > _len) || !_buff || !str._buff) return false;
    return (strncmp((_buff + fromidx), str._buff, str._len) == 0);
}

bool String::ends_with(const String& str) const {
    if ((_len < str._len) || !_buff || !str._buff) return false;
    return (strcmp((end() - str._len), str._buff) == 0);
}

/************************************************
 * Copy and Move
 ***********************************************/
String& String::copy(const char* cstr, size_t size) {
    // If it's possible to reserve some space, copy the contents
    if (reserve(size)) {
        strcpy(_buff, cstr);
        _len = size;
        return *this;
    }
    // If not, empty
    buffer_delete();
    return *this;
}

String& String::copy(const FlashString* pstr, uint16_t size) {
	// If it's possible to reserve some space, copy the contents
	if (reserve(size)) {
		_len = size;
		strcpy_P(_buff, (PGM_P)pstr);
		return *this;
	}
	// If not, empty
	buffer_delete();
	return *this;
}

void String::move(String& str) {
    if (_buff) {
        if (str && (_cap >= str._len)) {
            strcpy(_buff, str._buff);
            _len = str._len;
            str.buffer_delete();
            return;
        } else {
            free(_buff);
        }
    }
    _buff = str._buff;
    _cap = str._cap;
    _len = str._len;
    str.buffer_delete();
}

/************************************************
 * Operators
 ***********************************************/
String& String::operator =  (const char* cstr) {
	// If there's something to copy...
    if (cstr) copy(cstr, strlen(cstr));
    // In any other case, empty the string...
    else buffer_delete();
    return *this;
}

String& String::operator =  (const String& str) {
    // If is the same object...
    if (this == &str) return *this;
    // If there's something to copy...
    if (str._buff) copy(str._buff, str._len);
    // In any other case, empty the string...
    else buffer_delete();
    return *this;
}

String& String::operator =	(const FlashString* pstr) {
	// If there's something to copy...
	if (pstr) copy(pstr, strlen_P((PGM_P)pstr));
	// In any other case, empty the string...
	else buffer_delete();
	return *this;
}

bool  String::operator == (const String& str) const {
    return is_equal_to(str);
}

bool  String::operator == (const char* cstr) const {
    return is_equal_to(cstr);
}

bool  String::operator != (const String& str) const {
    return !is_equal_to(str);
}

bool  String::operator != (const char* cstr) const {
    return !is_equal_to(cstr);
}

bool  String::operator <  (const String& str) const {
    return (compare_to(str) < 0);
}

bool  String::operator >  (const String& str) const {
    return (compare_to(str) > 0);
}

bool  String::operator <= (const String& str) const {
    return (compare_to(str) <= 0);
}

bool  String::operator >= (const String& str) const {
    return (compare_to(str) >= 0);
}

char String::operator[] (uint16_t fromidx) const {
    // If is not initialized or the index is out of bounds...
    if (!_buff || (fromidx > _len)) return 0;
    // If nothing goes wrong...
    return _buff[fromidx];
}

char& String::operator[] (uint16_t fromidx) {
    // If is not initialized or the index is out of bounds...
    if (!_buff || (fromidx > _len)) {
        static char dummy = 0;
        return dummy;
    }
    // If nothing goes wrong...
    return _buff[fromidx];
}
