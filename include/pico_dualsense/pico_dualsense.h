#ifndef __PICO_DUALSENSE_H__
#define __PICO_DUALSENSE_H__

#include "btstack.h"

#include "pico_dualsense/dualsense_parser.h"

extern DualsenseParser dualsense_parser;

bool dualsense_bluetooth_init();

void dualsense_auto_connect(bd_addr_t address);

#endif
