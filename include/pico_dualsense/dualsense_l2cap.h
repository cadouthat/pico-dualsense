#ifndef __DUALSENSE_L2CAP_H__
#define __DUALSENSE_L2CAP_H__

#include <stdint.h>

#include "btstack.h"

void dualsense_interrupt_report_handler(uint8_t *packet, uint16_t size);

void dualsense_l2cap_outgoing_connection(bd_addr_t address);

void dualsense_l2cap_init();

#endif
