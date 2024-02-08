class LEDButton
{
  public static final int BUTTON_WIDTH = 30;
  
  private final int id;
  private final int x;
  private final int y;
  private boolean isLit;
  
  public LEDButton(int id, int x, int y)
  {
    this.id = id;
    this.x = x;
    this.y = y;
  }
  
  public void draw()
  {
    push();
    
    if (isLit)
    {
      fill(0, 255, 0);
    }
    else
    {
      fill(0, 100, 0);
    }
    
    circle(x, y, BUTTON_WIDTH);
    
    pop();
  }
  
  
  public void checkClick()
  {
    double distance = Math.sqrt(Math.pow((x - mouseX), 2) + Math.pow((y - mouseY), 2));
    if (distance < BUTTON_WIDTH / 2)
    {
      isLit = !isLit;
    }
  }
}
