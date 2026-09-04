#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

int main(void) {
  if (!gpio_is_ready_dt(&led))
    return 0;

  if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
    return 0;

  while (1) {
    if (gpio_pin_toggle_dt(&led) < 0)
      return 0;

    k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
  }
  return 0;
}
