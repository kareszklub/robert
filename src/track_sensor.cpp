#include "track_sensor.h"
#include <hardware/adc.h>
#include <pico/time.h>

TrackSensor::TrackSensor(int _adc_pin, int _adc_chan, int s1_pin, int s2_pin, int s3_pin)
    : adc_pin(_adc_pin), adc_chan(_adc_chan), s1(s1_pin), s2(s2_pin), s3(s3_pin) {
    adc_gpio_init(adc_pin);
    for (int i = 0; i < 7; i++) {
        min_value[i] = 0xfff;
        max_value[i] = 0;
    }
}

void TrackSensor::update_bounds() {
    adc_select_input(adc_chan);
    for (int i = 1; i < 8; i++) {
        s1.value(i & 1);
        s2.value((i >> 1) & 1);
        s3.value((i >> 2) & 1);
        sleep_us(20);
        uint16_t value = adc_read();
        if (min_value[i - 1] > value) {
            min_value[i - 1] = value;
        }
        if (max_value[i - 1] < value) {
            max_value[i - 1] = value;
        }
    }
}

float TrackSensor::read() {
    adc_select_input(adc_chan);
    int avg = 0, sum = 0;
    for (int i = 1; i < 8; i++) {
        s1.value(i & 1);
        s2.value((i >> 1) & 1);
        s3.value((i >> 2) & 1);
        sleep_us(20);
        int value = (adc_read() - min_value[i - 1]) * 1000 / (max_value[i - 1] - min_value[i - 1]);
        avg += value * dist[i - 1];
        sum += value;
    }
    return (float) avg / sum;
}
