/**
 * @code 	#include "hardware/twi.h" @endcode
 * @file 	libs/hardware/twi.h
 * @brief 	AVR interrupt I2C library.
 */
//###############################################
#ifndef _HARDWARE_TWI_H_
#define _HARDWARE_TWI_H_
//###############################################
#include <hardware/twi_defs.h>
//###############################################
ISR(TWI_vect);						//!< Declare this to simplify the ISR declaration.

//###############################################
/**
 * @class 	TWI
 * @brief	TWI bus general implementation.
 */
class TWI {
public:
	//======================================
	/**
	 * @brief	Get this class instance.
	 * @return	Device singleton.
	 */
	static TWI*	get_instance(void);

	/**
	 * @brief	Deletes the actual instance. This action turns off the
	 * 			interface and reset its configurations.
	 */
	static void delete_instance(void);

	//======================================
	/**
	 * @brief	Disable the TWI bus and clean, if slave operation was used,
	 * 			the slave RX buffer.
	 */
	void disable(void);

	//======================================
	/**
	 * @brief	Set the SCL clock frequency.
	 * @param	freq			Desired bus frequency in KHz.
	 */
	void set_clock(uint16_t freq=TWI_DEF_FREQ_KHZ);

	//======================================
	/**
	 * @brief	Configure the TWI bus to support master operation. This
	 * 			function initialize the bus with the default frequency
	 * 			(100KHz).
	 */
	void enable(void);

	/**
	 * @brief	write(uint8_t, uint8_t*, uint8_t, bool) handler for 1 byte "src".
	 * @param	addr		Slave address.
	 * @param	src			Source byte.
	 * @param	stop		Used to send a stop condition after the task
	 * 						is completed.
	 * @return	Number of bytes written (msb) and error code (lsb).
	 * 			See write(uint8_t, uint8_t*, uint8_t, bool).
	 */
	uint16_t write(uint8_t addr, uint8_t src, bool stop=true);

	/**
	 * @brief 	Write "size" bytes from "src" on the slave given by the "addr"
	 * 			address.
	 * @param	addr			Slave address.
	 * @param	src				Pointer to byte source array.
	 * @param	size			Number of bytes to send.
	 * @param	stop			Used to send a stop condition after the task
	 * 							is completed.
	 * @return	Number of bytes written (msb) and error code (lsb).
	 * 			| Code	| 					Error 					|
	 * 			| :--:	| 					:---:					|
	 * 			|	0	| Succeed                       			|
	 * 			|	1	| Address sent, NACK received              	|
	 * 			|	2	| Data sent, NACK received                 	|
	 * 			|	3	| Arbitration lost                         	|
	 * 			|	4	| Other TWI error (bus error, etc.)        	|
	 * 			|	5	| No buffer available or size not supported	|
	 */
	uint16_t write(uint8_t addr, uint8_t* src, size_t size, bool stop=true);

	/**
	 * @brief	write_reg(uint8_t, uint8_t, uint8_t*, uint8_t, bool) handler
	 * 			for 1 byte "src".
	 * @param	addr		Slave address.
	 * @param	reg			Slave register address.
	 * @param	src			Source byte.
	 * @param	stop		Used to send a stop condition after the task
	 * 						is completed.
	 * @return	Number of bytes written (msb) and error code (lsb).
	 *	 	 	See write(uint8_t, uint8_t*, uint8_t, bool).
	 */
	uint16_t write_reg(uint8_t addr, uint8_t reg, uint8_t src, bool stop=true);

	/**
	 * @brief 	Handler of write(uint8_t, uint8_t*, uint8_t, bool) to simplify
	 * 			the process of write data into a given register on the slave
	 * 			device. It writes "size" bytes from "src" to the "reg" register
	 * 			on the slave given by the "addr" address.
	 * @param	addr			Slave address.
	 * @param	reg 			Slave register address.
	 * @param	src				Pointer to source byte array.
	 * @param	size			Number of bytes to send.
	 * @param	stop			Used to send a stop condition after the task
	 * 							is completed.
	 * @return	Number of bytes written (msb) and error code (lsb).
	 * 			See write(uint8_t, uint8_t*, uint8_t, bool).
	 */
	uint16_t write_reg(uint8_t addr, uint8_t reg, uint8_t* src, size_t size, bool stop=true);

	/**
	 * @brief	read(uint8_t, uint8_t*, uint8_t, bool) handler for 1 byte "rec".
	 * @param	addr		Slave address.
	 * @param	rec			Receive byte.
	 * @param	stop		Used to send a stop condition after the task
	 * 						is completed.
	 * @return	Number of bytes read (msb) and error code (lsb).
	 * 			See read(uint8_t, uint8_t*, uint8_t, bool).
	 */
	uint16_t read(uint8_t addr, uint8_t* rec, bool stop=true);

	/**
	 * @brief 	Read "size" bytes from the slave given by the "addr" address
	 * 			and copy them to "rec".
	 * @param	addr			Slave address.
	 * @param	rec				Pointer to received byte array.
	 * @param	size			Number of bytes to read.
	 * @param	stop			Used to send a stop condition after the task
	 * 							is completed.
	 * @return	Number of bytes read (msb) and error code (lsb).
	 * 			| Code	| 					Error 					|
	 * 			| :--:	| 					:---:					|
	 * 			|	0	| Succeed                       			|
	 * 			|	1	| Address sent, NACK received              	|
	 * 			|	3	| Arbitration lost                         	|
	 * 			|	4	| Other TWI error (bus error, etc.)        	|
	 * 			|	5	| No buffer available or size not supported	|
	 */
	uint16_t read(uint8_t addr, uint8_t* rec, size_t size, bool stop=true);

	/**
	 * @brief	read_reg(uint8_t, uint8_t, uint8_t*, uint8_t, bool) handler
	 * 			for 1 byte "rec".
	 * @param	addr		Slave address.
	 * @param	reg			Slave register address.
	 * @param	rec			Receive byte.
	 * @param	stop		Used to send a stop condition after the task
	 * 						is completed.
	 * @return	Number of bytes read (msb) and error code (lsb).
	 * 			See read(uint8_t, uint8_t*, uint8_t, bool).
	 */
	uint16_t read_reg(uint8_t addr, uint8_t reg, uint8_t* rec, bool stop=true);

	/**
	 * @brief 	Handler of read(uint8_t, uint8_t*, uint8_t, bool) to simplify
	 * 			the process of read data from a given register on the slave
	 * 			device. It reads "size" bytes from the "reg" register on the
	 * 			slave given by the "addr" address and copy them to "rec".
	 * @param	addr			Slave address.
	 * @param	reg				Slave register address.
	 * @param	rec				Pointer to received byte array.
	 * @param	size			Number of bytes to read.
	 * @param	stop			Used to send a stop condition after the task
	 * 							is completed.
	 * @return	Number of bytes read (msb) and error code (lsb).
	 * 			See read(uint8_t, uint8_t*, uint8_t, bool).
	 */
	uint16_t read_reg(uint8_t addr, uint8_t reg, uint8_t* rec, size_t size, bool stop=true);

	//======================================
	/**
	 * @class	Device
	 * @brief	TWI device class. Use this class to simplify the TWI device
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
		 * @brief	device constructor. Initialize a TWI device and assign
		 * 			it's address.
		 * @param	twi		TWI bus manager.
		 * @param	addr	Device address.
		 */
		Device(TWI* twi, uint8_t addr);

		/**
		 * @brief	TWI::write(uint8_t, uint8_t, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t write(uint8_t src, bool stop=true);

		/**
		 * @brief	TWI::write(uint8_t, uint8_t*, uint8_t, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t write(uint8_t* src, size_t size, bool stop=true);

		/**
		 * @brief	TWI::write_reg(uint8_t, uint8_t, uint8_t, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t write_reg(uint8_t reg, uint8_t src, bool stop=true);

		/**
		 * @brief	TWI::write_reg(uint8_t, uint8_t, uint8_t*, uint8_t, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t write_reg(uint8_t reg, uint8_t* src, size_t size, bool stop=true);

		/**
		 * @brief	TWI::read(uint8_t, uint8_t*, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t read(uint8_t *rec, bool stop);

		/**
		 * @brief	TWI::read(uint8_t, uint8_t*, uint8_t, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t read(uint8_t *rec, size_t size, bool stop=true);

		/**
		 * @brief	TWI::read_reg(uint8_t, uint8_t, uint8_t*, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t read_reg(uint8_t reg, uint8_t* rec, bool stop=true);

		/**
		 * @brief	TWI::read_reg(uint8_t, uint8_t, uint8_t*, uint8_t, bool)
		 * 			with the slave address fixed.
		 */
		uint16_t read_reg(uint8_t reg, uint8_t* rec, size_t size, bool stop=true);
	protected:
		TWI*	_dev_twi;	//!< TWI manager.
		uint8_t	_dev_addr;	//!< Device address.
	};

	//======================================
	/**
	 * @brief	Configure the TWI buss to support slave operation. This
	 * 			function initialize the bus whit a given address and
	 * 			create a buffer to receive the bytes sent by the master.
	 * @param	addr		Slave address.
	 * @param	rx_buff_len Size of the slave RX buffer.
	 */
	void enable(uint8_t addr, uint8_t rx_buff_len);

	//======================================
	/**
	 * @brief	Configure the behavior of the slave when receive data
	 * 			from the master. This function will be executed after
	 * 			the reception of all the bytes. The input will be the
	 * 			number of bytes received.
	 * @param	fun			On receive ISR callback.
	 */
	void on_receive(fun_ptr_vd_u8 fun);

	/**
	 * @brief	Return the byte located on the "idx" position on the slave
	 * 			RX buffer.
	 * @param	idx			Index on the slave RX buffer.
	 * @return	Byte.
	 */
	uint8_t get_rx_buff(uint8_t idx);

	//======================================
	/**
	 * @brief	Configure the behavior of the slave when data is requested
	 * 			from the master. This function will be executed before
	 * 			send data to the master. The user need to prepare the response
	 * 			in this callback and prepare the TX slave buffer with the
	 * 			function send_response().
	 * @param	fun			On request ISR callback.
	 */
	void on_request(fun_ptr_vd_vd fun);

	/**
	 * @brief	This function is used to set and prepare the slave TX buffer
	 * 			before sending data to the master.
	 * @param	tx_buff			Pointer to source byte array.
	 * @param	tx_buff_size	Number of bytes to send.
	 */
	void send_response(uint8_t* tx_buff, uint8_t tx_buff_size);

private:
	//======================================
	static TWI*		_inst;						//!< Device Singleton.
	friend void TWI_vect();						//!< Used to make available private SPI methods on the ISR.

	//======================================
	/**
	 * @enum 	twi_state_t
	 * @brief 	TWI state machine states.
	 */
	typedef enum {
		TWI_READY,								//!< TWI bus available (operation finished).
		TWI_MTX,								//!< TWI master on write mode.
		TWI_MRX,								//!< TWI master in read mode.
		TWI_STX,								//!< TWI slave on write mode.
		TWI_SRX									//!< TWI slave on read mode.
	} twi_state_t;

	//======================================
	volatile twi_state_t	_state;				//!< TWI state machine state.
	volatile uint8_t		_slarw;				//!< TWI slave address.
	volatile uint8_t		_error;				//!< TWI transaction error.
	volatile bool			_send_stop;			//!< TWI send stop flag.
	volatile bool			_rep_start;			//!< TWI send rep. start flag.
	uint8_t* volatile 		_mx_buff;			//!< Master buffer.
	volatile uint8_t		_mx_buff_idx;		//!< Master buffer index.
	volatile uint8_t		_mx_buff_size;		//!< Master buffer size.
	uint8_t* volatile		_srx_buff;			//!< Slave RX buffer.
	volatile uint8_t 		_srx_buff_idx;		//!< Slave RX buffer index.
	volatile uint8_t 		_srx_buff_size;		//!< Slave RX buffer size.
	uint8_t* volatile		_stx_buff;			//!< Slave TX buffer.
	volatile uint8_t		_stx_buff_idx;		//!< Slave TX buffer index.
	volatile uint8_t		_stx_buff_size;		//!< Slave TX buffer size.
	static fun_ptr_vd_u8 	_user_on_receive;	//!< Slave on receive ISR callback.
	static fun_ptr_vd_vd 	_user_on_request;	//!< Slave on request ISR callback.

	//======================================
	/**
	 * @brief	TWI constructor.
	 */
	TWI(void);

	//======================================
	/**
	 * @brief 	TWI baudrate expression
	 * @param	xtal_cpu		System clock in MHz.
	 * @param	freq			SCL frequency in KHz.
	 * @return	TWBR value.
	 */
	uint8_t freq_sel(uint32_t xtal_cpu, uint32_t freq);

	//======================================
	/**
	 * @brief	Used internally to initialize the TWI bus
	 * 			(common to master and slave modes).
	 */
	void init(void);

	/**
	 * @brief	Used internally to start the TWI transaction
	 * 			or send a repeated start condition.
	 */
	void start(void);

	/**
	 * @brief	Used internally to stop the TWI transaction.
	 */
	void stop(void);

	/**
	 * @brief	Used internally to release the bus.
	 */
	void release_bus(void);

	/**
	 * @brief	Used internally to send ACK/NACK replies.
	 * @param	ack 			ACK/NACK selector
	 */
	void reply(bool ack);

	//======================================
	/**
	 * @brief	Internal slave on receive ISR callback.
	 */
	static void on_receive_callback(uint8_t last_rx_idx);

	/**
	 * @brief	Internal slave on request ISR callback.
	 */
	static void on_request_callback(void);

	//======================================
	/**
	 * @brief	Callback used to execute the TWI state machine on the ISR.
	 */
	inline void state_machine(void);

};

//###############################################
#endif /* _HARDWARE_TWI_H_ */
//###############################################
