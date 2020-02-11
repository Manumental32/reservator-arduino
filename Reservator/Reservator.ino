
//https://www.instructables.com/id/Arduino-Uno-Menu-Template/

//#include <ESP8266WiFi.h>
//#include <ArduinoJson.h>

String menuItems[] = {"01 NOKIA 1100", "02 IPHONE 3G", "03 MOTO V3", "04 GALAXY S", "05 BLACKBERRY 8520", "06 MOTO START", "07 SIEMENS C65" };

// Navigation button variables
int readKey;
int savedDistance = 0;

// Menu control variables
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5) + 1;
int cursorPosition = 0;

// Creates 3 custom characters for the menu display
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};



#include <Wire.h>
#include <ESP8266WiFi.h>
const char* ssid     = "Aquinowifi";      // SSID of local network "TF-Guests";
const char* password = "Nieto1234";   // Password on network "TrocafoneGuests;
String APIKEY = "yourAPIKEYgoesHere";

WiFiClient client;
char servername[]="https://enigmatic-caverns-58480.herokuapp.com/";  // remote server we will connect to


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

//#define D0 3
//#define D1 1
//#define D2 16
//#define D3 5
//#define D4 4
//#define D5 0
//#define D6 2
//#define D7 14
//#define D8 12
//#define D9 13
//#define D10 15

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
  delay(1000);
  introTrocaReservator();
  
  // Creates the byte for the 3 custom characters
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
  
}

void introTrocaReservator() {
  lcd.setCursor(0,0);
  lcd.print("TrocaReservator");
  lcd.setCursor(0,1);
  lcd.print("V 0.0.2");     // print a simple message
  delay(1000);
  lcd.clear();
}

void loop() {
//  Serial.println("algo");
//  lcd.setCursor(0,0);
//  lcd.print("V 0.0.2");     // print a simple message
//  lcd.setCursor(0,1);
//  counter = counter + 1;
//  Serial.println(counter);
//  lcd.print(counter);     // print a simple message
//  delay(1000);
  
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
}

// This function will generate the 2 menu items that can fit on the screen. They will change as you scroll through your menu. Up and down arrows will indicate your current menu position.
void mainMenuDraw() {
  delay(100);
  //Serial.print(menuPage);
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

// When called, this function will erase the current cursor and redraw it based on the cursorPosition and menuPage variables.
void drawCursor() {
  for (int x = 0; x < 2; x++) {     // Erases current cursor
    lcd.setCursor(0, x);
    lcd.print(" ");
  }

  // The menu is set up to be progressive (menuPage 0 = Item 1 & Item 2, menuPage 1 = Item 2 & Item 3, menuPage 2 = Item 3 & Item 4), so
  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  // If the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  // If the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}


void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    Serial.println(readKey);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // This case will execute if the "forward" button is pressed
        button = 0;
        Serial.println("a");
        Serial.println(cursorPosition);

        menuItem(cursorPosition + 1);
        
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        Serial.println("b");
        
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 3:
        button = 0;
        Serial.println("c");
        
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
    }
    
    delay(100); //no borrar!
  }
  
//  while (activeButton == 0) {
//    int button;
//    readKey = analogRead(0);
//    if (readKey < 790) {
//      delay(200);
//      readKey = analogRead(0);
//    }
//    button = evaluateButton(readKey);
//    //Serial.println(readKey);
//    switch (button) {
//      case 0: // When button returns as 0 there is no action taken
//        break;
//      case 1:  // This case will execute if the "forward" button is pressed
//        button = 0;
//
//        menuItem(cursorPosition + 1);
//        
//        activeButton = 1;
//        mainMenuDraw();
//        drawCursor();
//        break;
//      case 2:
//        button = 0;
//        if (menuPage == 0) {
//          cursorPosition = cursorPosition - 1;
//          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
//        }
//        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
//          menuPage = menuPage - 1;
//          menuPage = constrain(menuPage, 0, maxMenuPages);
//        }
//
//        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
//          menuPage = menuPage - 1;
//          menuPage = constrain(menuPage, 0, maxMenuPages);
//        }
//
//        cursorPosition = cursorPosition - 1;
//        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
//
//        mainMenuDraw();
//        drawCursor();
//        activeButton = 1;
//        break;
//      case 3:
//        button = 0;
//        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
//          menuPage = menuPage + 1;
//          menuPage = constrain(menuPage, 0, maxMenuPages);
//        }
//
//        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
//          menuPage = menuPage + 1;
//          menuPage = constrain(menuPage, 0, maxMenuPages);
//        }
//
//        cursorPosition = cursorPosition + 1;
//        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
//        mainMenuDraw();
//        drawCursor();
//        activeButton = 1;
//        break;
//    }
//  }
}

// This function is called whenever a button press is evaluated. The LCD shield works by observing a voltage drop across the buttons all hooked up to A0.
int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {
    result = 1; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 400) {
    result = 3; // down
  } else if (x < 790) {
//  } else if (x < 650) {
    result = 4; // left
  }
  return result;
}

void menuItem(int itemSelected) { // Function executes when you select the 2nd item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reservar sala:");
  lcd.print(itemSelected);

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    Serial.println(button);
    switch (button) {
      case 1:  // This case will execute if the "right" button is pressed
        Serial.println("RESERVANDO...");
        lcd.setCursor(0, 1);
        lcd.print("RESERVANDO...");
        delay(1000);
        lcd.setCursor(0, 1);
        lcd.print("YA ES TUYA!   ");
        break;
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        break;
    }
  }
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
