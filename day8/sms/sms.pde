import controlP5.*;
import java.util.*;
import processing.serial.*;
import java.nio.charset.StandardCharsets;


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
ScrollableList idSelection;
Textfield textMessage;
Textarea messageLog;

String serialPortPath = null;
Serial serialPort = null;

byte[] stringToBytes(String s, int maxBytes)
{
  s = s.substring(0, Math.min(s.length(), maxBytes));
  
  byte[] stringBytes = s.getBytes(StandardCharsets.UTF_8);
  while (stringBytes.length > maxBytes) 
  {
    s = s.substring(0, s.length() - 1);
    stringBytes = s.getBytes(StandardCharsets.UTF_8);
  }
  
  return stringBytes;
}


void setup() {
  size(340, 600);
  
  CallbackListener toFront = new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      theEvent.getController().bringToFront();
    }
  };
  
  
  font = loadFont("HelveticaNeue-14.vlw");
  controlFont = new ControlFont(font,14);
  textFont(font);
  
  cp5 = new ControlP5(this);  
       
  //
  // Serial Port Selection
  // 
  serialPortSelection = cp5.addScrollableList("sp")
     .setPosition(20, 60)
     .setSize(300, 400)
     .setBarHeight(25)
     .setItemHeight(25)
     .onEnter(toFront)
     .close();
  
  serialPortSelection.addItem("", "");
  for (String path : Serial.list())
  {
    serialPortSelection.addItem(path, path);
  }
  
  setScrollableListFont(serialPortSelection);
  serialPortSelection.setValue(0);
  

  //
  // Select our ID
  //
  idSelection = cp5.addScrollableList("id")
     .setPosition(20, 115)
     .setSize(300, 400)
     .setBarHeight(25)
     .setItemHeight(25)
     .onEnter(toFront)
     .close();

  for (int i = 1; i < 256; i++)
  {
    idSelection.addItem("" + i, i);
  }
   
  setScrollableListFont(idSelection);
  idSelection.setValue(1);

  //
  // Message Log
  //
  messageLog = cp5.addTextarea("messageLog")
    .setPosition(20, 160)
    .setSize(300, 370)
    .setFont(font)
    .setLineHeight(14)
    .setColor(color(255, 255, 255))
    .setColorBackground(color(50, 50, 50));

   //
   // Text Message
   //
   textMessage = cp5.addTextfield("textMessage")
     .setPosition(20, height - 50)
     .setSize(300, 25);
   textMessage.getValueLabel()
     .setFont(controlFont)
     .toUpperCase(false)
     .setSize(14);
   textMessage.getCaptionLabel().setText("");
}


void setScrollableListFont(controlP5.ScrollableList l)
{
  l.getCaptionLabel()
   .setFont(controlFont)
   .toUpperCase(false)
   .setSize(14);
   
  l.getValueLabel()
   .setFont(controlFont)
   .toUpperCase(false)
   .setSize(14);
}

void draw() {
  background(0);
  
  textSize(14);
  fill(255);
  
  text("Select Serial Port", 20, 55); 
  text("Select your ID", 20, 110);
  
  checkIncomingMessage();
}


public void checkIncomingMessage()
{
  int messageLength = PACKET_LENGTH + (NUM_PREFIX_SUFFIX_BYTES * 2);
  if (serialPort == null || serialPort.available() < messageLength)
  {
    return;
  }
  
  byte[] m = serialPort.readBytes(messageLength);
  
  for (int i = 0; i < NUM_PREFIX_SUFFIX_BYTES; i++)
  {
    if (m[i] != PREFIX_BYTE || m[messageLength - 1 - i] != SUFFIX_BYTE)
    {
      serialPort.clear();
      return;
    }
  }
  
  println("relayed message received");
  String encoding = StandardCharsets.UTF_8.name();
  byte[] sliceArray = new byte[PACKET_LENGTH];
  byte senderId = m[NUM_PREFIX_SUFFIX_BYTES];
  System.arraycopy(m, NUM_PREFIX_SUFFIX_BYTES + 1, sliceArray, 0, PACKET_LENGTH - 1);
  try 
  {
    String relayedMessage = senderId + ": " + new String(sliceArray, encoding);
    relayedMessage += System.lineSeparator() + System.lineSeparator();
    messageLog.setText(messageLog.getText() + relayedMessage);
  } 
  catch (Exception e)
  {
  }
}

public void controlEvent(ControlEvent e) {
  if(serialPort != null && e.getName().equals("textMessage")) {
    String userMessage = e.getStringValue();
    byte[] userMessageBytes = stringToBytes(userMessage, PACKET_LENGTH - 1);
    int id = (Integer) idSelection.getItem((int) idSelection.getValue()).get("value");
    byte[] prefix = {PREFIX_BYTE, PREFIX_BYTE, PREFIX_BYTE, (byte) id};
    byte[] suffix = {SUFFIX_BYTE, SUFFIX_BYTE, SUFFIX_BYTE};
    
    serialPort.write(prefix);
    
    serialPort.write(userMessageBytes);
    int paddingRequired = PACKET_LENGTH - 1 - userMessageBytes.length;
    for (int i = 0; i < paddingRequired; i++)
    {
      serialPort.write((byte) 0x00);
    }
    
    serialPort.write(suffix);
    println("wrote message");
    
    String relayedMessage = id + " (you): " + userMessage;
    relayedMessage += System.lineSeparator() + System.lineSeparator();
    messageLog.setText(messageLog.getText() + relayedMessage);
    
    return;
  }
  
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
    serialPort = new Serial(this, serialPortPath, SERIAL_PORT_BAUD_RATE);
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
  }

  if (serialPort == null)
  {
   return; 
  }
}
