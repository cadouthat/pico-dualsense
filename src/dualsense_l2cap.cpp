#include "pico_dualsense/dualsense_l2cap.h"

#include <stdint.h>
#include <stdio.h>

#include "btstack.h"

namespace {

uint16_t outgoing_control_cid = 0;
uint16_t outgoing_interrupt_cid = 0;
uint16_t incoming_control_cid = 0;
uint16_t incoming_interrupt_cid = 0;

void on_channel_closed(uint16_t cid) {
  if (cid == outgoing_interrupt_cid) {
    outgoing_interrupt_cid = 0;
  }
  if (cid == outgoing_control_cid) {
    outgoing_control_cid = 0;
  }
  if (cid == incoming_interrupt_cid) {
    incoming_interrupt_cid = 0;
  }
  if (cid == incoming_control_cid) {
    incoming_control_cid = 0;
  }
}

void l2cap_event_handler(uint16_t channel, uint8_t *packet, uint16_t size) {
  switch (hci_event_packet_get_type(packet)) {
    case L2CAP_EVENT_INCOMING_CONNECTION:
      switch (l2cap_event_incoming_connection_get_psm(packet)) {
        case PSM_HID_CONTROL:
          incoming_control_cid = l2cap_event_incoming_connection_get_local_cid(packet);
          l2cap_accept_connection(channel);
          break;
        case PSM_HID_INTERRUPT:
          incoming_interrupt_cid = l2cap_event_incoming_connection_get_local_cid(packet);
          l2cap_accept_connection(channel);
          break;
        default:
          l2cap_decline_connection(channel);
          break;
      }
      break;

    case L2CAP_EVENT_CHANNEL_OPENED: {
      uint16_t cid = l2cap_event_channel_opened_get_local_cid(packet);
      uint8_t status = l2cap_event_channel_opened_get_status(packet);

      if (status == ERROR_CODE_SUCCESS) {
        printf("L2CAP channel opened (cid %d)\n", (int)cid);
      } else {
        if (status == L2CAP_CONNECTION_RESPONSE_RESULT_REFUSED_SECURITY) {
          // Assume remote has forgotten link key, delete it and try again
          printf("Dropping link key due to L2CAP_CONNECTION_RESPONSE_RESULT_REFUSED_SECURITY\n");
          bd_addr_t addr;
          l2cap_event_channel_opened_get_address(packet, addr);
          gap_drop_link_key_for_bd_addr(addr);
          gap_disconnect(l2cap_event_channel_opened_get_handle(packet));
        } else {
          printf("L2CAP connection failed (cid %d, status 0x%02x)\n", (int)cid, (int)status);
        }
        on_channel_closed(cid);
      }
      break;
    }

    case L2CAP_EVENT_CHANNEL_CLOSED: {
      uint16_t cid  = l2cap_event_channel_closed_get_local_cid(packet);

      printf("L2CAP channel closed (cid %d)\n", (int)cid);
      on_channel_closed(cid);
      break;
    }
  }
}

void l2cap_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
  switch (packet_type) {
    case L2CAP_DATA_PACKET:
      dualsense_interrupt_report_handler(packet, size);
      break;

    case HCI_EVENT_PACKET:
      l2cap_event_handler(channel, packet, size);
      break;
  }
}

} // namespace

void dualsense_l2cap_outgoing_connection(bd_addr_t address) {
  l2cap_create_channel(l2cap_handler, address, PSM_HID_CONTROL, 0xffff, &outgoing_control_cid);
  l2cap_create_channel(l2cap_handler, address, PSM_HID_INTERRUPT, 0xffff, &outgoing_interrupt_cid);
}

void dualsense_l2cap_init() {
  sdp_init();
  l2cap_init();
  l2cap_register_service(l2cap_handler, PSM_HID_INTERRUPT, 0xffff, gap_get_security_level());
  l2cap_register_service(l2cap_handler, PSM_HID_CONTROL, 0xffff, gap_get_security_level());
}
