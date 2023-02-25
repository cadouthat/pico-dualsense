#ifndef __DUALSENSE_PARSER_H__
#define __DUALSENSE_PARSER_H__

#include <stdint.h>

#include "dualsense_report.h"

class DualsenseParser {
 public:
  bool parseReport(uint8_t *packet, uint16_t size);

  bool hasReport();
  uint32_t reportAgeMs();

  double leftStickX();
  double leftStickY();
  double rightStickX();
  double rightStickY();
  double leftTrigger();
  double rightTrigger();

  bool crossPressed();
  bool trianglePressed();

 private:
  volatile dualsense_input_report last_report;
  volatile uint32_t last_report_ms = 0;

  void writeReport(const void* data);
};

#endif
