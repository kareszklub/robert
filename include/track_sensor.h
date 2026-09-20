#pragma once

#include <array>
#include <cstdint>
#include "pin.h"

class TrackSensor {
    int adc_pin;
    int adc_chan;
    Pin s1;
    Pin s2;
    Pin s3;

    int min_value[7];
    int max_value[7];

    const int dist[7] = { -240, -150, -70, 0, 70, 150, 240 };

public:
    TrackSensor(int _adc_pin, int _adc_chan, int s1_pin, int s2_pin, int s3_pin);

    void update_bounds();

    float read();
};
