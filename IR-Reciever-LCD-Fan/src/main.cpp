#include <Arduino.h>
#include <LiquidCrystal.h>
#include <IRremote.hpp>


// Variables

bool fanIsOn = false;
int fanSpeedValue = 255;

// LCD Pin Vars
const int RS=8, E=7, D4=6, D5=5, D6=4, D7=3;

// IR reciever pin var
const int IR_RECIEVE_PIN = 9;

// L293D pin vars
const int IN2=12, IN1=11, EN1=10;

// Objects
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    IrReceiver.begin(IR_RECIEVE_PIN, ENABLE_LED_FEEDBACK); // Start the reciever

    pinMode(IN2, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(EN1, OUTPUT);
}

void loop() {


    if (IrReceiver.decode()){
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT){
            IrReceiver.resume();
            return;
        }

        int commandValue = IrReceiver.decodedIRData.command;
        switch(commandValue){
            case 69:
                fanIsOn = !fanIsOn;
                // Reset fanSpeedValue upon on off
                if(fanIsOn) fanSpeedValue = 255;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(fanIsOn ? "Fan on":"Fan Off");
                digitalWrite(IN1, fanIsOn ? HIGH:LOW);
                digitalWrite(IN2, fanIsOn ? LOW:HIGH);
                // Kickstart fan due to low torque
                analogWrite(EN1, fanIsOn ? 255:0);
                delay(100);
                analogWrite(EN1, fanIsOn ? fanSpeedValue:0);
                break;
            case 68:
                if (fanIsOn){
                    if (fanSpeedValue - 15 >= 80){
                        fanSpeedValue -= 15;
                        int fanSpeedPercent = int((float(fanSpeedValue)/255.0) * 100);
                        analogWrite(EN1, fanSpeedValue);
                        String fanSpeed = String(fanSpeedPercent) + "%";
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("Fan Speed:");
                        lcd.setCursor(0, 1);
                        lcd.print(fanSpeed);
                    } else {
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("Fan Speed Min");
                        delay(1000);
                        lcd.clear();
                        lcd.print("Fan on");
                    }
                } else {
                    lcd.clear();
                    lcd.print("Fan Not On");
                }
                break;
            case 67:
                if (fanIsOn){
                    if (fanSpeedValue + 15 <= 255){
                        fanSpeedValue += 15;
                        int fanSpeedPercent = int((float(fanSpeedValue)/255.0) * 100);
                        analogWrite(EN1, fanSpeedValue);
                        String fanSpeed = String(fanSpeedPercent) + "%";
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("Fan Speed:");
                        lcd.setCursor(0, 1);
                        lcd.print(fanSpeed);
                    } else {
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("Fan Speed Max");
                        delay(1000);
                        lcd.clear();
                        lcd.print("Fan on");
                    }
                } else {
                    lcd.clear();
                    lcd.print("Fan Not On");
                }
                break;
        }
        IrReceiver.resume(); // Ready for next signal
    }
    delay(500);
}
