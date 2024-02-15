import controlP5.*;
import java.util.*;
import processing.serial.*;
import java.nio.charset.StandardCharsets;

static final String SERIAL_PORT_PATH = "/dev/tty.usbserial-0001";
static final int CANVAS_DIM = 600;

static final int PACKET_LENGTH = 128;
static final byte PREFIX_BYTE = 0x00;
static final byte SUFFIX_BYTE = (byte) 0xff;
static final int NUM_PREFIX_SUFFIX_BYTES = 3;

static final String SELECT_SERIAL_PORT_LABEL = "";
static final int SERIAL_PORT_BAUD_RATE = 115200;


ControlP5 cp5;

PFont font;
ControlFont controlFont;

ScrollableList serialPortSelection;

String serialPortPath = null;
Serial serialPort = null;
Stack<String> tokens = new Stack<String>();

boolean inTransaction = false;

void settings()
{
  size(CANVAS_DIM, CANVAS_DIM);
}

void setup() {  
  //for (String path : Serial.list())
  //{
  //  println(path);
  //}

  serialPort = new Serial(this, SERIAL_PORT_PATH, SERIAL_PORT_BAUD_RATE);
  serialPort.setDTR(false);
  serialPort.setRTS(false);
  delay(100);
  serialPort.setDTR(true);
  serialPort.setRTS(true);
  delay(100);
  serialPort.setDTR(false);
  serialPort.setRTS(false);
  delay(100);    
  serialPort.write("912,500,9,8,22\n");

  frameRate(200);
}


void acceptCommands()
{
  String message = checkIncomingMessage();
  if (message == null)
  {
    return;  
  }
  
  message = message.replaceAll("\u0000", "");
  println("message: " + message);
  String[] parts = message.split(",(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)", -1);
  Collections.reverse(Arrays.asList(parts));
  Stack<String> tokens = new Stack<String>();
  Collections.addAll(tokens, parts);  
  
  while(!tokens.empty())
  {
    String command = tokens.pop();
    if (command.equals("bt"))
    {
      inTransaction = true;
      println("in transaction");
    }
    else if (command.equals("et"))
    {
      inTransaction = false;
      println("end transaction");
    }
    else if (command.equals("b"))
    {
      int color_r = Integer.parseInt(tokens.pop());
      int color_g = Integer.parseInt(tokens.pop());
      int color_b = Integer.parseInt(tokens.pop());
      background(color_r, color_g, color_b);
    }
    else if (command.equals("s")) 
    {
      String text = tokens.pop();
      int x = Integer.parseInt(tokens.pop());
      int y = Integer.parseInt(tokens.pop());
      text(text, x, y); 
    }
    else if (command.equals("a"))
    {
      int a = Integer.parseInt(tokens.pop());
      int b = Integer.parseInt(tokens.pop());
      int c = Integer.parseInt(tokens.pop());
      int d = Integer.parseInt(tokens.pop());
      int start = Integer.parseInt(tokens.pop());
      int stop = Integer.parseInt(tokens.pop());
      String m = tokens.pop();
      
      int mode = OPEN;
      if (m.equals("O"))
      {
        mode = OPEN;
      }
      else if (m.equals("C"))
      {
        mode = CHORD;
      }
      else if (m.equals("P"))
      {
        mode = PIE;
      }

      arc(a, b, c, d, start, stop, mode);
    }
    else if (command.equals("e"))
    {
      int x = Integer.parseInt(tokens.pop());
      int y = Integer.parseInt(tokens.pop());
      int w = Integer.parseInt(tokens.pop());
      int h = Integer.parseInt(tokens.pop());
      ellipse(x, y, w, h);
    }
    else if (command.equals("l"))
    {
      int x1 = Integer.parseInt(tokens.pop());
      int y1 = Integer.parseInt(tokens.pop());
      int x2 = Integer.parseInt(tokens.pop());
      int y2 = Integer.parseInt(tokens.pop());
      line(x1, y1, x2, y2);
    }
    else if (command.equals("r"))
    {
      rectMode(CORNER);
      int x = Integer.parseInt(tokens.pop());
      int y = Integer.parseInt(tokens.pop());
      int w = Integer.parseInt(tokens.pop());
      int h = Integer.parseInt(tokens.pop());
      rect(x, y, w, h);
    }
    else if (command.equals("cr"))
    {
      rectMode(CENTER);
      int x = Integer.parseInt(tokens.pop());
      int y = Integer.parseInt(tokens.pop());
      int w = Integer.parseInt(tokens.pop());
      int h = Integer.parseInt(tokens.pop());
      rect(x, y, w, h);
    }    
    else if (command.equals("t"))
    {
      int x1 = Integer.parseInt(tokens.pop());
      int y1 = Integer.parseInt(tokens.pop());
      int x2 = Integer.parseInt(tokens.pop());
      int y2 = Integer.parseInt(tokens.pop());
      int x3 = Integer.parseInt(tokens.pop());
      int y3 = Integer.parseInt(tokens.pop());    
      triangle(x1, y1, x2, y2, x3, y3);
    }
    else if (command.equals("f"))
    {
      int color_r = Integer.parseInt(tokens.pop());
      int color_g = Integer.parseInt(tokens.pop());
      int color_b = Integer.parseInt(tokens.pop());
      fill(color_r, color_g, color_b);
    }
    else if (command.equals("nf"))
    {
      noFill();
    }
    else if (command.equals("ns"))
    {
      noStroke();
    }  
    else if (command.equals("sw"))
    {
      int weight = Integer.parseInt(tokens.pop());
      strokeWeight(weight);
    }
    else if (command.equals("sc"))
    {
      int color_r = Integer.parseInt(tokens.pop());
      int color_g = Integer.parseInt(tokens.pop());
      int color_b = Integer.parseInt(tokens.pop());
      stroke(color_r, color_g, color_b);    
    }
    else if (command.equals("ts"))
    {
      int size = Integer.parseInt(tokens.pop());
      textSize(size);
    }
    else if (command.equals("rot"))
    {
      float angle = Float.parseFloat(tokens.pop());
      println("rot: " + angle);
      rotate(radians(angle));
    }
    else if (command.equals("tsl"))
    {
      int tx = Integer.parseInt(tokens.pop());
      int ty = Integer.parseInt(tokens.pop());
      translate(tx, ty);
    }
    else if (command.equals("psh"))
    {
      pushMatrix();
    }
    else if (command.equals("pop"))
    {
      popMatrix();
    }
    else if (command.equals("rm"))
    {
      resetMatrix();
    }
    else{
      println("Unknown command: " + command);
    }
  }
}

void draw() {
  acceptCommands();
  if (inTransaction)
  {
    while(inTransaction)
    {
      acceptCommands();
    }
  }
}


public String checkIncomingMessage()
{
  int messageLength = PACKET_LENGTH + (NUM_PREFIX_SUFFIX_BYTES * 2);
  if (serialPort == null || serialPort.available() < messageLength)
  {
    return null;
  }
  
  byte[] m = serialPort.readBytes(messageLength);
  
  for (int i = 0; i < NUM_PREFIX_SUFFIX_BYTES; i++)
  {
    if (m[i] != PREFIX_BYTE || m[messageLength - 1 - i] != SUFFIX_BYTE)
    {
      serialPort.clear();
      return null;
    }
  }
  
  byte[] sliceArray = new byte[PACKET_LENGTH];
  System.arraycopy(m, NUM_PREFIX_SUFFIX_BYTES, sliceArray, 0, PACKET_LENGTH);
  return new String(sliceArray, StandardCharsets.UTF_8);
}

public void controlEvent(ControlEvent e) { 
  int selectedIndex = (int) serialPortSelection.getValue();
  String selectedSerialPort = serialPortSelection.getItem(selectedIndex).get("name").toString();
  if (selectedSerialPort.equals(SELECT_SERIAL_PORT_LABEL))
  {
   return; 
  }

  if (!selectedSerialPort.equals(serialPortPath)) 
  {
    println("Selected a serial port.");
    if (serialPort != null)
    {
      serialPort.stop(); 
    }
    
    serialPortPath = selectedSerialPort;
    
  }

  if (serialPort == null)
  {
   return; 
  }
}
