#include <inttypes.h>
#include <stdio.h>

#include "btstack.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "pico_dualsense/pico_dualsense.h"

// An easy way to find your controller MAC is to pair it with your phone and look in settings
constexpr const char* REMOTE_ADDR_STRING = "FF:FF:FF:FF:FF:FF"; // YOUR CONTROLLER MAC HERE

int main() {
  stdio_init_all();

  if (!dualsense_bluetooth_init()) {
    printf("Failed to init bluetooth\n");
    while (1) tight_loop_contents();
  }

  bd_addr_t remote_addr;
  if (sscanf_bd_addr(REMOTE_ADDR_STRING, remote_addr) != 1) {
    printf("Failed to parse target address!\n");
    while (1) tight_loop_contents();
  }

  uint32_t last_print_ms = 0;
  while (1) {
    dualsense_auto_connect(remote_addr);

    uint32_t now_ms = to_us_since_boot(get_absolute_time());
    if (now_ms - last_print_ms > 100) {
      if (dualsense_parser.crossPressed()) {
        printf("X is pressed\n");
        last_print_ms = now_ms;
      }
    }

    sleep_ms(10);
  }
  return 0;
}
