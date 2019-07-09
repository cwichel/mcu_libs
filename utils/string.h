/**
 * @code	#include "utils/string.h" @endcode
 * @file 	libs/utils/string.h
 * @brief 	Strings handling library.
 */
//###############################################
#ifndef _UTILS_STRING_H_
#define _UTILS_STRING_H_
//###############################################
#include <utils/string_defs.h>
//###############################################
/**
 * @class 	FlashString
 * @brief	Used as handler to save strings on flash. Use with the following
 * 			gcc parameters to improve performance and memory efficiency:
 * 				-felide-constructors
 *    			-std=c++0x
 */
class FlashString;
#define F(str) 		(reinterpret_cast<const FlashString*>(PSTR(str)))

//###############################################
/**
 * @class 	String
 * @brief	String handling implementation.
 */
class String {
private:
    //======================================
	/**
	 * @brief	This type is used to create a method to associate a boolean
	 * 			value to the string object. See more on:
	 * 			http://www.artima.com/cppsource/safebool.html
	 */
    typedef void (String::*if_bool_type)() const;

    /**
     * @brief	Used to apply the safe bool method.
     */
    void if_bool(void) const {}

public:
    //======================================
    /**
     * @brief	String object constructor. This allows to initialize
     * 			an object with a char array.
     * @param	cstr		Char array string.
     */
    String(const char* cstr = "");

    /**
     * @brief	String object constructor. This allows to initialize
     * 			an object copying the contents of another object.
     * @param	str			String object.
     */
    String(const String& str);

    /**
	 * @brief	String object constructor. This allows to initialize
	 * 			an object with a progmem space string.
	 * @param	pstr		Progmem string.
	 */
	String(const FlashString* pstr);

    /**
     * @brief	String object constructor. Used for any input type.
     * @param	value		Input value.
     */
    template <typename T>
    explicit String(T value);

    //======================================
    /**
     * @brief	Returns the capacity of the string array.
     * @return	String capacity.
     */
    inline uint16_t get_capacity(void)  const   {return _cap;}

    /**
	 * @brief	Returns the length of the string array.
	 * @return	String length.
	 */
    inline uint16_t get_length(void)    const   {return _len;}

    /**
     * @brief	This method is used to increase the size of the string if needed.
     * @param	size		String desired length.
     * @return	True if success, False otherwise.
     */
    bool reserve(size_t size);

    /**
     * @brief	Resize the string to any size.
     * @param	size		String desired length.
     * @return	True if success, False otherwise.
     */
    bool resize(size_t size);

    //======================================
    /**
     * @brief	Set the given character 'c' on the position 'idx' of the
     * 			string.
     * @param	idx			Position to modify.
     * @param	c			Character to write.
     */
    void set_char(size_t idx, char c);

    /**
     * @brief	Get a given character from the position 'fromidx' of the
     * 			string.
     * @param	fromidx		Position from where the character is retrieved.
     */
    char get_char(size_t fromidx) const;

    /**
	 * @brief	Used to retrieve a character array from the string.
	 * @copydetails String::get_bytes()
	 */
    void get_chars(char* buff, size_t size, size_t fromidx);

    /**
	 * @brief	Retrieve a byte array from the string.
	 * @param	buff		Pointer to byte array.
	 * @param	size		Number of bytes to retrieve.
	 * @param	fromidx		Position to begin the extraction.
	 */
    void get_bytes(uint8_t* buff, size_t size, size_t fromidx);

    /**
     * @brief	Retrieve the address of the first character in the
     * 			string.
     * @return	Address of the first string.
     */
    char* begin(void);

    /**
	 * @brief	Retrieve the address of the last character in the
	 * 			string.
	 * @return	Address of the last string.
	 */
    char* end(void);

    /** @copydoc String::begin() */
    const char* begin(void) const;

    /** @copydoc String::end() */
    const char* end(void) const;

    //======================================
    /**
	 * @brief	Used to concatenate a char array.
	 * @param	cstr		Character array.
	 * @return	True if success, False otherwise.
	 */
	bool concat(const char* cstr);

    /**
     * @brief	Used to concatenate another string object.
     * @param	str			String object.
     * @return	True if success, False otherwise.
     */
    bool concat(const String& str);

    /**
     * @brief	Used to concatenate a progmem string.
     * @param 	pstr		Progmem string.
     * @return	True if success, False otherwise.
     */
    bool concat(const FlashString* pstr);

    /**
	 * @brief	Used to concatenate a single character.
	 * @param	c			Character.
	 * @return	True if success, False otherwise.
	 */
    bool concat(char c);

    /**
	 * @brief	Used to concatenate a given number.
	 * @param	num			Value.
	 * @param	base		Numeric base to perform conversion.
	 * @return	True if success, False otherwise.
	 */
    bool concat(uint32_t num, num_base_t base=NUM_BASE_DEC);

    /** @copydoc String::concat(uint32_t, num_base_t) */
    bool concat(uint16_t num, num_base_t base=NUM_BASE_DEC);

    /** @copydoc String::concat(uint32_t, num_base_t) */
    bool concat(uint8_t num, num_base_t base=NUM_BASE_DEC);

    /** @copydoc String::concat(uint32_t, num_base_t) */
    bool concat(int32_t num, num_base_t base=NUM_BASE_DEC);

    /** @copydoc String::concat(uint32_t, num_base_t) */
    bool concat(int16_t num, num_base_t base=NUM_BASE_DEC);

    /** @copydoc String::concat(uint32_t, num_base_t) */
    bool concat(int8_t num, num_base_t base=NUM_BASE_DEC);

    /**
     * @brief	Used to concatenate a double value.
     * @param	num			Value.
     * @param	p			Precision.
	 * @return	True if success, False otherwise.
     */
    bool concat(double num, uint8_t p=3);

    //======================================
    /**
     * @brief	Replace a given character on the string.
     * @param	find		Character to be replaced.
     * @param	replace		Character replacement.
     */
    void replace(char find, char replace);

    /**
	 * @brief	Replace a given substring on the string.
	 * @param	find		substring to be replaced.
	 * @param	replace		substring replacement.
	 */
    void replace(const String& find, const String& replace);

    /**
     * @brief	Remove a single character from the string. The position of the
     * 			character is given by the index 'idx'.
     * @param	idx			Character index.
     */
    void remove(size_t idx);

    /**
     * @brief	Return 'size' number of characters from the string starting
     * 			from the index 'idx'.
     * @param	idx			Starting index.
     * @param	size		Number of characters to be removed.
     */
    void remove(size_t idx, size_t size);

    /**
     * @brief	Upper case all the characters on the string.
     */
    void to_upper_case(void);

    /**
	 * @brief	Lower case all the characters on the string.
	 */
    void to_lower_case(void);

    /**
	 * @brief	Remove all the white space characters from the beginning and
	 * 			end of the string.
	 */
    void trim(void);

    //======================================
    /**
     * @brief	Tries to convert the string into a integer value.
     * @return	Integer value.
     */
    int32_t to_int(void);

    /**
	* @brief	Tries to convert the string into a double value.
	* @return	Double value.
	*/
    double to_double(void);

    //======================================
    /**
     * @brief	Search for a character on the string and return the index
     * 			where it's first found (if exists).
     * @param	c			Character to be found.
     * @return	Index in which the character is found. If the character is
     * 			not found, returns -1.
     */
    int16_t index_of(char c) const;

    /**
	 * @brief	Search for a character on the string and return the index
	 * 			where it's first found (if exists).
	 * @param	c			Character to be found.
	 * @param	fromidx		Index from the search is started.
	 * @return	Index in which the character is found. If the character is
	 * 			not found, returns -1.
	 */
    int16_t index_of(char c, size_t fromidx) const;

    /**
	 * @brief	Search for a substring on the string and return the index
	 * 			where it's first found (if exists).
	 * @param	str			String to be found.
	 * @return	Index in which the substring is found. If the substring is
	 * 			not found, returns -1.
	 */
    int16_t index_of(const String& str) const;

    /**
   	 * @brief	Search for a substring on the string and return the index
   	 * 			where it's first found (if exists).
   	 * @param	str			String to be found.
   	 * @param	fromidx		Index from the search is started.
   	 * @return	Index in which the substring is found. If the substring is
   	 * 			not found, returns -1.
   	 */
    int16_t index_of(const String& str, size_t fromidx) const;

    /**
     * @brief	Same as String::index_of(char)const but backwards.
     * @copydetails  String::index_of(char)const
     */
    int16_t last_index_of(char c) const;

    /**
	 * @brief	Same as String::index_of(char, size_t)const but backwards.
	 * @copydetails  String::index_of(char, size_t)const
	 */
    int16_t last_index_of(char c, size_t fromidx) const;

    /**
	 * @brief	Same as String::index_of(const String&)const but backwards.
	 * @copydetails  String::index_of(const String&)const
	 */
    int16_t last_index_of(const String& str) const;

    /**
	 * @brief	Same as String::index_of(const String&, size_t)const but backwards.
	 * @copydetails  String::index_of(const String&, size_t)const
	 */
    int16_t last_index_of(const String& str, size_t fromidx) const;

    /**
     * @brief	Similar to String::get_substring(uint16_t, uint16_t)const
     * 			but takes all the characters from 'fromidx' to the end of the
     * 			string.
     * @param	fromidx		Index of the first character of the substring.
     * @return	Desired substring.
     */
    String get_substring(size_t fromidx) const;

    /**
     * @brief	Get a substring from the string starting from 'fromidx'
     * 			and ending on 'toidx'.
     * @param	fromidx		Index of the first character of the substring.
     * @param	toidx		Index of the last character of the substring.
     * @return	Desired substring.
     */
    String get_substring(size_t fromidx, size_t toidx) const;

    //======================================
    /**
     * @brief	Compare 2 strings.
     * @param	str			String to be compared to.
     * @return	Comparison result.
     */
    int16_t compare_to(const String& str) const;

    /**
	 * @brief	Check if the string is equal to a given substring.
	 * @param	str			String to be compared to.
	 * @return	True if matched, False otherwise.
	 */
    bool is_equal_to(const String& str) const;

    /**
	 * @brief	Check if the string is equal to a given character array.
	 * @param	cstr		Character array to be compared to.
	 * @return	True if matched, False otherwise.
	 */
    bool is_equal_to(const char* cstr) const;

    /**
   	 * @brief	Check if the string starts with a given substring.
   	 * @param	str			String to be compared to.
   	 * @return	True if matched, False otherwise.
   	 */
    bool start_with(const String& str) const;

    /**
	 * @brief	Check if the string starts with a given substring.
	 * @param	str			String to be compared to.
	 * @param	fromidx		Index from which the comparison will be done.
	 * @return	True if matched, False otherwise.
	 */
    bool start_with(const String& str, size_t fromidx) const;

    /**
     * @brief	Check if the string ends with a given substring.
     * @param	str			String to be compared to.
     * @return	True if matched, False otherwise.
     */
    bool ends_with(const String& str) const;

    //======================================
    /**
     * @brief	Safe bool method implementation.
     */
    operator if_bool_type(void) const { return (_buff) ? &String::if_bool : NULL; }

    String& operator =  (const char* cstr);
    String& operator =  (const String& str);
    String& operator = 	(const FlashString* pstr);

    template <typename T>
    String& operator += (T value);

    template <typename T>
    String& operator +  (T value);

    bool    operator == (const String& str) const;
    bool    operator == (const char* cstr) const;
    bool    operator != (const String& str) const;
    bool    operator != (const char* cstr) const;
    bool    operator <  (const String& str) const;
    bool    operator >  (const String& str) const;
    bool    operator <= (const String& str) const;
    bool    operator >= (const String& str) const;

    char    operator [] (uint16_t fromidx) const;
    char&   operator [] (uint16_t fromidx);

protected:
    char*		_buff;		//!< The actual string.
    uint16_t	_len;       //!< The string length (Not counting the final '\0').
    uint16_t	_cap;       //!< The array length minus one (reserve for '\0').

    //======================================
    /**
     * @brief	Change the size of the string buffer.
     * @param	size		Desired size of the buffer.
     * @return	True if success, False otherwise.
     */
    bool buffer_change(size_t size);

    /**
     * @brief	Delete the contents of the string buffer and reset the length and
     * 			capacity to 0.
     */
    void buffer_delete(void);

    /**
     * @brief	Reset the buffer, it's length and capacity.
     */
    void buffer_init(void);

    //======================================
    /**
     * @brief	Add a character array 'cstr' with size 'size' to the string.
     * @param	cstr		Character array pointer.
     * @param	size		Size of the character array.
     * @return	True if success, False otherwise.
     */
    bool concat(const char* cstr, size_t size);

    //======================================
    /**
     * @brief	Tries to copy a character array into the string buffer. If it's
     * 			not possible, reset the buffer.
     * @param	cstr		Character array pointer.
     * @param	size		Size of the character array.
     * @return	Modified string.
     */
    String& copy(const char* cstr, size_t size);

    /**
	 * @brief	Tries to copy a progmem character array into the string buffer. If
	 * 			it's not possible, reset the buffer.
	 * @param	pstr		Progmem string.
	 * @param	size		Size of the string.
	 * @return	Modified string.
	 */
	String& copy(const FlashString* pstr, uint16_t size);

    /**
     * @brief	Tries to move the contents of 'str' to the string, after this
     * 			deletes 'str'.
     * @param	str			String to be moved.
     */
    void move(String& str);
};

//###############################################
template <typename T>
String::String(T value) {
    buffer_init();
    concat(value);
}

template <typename T>
String& String::operator += (T value) {
    concat(value);
    return *this;
}

template <typename T>
String& String::operator +  (T value) {
    String tmp(value);
    concat(tmp);
    return *this;
}

//###############################################
#endif /* _UTILS_STRING_H_ */
//###############################################
