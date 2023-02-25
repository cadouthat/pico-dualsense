#include "pico_dualsense/dualsense_hci.h"

#include <stdint.h>

#include "btstack.h"

namespace {

btstack_packet_callback_registration_t dualsense_hci_event_callback_registration;

void hci_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
  if (packet_type != HCI_EVENT_PACKET) {
    return;
  }

  switch (hci_event_packet_get_type(packet)) {
    case BTSTACK_EVENT_STATE:
      if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
        dualsense_can_try_connect = true;
      }
      break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
      dualsense_can_try_connect = true;
      break;
  }
}

} // namespace

bool dualsense_can_try_connect = false;

void dualsense_hci_init() {
  dualsense_hci_event_callback_registration.callback = hci_handler;
  hci_add_event_handler(&dualsense_hci_event_callback_registration);
  hci_set_master_slave_policy(HCI_ROLE_MASTER);
  hci_power_control(HCI_POWER_ON);
}
