#ifndef __DUALSENSE_PARSER_H__
#define __DUALSENSE_PARSER_H__

#include <stdint.h>

#include "dualsense_report.h"

class DualsenseParser {
 public:
  bool parseReport(uint8_t *packet, uint16_t size);

  bool hasReport();
  // Warning: do not use for one-liner age computation (e.g. `micros() - getFrameTimeUs()`). micros() may be
  // called first, and getFrameTimeUs() may return a greater value leading to overflow.
  uint32_t reportTimeUs();
  uint32_t reportAgeUs();

  double leftStickX();
  double leftStickY();
  double rightStickX();
  double rightStickY();
  double leftTrigger();
  double rightTrigger();

  bool squarePressed();
  bool crossPressed();
  bool circlePressed();
  bool trianglePressed();
  bool L1Pressed();
  bool R1Pressed();
  bool L2Pressed();
  bool R2Pressed();
  bool sharePressed();
  bool optionsPressed();
  bool L3Pressed();
  bool R3Pressed();

  dualsense_dpad_ordinal_t dpad();

  bool PSPressed();
  bool touchpadPressed();

 private:
  volatile dualsense_input_report last_report;
  volatile uint32_t last_report_us = 0;

  void writeReport(const void* data);
};

#endif
