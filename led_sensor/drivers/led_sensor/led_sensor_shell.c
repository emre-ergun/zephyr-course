#include <led_sensor.h>
#include <stdint.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

static int cmd_fetch_handler(const struct shell *sh, int argc, char **argv) {
  const struct device *dev = shell_device_get_binding(argv[1]);

  if (!dev) {
    shell_error(sh, "Could not get the device %s", argv[1]);
    return -EFAULT;
  }

  int ret = sensor_sample_fetch_chan(dev, SENSOR_CHAN_LIGHT);

  if (ret != 0) {
    shell_error(sh, "Could not fetch channel, got %d", ret);
    return -EFAULT;
  }

  shell_info(sh, "Sensor channel fetch");

  return 0;
}

static int cmd_read_handler(const struct shell *sh, int argc, char **argv) {
  const struct device *dev = shell_device_get_binding(argv[1]);

  if (!dev) {
    shell_error(sh, "Could not get the device %s", argv[1]);
    return -EFAULT;
  }

  int ret = sensor_channel_get(dev, SENSOR_CHAN_LIGHT, NULL);

  if (ret != 0) {
    shell_error(sh, "Could not get channel, got %d", ret);
    return -EFAULT;
  }

  shell_info(sh, "Sensor channel get");

  return 0;
}

static int cmd_info_handler(const struct shell *sh, int argc, char **argv) {
  const struct device *dev = shell_device_get_binding(argv[1]);

  if (!dev) {
    shell_error(sh, "Could not get the device %s", argv[1]);
    return -EFAULT;
  }

  shell_info(sh, "Device %s: %s", dev->name,
             (dev->state->init_res == 0 ? "READY" : "Not READY"));

  return 0;
}

static int cmd_set_handler(const struct shell *sh, size_t argc, char **argv) {
  // statically defined sensor
  const struct device *dev = shell_device_get_binding("led_sensor0");

  if (!dev) {
    shell_error(sh, "Could not get the device: led_sensor0");
    return -EFAULT;
  }

  int err = 0;
  uint32_t val = shell_strtoul(argv[1], 10, &err);

  if (err) {
    shell_error(sh, "Invalid integer argument: %s (err %d)", argv[1], err);
    return -EINVAL;
  }

  err = led_sensor_set_count(dev, val);

  if (err) {
    shell_error(sh, "Error to set data(%d)", err);
    return -EFAULT;
  }

  shell_info(sh, "Sensor data set to: %d", val);

  return 0;
}

static int cmd_get_handler(const struct shell *sh, int argc, char **argv) {
  // statically defined sensor
  const struct device *dev = shell_device_get_binding("led_sensor0");

  if (!dev) {
    shell_error(sh, "Could not get the device: led_sensor0");
    return -EFAULT;
  }

  uint32_t count;
  int err;
  err = led_sensor_get_count(dev, &count);

  if (err) {
    shell_error(sh, "Error to read data(%d)", err);
    return -EFAULT;
  }

  shell_info(sh, "Sensor data: %d", count);

  return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    led_sensor_subcmd,
    SHELL_CMD_ARG(fetch, NULL, "Fetch channel of Led Sensor", cmd_fetch_handler,
                  2, 0),
    SHELL_CMD_ARG(read, NULL, "Get channel of Led Sensor", cmd_read_handler, 2,
                  0),
    SHELL_CMD_ARG(info, NULL, "Get info of Led Sensor", cmd_info_handler, 2, 0),
    SHELL_CMD_ARG(set_value, NULL, "Set sensor value", cmd_set_handler, 2, 0),
    SHELL_CMD_ARG(get_value, NULL, "Get sensor value", cmd_get_handler, 1, 0),
    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(led_sensor, &led_sensor_subcmd, "Led Sensor Command", NULL);
