import java.util.Collection;
import java.util.ArrayList;

Collection<ClickMarker> clickMarkers;

void drawCoordinate(int x, int y)
{
  rectMode(CENTER);
  
  fill(255, 0, 0);
  rect(x, y, 5, 5);
  textSize(24);
  text("(" + x + "," + y + ")", x + 10, y + 15);  
}

void mouseClicked(){
  ClickMarker clickMarker = new ClickMarker(mouseX, mouseY);
  clickMarkers.add(clickMarker);
  println("click");
}

void setup()
{
  clickMarkers = new ArrayList<ClickMarker>();
  size(700, 500); 
}


void draw()
{
  colorMode(RGB, 360);
  background(150, 150, 150);
  
  drawCoordinate(10, 10);
  drawCoordinate(10, height - 30);
  drawCoordinate(width / 2, height / 2);
  drawCoordinate(width - 120, 10);
  drawCoordinate(width - 120, height - 30);
  
  for (ClickMarker cm : clickMarkers)
  {
    cm.draw();  
  }
}
