/*
Kessil-ESPHome v1.0 by Josto Salis

ESPHome Light Component for Kassil lights. This component allows to control Kessil lights from Home Assistant via an ESPHome controller.
The component has been tested with A160 lights, but should work also with A80 and A360 lights.

https://github.com/JSa1987/Kessil-ESPHome
*/
#pragma once

#include "esphome.h"

class KessilLight : public Component, public LightOutput {
    public:

    KessilLight(FloatOutput *brightness_output, FloatOutput *color_output)
    {
        brightness_output_ = brightness_output;
        color_output_ = color_output;
    }

    LightTraits get_traits() override {
        auto traits = LightTraits();
        traits.set_supports_brightness(true);
        traits.set_supports_color_temperature(false);
        traits.set_supports_rgb(false);
        traits.set_supports_rgb_white_value(true);
        return traits;
    }

    void write_state(LightState *state) override {
        float brightness, red, green, blue, white;
        // use any of the provided current_values methods
        state->current_values_as_rgbw(&red, &green, &blue, &white, this->color_interlock_);
        state->current_values_as_brightness (&brightness);
        white = white / brightness;
        if (brightness == 0)
                {
            this->brightness_output_->set_level(0);
            this->color_output_->set_level(0);
        }
        else
        {
            this->brightness_output_->set_level(brightness);
            this->color_output_->set_level(white);
        }
        
    }
    
    protected:
    FloatOutput *brightness_output_;
    FloatOutput *color_output_;
    bool color_interlock_;
};