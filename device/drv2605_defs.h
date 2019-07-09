/**
 * @code 	#include "device/drv2605_defs.h" @endcode
 * @file 	libs/device/drv2605_defs.h
 * @brief	DRV2650 driver definitions.
 */
//###############################################
#ifndef _DEVICES_DRV2605_DEFS_H_
#define _DEVICES_DRV2605_DEFS_H_
//###############################################
#include <hardware/twi.h>
#include <hardware/timing.h>

//###############################################
#define DRV2605_ADDR 				0x5A		//!< Device Address.
#define DRV2605_REGS_NUM			35			//!< Number of registers.

//###############################################
/* Register address. */
#define DRV2605_REG_STAT			0x00		//!< Status register.
#define DRV2605_REG_MODE			0x01		//!< Mode register.
#define DRV2605_REG_RTPIN			0x02		//!< Real-Time Playback Input register.
#define DRV2605_REG_LIBSEL			0x03		//!< Library Selection register.
#define DRV2605_REG_WAVSEQ1			0x04		//!< Waveform Sequencer 1 register.
#define DRV2605_REG_WAVSEQ2			0x05		//!< Waveform Sequencer 2 register.
#define DRV2605_REG_WAVSEQ3			0x06		//!< Waveform Sequencer 3 register.
#define DRV2605_REG_WAVSEQ4			0x07		//!< Waveform Sequencer 4 register.
#define DRV2605_REG_WAVSEQ5			0x08		//!< Waveform Sequencer 5 register.
#define DRV2605_REG_WAVSEQ6			0x09		//!< Waveform Sequencer 6 register.
#define DRV2605_REG_WAVSEQ7			0x0A		//!< Waveform Sequencer 7 register.
#define DRV2605_REG_WAVSEQ8			0x0B		//!< Waveform Sequencer 8 register.
#define DRV2605_REG_GO				0x0C		//!< Go register.
#define DRV2605_REG_ODTO			0x0D		//!< Overdrive Time Offset register.
#define DRV2605_REG_STP				0x0E		//!< Sustain Time Offset Positive register.
#define DRV2605_REG_STN				0x0F		//!< Sustain Time Offset Negative register.
#define DRV2605_REG_BRT				0x10		//!< Brake Time Offset register.
#define DRV2605_REG_A2VCTRL			0X11		//!< Audio-to-Vibe Control register.
#define DRV2605_REG_A2VIN_MIN		0x12		//!< Audio-to-Vibe Min Input LVL register.
#define DRV2605_REG_A2VIN_MAX		0x13		//!< Audio-to-Vibe Max Input LVL register.
#define DRV2605_REG_A2VOUT_MIN		0x14		//!< Audio-to-Vibe Min Output LVL register.
#define DRV2605_REG_A2VOUT_MAX		0x15		//!< Audio-to-Vibe Max Output LVL register.
#define DRV2605_REG_RATEDV			0x16		//!< Rated Voltage register.
#define DRV2605_REG_CLAMPV			0x17		//!< OC Clamp Voltage register.
#define DRV2605_REG_ACAL_COMP		0x18		//!< Auto-Calibration Compensation register.
#define DRV2605_REG_ACAL_BEMF		0x19		//!< Auto-Calibration Back-EMF register.
#define DRV2605_REG_FB_CTRL			0x1A		//!< Feedback Control register.
#define DRV2605_REG_CTRL1			0x1B		//!< Control 1 register.
#define DRV2605_REG_CTRL2			0x1C		//!< Control 2 register.
#define DRV2605_REG_CTRL3			0x1D		//!< Control 3 register.
#define DRV2605_REG_CTRL4			0x1E		//!< Control 4 register.
#define DRV2605_REG_CTRL5			0x1F		//!< Control 5 register.
#define DRV2605_REG_LRA_OPLP		0x20		//!< LRA Open Loop Period register.
#define DRV2605_REG_VBAT			0x21		//!< Voltage Monitor Register register.
#define DRV2605_REG_LRA_RES			0x22		//!< LRA Resonance-Period register.

//###############################################
/* Register default values (only for r/w or w registers) */
#define DRV2605_DEF_MODE			0x40		//!< Mode register default value.
#define DRV2605_DEF_RTPIN			0x00		//!< Real-Time Playback Input register default value.
#define DRV2605_DEF_LIBSEL			0x01		//!< Library Selection register default value.
#define DRV2605_DEF_WAVSEQ1			0x01		//!< Waveform Sequencer 1 register default value.
#define DRV2605_DEF_WAVSEQX			0x00		//!< Waveform Sequencer X register default value.
#define DRV2605_DEF_GO				0x00		//!< Go register default value.
#define DRV2605_DEF_ODTO			0x00		//!< Overdrive Time Offset register default value.
#define DRV2605_DEF_STP				0x00		//!< Sustain Time Offset Positive register default value.
#define DRV2605_DEF_STN				0x00		//!< Sustain Time Offset Negative register default value.
#define DRV2605_DEF_BRT				0x00		//!< Brake Time Offset register default value.
#define DRV2605_DEF_A2VCTRL			0X05		//!< Audio-to-Vibe Control register default value.
#define DRV2605_DEF_A2VIN_MIN		0x19		//!< Audio-to-Vibe Min Input LVL register default value.
#define DRV2605_DEF_A2VIN_MAX		0xFF		//!< Audio-to-Vibe Max Input LVL register default value.
#define DRV2605_DEF_A2VOUT_MIN		0x19		//!< Audio-to-Vibe Min Output LVL register default value.
#define DRV2605_DEF_A2VOUT_MAX		0xFF		//!< Audio-to-Vibe Max Output LVL register default value.
#define DRV2605_DEF_RATEDV			0x3E		//!< Rated Voltage register default value.
#define DRV2605_DEF_CLAMPV			0x8C		//!< OC Clamp Voltage register default value.
#define DRV2605_DEF_ACAL_COMP		0x0C		//!< Auto-Calibration Compensation register default value.
#define DRV2605_DEF_ACAL_BEMF		0x6C		//!< Auto-Calibration Back-EMF register default value.
#define DRV2605_DEF_FB_CTRL			0x36		//!< Feedback Control register default value.
#define DRV2605_DEF_CTRL1			0x93		//!< Control 1 register default value.
#define DRV2605_DEF_CTRL2			0xF5		//!< Control 2 register default value.
#define DRV2605_DEF_CTRL3			0xA0		//!< Control 3 register default value.
#define DRV2605_DEF_CTRL4			0x20		//!< Control 4 register default value.
#define DRV2605_DEF_CTRL5			0x80		//!< Control 5 register default value.

//###############################################
/* Register mapping */
/**
 * @struct	drv2605_reg_stat_t.
 * @brief	STATUS register structure.
 */
typedef struct {
	uint8_t OC_DET	 : 1;	//!< Over-current flag.
	uint8_t OTEMP	 : 1;   //!< Over-temperature flag.
	uint8_t unused_1 : 1;   //!< Unused bit.
	uint8_t DIAG_RES : 1;   //!< Diagnostic/Auto-calibration result.
	uint8_t unused_2 : 1;   //!< Unused bit.
	uint8_t	DEV_ID	 : 3;   //!< Device ID.
} drv2605_reg_stat_t;

/**
 * @struct	drv2605_reg_mode_t.
 * @brief	MODE register structure.
 */
typedef struct {
	uint8_t	MODE	 : 3;	//!< Operation mode.
	uint8_t unused	 : 3;   //!< Unused bit.
	uint8_t SBY	 	 : 1;   //!< Standby flag.
	uint8_t DEV_RST	 : 1;   //!< Device reset.
} drv2605_reg_mode_t;

/**
 * @struct	drv2605_reg_libsel_t.
 * @brief	LIBSEL register structure.
 */
typedef struct {
	uint8_t LIB_SEL	 : 3;	//!< Waveform library selection.
	uint8_t unused_1 : 1;   //!< Unused bit.
	uint8_t HI_Z	 : 1;   //!< High impedance state.
	uint8_t	unused_2 : 3;   //!< Unused bit.
} drv2605_reg_libsel_t;

/**
 * @struct	drv2605_reg_wavseq_t.
 * @brief	WAVSEQ register structure.
 */
typedef struct {
	uint8_t WAV_FRM	 : 7;	//!< Waveform sequence value.
	uint8_t WAIT	 : 1;	//!< Wait flag.
} drv2605_reg_wavseq_t;

/**
 * @struct	drv2605_reg_go_t.
 * @brief	GO register structure.
 */
typedef struct {
	uint8_t GO		 : 1;	//!< Do programmed action.
	uint8_t unused	 : 7;	//!< Unused bit.
} drv2605_reg_go_t;

/**
 * @struct	drv2605_reg_a2v_ctrl_t.
 * @brief	A2VCTRL register structure.
 */
typedef struct {
	uint8_t A2V_FLTR : 2;	//!< Low pass filter.
	uint8_t PK_TM	 : 2;	//!< Peak detection time.
	uint8_t unused	 : 4;	//!< Unused bit.
} drv2605_reg_a2v_ctrl_t;

/**
 * @struct	drv2605_reg_fbctrl_t.
 * @brief	FB_CTRL register structure.
 */
typedef struct {
	uint8_t BEMF_GN	 : 2;	//!< BEMF gain.
	uint8_t LP_GN	 : 2;	//!< Loop gain.
	uint8_t BK_FCTR	 : 3;	//!< Break factor.
	uint8_t IS_LRA	 : 1;	//!< Motor type selector.
} drv2605_reg_fbctrl_t;

/**
 * @struct	drv2605_reg_ctrl1_t.
 * @brief	CTRL1 register structure.
 */
typedef struct {
	uint8_t DRV_TM	 : 5;	//!< Drive time.
	uint8_t	AC_CPL	 : 1;	//!< AC coupling.
	uint8_t unused	 : 1;	//!< Unused bit.
	uint8_t ST_BST	 : 1;	//!< Startup boost.
} drv2605_reg_ctrl1_t;

/**
 * @struct	drv2605_reg_ctrl2_t.
 * @brief	CTRL2 register structure.
 */
typedef struct {
	uint8_t IDSS_TM  : 2;	//!< Current dissipation time LSB.
	uint8_t BLKG_TM	 : 2;	//!< Blanking time LSB.
	uint8_t SPL_TM	 : 2;	//!< Sample time.
	uint8_t BK_STB	 : 1;	//!< Brake stabilizer.
	uint8_t BDIR_IN	 : 1;	//!< Bi-directional input mode.
} drv2605_reg_ctrl2_t;

/**
 * @struct	drv2605_reg_ctrl3_t.
 * @brief	CTRL3 register structure.
 */
typedef struct {
	uint8_t LRA_O_LP : 1;	//!< LRA open loop/auto-resonance mode.
	uint8_t	PWM_IN	 : 1;	//!< PWM/Analog input selector.
	uint8_t LRA_DRV_M: 1; 	//!< LRA drive mode selector.
	uint8_t D_FMT_RTP: 1;	//!< RTP data format.
	uint8_t SPLY_COM : 1;	//!< Supply compensation disable.
	uint8_t ERM_O_LP : 1;	//!< LRA open/closed loop mode.
	uint8_t NG_THS	 : 1; 	//!< PWM noise-gate threshold selector.
} drv2605_reg_ctrl3_t;

/**
 * @struct	drv2605_reg_ctrl4_t.
 * @brief	CTRL4 register structure.
 */
typedef struct {
	uint8_t OTP_PROG : 1;	//!< OTP program launch.
	uint8_t unused_1 : 1;	//!< Unused bit.
	uint8_t OTP_STAT : 1; 	//!< OTP memory status.
	uint8_t unused_2 : 1; 	//!< Unused bit.
	uint8_t A_CAL_TM : 2;	//!< Auto-calibration time.
	uint8_t ZC_DET_TM: 2;	//!< Zero-Crossing detection time.
} drv2605_reg_ctrl4_t;

/**
 * @struct	drv2605_reg_ctrl5_t.
 * @brief	CTRL5 register structure.
 */
typedef struct {
	uint8_t IDSS_TM  : 2;	//!< Current dissipation time MSB.
	uint8_t BLKG_TM	 : 2;	//!< Blanking time MSB.
	uint8_t PB_INT	 : 1;	//!< Playback interval.
	uint8_t LRA_AO_LP: 1;	//!< LRA auto open loop mode.
	uint8_t AOL_CNT	 : 2;	//!< Auto open loop synchronization tries.

} drv2605_reg_ctrl5_t;

//###############################################
/* Definitions */
/**
 * @enum	drv2605_bkfc_t
 * @brief	DRV2605 brake factor configuration values.
 */
typedef enum {
	DRV2605_BKFC_DISABLED = 0x07,	//!< Brake factor disabled.
	DRV2605_BKFC_X1	 = 0x00,		//!< Brake factor x1
	DRV2605_BKFC_X2	 = 0x01,		//!< Brake factor x2
	DRV2605_BKFC_X3	 = 0x02,		//!< Brake factor x3
	DRV2605_BKFC_X4	 = 0x03,		//!< Brake factor x4
	DRV2605_BKFC_X6	 = 0x04,		//!< Brake factor x6
	DRV2605_BKFC_X8	 = 0x05,		//!< Brake factor x8
	DRV2605_BKFC_X16 = 0x06,		//!< Brake factor x16
} drv2605_bkfc_t;

/**
 * @enum	drv2605_lpg_t
 * @brief	DRV2605 loop gain configuration values.
 */
typedef enum {
	DRV2605_LPG_LOW = 0x00,			//!< Low.
	DRV2605_LPG_MED = 0x01,			//!< Medium.
	DRV2605_LPG_HI	= 0x02,			//!< High.
	DRV2605_LPG_VHI = 0x03,			//!< Very high.
} drv2605_lpg_t;

/**
 * @enum	drv2605_acalt_t
 * @brief	DRV2605 auto-calibration time configuration values.
 */
typedef enum {
	DRV2605_ACALT_150MS = 0x00,		//!< 150[ms] (min) - 350[ms] (max)
	DRV2605_ACALT_250MS = 0x01,		//!< 250[ms] (min) - 450[ms] (max)
	DRV2605_ACALT_500MS = 0x02,		//!< 500[ms] (min) - 700[ms] (max)
	DRV2605_ACALT_1S	= 0x03		//!< 1000[ms] (min) - 1200[ms] (max)
} drv2605_acalt_t;

//###############################################
/* Data types */
/**
 * @union	drv2605_reg_t
 * @brief	General register handler.
 */
typedef union {
	uint8_t 				raw;		//!< Raw data.
	drv2605_reg_go_t		go;         //!< Data formatted as GO register.
	drv2605_reg_stat_t		stat;       //!< Data formatted as STATUS register.
	drv2605_reg_mode_t		mode;       //!< Data formatted as MODE register.
	drv2605_reg_libsel_t	libsel;     //!< Data formatted as LIBSEL register.
	drv2605_reg_wavseq_t	wavseq;     //!< Data formatted as WAVSEQ register.
	drv2605_reg_a2v_ctrl_t	a2v_ctrl;   //!< Data formatted as A2VCTRL register.
	drv2605_reg_fbctrl_t	fb_ctrl;    //!< Data formatted as FB_CTRL register.
	drv2605_reg_ctrl1_t		ctrl1;      //!< Data formatted as CTRL1 register.
	drv2605_reg_ctrl2_t		ctrl2;      //!< Data formatted as CTRL2 register.
	drv2605_reg_ctrl3_t		ctrl3;      //!< Data formatted as CTRL3 register.
	drv2605_reg_ctrl4_t		ctrl4;      //!< Data formatted as CTRL4 register.
	drv2605_reg_ctrl5_t		ctrl5;      //!< Data formatted as CTRL5 register.
} drv2605_reg_t;

//###############################################
#endif /* _DEVICES_DRV2605_DEFS_H_ */
//###############################################
