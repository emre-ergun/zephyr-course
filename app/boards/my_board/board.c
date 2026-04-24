#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void board_early_init_hook(void) {}

static int board_print_init(void) {
  printk("Board Initialized\n");
  return 0;
}

SYS_INIT(board_print_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
