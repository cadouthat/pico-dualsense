#include "pico_dualsense/pico_dualsense.h"

#include <stdint.h>
#include <stdio.h>

#include "btstack.h"

#include "pico_dualsense/dualsense_parser.h"
#include "pico_dualsense/dualsense_hci.h"
#include "pico_dualsense/dualsense_l2cap.h"

DualsenseParser dualsense_parser;

void dualsense_interrupt_report_handler(uint8_t *packet, uint16_t size) {
  dualsense_parser.parseReport(packet, size);
}

bool dualsense_bluetooth_init() {
  sm_init();
  sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

  gap_set_default_link_policy_settings(LM_LINK_POLICY_ENABLE_SNIFF_MODE);
  gap_connectable_control(1);

  dualsense_l2cap_init();
  dualsense_hci_init();

  return true;
}

void dualsense_auto_connect(bd_addr_t address) {
  if (!dualsense_can_try_connect) {
    return;
  }
  dualsense_can_try_connect = false;

  printf("Attempting to connect to %s\n", bd_addr_to_str(address));
  dualsense_l2cap_outgoing_connection(address);
}
