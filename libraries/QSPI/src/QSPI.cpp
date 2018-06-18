#include "stm32l4_wiring_private.h"
#include "QSPI.h"

QSPIClass::QSPIClass(struct _stm32l4_qspi_t *qspi, unsigned int instance, const struct _stm32l4_qspi_pins_t *pins, unsigned int priority, unsigned int mode)
{
	_qspi = qspi;

	stm32l4_qspi_create(qspi, instance, pins, priority, mode);
    _selected = false;

    _completionCallback = NULL;
}

void QSPIClass::begin(uint32_t clock, uint32_t option)
{
	stm32l4_qspi_enable(_qspi, clock, option, QSPIClass::_eventCallback, (void *)this, QSPI_EVENT_TRANSMIT_DONE|QSPI_EVENT_RECEIVE_DONE|QSPI_EVENT_WAIT_DONE);
}

void QSPIClass::end()
{
	if (_selected)
		endTransaction();
    stm32l4_qspi_disable(_qspi);
}

void QSPIClass::beginTransaction()
{
    stm32l4_qspi_select(_qspi);
	_selected = true;
}

void QSPIClass::endTransaction()
{
    stm32l4_qspi_unselect(_qspi);
    _selected = false;
}

bool QSPIClass::transmit(uint32_t command, void *data, unsigned int count)
{
	if (!_selected)
		return false;
	command = (command & ~QSPI_COMMAND_ADDRESS_MASK) | QSPI_COMMAND_ADDRESS_NONE;
	return stm32l4_qspi_transmit(_qspi, command, 0, (uint8_t*)data, count, 0);
}

bool QSPIClass::receive(uint32_t command, void *data, unsigned int count)
{
	if (!_selected)
		return false;
	command = (command & ~QSPI_COMMAND_ADDRESS_MASK) | QSPI_COMMAND_ADDRESS_NONE;
	return stm32l4_qspi_receive(_qspi, command, 0, (uint8_t*)data, count, 0);
}

void QSPIClass::EventCallback(uint32_t events)
{
    void(*callback)(void);
  
    callback = _completionCallback;
    _completionCallback = NULL;

    if (callback) {
		(*callback)();
    }
}

void QSPIClass::_eventCallback(void *context, uint32_t events)
{
    reinterpret_cast<class QSPIClass*>(context)->EventCallback(events);
}

#if QSPI_INTERFACES_COUNT > 0

extern const stm32l4_qspi_pins_t g_QSPIPins;
extern const unsigned int g_QSPIInstance;
extern const unsigned int g_QSPIMode;

static stm32l4_qspi_t _QSPI;

QSPIClass QSPI(&_QSPI, g_QSPIInstance, &g_QSPIPins, STM32L4_SPI_IRQ_PRIORITY, g_QSPIMode);

#endif
