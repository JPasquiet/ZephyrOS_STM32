#include <zephyr/kernel.h>         // Core Zephyr kernel API (e.g., k_sleep)
#include <zephyr/drivers/gpio.h>   // Zephyr GPIO API for controlling pins

// Retrieve device tree node identifiers for the aliases 'led1' and 'led3'.
// These aliases are typically defined in the board's .dts or overlay file.
// CAUTION, LED2 in the board is associated to led0 in the dts
#define LED1_NODE DT_ALIAS(led1)
#define LED3_NODE DT_ALIAS(led3)

// Create GPIO specification structures using Device Tree macros.
// These structures contain information about the GPIO port, pin number, and flags.
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

// Entry point of the program
int main(void)
{
    // Check if the device (GPIO port) associated with each led is ready.
    // This ensures the driver is initialized before we use it.
    if (!device_is_ready(led1.port)) {
        return -1;  // Return an error code if the device is not ready
    }
    if (!device_is_ready(led3.port)) {
        return -1;
    }

    // Configure led1 as an output and set it initially to inactive (off).
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);

    // Configure led3 as an output and set it initially to active (on).
    gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);

    // Infinite loop: toggle both LEDs every 1000 ms (1 second)
    while (1) {
        // Toggle the state of led1: if it was on, turn it off, and vice versa.
        gpio_pin_toggle_dt(&led1);

        // Same toggle operation for led3
        gpio_pin_toggle_dt(&led3);

        // Sleep the current thread for 1000 milliseconds.
        // This is a non-blocking delay that allows the RTOS to schedule other tasks.
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
