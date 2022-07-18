#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>

#ifdef __arm__
#include <bsp/bsp.h>
#endif

#define GPIO_PIN_NUM 28U
#define DELAY_S 2

/**
 * exceptionPinArr is an array of GPIO pin numbers that should be excluded
 * from the example. This may be necessary if some of the pins are already
 * used for other functions, for example, if the pins are used for UART
 * connections during debugging.
 */
 GpioHandle handle = NULL;
 
const int exceptionPinArr[] = {14, 15};

static bool IsExceptionPin(int pin)
{
    bool ret = false;

    for(int i = 0; i < sizeof(exceptionPinArr) / sizeof(int); i++)
    {
        if(exceptionPinArr[i] == pin)
        {
            ret = true;

            break;
        }
    }

    return ret;
}

void setting_mode()
{
	GpioSetMode(handle, 6, GPIO_DIR_OUT);
	GpioSetMode(handle, 26, GPIO_DIR_OUT);
	GpioSetMode(handle, 12, GPIO_DIR_OUT);
	GpioSetMode(handle, 13, GPIO_DIR_OUT);
	GpioSetMode(handle, 20, GPIO_DIR_OUT);
	GpioSetMode(handle, 21, GPIO_DIR_OUT);
}

void engine_on()
{
	GpioOut(handle, 6, 1);
	GpioOut(handle, 26, 1);
}

void move_background(int i, int j)
{
	GpioOut(handle, 12, 1);
	GpioOut(handle, 13, 0);
	GpioOut(handle, 20, 0);
	GpioOut(handle, 21, 1);
	sleep(i);
	usleep(j);
}

void move_forward( int i, int j)
{
	GpioOut(handle, 12, 0);
	GpioOut(handle, 13, 1);
	GpioOut(handle, 20, 1);
	GpioOut(handle, 21, 0);
	sleep(i);
	usleep(j);
}

void turn_left( int i, int j)
{
	GpioOut(handle, 12, 0);
	GpioOut(handle, 13, 0);
	GpioOut(handle, 20, 0);
	GpioOut(handle, 21, 1);
	sleep(i);
	usleep(j);
}

void turn_right( int i, int j)
{
	GpioOut(handle, 12, 1);
	GpioOut(handle, 13, 0);
	GpioOut(handle, 20, 0);
	GpioOut(handle, 21, 0);
	sleep(i);
	usleep(j);
}

void stop_engine()
{
	GpioOut(handle, 12, 0);
	GpioOut(handle, 13, 0);
	GpioOut(handle, 20, 0);
	GpioOut(handle, 21, 0);
}

void engine_off()
{
	GpioOut(handle, 6, 0);
	GpioOut(handle, 26, 0);
}

int main(int argc, const char *argv[])
{
    fprintf(stderr, "Start GPIO_output test\n");

#ifdef __arm__
    /**
     * Initialize board support package (BSP) driver and set configuration
     * for GPIO pins. It`s required for stdout by UART.
     */
    {
        BspError rc = BspInit(NULL);
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to initialize BSP\n");
            return EXIT_FAILURE;
        }

        rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to set mux configuration for gpio0 module\n");
            return EXIT_FAILURE;
        }
    }
#endif

    if (GpioInit())
    {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    /* initialize and setup gpio0 port */
    //GpioHandle handle = NULL;
    
    
    
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE)
    {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }
    
    setting_mode();
      engine_on();
    move_background(3, 300000);
    turn_right(0, 300000);
     
    move_background(1, 700000);
    turn_right(0, 300000);
    
    move_background(3, 300000);
    turn_right(0, 300000);
    
    move_background(1, 700000); 
    turn_right(0, 300000);
    //turn_left(1);
    
    //move_forward(4);
    stop_engine();
    engine_off();

    if(GpioClosePort(handle))
    {
        fprintf(stderr, "GpioClosePort failed\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Test finished.\n");

    return EXIT_SUCCESS;
}
