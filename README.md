# pico-dualsense
Library for reading DualSense controller inputs via Pico W Bluetooth

Note: this implementation does not prevent MITM attacks, and will clear stored link keys on connection failure.

## Run Loop
Users of this library are responsible for initializing / servicing BTstack. For [example](example/CMakeLists.txt), linking against `pico_cyw43_arch_none` will handle the run loop in a background async context, but still requires calling `cyw43_arch_init()` to initialize BTstack.

## Future Work / Missing Features
* Battery level
* LED control
* Advanced inputs (touchpad, gyro/accel, etc.)
* Disconnect / power off
* More intelligent pairing
