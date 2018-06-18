/*
 * Library for Quad SPI interface
 */

#ifndef _QSPI_H_INCLUDED
#define _QSPI_H_INCLUDED

#include <Arduino.h>
#include <stm32l4_qspi.h>

class QSPIClass {
public:
	QSPIClass(struct _stm32l4_qspi_t *qspi, unsigned int instance, const struct _stm32l4_qspi_pins_t *pins, unsigned int priority, unsigned int mode);

	// Clock mode options for QSPI.begin
	const uint32_t Mode0 = QSPI_OPTION_MODE_0;	// CPOL=0 CPHA=0
	const uint32_t Mode3 = QSPI_OPTION_MODE_3;	// CPOL=1 CPHA=1

	// Command for data-only quad transfer
	const uint32_t CommandQuad = 
		QSPI_COMMAND_INSTRUCTION_NONE |
		QSPI_COMMAND_ADDRESS_NONE |
		QSPI_COMMAND_MODE_NONE |
		QSPI_COMMAND_DATA_QUAD;

	// Initialise QSPI interface.
	// clock: clock speed in Hz, must be an integer factor of CPU speed
	// mode: clock mode = 0 (idle state is low) or 3 (idle state is high)
	void begin(uint32_t clock, uint32_t mode);
	// Finish using QSPI interface.
	void end();
	// Enable QSPI hardware.
	void beginTransaction();
	// Disable QSPI hardware.
	void endTransaction();
	// Transmit count bytes of data from slave to master, with optional command prefix.
	bool transmit(uint32_t command, void *data, unsigned int count);
	// Receive count bytes of data from master to slave, with optional command prefix.
	bool receive(uint32_t command, void *data, unsigned int count);

	// Transmit count bytes of data from slave to master.
	bool write(void *data, unsigned int count) { return transmit(CommandQuad, data, count); }
	// Receive count bytes of data from master to slave.
	bool read(void *data, unsigned int count) { return receive(CommandQuad, data, count); }

private:
	struct _stm32l4_qspi_t *_qspi;
	bool _selected;

    void (*_completionCallback)(void);
    static void _eventCallback(void *context, uint32_t events);
    void EventCallback(uint32_t events);
};

extern QSPIClass QSPI;
#endif

