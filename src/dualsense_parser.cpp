#include "pico_dualsense/dualsense_parser.h"

#include <stdint.h>

#include "pico/time.h"

namespace {

double getAnalog(uint8_t value) {
  return value / 127.5 - 1;
}
double getAnalogUnsigned(uint8_t value) {
  return (getAnalog(value) + 1) / 2;
}

} // namespace

bool DualsenseParser::parseReport(uint8_t *packet, uint16_t size) {
  if (size != sizeof(last_report)) {
    return false;
  }
  if (packet[0] != DUALSENSE_INPUT_REPORT_HEADER || packet[1] != DUALSENSE_INPUT_REPORT_ID) {
    return false;
  }
  last_report_us = to_us_since_boot(get_absolute_time());
  writeReport(packet);
  return true;
}

bool DualsenseParser::hasReport() { return last_report.header == DUALSENSE_INPUT_REPORT_HEADER; }
uint32_t DualsenseParser::reportTimeUs() { return last_report_us; }
uint32_t DualsenseParser::reportAgeUs() {
  if (!hasReport()) {
    return UINT32_MAX;
  }
  // Grab a snapshot of the volatile value, to ensure it does not update after we fetch the time
  uint32_t last_us = last_report_us;
  return to_ms_since_boot(get_absolute_time()) - last_us;
}

double DualsenseParser::leftStickX() { return getAnalog(last_report.left_stick_x); }
double DualsenseParser::leftStickY() { return getAnalog(last_report.left_stick_y); }
double DualsenseParser::rightStickX() { return getAnalog(last_report.right_stick_x); }
double DualsenseParser::rightStickY() { return getAnalog(last_report.right_stick_y); }
double DualsenseParser::leftTrigger() { return getAnalogUnsigned(last_report.left_trigger); }
double DualsenseParser::rightTrigger() { return getAnalogUnsigned(last_report.right_trigger); }

bool DualsenseParser::squarePressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_SQUARE; }
bool DualsenseParser::crossPressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_CROSS; }
bool DualsenseParser::circlePressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_CIRCLE; }
bool DualsenseParser::trianglePressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_TRIANGLE; }
bool DualsenseParser::L1Pressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_L1; }
bool DualsenseParser::R1Pressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_R1; }
bool DualsenseParser::L2Pressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_L2; }
bool DualsenseParser::R2Pressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_R2; }
bool DualsenseParser::sharePressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_SHARE; }
bool DualsenseParser::optionsPressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_OPTIONS; }
bool DualsenseParser::L3Pressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_L3; }
bool DualsenseParser::R3Pressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_R3; }

dualsense_dpad_ordinal_t DualsenseParser::dpad() {
  uint16_t ordinal = last_report.buttons & DUALSENSE_BUTTON_MASK_DPAD;
  if (ordinal >= DUALSENSE_DPAD_NONE) {
    return DUALSENSE_DPAD_NONE;
  }
  return (dualsense_dpad_ordinal_t)ordinal;
}

bool DualsenseParser::PSPressed() { return last_report.ps_buttons & DUALSENSE_PS_BUTTON_BIT_PS; }
bool DualsenseParser::touchpadPressed() { return last_report.ps_buttons & DUALSENSE_PS_BUTTON_BIT_TOUCHPAD; }

void DualsenseParser::writeReport(const void* data) {
  const uint8_t* src = (const uint8_t*)data;
  uint8_t* dest = (uint8_t*)&last_report;
  uint n = sizeof(last_report);

  while (n--) {
    dest[n] = src[n];
  }
}
