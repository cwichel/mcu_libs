/**
 * @code	#include "base/base.h" @endcode
 * @file 	libs/base/base.h
 * @brief 	Basic definitions.
 */
//###############################################
#ifndef _BASE_BASE_H_
#define _BASE_BASE_H_
//###############################################
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
}
#include <base/fix_cpp.h>

//###############################################
//###############################################
#if !defined(_BV)
#define _BV(x)						(1 << (x))
#endif

#define LSB_U16(x)					((x) & 0x00FF)

#define MSB_U16(x)					LSB_U16((x) >> 8)

#define BYTES_U16(x) 				{LSB_U16(x), MSB_U16(x)}

#define MAKE_U16(msb, lsb) \
	((uint16_t)msb << 8) |	\
	(uint16_t) lsb

#define LSB_U32(x)					((x) & 0x000000FF)

#define B1_U32(x)					LSB_U32((x) >> 8)

#define B2_U32(x)					LSB_U32((x) >> 16)

#define MSB_U32(x)					LSB_U32((x) >> 24)

#define BYTES_U32(x) 				{LSB_U16(x), B1_U32(x), B2_U32(x), MSB_U16(x)}

#define MAKE_U32(msb, b2, b1, lsb) \
	((uint32_t)msb << 24) |	\
	((uint32_t)b2 << 16)  |	\
	((uint32_t)b1 << 8)   | \
	(uint32_t)lsb

//===============================================
/**
 * @struct u16_u8_t
 * @brief Used to represent a 16bit value with 8bit values.
 */
typedef struct {
	uint8_t lsb;			//!< LSB byte.
	uint8_t msb;			//!< MSB byte.
} u16_u8_t;

/**
 * @struct u32_u8_t
 * @brief Used to represent a 32bit value with 8bit values.
 */
typedef struct {
	uint8_t lsb;			//!< LSB byte.
	uint8_t b1;				//!< Mid LSB byte.
	uint8_t b2;				//!< Mid MSB byte.
	uint8_t msb;			//!< MSB byte.
} u32_u8_t;

/**
 * @union bk_u16_u8_t
 * @brief Simplify conversions between 16bit and 8bit values.
 */
typedef union {
	u16_u8_t u8;			//!< u16_u8_t value
	uint16_t u16;			//!< 16bit value
} bk_u16_u8_t;

/**
 * @union bk_u32_u8_t
 * @brief Simplify conversions between 32bit and 8bit values.
 */
typedef union {
	u32_u8_t u8;			//!< u32_u8_t value
	uint32_t u32;			//!< 32bit value
} bk_u32_u8_t;

//===============================================
/**
 * @brief	Return the bytes inverted, for example 0xaabb becomes 0xbbaa.
 * @param	val				16bit value.
 * @return 	Bytes inverted.
 */
extern uint16_t bswap16(uint16_t val);

/**
 * @brief	Breaks a 16bit value into 8bit values.
 * @param 	val				16bit value.
 * @return	8bit struct.
 */
extern u16_u8_t break_uint16(uint16_t val);

extern uint16_t make_uint16(int16_t val);
extern uint16_t make_uint16(uint16_t val);
extern uint16_t make_uint16(u16_u8_t val);
extern uint16_t make_uint16(uint8_t msb, uint8_t lsb);

/**
 * @brief	Similar to bswap16() but for 32bit.
 * @param	val				32bit value.
 * @return	Bytes inverted.
 */
extern uint32_t bswap32(uint32_t val);

/**
 * @brief	Similar to break_uint16() but for 32bit.
 * @param	val				32bit value.
 * @return 	8bit struct.
 */
extern u32_u8_t break_uint32(uint32_t val);

extern uint32_t make_uint32(int32_t val);
extern uint16_t make_uint16(uint32_t val);
extern uint32_t make_uint32(u32_u8_t val);
extern uint32_t make_uint32(uint8_t msb, uint8_t b2, uint8_t b1, uint8_t lsb);

//===============================================
/**
 * @brief 	Get the field mask.
 * @param	pos				Position of the field (lsb).
 * @param	len  			Length of the field (bits).
 * @return	Field mask.
 */
extern uint8_t get_field_mask(uint8_t pos, uint8_t len);

/**
 * @brief 	Get a field value from a register.
 * @param	reg				Target register.
 * @param	pos				Position of the field (lsb).
 * @param	len 			Length of the field (bits).
 * @return	Field value.
 */
extern uint8_t get_field_value(uint8_t reg, uint8_t pos, uint8_t len);

/**
 * @brief 	Set a field value on a register.
 * @param	reg				Target register.
 * @param	pos				Position of the field (lsb).
 * @param	len 			Length of the field (bits).
 * @param	value			Field value.
 * @return	Modified register.
 */
extern uint8_t set_field_value(uint8_t reg, uint8_t pos, uint8_t len, uint8_t value);

//###############################################
//###############################################
/** Pointer to function (input: void, output: void). */
typedef void (*fun_ptr_vd_vd)(void);

/** Pointer to function (input: uint8_t, output: void). */
typedef void (*fun_ptr_vd_u8)(uint8_t);

/** Pointer to function (input: uint8_t, output: uint8_t). */
typedef uint8_t (*fun_ptr_u8_u8)(uint8_t);

//===============================================
/**
 * @brief	Dummy function, does nothing. Used to initialize fun_ptr_vd_vd
 * 			pointers.
 */
extern void fun_vd_vd_dummy(void);

/**
 * @brief	Dummy function, does nothing. Used to initialize fun_ptr_vd_u8
 * 			pointers.
 * @param	value
 */
extern void fun_vd_u8_dummy(uint8_t value);

/**
 * @brief	Dummy function, does nothing. Used to initialize fun_ptr_u8_u8
 * 			pointers.
 * @param	value
 * @return 	0x00.
 */
extern uint8_t fun_u8_u8_dummy(uint8_t value);

//###############################################
//###############################################
#define PI			 3.14159265358979323846
#define RAD_2_DEG	57.29577951308232087679
#define DEG_2_RAD	 0.01745329251994329576

#if defined(abs)
#undef abs
#endif

#define RAD(deg)	((deg) * DEG_2_RAD)

#define DEG(rad)	((rad) * RAD_2_DEG)

#define min(a, b)	(((a)<(b))?(a):(b))

#define max(a, b)	(((a)<(b))?(a):(b))

#define abs(x)		(((x)>=0)?(x):-(x))

//###############################################
#endif /* _BASE_BASE_H_ */
//###############################################
