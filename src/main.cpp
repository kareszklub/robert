#include <pico/cyw43_arch.h>
#include <cstdio>
#include "h_bridge.h"
#include "hardware/adc.h"
#include "onboard_led.h"
#include "pico/stdlib.h"
#include "track_sensor.h"

#define IR_SENSOR_PIN 26
#define IR_SENSOR_ADC_CHAN 0
#define MUX_S0_PIN 21
#define MUX_S1_PIN 20
#define MUX_S2_PIN 19

int main() {
    stdio_init_all();
    adc_init();
    cyw43_arch_init();

    // TODO: This class has been implemented for a 7-sensor prototype, so this needs to be updated
    TrackSensor track(IR_SENSOR_PIN, IR_SENSOR_ADC_CHAN, MUX_S0_PIN, MUX_S1_PIN, MUX_S2_PIN);
    Pin button(0, Pin::Direction::In, Pin::Pull::Up);
    OnboardLed led;
    HBridge hb(13, 12, 10, 11);

    // TODO: HBridge sleep pin, this should be a member of the HBridge class
    Pin eep(14);
    eep.value(1);

    // hb.drive(0xffff, 0xffff);

    // This section performs a sensor calibrating sequence
    // ----------------------------------- //
    while (button.value())
        sleep_ms(10);

    led.on();
    for (int i = 0; i < 3000; i++) {
        track.update_bounds();
        sleep_ms(1);
    }
    led.off();
    // ----------------------------------- //

    while (true) {
        float data = track.read();
        printf("%2.4f\n", data);
        sleep_ms(10);
    }
}
