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
      lcd_.setCursor(0,1);
      lcd_.print("connecting..."); 
    }

    void process(){
      lcd_.setCursor(0,1);
      if(state.inMenu){
        lcd_.print("Select T*: ");
        lcd_.print(state.target_temperature);

      }else{
        lcd_.print("T*:");
        lcd_.print(state.target_temperature);
        lcd_.print(" T:");
        lcd_.print(state.temperatureAvg);
      }

    
      int lcd_key = read_LCD_buttons();
      /*
      switch(lcd_key){
        case btnUP:
          if(state.inMenu){
            Serial.println("pressed up");
            state.target_temperature += 0.5;
            delay(200);
          }
          break;
        case btnDOWN:
          if(state.inMenu){
            Serial.println("Pressed down");
            state.target_temperature -= 0.5;
            delay(200);
          }
          break;
        case btnSELECT:
          Serial.println("Pressed select");
          state.inMenu = true;
          break;
        case btnLEFT:
          Serial.println("Pressed left");
          state.inMenu = false;
          break;
        default:
          break;
      }
      */
    }

  private:
    LiquidCrystal lcd_;

    int read_LCD_buttons(){
      int adc_key_in = analogRead(A0); // read the value from the sensor
      //Serial.println(adc_key_in);
      if (adc_key_in < 50) return btnRIGHT;
      if (adc_key_in < 195) return btnUP;
      if (adc_key_in < 450) return btnDOWN;
      if (adc_key_in < 700) return btnLEFT;
      if (adc_key_in < 1023) return btnSELECT;
      
      return btnNONE; // when all others fail, return this...
    }
  
};
