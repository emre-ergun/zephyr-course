#include <zephyr/init.h>
#include <zephyr/kernel.h>

int board_early_init_hook(void) {
  printk("Board Initialized\n");
  return 0;
}

SYS_INIT(board_early_init_hook, APPLICATION, 0);
