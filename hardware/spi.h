/**
 * @code	#include "hardware/spi.h" @endcode
 * @file 	libs/hardware/spi.h
 * @brief 	AVR SPI library.
 */
//###############################################
#ifndef _HARDWARE_SPI_H_
#define _HARDWARE_SPI_H_
//###############################################
#include <hardware/spi_defs.h>
//###############################################
ISR(SPI_STC_vect);					//!< Declare this to simplify the ISR declaration.

//###############################################
/**
 * @class	SPI
 * @brief	SPI bus general implementation.
 */
class SPI {
public:
	//======================================
	/**
	 * @brief	Get this class instance.
	 * @return	Device singleton.
	 */
	static SPI* get_instance(void);

	/**
	 * @brief	Deletes the actual instance. This action turns off the
	 * 			interface and reset its configurations.
	 */
	static void delete_instance(void);

	//======================================
	/**
	 * @brief	Disable the SPI bus.
	 */
	void disable(void);

	//======================================
	/**
	 * @brief	Configure the MOSI, MISO, SCLK and SS pins for master operation.
	 */
	void enable(void);

	/**
	 * @brief	Start the SPI bus as master with the given configuration.
	 * @param	conf			Pointer to SPI bus configuration spi_conf_t.
	 */
	void start(spi_conf_t* conf);

	/**
	 * @brief	Initialize the master configuration struct with the default
	 * 			values: F_CPU/4[Hz], SPI_MODE_0, MSB_FIRST.
	 * @param	conf			Pointer to SPI bus configuration spi_conf_t.
	 */
	void get_config(spi_conf_t* conf);

	/**
	 * @brief	Set the SPI configuration.
	 * @param	conf			Pointer to SPI bus configuration spi_conf_t.
	 * @param	freq			SPI SCLK frequency.
	 * @param	tx_mode			SPI transmission mode spi_tx_mode_t.
	 * @param	bit_order		SPI data transmission mode spi_ds_mode_t.
	 */
	void get_config(spi_conf_t* conf, uint32_t freq, spi_tx_mode_t tx_mode, spi_ds_mode_t bit_order);

	//======================================
	/**
	 * @brief 	The master send a single byte to the slave and returns it's
	 * 			response.
	 * @param	data			Byte to send.
	 * @return	Slave response byte.
	 */
	uint8_t transfer(uint8_t data);

	/**
	 * @brief	Perform a data transfer between the SPI master and the slave.
	 * 			Send "size" bytes from "src" and save the responses on "rec".
	 * 			The method allow to start saving the responses after "offset"
	 * 			bytes sent (the master will send "offset" dummy bytes after
	 * 			"src" is transmitted to get the full response) and add
	 *			delay after each transfer.
	 * @param	src				Pointer to source byte array.
	 * @param	rec				Pointer to received byte array.
	 * @param	size			Bytes to be transfered.
	 * @param	off				Offset (in bytes).
	 * @param	dly				Time to wait between transfers (in [us]).
	 */
	void transfer(uint8_t* src, uint8_t* rec, size_t size, uint8_t off=0, uint16_t dly=0);

	/**
	 * @brief	write(uint8_t*, uint8_t, uint16_t)
	 * 			handler for 1 byte "src".
	 * @param	src			Source byte.
	 * @param	dly			Time to wait after write (in [us]).
	 */
	void write(uint8_t src, uint16_t dly=0);

	/**
	 * @brief	Write "size" bytes from "src" to the slave.
	 * @param	src			Pointer to source byte array.
	 * @param	size		Bytes to be transmitted.
	 * @param	dly			Time to wait between writes (in [us]).
	 */
	void write(uint8_t* src, size_t size, uint16_t dly=0);

	/**
	 * brief	write_reg(uint8_t, uint8_t*, uint8_t, uint16_t) handler for 1 byte "src".
	 * @param	reg			Slave register address.
	 * @param	src			Source byte.
	 * @param	dly			Time to wait after write (in [us]).
	 */
	void write_reg(uint8_t reg, uint8_t src, uint16_t dly);

	/**
	 * @brief	Handler of spi_master_write() to simplify the process of write
	 * 			data into a given register on the slave device. It writes "size"
	 * 			bytes from "src" to the "reg" register on the slave.
	 * @param	reg				Slave register address.
	 * @copydetails write(uint8_t*, size_t, uint16_t)
	 */
	void write_reg(uint8_t reg, uint8_t* src, size_t size, uint16_t dly);

	/**
	 * @brief	Read "size" bytes from the slave to "rec".
	 * @param	rec			Pointer to received byte array.
	 * @param	size		Bytes to be read.
	 * @param	dly			Time to wait between reads (in [us]).
	 */
	void read(uint8_t* rec, size_t size, uint16_t dly=0);

	/**
	 * brief	read_reg(uint8_t, uint8_t*, uint8_t, uint16_t) handler for 1 byte "rec".
	 * @param	reg			Slave register address.
	 * @param	rec			Receive byte.
	 * @param	dly			Time to wait after write (in [us]).
	 */
	void read_reg(uint8_t reg, uint8_t* rec, uint16_t dly);

	/**
	 * @brief	Handler of spi_master_read() to simplify the process of read
	 * 			data from a given register on the slave device. It reads "size"
	 * 			bytes from the "reg" register on the slave and copy them to "rec".
	 * @param	reg				Slave register address.
	 * @copydetails read(uint8_t*, size_t, uint16_t)
	 */
	void read_reg(uint8_t reg, uint8_t* rec, size_t size, uint16_t dly);

	//======================================
	/**
	 * @class 	Device
	 * @brief	SPI device class. Use this class to simplify the SPI device
	 * 			creation and configuration.
	 */
	class Device {
	public:
		/**
		 * @brief	Device empty constructor. Useful in the case that one
		 * 			device can be implemented with more than one communication
		 * 			bus.
		 */
		Device(void);

		/**
		 * @brief	Device constructor. Initialize a SPI device and assign the
		 * 			SS pin. Also, this method allows to define the level which
		 * 			the device become active.
		 * @param	spi		SPI bus manager.
		 * @param	ss_pin	SS pin.
		 * @param	dev_on	Level which have to have the SS pin to activate the
		 * 					device. By default is LOW(false).
		 */
		Device(SPI* spi, pins_t ss_pin, bool dev_on=false);

		/**
		 * @brief	Set the configuration required to communicate with the
		 * 			device.
		 * @param	freq			SPI SCLK frequency.
		 * @param	tx_mode			SPI transmission mode spi_tx_mode_t.
		 * @param	bit_order		SPI data transmission mode spi_ds_mode_t.
		 */
		void configure(uint32_t freq, spi_tx_mode_t tx_mode, spi_ds_mode_t bit_order);

		/**
		 * @brief	Activate the device changing the state of the SS pin.
		 */
		void begin_transaction(void);

		/**
		 * @brief	Deactivate the device changing the state of the SS pin.
		 */
		void end_transaction(void);

		/** @copydoc SPI::transfer(uint8_t) */
		uint8_t transfer(uint8_t data);

		/** @copydoc SPI::transfer(uint8_t*, uint8_t*, size_t, uint8_t, uint16_t) */
		void transfer(uint8_t* src, uint8_t* rec, size_t size, uint8_t off=0, uint16_t dly=0);

		/** @copydoc SPI::write(uint8_t, uint16_t) */
		void write(uint8_t src, uint16_t dly=0);

		/** @copydoc SPI::write(uint8_t*, size_t, uint16_t) */
		void write(uint8_t* src, size_t size, uint16_t dly=0);

		/** @copydoc SPI::write_reg(uint8_t, uint8_t, uint16_t) */
		void write_reg(uint8_t reg, uint8_t src, uint16_t dly=0);

		/** @copydoc SPI::write_reg(uint8_t, uint8_t*, size_t, uint16_t) */
		void write_reg(uint8_t reg, uint8_t* src, size_t size, uint16_t dly=0);

		/** @copydoc SPI::read(uint8_t*, size_t, uint16_t) */
		void read(uint8_t* rec, size_t size, uint16_t dly=0);

		/** @copydoc SPI::read_reg(uint8_t, uint8_t*, uint16_t) */
		void read_reg(uint8_t reg, uint8_t* rec, uint16_t dly=0);

		/** @copydoc SPI::read_reg(uint8_t reg, uint8_t* rec, size_t, uint16_t) */
		void read_reg(uint8_t reg, uint8_t* rec, size_t size, uint16_t dly=0);

	protected:
		SPI* 		_dev_spi;	//!< SPI manager.
		spi_conf_t	_dev_conf;	//!< SPI settings.
		pins_t		_dev_ss;	//!< Device SS.
		bool 		_dev_on;	//!< Device on SS status.
	};

	//======================================
	/**
	 * @brief	Prepare the MOSI, MISO, SCLK and SS pins for slave operation
	 * 			and start the slave routine. The SS pin of the board has to be
	 * 			connected to a pin with external or pin-change interrupt
	 * 			capabilities (used to control the SPI flow). It's possible to
	 * 			set a user-defined function that executes before the slave
	 * 			interruption is handled (if required).
	 * @param	ss_int 		Board pin value, interrupt compatible.
	 * @param	fun			ISR callback.
	 * @return 	Return if it was possible to set the SPI as slave. If it was a
	 * 			success returns true, else returns false.
	 */
	bool enable(pins_t ss_int, fun_ptr_vd_vd fun=NULL);

	/**
	 * @brief	Set a user-defined function to handle the data received from the
	 * 			master. The input of the function is the byte received and the
	 * 			output is the response generated. By default the slave will send
	 * 			0x00 for every byte received.
	 * @param	fun			ISR callback.
	 */
	void on_receive(fun_ptr_u8_u8 fun);

private:
	//======================================
	static SPI*		_inst;						//!< Device Singleton.
	friend void SPI_STC_vect();					//!< Used to make available private SPI methods on the ISR.

	//======================================
	/**
	 * @enum	spi_mode_t
	 * @brief	SPI modes.
	 */
	typedef enum {
		SPI_NONE,								//!< SPI not configured.
		SPI_MASTER,								//!< SPI master mode.
		SPI_SLAVE								//!< SPI slave mode.
	} spi_mode_t;

	//======================================
	volatile spi_mode_t 	_mode;				//!< SPI mode.
	static pins_t 			_ss_pin;			//!< Slave SS interrupt pin.
	static bool 			_slave_active;		//!< Slave on-request flag.
	static fun_ptr_u8_u8 	_user_on_receive;	//!< Slave on receive ISR callback.
	static fun_ptr_vd_vd 	_user_on_request;	//!< Slave on request ISR callback.

	//======================================
	/**
	 * @brief	SPI constructor.
	 */
	SPI(void);

	//======================================
	/**
	 * @brief	Calculate the most adequate prescaler depending on the
	 * 			selected frequency.
	 * @return	Prescaler value.
	 */
	uint8_t get_prescaler(uint32_t freq);

	//======================================
	/**
	 * @brief	Internal slave on receive ISR callback.
	 * @param	m_data		Byte received from the master.
	 */
	static uint8_t on_receive_callback(uint8_t m_data);

	/**
	 * @brief	Internal slave on request ISR callback.
	 */
	static void on_request_callback(void);

};

//###############################################
#endif /* _HARDWARE_SPI_H_ */
//###############################################
