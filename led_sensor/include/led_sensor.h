#ifndef _LED_SENSOR_H_
#define _LED_SENSOR_H_

#include <stdint.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t led_sensor_get_instance_count(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif // _LED_SENSOR_H_
