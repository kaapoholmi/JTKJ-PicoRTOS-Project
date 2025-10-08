
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/stdio.h"

#include "FreeRTOS.h"
#include "task.h"


#define TEMP_MIN        0
#define TEMP_MAX        40
#define LUX_MIN         100
#define LUX_MAX         1500
#define BUFFER_SIZE     64

static volatile int temp; 
static volatile int lux;


/**
 * @brief Generate a random integer within a given range.
 *
 * This helper function simulates a sensor signal by producing
 * random integer values between the given minimum and maximum limits.
 * 
 * @param min The minimum value of the range (inclusive).
 * @param max The maximum value of the range (inclusive).
 * @return A pseudo-random integer between min and max.
 *
 * @note The random number generator should be seeded once
 *       using srand() before calling this function repeatedly,
 *       for example in the main function.
 */
static int rand_in_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

/**
 * @brief FreeRTOS task that simulates sensor data generation.
 *
 * This task periodically produces random sensor values for
 * temperature and light intensity. The values are generated
 * once every second using the rand_in_range() helper function,
 * which creates pseudo-random numbers within the defined limits.
 *
 * The generated values are stored in global variables (e.g. temp, lux)
 * so that other tasks, such as the display or logging task, can access them.
 * 
 *  @note The task runs indefinitely with a 1-second delay between updates.
 *  
 */
static void sensorTask (void *arg){
    
    while (1) {
        //Generated random numbers
        temp = rand_in_range(TEMP_MIN, TEMP_MAX);
        lux  = rand_in_range(LUX_MIN,  LUX_MAX);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief FreeRTOS task that prints simulated sensor values to the console.
 *
 * This task periodically displays the current timestamp (in milliseconds since
 * system start) along with the latest simulated sensor readings for temperature
 * and light intensity. The timestamp is obtained using xTaskGetTickCount() and
 * converted to milliseconds with portTICK_PERIOD_MS.
 * 
 * @note The task runs indefinitely and updates every 1.5 seconds.
 **/

static void printTask(void *arg) {
    (void)arg;

    //Wait till someone is listening
    while (!stdio_usb_connected()){
        sleep_ms(10);
    }
    stdio_puts("=== Printing values for temperature and sensor ===\n");
    char buf[BUFFER_SIZE];

    while (1) {
        TickType_t ticks = xTaskGetTickCount();
        uint32_t ms = ticks * portTICK_PERIOD_MS;
        sprintf(buf,"time:%lu,temp:%d,lux:%d\n",(unsigned long)ms,temp,lux);
        stdio_puts(buf);

        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}

int main (void) {
    //Initialize the stdio
    stdio_init_all();
    
    //Generating random seed
    srand((unsigned)time(NULL));

    TaskHandle_t mySensorHandle = NULL;
    TaskHandle_t myPrintHandle = NULL;

    // Create tasks
    xTaskCreate(printTask, "print", 1024, NULL, 3, &myPrintHandle);
    xTaskCreate(sensorTask, "usb", 1024, NULL, 2, &mySensorHandle);

    vTaskStartScheduler();

}