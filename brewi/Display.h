#pragma once

#include <LiquidCrystal.h>
#include "State.h"

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

class Display{
  public:
    Display() : lcd_{D8, D9, D4, D5, D6, D7} {};

    void setup(){
      lcd_.begin(16, 2); // start the library
      lcd_.setCursor(0,0);
      lcd_.print("Slow cooker"); 
    }

    void process(){
      lcd_.setCursor(0,1);
      lcd_.print("T*:");
      lcd_.print(state.target_temperature);
      lcd_.print(" T:");
      lcd_.print(state.temperatureAvg);
    
      int lcd_key = read_LCD_buttons();
      switch(lcd_key){
        case btnUP:
          Serial.println("pressed up");
          state.target_temperature += 0.1;
          delay(100);
          break;
        case btnDOWN:
          Serial.println("Pressed down");
          state.target_temperature -= 0.1;
          delay(100);
          break;
        default:
          break;
      }
    }

  private:
    LiquidCrystal lcd_;

    int read_LCD_buttons(){
      int adc_key_in = analogRead(A0); // read the value from the sensor
      if (adc_key_in < 50) return btnRIGHT;
      if (adc_key_in < 195) return btnUP;
      if (adc_key_in < 450) return btnDOWN;
      if (adc_key_in < 555) return btnLEFT;
      if (adc_key_in < 790) return btnSELECT;
      
      return btnNONE; // when all others fail, return this...
    }
  
};
