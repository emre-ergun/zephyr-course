#ifndef _LED_SENSOR_H_
#define _LED_SENSOR_H_

#include <stdint.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int led_sensor_set_count(const struct device *dev, uint32_t count);
int led_sensor_get_count(const struct device *dev, uint32_t *count);

#ifdef __cplusplus
}
#endif

#endif // _LED_SENSOR_H_
