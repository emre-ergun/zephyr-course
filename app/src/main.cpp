#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(rpm_counter, LOG_LEVEL_INF);

#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

#define PULSE_NODE DT_ALIAS(pulse_in)
#if !DT_NODE_HAS_STATUS(PULSE_NODE, okay)
#error "Missing 'pulse-in' alias in this board's/overlay's devicetree";
#endif

static const struct gpio_dt_spec pulse_in = GPIO_DT_SPEC_GET(PULSE_NODE, gpios);
static struct gpio_callback pulse_cb_data;

static volatile uint32_t pulse_count;

static void pulse_isr(const struct device *dev, struct gpio_callback *cb,
                      uint32_t pins) {
  ARG_UNUSED(dev);
  ARG_UNUSED(cb);
  ARG_UNUSED(pins);

  pulse_count++;
}

int main(void) {
  if (!gpio_is_ready_dt(&led))
    return 0;

  if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
    return 0;

  if (!gpio_is_ready_dt(&pulse_in)) {
    LOG_ERR("Pulse sensor is not ready");
    return 0;
  }

  int ret = gpio_pin_configure_dt(&pulse_in, GPIO_INPUT);
  if (ret) {
    LOG_ERR("Error configuration of pulse-in: %d", ret);
    return 0;
  }

  ret = gpio_pin_interrupt_configure_dt(&pulse_in, GPIO_INT_EDGE_RISING);
  if (ret) {
    LOG_ERR("Error configuration interrupt: %d", ret);
    return 0;
  }

  gpio_init_callback(&pulse_cb_data, pulse_isr, BIT(pulse_in.pin));
  gpio_add_callback(pulse_in.port, &pulse_cb_data);

  while (1) {
    if (gpio_pin_toggle_dt(&led) < 0)
      return 0;

    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    LOG_INF("Pulses=%d", pulse_count);
  }
  return 0;
}
