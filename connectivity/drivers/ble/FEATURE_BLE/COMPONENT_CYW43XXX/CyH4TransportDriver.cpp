/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if DEVICE_SERIAL && DEVICE_SERIAL_FC

#include "CyH4TransportDriver.h"
#include "mbed_power_mgmt.h"
#include "drivers/InterruptIn.h"
#include "Callback.h"
#include "rtos/ThisThread.h"
#include <chrono>

namespace ble {
namespace vendor {
namespace cypress_ble {

using namespace std::chrono_literals;


CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, PinName bt_power_name, int baud, PinName bt_host_wake_name, PinName bt_device_wake_name, uint8_t host_wake_irq, uint8_t dev_wake_irq) :
    uart(tx, rx), cts(cts), rts(rts),
    bt_host_wake_name(bt_host_wake_name),
    bt_device_wake_name(bt_device_wake_name),
    bt_power(bt_power_name, PIN_OUTPUT, PullNone, 0),
    bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
    bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullNone, 1),
    host_wake_irq_event(host_wake_irq),
    dev_wake_irq_event(dev_wake_irq)

{
    enabled_powersave = true;
    bt_host_wake_active = false;
}

CyH4TransportDriver::CyH4TransportDriver(PinName tx, PinName rx, PinName cts, PinName rts, PinName bt_power_name, int baud) :
    uart(tx, rx), cts(cts),
    rts(rts),
    tx(tx), rx(rx),
    bt_host_wake_name(NC),
    bt_device_wake_name(NC),
    bt_power(bt_power_name, PIN_OUTPUT, PullNone, 0),
    bt_host_wake(bt_host_wake_name),
    bt_device_wake(bt_device_wake_name)

{
    enabled_powersave = false;
    bt_host_wake_active = false;

    sleep_manager_lock_deep_sleep();        // locking deep sleep because this option 
                                            // does not include a host wake pin
    holding_deep_sleep_lock = true;
}

CyH4TransportDriver::~CyH4TransportDriver()
{
    if (holding_deep_sleep_lock)
    {
       sleep_manager_unlock_deep_sleep();
       holding_deep_sleep_lock = false;
    }
}

void CyH4TransportDriver::bt_host_wake_rise_irq_handler(void)
{
    if (host_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
        if(bt_host_wake_active == true)
        {
            /* lock PSoC 6 DeepSleep entry as long as host_wake is asserted */
            sleep_manager_unlock_deep_sleep();
            bt_host_wake_active = false;
        }
    } else {
        /* lock PSoC 6 DeepSleep entry as long as host_wake is asserted */
        sleep_manager_lock_deep_sleep();
        bt_host_wake_active = true;
    }
}

void CyH4TransportDriver::bt_host_wake_fall_irq_handler(void)
{
    if (host_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
        /* lock PSoC 6 DeepSleep entry as long as host_wake is asserted */
        sleep_manager_lock_deep_sleep();
        bt_host_wake_active = true;
    } else {
        if(bt_host_wake_active == true)
        {
            /* lock PSoC 6 DeepSleep entry as long as host_wake is asserted */
            sleep_manager_unlock_deep_sleep();
            bt_host_wake_active = false;
        }
    }
}

void CyH4TransportDriver::on_controller_irq()
{
    sleep_manager_lock_deep_sleep();

    while (uart.readable()) {
        uint8_t char_received;
        if (uart.read(&char_received, 1)) {
            CordioHCITransportDriver::on_data_received(&char_received, 1);
        }
    }

    sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::initialize()
{
    sleep_manager_lock_deep_sleep();

    bt_power = 0;
    rtos::ThisThread::sleep_for(1ms);

    uart.baud(DEF_BT_BAUD_RATE);

    uart.format(
        /* bits */ 8,
        /* parity */ mbed::SerialBase::None,
        /* stop bit */ 1
    );

    uart.set_flow_control(
        /* flow */ mbed::SerialBase::RTSCTS,
        /* rts */ rts,
        /* cts */ cts
    );

    uart.attach(
        mbed::callback(this, &CyH4TransportDriver::on_controller_irq),
        mbed::SerialBase::RxIrq
    );

    bt_power = 1;

#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    if (bt_host_wake_name != NC) {
        //Register IRQ for Host WAKE
        host_wake_pin = new mbed::InterruptIn(bt_host_wake_name);
        host_wake_pin->fall(mbed::callback(this, &CyH4TransportDriver::bt_host_wake_fall_irq_handler));
        host_wake_pin->rise(mbed::callback(this, &CyH4TransportDriver::bt_host_wake_rise_irq_handler));
    }
#endif
    if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
       if (bt_device_wake_name != NC)
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
    } else {
       if (bt_device_wake_name != NC)
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
    }
    sleep_manager_unlock_deep_sleep();
}

void CyH4TransportDriver::terminate()
{
    if(bt_host_wake.is_connected())
    {
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
        delete host_wake_pin;
#endif
    }

    deassert_bt_dev_wake();

    bt_power = 0; //BT_POWER is an output, should not be freed only set inactive

    uart.close();
}

uint16_t CyH4TransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    sleep_manager_lock_deep_sleep();
    assert_bt_dev_wake();

    while (i < len + 1) {
        uint8_t to_write = i == 0 ? type : pData[i - 1];
        while (uart.writeable() == 0);
        uart.write(&to_write, 1);
        ++i;
    }
    while (uart.writeable() == 0);

    deassert_bt_dev_wake();
    sleep_manager_unlock_deep_sleep();
    return len;
}

void CyH4TransportDriver::assert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    if (enabled_powersave) {
        if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
        } else {
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
        }
    }
#endif
}

void CyH4TransportDriver::deassert_bt_dev_wake()
{
#if (defined(MBED_TICKLESS) && DEVICE_SLEEP && DEVICE_LPTICKER)
    if (enabled_powersave) {
        if (dev_wake_irq_event == WAKE_EVENT_ACTIVE_LOW) {
           bt_device_wake = WAKE_EVENT_ACTIVE_HIGH;
        } else {
           bt_device_wake = WAKE_EVENT_ACTIVE_LOW;
        }
    }
#endif
}


void CyH4TransportDriver::update_uart_baud_rate(int baud)
{
    uart.baud((uint32_t)baud);
}

bool CyH4TransportDriver::get_enabled_powersave()
{
    return (enabled_powersave);
}

uint8_t CyH4TransportDriver::get_host_wake_irq_event()
{
    return (host_wake_irq_event);
}

uint8_t CyH4TransportDriver::get_dev_wake_irq_event()
{
    return (dev_wake_irq_event);
}


} // namespace cypress_ble
} // namespace vendor
} // namespace ble

ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_default_h4_transport_driver()
{
#if (defined(CYBSP_BT_HOST_WAKE) && defined(CYBSP_BT_DEVICE_WAKE))
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
       /* TX */ CYBSP_BT_UART_TX, /* RX */ CYBSP_BT_UART_RX,
       /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, CYBSP_BT_POWER, DEF_BT_BAUD_RATE,
       CYBSP_BT_HOST_WAKE, CYBSP_BT_DEVICE_WAKE
    );

#else
    static ble::vendor::cypress_ble::CyH4TransportDriver s_transport_driver(
        /* TX */ CYBSP_BT_UART_TX, /* RX */ CYBSP_BT_UART_RX,
        /* cts */ CYBSP_BT_UART_CTS, /* rts */ CYBSP_BT_UART_RTS, CYBSP_BT_POWER, DEF_BT_BAUD_RATE);
#endif
    return s_transport_driver;
}

MBED_WEAK
ble::vendor::cypress_ble::CyH4TransportDriver& ble_cordio_get_h4_transport_driver()
{
    return (ble_cordio_get_default_h4_transport_driver());
}

#endif
