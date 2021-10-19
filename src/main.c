#include <stdlib.h>
#include <stdio.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define NOTE packet[2]

void midi_task(void);

void init_task(void);


/*------------- MAIN -------------*/
int main(void) {
    tusb_init();
    init_task();

    while (1) {
        tud_task(); // tinyusb device task
        midi_task();
    }

    return 0;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {

}

// Invoked when device is unmounted
void tud_umount_cb(void) {

}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {

}


void init_task(void) {
    gpio_set_dir(25, GPIO_OUT);

}

void midi_task(void) {

}

#pragma clang diagnostic pop

