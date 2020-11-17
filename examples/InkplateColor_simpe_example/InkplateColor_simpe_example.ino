#include "InkplateColor.h"
#include "logoImg.h"

// Create object display on Inkplate
Inkplate display;

// Store string that you want to display on the screen
const char string[] = {"e-radionica.com"};

void setup()
{
    // Activate serial (UART) communication with baud rate (speed) of 115200 baud
    Serial.begin(115200);
    Serial.print("Epaper init ");
    
    // Init epaper panel. If init fails, stop the code!
    if (display.begin())
    {
        Serial.println("ok!");
    }
    else
    {
        Serial.print("fail!");
        while(true);
    }

    // If you have problem with ghosting, uncomment this functions to additionally clean the screen.
    // Just remember to power up panel before calling this!
    // display.clean();
    // delay(1000);
     
    // Set font to be scaled up three times
    display.setTextSize(3);
    
    // Write our string (e-radionica.com) in every color of epaper (except white)
    display.setTextColor(INKPLATE_BLACK);
    display.println(string);
    display.setTextColor(INKPLATE_BLUE);
    display.println(string);
    display.setTextColor(INKPLATE_ORANGE);
    display.println(string);
    display.setTextColor(INKPLATE_GREEN);
    display.println(string);
    display.setTextColor(INKPLATE_RED);
    display.println(string);
    display.setTextColor(INKPLATE_YELLOW);
    display.println(string);
    
    // Draw some black line with first cooridinate X = 600, Y = 400 and second cooridinate X = 250, Y = 420
    display.drawLine(60, 400, 250, 420, INKPLATE_BLACK);
    
    // And the last one, draw e-radionica logo in blue color (using Inkplate image converter that can be found on inkplate.io)
    // Note: In order to display image properly you have to select Invert option on Inkplate image converter, otherwise bitmap color would be inverted
    display.drawBitmap(400, 50, logo, logo_w, logo_h, INKPLATE_BLUE);
    
    // Write image to the screen
    display.display();
    
    // Power off the screen (put it into deep sleep mode)
    display.setPanelState(false);

    // Wait for 10 seconds
    delay(10000);

    // Wake the panel up from deep sleep
    display.setPanelState(true);

    // Clear whole frame buffer
    display.clearDisplay();

    // Set text cursor on position X = 100, Y = 20
    display.setCursor(100, 20);

    // Set text color to blue and text background color to yellow
    display.setTextColor(INKPLATE_BLUE, INKPLATE_YELLOW);

    // Write some text on screen
    display.print("This is Inkplate color");

    // Display all colors of Inkplate color!
    display.drawRect(0, 300, 562, 102, INKPLATE_BLACK);
    for (int i = 0; i < 7; i++)
    {
      display.fillRect((i*80) + 1, 301, 80, 100, i);
    }

    // Write everthing to the screen
    display.display();

    // Power off the screen (put it into deep sleep mode)
    display.setPanelState(false);
}

void loop()
{
    // Nothing...
}