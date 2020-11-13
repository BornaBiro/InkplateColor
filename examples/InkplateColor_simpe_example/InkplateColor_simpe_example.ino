#include "InkplateColor.h"
#include "logoImg.h"
Inkplate display;

void setup() {
  // put your setup code here, to run once:
  display.begin();
  display.setTextSize(3);
  display.setTextColor(INKPLATE_BLACK);
  display.println("e-radionica.com");
  display.setTextColor(INKPLATE_BLUE);
  display.println("e-radionica.com");
  display.setTextColor(INKPLATE_ORANGE);
  display.println("e-radionica.com");
  display.setTextColor(INKPLATE_GREEN);
  display.println("e-radionica.com");
  display.setTextColor(INKPLATE_RED);
  display.println("e-radionica.com");
  display.setTextColor(INKPLATE_YELLOW);
  display.println("e-radionica.com");
  display.drawLine(60, 400, 250, 420, INKPLATE_BLACK);
  display.drawBitmap(400, 50, logo, logo_w, logo_h, INKPLATE_BLUE);
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
