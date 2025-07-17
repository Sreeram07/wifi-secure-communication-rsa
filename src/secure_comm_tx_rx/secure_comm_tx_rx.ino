#include <LiquidCrystal.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial espSerial(9, 10);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {8, 7, 6, A5};
byte colPins[COLS] = {A4, A3, A2, A1};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long mod_exp(long base, long exp, long mod) {
  long result = 1;
  while (exp > 0) {
    if (exp % 2 == 1)
      result = (result * base) % mod;
    exp = exp >> 1;
    base = (base * base) % mod;
  }
  return result;
}

void setup() {
  lcd.begin(16, 2);
  espSerial.begin(9600);
  lcd.print("Enter Message:");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    lcd.setCursor(0, 1);
    lcd.print(key);
    long encrypted = mod_exp((int)key, 17, 3233); // example RSA keys
    espSerial.println(encrypted);
  }
}