//
// Created by martin on 10/19/21.
//

#ifndef TC_DDS_DDS_H
#define TC_DDS_DDS_H

#define DDS_OUTPUT_PIN 0

#define log2_CHANNELS 3
#define CHANNELS (1 << log2_CHANNELS)

/**
 * Entry point for DDS on core 1
 *
 * This function sets up everything that DDS may need
 */
 void core1_entry();

 /**
  * PWM IRQ handler for DDS
  */
 void handle_DDS_IRQ();

#endif //TC_DDS_DDS_H
