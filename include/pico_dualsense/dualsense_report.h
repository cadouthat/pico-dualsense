#ifndef __DUALSENSE_REPORT_H__
#define __DUALSENSE_REPORT_H__

#include <stdint.h>

#include "btstack.h"

constexpr uint8_t DUALSENSE_INPUT_REPORT_HEADER = (HID_MESSAGE_TYPE_DATA << 4) | HID_REPORT_TYPE_INPUT;
constexpr uint8_t DUALSENSE_INPUT_REPORT_ID = 0x01;

enum dualsense_dpad_ordinal {
  DUALSENSE_DPAD_UP = 0,
  DUALSENSE_DPAD_UP_RIGHT,
  DUALSENSE_DPAD_RIGHT,
  DUALSENSE_DPAD_DOWN_RIGHT,
  DUALSENSE_DPAD_DOWN,
  DUALSENSE_DPAD_DOWN_LEFT,
  DUALSENSE_DPAD_LEFT,
  DUALSENSE_DPAD_UP_LEFT,
};

constexpr uint16_t DUALSENSE_BUTTON_MASK_DPAD = 0b1111;
constexpr uint16_t DUALSENSE_BUTTON_BIT_SQUARE = (1 << 4);
constexpr uint16_t DUALSENSE_BUTTON_BIT_CROSS = (1 << 5);
constexpr uint16_t DUALSENSE_BUTTON_BIT_CIRCLE = (1 << 6);
constexpr uint16_t DUALSENSE_BUTTON_BIT_TRIANGLE = (1 << 7);
constexpr uint16_t DUALSENSE_BUTTON_BIT_L1 = (1 << 8);
constexpr uint16_t DUALSENSE_BUTTON_BIT_R1 = (1 << 9);
constexpr uint16_t DUALSENSE_BUTTON_BIT_L2 = (1 << 10);
constexpr uint16_t DUALSENSE_BUTTON_BIT_R2 = (1 << 11);
constexpr uint16_t DUALSENSE_BUTTON_BIT_SHARE = (1 << 12);
constexpr uint16_t DUALSENSE_BUTTON_BIT_OPTIONS = (1 << 13);
constexpr uint16_t DUALSENSE_BUTTON_BIT_L3 = (1 << 14);
constexpr uint16_t DUALSENSE_BUTTON_BIT_R3 = (1 << 15);

constexpr uint8_t DUALSENSE_PS_BUTTON_BIT_PS = (1 << 0);
constexpr uint8_t DUALSENSE_PS_BUTTON_BIT_TOUCHPAD = (1 << 1);

struct dualsense_input_report {
  uint8_t header = 0;
  uint8_t report_id = 0;
  uint8_t left_stick_x = 127;
  uint8_t left_stick_y = 127;
  uint8_t right_stick_x = 127;
  uint8_t right_stick_y = 127;
  uint16_t buttons = 0;
  uint8_t ps_buttons = 0;
  uint8_t left_trigger = 0;
  uint8_t right_trigger = 0;
} __packed;

#endif
