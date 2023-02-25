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
  last_report_ms = to_ms_since_boot(get_absolute_time());
  writeReport(packet);
  return true;
}

bool DualsenseParser::hasReport() { return last_report.header == DUALSENSE_INPUT_REPORT_HEADER; }
uint32_t DualsenseParser::reportAgeMs() {
  if (!hasReport()) {
    return UINT32_MAX;
  }
  // Grab a snapshot of the volatile value in a single operation
  uint32_t last_ms = last_report_ms;
  // Now the local var can be used safely in subtraction
  return to_ms_since_boot(get_absolute_time()) - last_ms;
}

double DualsenseParser::leftStickX() { return getAnalog(last_report.left_stick_x); }
double DualsenseParser::leftStickY() { return getAnalog(last_report.left_stick_y); }
double DualsenseParser::rightStickX() { return getAnalog(last_report.right_stick_x); }
double DualsenseParser::rightStickY() { return getAnalog(last_report.right_stick_y); }
double DualsenseParser::leftTrigger() { return getAnalogUnsigned(last_report.left_trigger); }
double DualsenseParser::rightTrigger() { return getAnalogUnsigned(last_report.right_trigger); }

bool DualsenseParser::crossPressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_CROSS; }
bool DualsenseParser::trianglePressed() { return last_report.buttons & DUALSENSE_BUTTON_BIT_TRIANGLE; }

void DualsenseParser::writeReport(const void* data) {
  const uint8_t* src = (const uint8_t*)data;
  uint8_t* dest = (uint8_t*)&last_report;
  uint n = sizeof(last_report);

  while (n--) {
    dest[n] = src[n];
  }
}
