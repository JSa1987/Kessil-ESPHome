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