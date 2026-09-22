if (CONFIG_LED_SENSOR_DRIVER)
  zephyr_library()
  zephyr_library_sources(led_sensor.c)
endif()
