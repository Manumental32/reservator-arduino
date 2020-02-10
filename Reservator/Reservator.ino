
//#include <ESP8266WiFi.h>
//#include <ArduinoJson.h>

#include <LiquidCrystal.h>
#define D0 3 // GPIO3 maps to Ardiuno D0
#define D1 1 // GPIO1 maps to Ardiuno D1
#define D2 16 // GPIO16 maps to Ardiuno D2
#define D3 5 // GPIO5 maps to Ardiuno D3
#define D4 4 // GPIO4 maps to Ardiuno D4
#define D5 14 // GPIO14 maps to Ardiuno D5
#define D6 12 // GPIO12 maps to Ardiuno D6
#define D7 13 // GPIO13 maps to Ardiuno D7
#define D8 0 // GPIO0 maps to Ardiuno D8
#define D9 2 // GPIO2 maps to Ardiuno D9
#define D10 15 // GPIO15 maps to Ardiuno D10

// Setting the LCD shields pins
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //for Arduino uno
//LiquidCrystal lcd(0, 2, 4, 14, 12, 13); //for Wemos D1 (se resetea :S)
LiquidCrystal lcd(D8,D9,D4,D5,D6,D7);

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons();  // para leer los botones

String result;
int  counter = 60;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);              // Inicializar el LCD
  lcd.setCursor(0,0);  
  lcd.begin(16, 2);              // Inicializar el LCD
  lcd.setCursor(0,0);
  lcd.print("TrocaReservator");     // print a simple message
  lcd.setCursor(0,1);
  lcd.print("V 0.0.1");     // print a simple message
  delay(1000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  int btn = read_LCD_buttons();            // Inicializar el LCD
  lcd.setCursor(0,0); 
  drawInstructions();
  lcd.setCursor(0, 1); // Set cursor to the bottom line
  lcd.print("BTN");     // print a simple message
  lcd.println(btn);     // print a simple message
}

int read_LCD_buttons()  
  { adc_key_in = analogRead(0);      // Leemos A0
    // Mis botones dan:  0, 145, 329,507,743
    // Y ahora los comparamos con un margen comodo
    if (adc_key_in > 700) return btnNONE;     // Ningun boton pulsado 
    if (adc_key_in < 10)   return btnRIGHT; 
    if (adc_key_in < 100)  return btnUP;
    if (adc_key_in < 300)  return btnDOWN;
    if (adc_key_in < 500)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT; 

    return btnNONE;  // Por si todo falla
  }

 // If there are common usage instructions on more than 1 of your menu items you can call this function from the sub
// menus to make things a little more simplified. If you don't have common instructions or verbage on multiple menus
// I would just delete this void. You must also delete the drawInstructions()function calls from your sub menu functions.
void drawInstructions() {
  lcd.setCursor(0, 0); // Set cursor to top line
  lcd.print("Use ");
  lcd.write(byte(1)); // Up arrow
  lcd.print("/");
  lcd.write(byte(2)); // Down arrow
  lcd.print(" buttons");
}
