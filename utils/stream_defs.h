/**
 * @code	#include "utils/stream_defs.h" @endcode
 * @file 	libs/utils/stream_defs.h
 * @brief 	Stream handling library definitions.
 */
//###############################################
#ifndef _UTILS_STREAM_DEFS_H_
#define _UTILS_STREAM_DEFS_H_
//###############################################
#include <hardware/timing.h>
#include <utils/string.h>
//###############################################
/**
 * @enum	stream_look_mode_t
 * @brief	Look ahead methods for digit parsing.
 */
typedef enum {
	SKIP_NONE		= 0x00,		//!< Nothing is skipped unless the first waiting character is valid.
	SKIP_ALL		= 0x01,    	//!< All invalid characters skipped.
	SKIP_WHITESPACE	= 0x02   	//!< Only tabs, spaces, line feeds & carriage returns are skipped.
} stream_look_mode_t;

//###############################################
#endif /* _UTILS_STREAM_DEFS_H_ */
//###############################################
