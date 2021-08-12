# Kessil-ESPHome
ESPHome Light Component for Kassil lights. This component allows to control Kessil lights from Home Assistant via an ESPHome controller. The component has been tested with A160 lights, but should work also with A80 and A360 lights.
The ESPHome component is based on the work from SSIEB (https://github.com/ssieb/custom_components).

## Circuit
Brightness and color of Kessil lights are controlled by two 0-10v analogue signals.
![image](https://user-images.githubusercontent.com/62815008/126909597-28b7f3ed-1639-4644-8b0d-72198e0200e0.png)

A transitor and a low pass filter can be used to convert the 0-3.3v PWM signal from the controller (generated directly by the ESP8266 or by a PCA9685) into the 0-10v analogue needed for the Kessil light:
![image](https://user-images.githubusercontent.com/62815008/126909824-d7057c6c-7d81-45c9-9d99-39e8041a71b3.png)

## Configuration
In the ESPHome .yaml configuration file the external componet needs to be loaded: 
```
external_components:
  # Import the Kessil light component from GitHub
  - source:
      type: git
      url: https://github.com/JSa1987/Kessil-ESPHome
      ref: main
    components: [ kessil ]
```    

The light can be defined using the kessil platform and refering to two PWM outputs:
```
light:
  - platform: kessil
    id: reef_tank_light
    name: "Reef Tank - Light"
    brightness: reef_tank_light_brightness_pwm
    color_temperature: reef_tank_light_color_pwm
```

Two sensor can also be defined the return the actual brighness and color during transitions:
sensor:
```
sensor:
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
    unit_of_measurement: "K"
    icon: "mdi:palette-outline"
    lambda: |-
      return (int(1000000 / id(reef_tank_light).current_values.get_color_temperature() ));
```

## Home Assistant setup
The Light Entity Card from ljmerza (https://github.com/ljmerza/light-entity-card) can be used in the Home Assistant to manually control the Kessil light:

![Kessil_card](https://user-images.githubusercontent.com/62815008/129251856-3e61b97e-cc63-45ea-814a-09d8796ddee2.png)

```
type: custom:light-entity-card
header: Sunlight
entity: light.reef_tank_light
shorten_cards: false
consolidate_entities: true
child_card: false
hide_header: false
color_wheel: false
persist_features: false
brightness: true
color_temp: true
white_value: false
color_picker: false
smooth_color_wheel: false
show_slider_percent: true
full_width_sliders: true
brightness_icon: weather-sunny
white_icon: file-word-box
temperature_icon: thermometer
```
