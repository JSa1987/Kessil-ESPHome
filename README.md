# Kessil-ESPHome
ESPHome Light Component for Kassil lights. This component allows to control Kessil lights from Home Assistant via an ESPHome controller. The component has been tested with A160 lights, but should work also with A80 and A360 lights.

## Circuit
The brightness and color of Kessil lights are controlled with by two 0-10v analogue signals.
![image](https://user-images.githubusercontent.com/62815008/126909597-28b7f3ed-1639-4644-8b0d-72198e0200e0.png)

A transitor and a low pass filter can be used to convert the 0-3.3v PWM signal from the controller (generated directly by the ESP8266 or by a PCA9685) into the 0-10v analogue needed for the Kessil light:
![image](https://user-images.githubusercontent.com/62815008/126909824-d7057c6c-7d81-45c9-9d99-39e8041a71b3.png)

## Configuration
In the ESPHome .yaml configuration file custom light needs to be defined refering to two PWM outputs:
```
light:
  - platform: custom
    lambda: |-
      auto kessil_light = new KessilLight(id(reef_tank_light_brightness_pwm), id(reef_tank_light_color_pwm));
      App.register_component(kessil_light);
      return {kessil_light};
    lights:
      - name: "Reef Tank - Light"
        id: reef_tank_light
        gamma_correct: 1
```

The *gamma_correct* attribute needs to be set to 1.

Two sensor should also be defined the return the actual brighness and color during transitions:
sensor:
```
  - platform: template
    name: "Reef Tank - Light Brightness"
    id: reef_tank_light_brightness
    update_interval: 20s
    accuracy_decimals: 0
    unit_of_measurement: "%"
    icon: "mdi:weather-sunny"
    lambda: |-
      return (int(id(reef_tank_light).current_values.get_brightness() * 100));
  - platform: template
    name: "Reef Tank - Light Color"
    id: reef_tank_light_color
    update_interval: 20s
    accuracy_decimals: 0
    unit_of_measurement: "%"
    icon: "mdi:palette-outline"
    lambda: |-
      return (int(id(reef_tank_light).current_values.get_white() * 100));
```

## Home Assistant setup
The Light Entity Card from ljmerza (https://github.com/ljmerza/light-entity-card) can be used in the Home Assistant to control the Kessil light:

![image](https://user-images.githubusercontent.com/62815008/126910090-54548022-78fe-4fba-ac3f-d53b1315bc70.png)

```
type: custom:light-entity-card
header: Sunlight
entity: light.reef_tank_light
shorten_cards: false
color_wheel: false
persist_features: false
brightness: true
white_value: true
smooth_color_wheel: false
hide_header: false
child_card: false
brightness_icon: weather-sunny
white_icon: file-word-box
temperature_icon: thermometer
full_width_sliders: true
color_temp: false
color_picker: false
show_slider_percent: true
consolidate_entities: true
```
