#include "zephyr/device.h"
#include <led_sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT engram_led_sensor

LOG_MODULE_REGISTER(engram_led_sensor, LOG_LEVEL_INF);

static uint8_t instance_counter;

uint8_t get_instance_count(void) { return instance_counter; }

struct led_sensor_config {
  struct gpio_dt_spec led_gpio;
};

static int led_sensor_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan) {
  const struct led_sensor_config *cfg = dev->config;
  int ret;

  if (chan != SENSOR_CHAN_ALL && chan != SENSOR_CHAN_LIGHT) {
    return -ENOTSUP;
  }

  ret = gpio_pin_set_dt(&cfg->led_gpio, 1);
  if (ret < 0) {
    LOG_ERR("[LED SENSOR(sample_fetch)]: failed to turn LED ON: %d", ret);
    return ret;
  }

  LOG_INF("[LED SENSOR(sample_fetch)]: LED turned ON");

  return 0;
}

static int led_sensor_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val) {
  const struct led_sensor_config *cfg = dev->config;
  int ret;

  if (chan != SENSOR_CHAN_LIGHT && chan != SENSOR_CHAN_ALL) {
    return -ENOTSUP;
  }

  ret = gpio_pin_set_dt(&cfg->led_gpio, 0);
  if (ret < 0) {
    LOG_ERR("[LED SENSOR(channel_get)]: failed to turn LED OFF: %d", ret);
    return ret;
  }

  LOG_INF("[LED SENSOR(channel_get)]: LED turned OFF");

  return 0;
}

static DEVICE_API(sensor, engram_led_sensor_api) = {
    .channel_get = led_sensor_channel_get,
    .sample_fetch = led_sensor_sample_fetch,
};

static int led_sensor_init(const struct device *dev) {
  const struct led_sensor_config *cfg = dev->config;
  int ret;

  if (!gpio_is_ready_dt(&cfg->led_gpio)) {
    LOG_ERR("[LED SENSOR(init)]: device not ready");
    return -ENODEV;
  }

  ret = gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);
  if (ret < 0) {
    LOG_ERR("[LED SENSOR(init)]: failed to configure LED GPIO pin: %d", ret);
    return ret;
  }

  LOG_INF("[LED SENSOR(init)]: driver initialized successfully");
  instance_counter++;

  return 0;
}

#define LED_SENSOR_INIT(inst)                                                  \
  static const struct led_sensor_config led_sensor_config_##inst = {           \
      .led_gpio = GPIO_DT_SPEC_INST_GET(inst, led_gpios),                      \
  };                                                                           \
  DEVICE_DT_INST_DEFINE(inst, led_sensor_init, NULL, NULL,                     \
                        &led_sensor_config_##inst, POST_KERNEL, 80,            \
                        &engram_led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_INIT)
