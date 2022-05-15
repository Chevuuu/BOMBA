#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

const uint8_t SEG_SUCC[] = {
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,
  SEG_A | SEG_F | SEG_E | SEG_D,
  SEG_A | SEG_F | SEG_E | SEG_D,
};

const uint8_t SEG_PASS[] = {
  SEG_E | SEG_F | SEG_A | SEG_G | SEG_B,
  SEG_E | SEG_F | SEG_A | SEG_B | SEG_C | SEG_G,
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
};

TM1637Display display(CLK, DIO);
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '4', 'S', '7'},
  {'A', 'B', 'D', 'C'},
  {'2', '5', '0', '8'},
  {'3', '6', 'P', '9'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {13, 12, 11, 10}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int k;
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

unsigned long ky;
// All segments on
void setup() {

  Serial.begin(9600);
  display.setBrightness(0x0f);
  display.setSegments(SEG_PASS);


}
int prvi = 1;
long password = 0;
long digit4 = 0;
int timer = 120;
int num;
void loop() {
  char customKey = customKeypad.getKey();
  num = customKey - 48;


  if (customKey) {

    if (customKey == 'P')
    {
      prvi = 1;
      password = 0;
      digit4 = 0;
      display.clear();
      return;
    }
    if (prvi)
    {
      Serial.println("PLANTING");
      password = num;
      digit4 = num;
      prvi = 0;
    }
    else
    {
      password = (password * 10 + num);
      digit4 = password % 10000;
    }

    display.showNumberDec(digit4, false);

    if (customKey == 'S')
    {
      if (password == 27452495)//2745246
      {
        display.setSegments(SEG_SUCC);
        Serial.println("PLANTED");
        delay(600);
        prvi = 1;
        password = 0;
        digit4 = 0;
        customKey = 'Q';
        int fi = 1;
        char s = '\0';
        while (timer)
        {
          s = '\0';
          ky = millis();
          if (prvi == 1)
          {
            int minut=timer/60;
            int sekund=timer-minut*60;
               display.showNumberDecEx(minut*100+sekund, (0x80 >> 1), true);

            //display.showNumberDec(timer, false);
          }
          while (1)
          { s = '\0';
            if (s == '\0') s = customKeypad.getKey();
            if (s != '\0' && !prvi)
            {
              num = s - 48;
              password = (password * 10 + num);
              digit4 = password % 10000;
              display.showNumberDec(digit4, false);

            }
            if (s != '\0' && prvi)
            {
              Serial.println("DEFUSING");
              prvi = 0;
              password = s - 48;
              display.showNumberDec(s - 48, false);
            }
            if (s == 'P')
            {
              prvi = 1;
              password = 0;
              digit4 = 0;
              display.clear();

            }


            if (s == 'S')
            {
              if (password == 27452495)//2745246
              {
                Serial.println("DEFUSED");
                 display.setSegments(SEG_SUCC);
                delay(200000);
              }

            }
            if (millis() - ky > 1000) break;
          }
          timer -= 1;

          Serial.println(timer);

        }
        Serial.println("BOOM");
        delay(20000);



      }

    }





  }
}
