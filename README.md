NOTE: There is no video in this repo as they are too large for GH. But if you wire everything correctly, use the correct code and libraries, then it should work.

# IR-Controlled Fan with LCD Feedback

This Arduino-based project demonstrates IR remote control of a DC fan using an L293D motor driver, with real-time feedback displayed on a 1602 LCD. It includes PWM speed control, on/off toggling, and visual speed percentage tracking.

## 🎯 Features

- IR remote toggles fan power, reduces and increases speed incrementally
- LCD displays fan status and current speed as a percentage
- Kick-start logic ensures reliable motor startup
- Real-time feedback and error handling for low-speed thresholds

## 🧰 Components Used

| Component            | Description                                     |
|----------------------|-------------------------------------------------|
| Arduino Uno          | Microcontroller board                           |
| IR Receiver Module   | Receives signals from IR remote                 |
| IR Remote            | Standard 21-button remote (kit-provided)        |
| L293D Motor Driver   | Controls DC motor direction and speed           |
| DC Fan Motor         | Load driven by L293D                            |
| 1602 LCD (Parallel)  | Displays fan status and speed                   |
| Jumper Wires         | For all connections                             |
| Breadboard           | Prototyping platform                            |
| Power Supply         | 9V battery or regulated DC source               |
| Potentiometer        | Used for LCD contrast adjustments               |
| 220 Ohm Resistor     | Needed for LCD                                  |
|----------------------|-------------------------------------------------|

## 🧠 Logic Overview

- **Button 69**: Toggles fan on/off
- **Button 68**: Reduces fan speed by 15 PWM units per press
- **Button 67**: Increases fan speed by 15 PWM units per press
- **PWM control**: `analogWrite()` to EN1 pin of L293D
- **LCD feedback**: Displays fan status and speed percentage

## 🐞 Common Errors & Fixes

### 1. **Random Characters on LCD**
- **Cause**: Using `analogRead()` on a PWM output pin (EN1)
- **Fix**: Replaced with a tracked `fanSpeedValue` variable

### 2. **Fan Doesn’t Start Without a Push**
- **Cause**: Motor stall torque not overcome at low PWM
- **Fix**: Added kick-start logic with `analogWrite(EN1, 255)` and `delay(100)` before settling to target speed

### 3. **Speed Down Only Applies Once**
- **Cause**: Temporary speed value used without updating global `fanSpeedValue`
- **Fix**: Updated `fanSpeedValue` after each decrement

### 4. **Percentage Display Shows 0% Too Early**
- **Cause**: Integer division truncating decimals
- **Fix**: Used `float` casting and mapped PWM to percentage with `int((float(fanSpeedValue)/255.0)*100)`

### 5. **Fan Stops at 29%**
- **Cause**: PWM value below motor’s usable threshold (~74)
- **Fix**: Set minimum PWM floor at 80 and remapped display percentage using `map(fanSpeedValue, 80, 255, 0, 100)`

### 6. **Gibberish on LCD After Power Toggle**
- **Cause**: `fanSpeedValue` reset to 0, breaking percentage logic
- **Fix**: Only reset `fanSpeedValue` when turning fan **on**, not off

### 7. **IR Remote Unresponsive**
- **Cause**: Ambient IR interference from monitor light
- **Fix**: Disabled monitor bar light; IR receiver worked reliably

## 📐 Wiring Notes

- **LCD**: RS → D8, E → D7, D4–D7 → D6–D3
- **IR Receiver**: Signal → D9
- **L293D**: IN1 → D11, IN2 → D12, EN1 → D10
- **Motor**: Connected to L293D output pins
- **Power**: 9V battery or regulated supply recommended

## 🚀 Future Improvements

- Modularize motor control logic
- Implement reverse direction toggle
- Add EEPROM save/load for last speed setting

----

