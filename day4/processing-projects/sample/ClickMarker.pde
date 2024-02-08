class ClickMarker{
  private final int x;
  private final int y;
  private final float rotation;
  
  public ClickMarker(int x, int y)
  {
    this.x = x;
    this.y = y;
    this.rotation = radians((float) Math.random() * 360.0);
  }
  
  public void draw()
  {
    push();
    
    colorMode(HSB, 2 * PI);
    fill(this.rotation, 2 * PI, 2 * PI);
    rectMode(CENTER);
    translate(this.x, this.y);
    rotate(this.rotation);
    rect(0, 0, 50, 50);
    
    pop();
  }
}
