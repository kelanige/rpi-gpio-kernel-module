# rpi-gpio-kernel-module
Kernel module to interface with the GPIO of a Raspberry Pi 4.

## Sample Usage

This kernel module instantiates a char device at `/dev/ecearpi_device`.
Writes to this char device will change the state of the GPIO pin.
Reads from this char device will reflect the current state of the GPIO pin.

```
# Turn GPIO pin HIGH.
$ echo "1" > /dev/ecearpi_device

# Turn GPIO pin LOW.
$ echo "0" > /dev/ecearpi_device

# Read GPIO pin state.
$ cat /dev/ecearpi_device
```
