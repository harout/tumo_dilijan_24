import processing.serial.*;


final float MAX_ANGLE = PI;
final float HALF_PI = PI / 2.0;
final float NEGATIVE_HALF_PI = -1 * HALF_PI;
final float PI_OVER_100 = PI / 100.0;

final int LF = 10;

Serial serialPort;
float percentage = 0.0;


void setup() {
  frameRate(1500);
  size(500, 300);
  
  String serialDevicePath = null;
  String[] serialDevicePaths = Serial.list();
  for (String path : serialDevicePaths)
  {
    if (path.contains("usbmodem") || path.contains("usbserial")) {
      serialDevicePath = path;
      break;
    }
  }
  
  if (serialDevicePath != null) 
  {
    serialPort = new Serial(this, serialDevicePath, 115200);
  }  
  else
  {
    println("Arduino not found.");
    exit();
  }
}


void draw() {
  background(0);
  
  // Draw the three circles used for selecting
  // a color
  noStroke();
  fill(255, 0, 0);
  circle(30, 30, 25);
  
  fill(0, 0, 255);
  circle(80, 30, 25);
  
  fill(255, 165, 0);
  circle(130, 30, 25);
  
  
  if (serialPort.available() != 0) 
  { 
    String read = serialPort.readStringUntil(LF);
    if (read == null)
    {
      return;
    }
    
    read = read.replace("\n", "");
    read = read.replace("\r", "");
    String[] parts = read.split(",");
    try {
      percentage = Integer.parseInt(parts[1]);
    } catch (Exception e) {
      println("e " + read);  
    }
  }

  int dialX = (int) (width / 2);
  int dialY = height - 30;
  
  
  push();
  fill(255, 0, 0);
  stroke(255, 0, 0);
  strokeWeight(3);
  translate(dialX, dialY);
  circle(0, 0, 35);
  rotate(NEGATIVE_HALF_PI + PI_OVER_100 * percentage);
  line(-5, 0, 0, -200);
  line(5, 0, 0, -200);
  pop(); 
}


void mouseClicked()
{
  int clr = get(mouseX, mouseY);
  int r = (clr & (255 << 16)) >> 16;
  int g = (clr & (255 << 8)) >> 8;
  int b = clr & 255;
  // Adjusting for the LED intensity / eye sensitivity
  g = g / 3;
  serialPort.write(r + "," + g + "," + b);
  System.out.println(r + "," + g + "," + b);
}
