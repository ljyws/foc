#include "drv_led.h"


static void led1_write(bool val);
static void led1_toggle(void);

static void led2_write(bool val);
static void led2_toggle(void);

static void led3_write(bool val);
static void led3_toggle(void);

gpio_op_t led1 = {
    .gpio_x = LED1_GPIO_Port,
    .pin = LED1_Pin,
	.read = NULL,
	.write = led1_write,
	.toggle = led1_toggle,
};

gpio_op_t led2 = {
    .gpio_x = LED2_GPIO_Port,
    .pin = LED2_Pin,
	.read = NULL,
	.write = led2_write,
	.toggle = led2_toggle,
};

gpio_op_t led3 = {
    .gpio_x = LED3_GPIO_Port,
    .pin = LED3_Pin,
	.read = NULL,
	.write = led3_write,
	.toggle = led3_toggle,
};

static void led1_write(bool val)
{
	gpio_op_write(&led1, val);
}

static void led1_toggle(void)
{
	gpio_op_toggle(&led1);
}

static void led2_write(bool val)
{
	gpio_op_write(&led2, val);
}

static void led2_toggle(void)
{
	gpio_op_toggle(&led2);
}

static void led3_write(bool val)
{
	gpio_op_write(&led3, val);
}

static void led3_toggle(void)
{
	gpio_op_toggle(&led3);
}


