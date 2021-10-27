#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "bsp/board.h"
#include "tusb.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

#include "DDS.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define NOTE packet[2]

void midi_task(void);

void init_task(void);

extern uint32_t increment[8];
extern uint32_t accumulator[8];

int notes[] = {-1, -1, -1, -1, -1 ,-1 ,-1, -1};

int active_notes = 0;

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
    multicore_launch_core1(core1_entry);
}

int find_first(int value, const int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value)
            return i;
    }
    return -1;
}

void midi_task(void) {
    uint8_t packet[4];

    while (tud_midi_available()) {
        tud_midi_packet_read(packet);

        // NoteOn
        if (packet[0] == 9) {
            uint slice_num = find_first(-1, notes, 8);
            if (slice_num != -1) {
                int32_t new_frequency = (int32_t) (440 * exp2f((float) (NOTE - 69) / 12.f));
                notes[slice_num] = packet[2];
                active_notes++;
                increment[slice_num] = 114349 * new_frequency;
            }
        }

        // Note Off
        else if (packet[0] == 0x8) {
            uint slice_num = find_first(NOTE, notes, 8);
            if (slice_num != -1) {
                notes[slice_num] = -1;
                active_notes--;
                increment[slice_num] = 0;
                accumulator[slice_num] = 0;
            }
        }
    }
}

#pragma clang diagnostic pop

