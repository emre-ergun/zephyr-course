#include "zephyr/drivers/sensor.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor0));

int main(void) {
  // LED SENSOR Device
  if (!device_is_ready(dev)) {
    LOG_ERR("[LED SENSOR]: driver not ready");
    return 0;
  }

  // APP LED with GPIO
  if (!gpio_is_ready_dt(&led)) {
    LOG_ERR("[APP LED]: device is not ready");
    return 0;
  }

  if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
    LOG_ERR("[APP LED]: failed to configure App LED pin");
    return 0;
  }

  while (1) {
    if (gpio_pin_toggle_dt(&led) < 0)
      return 0;

    LOG_INF("[LED SENSOR]: sample_fetch");
    sensor_sample_fetch(dev);

    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

    LOG_INF("[LED SENSOR]: channel_get");
    sensor_channel_get(dev, SENSOR_CHAN_LIGHT, NULL);

    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
  }
  return 0;
}
