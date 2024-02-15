import java.util.Collection;
import java.util.Collections.*;
import processing.serial.*;

static final int CIRCLE_SIZE = 30;
static final int CIRCLE_MARGIN = 10;

Serial serialPort; 
Collection<LEDButton> ledButtons;

void setup(){
  size(500, 388);
  PImage img = loadImage("background.jpg");
  image(img, 0, 0);

  ledButtons = new ArrayList<LEDButton>();
  
  for (int i = 0; i < 8; i++)
  {
    int x;
    int y;
    if (i % 2 == 0) 
    {
      x = 45;
      y = 10 + CIRCLE_SIZE + CIRCLE_MARGIN + (CIRCLE_SIZE + CIRCLE_MARGIN) * i;      
    }
    else
    {
      x = 110;
      y = 10 + CIRCLE_SIZE + CIRCLE_MARGIN + (CIRCLE_SIZE + CIRCLE_MARGIN) * i;      
    }

    LEDButton button = new LEDButton(i, x, y);
    ledButtons.add(button);
  }
  
  String serialDevicePath = null;
  String[] serialDevicePaths = Serial.list();
  for (String path : serialDevicePaths)
  {
    if (path.contains("usbmodem")) {
      serialDevicePath = path;
      break;
    }
  }
  
  if (serialDevicePath == null)
  {
    println("Could not find serial device!");
    exit();  
  }

  serialPort = new Serial(this, serialDevicePath, 9600);
  delay(500);
  serialPort.write(0);
}

void draw(){
  for (LEDButton button : ledButtons)
  {
    button.draw();  
  }
}


void mouseClicked()
{
  for (LEDButton button : ledButtons)
  {
    button.checkClick();  
  }
  
  byte ledStates = 0;
  for (LEDButton button : ledButtons)
  {
    if (button.isLit)
    {
      ledStates += Math.pow(2, button.id);
    }
  }
  
  serialPort.write(ledStates);
}
