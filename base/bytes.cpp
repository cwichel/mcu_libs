/**
 * @file 	libs/base/bytes.cpp
 * @brief	Byte manipulation library.
 */
//###############################################
#include <base/base.h>
//###############################################
uint16_t bswap16(uint16_t val) {
	return (
		((val & 0x00FF) << 8) |
		((val & 0xFF00) >> 8)
	);
}

u16_u8_t break_uint16(uint16_t val) {
	bk_u16_u8_t bk16;
	bk16.u16 = val;
	return bk16.u8;
}

uint16_t make_uint16(int16_t val) {
	return (uint16_t) val;
}

uint16_t make_uint16(uint16_t val) {
	return val;
}

uint16_t make_uint16(u16_u8_t val) {
	bk_u16_u8_t mk16;
	mk16.u8 = val;
	return mk16.u16;
}

uint16_t make_uint16(uint8_t msb, uint8_t lsb) {
	return MAKE_U16(msb, lsb);
}

//===============================================
uint32_t bswap32(uint32_t val) {
	return (
		((val & 0xFF000000) >> 24) |
		((val & 0x00FF0000) >> 8)  |
		((val & 0x0000FF00) << 8)  |
		((val & 0x000000FF) << 24)
	);
}

u32_u8_t break_uint32(uint32_t val) {
	bk_u32_u8_t bk32;
	bk32.u32 = val;
	return bk32.u8;
}

uint32_t make_uint32(int32_t val) {
	return (uint32_t) val;
}

uint32_t make_uint32(uint32_t val) {
	return val;
}

uint32_t make_uint32(u32_u8_t value) {
	bk_u32_u8_t mk32;
	mk32.u8 = value;
	return mk32.u32;
}

uint32_t make_uint32(uint8_t msb, uint8_t b2, uint8_t b1, uint8_t lsb) {
	return MAKE_U32(msb, b2, b1, lsb);
}

//===============================================
uint8_t get_field_mask(uint8_t pos, uint8_t len) {
	return (((1 << len) - 1) << pos);
}

uint8_t get_field_value(uint8_t reg, uint8_t pos, uint8_t len) {
	return ((reg >> pos) & ((1 << len) - 1));
}

uint8_t set_field_value(uint8_t reg, uint8_t pos, uint8_t len, uint8_t value) {
	return ((reg &~ get_field_mask(pos, len)) | ((value & ((1 << len) - 1)) << pos));
}





