#include "DDS.h"

#include <hardware/irq.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"

//TODO: add carrier_frequency variable
volatile uint32_t accumulator[CHANNELS] = {0};
volatile uint32_t increment[CHANNELS] = {0};
int channel_counter = 0;
uint carry_slice = 0; // initialization of variable isn't necessary, it just feels safer to have it set in advance

void handle_DDS_IRQ()
{
    accumulator[channel_counter] += increment[channel_counter];
    channel_counter = (channel_counter < 7) ? (channel_counter + 1) : 0;
    pwm_set_chan_level(carry_slice,
                       PWM_CHAN_A,
                       26 + 52 * (accumulator[channel_counter] > (1 << 29))); // magic number is 3/4 of uint32
                       //TODO: get rid of magic number here
}

void core1_entry()
{
    carry_slice = pwm_gpio_to_slice_num(DDS_OUTPUT_PIN);
    pwm_set_enabled(carry_slice, false);
    gpio_init(DDS_OUTPUT_PIN);
    gpio_set_function(DDS_OUTPUT_PIN, GPIO_FUNC_PWM);


    // Mask IRQ output into the PWM block's interrupt line,
    // and register the interrupt handler
    pwm_clear_irq(carry_slice);
    pwm_set_irq_enabled(carry_slice, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, handle_DDS_IRQ);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Initialize PWM for carry signal
    //TODO: calculate numbers based on frequency
    pwm_set_clkdiv(carry_slice, 4.f);
    pwm_set_wrap(carry_slice, 104);
    pwm_set_chan_level(carry_slice, PWM_CHAN_A, 52);

    pwm_set_enabled(carry_slice, true);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    while (1)
        tight_loop_contents();
#pragma clang diagnostic pop
}