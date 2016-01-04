#include <QueueList.h>

#include <Sparki.h> // include the sparki library
#include <QueueList.h>

float x = 0;
float y = 0;
float th = 0;
int counter = 1;
int seat = 0;
int drink = 0;
int threshold = 500;
int direction = 1;
int new_order[] = {0,0};
bool busyStatus = false;

#define TH_LOW    400
#define TH_HIGH   700
QueueList <int> seatList;
QueueList <int> drinkList;

void song()
{
  int tempo = 250;
  sparki.beep(440);
  delay(tempo);
  sparki.beep(440);
  delay(tempo);
  sparki.beep(440);
  delay(tempo);
  sparki.beep(330);
  delay(tempo);
  sparki.beep(330);
  delay(tempo);
  sparki.beep(330);
  delay(tempo);
  sparki.beep(440);
  delay(tempo);
  sparki.beep(440);
  delay(tempo);
  sparki.beep(440);
  delay(tempo);
  sparki.beep(440);
  delay(tempo*3);
  
  sparki.beep(494);
  delay(tempo);
  sparki.beep(494);
  delay(tempo);
  sparki.beep(494);
  delay(tempo);
  sparki.beep(370);
  delay(tempo);
  sparki.beep(370);
  delay(tempo);
  sparki.beep(370);
  delay(tempo);
  sparki.beep(494);
  delay(tempo*6);
}

void correctPose()
{
  // if the left sensor hits an line or edge, turn left until perpendicular
  if (sparki.lineLeft() < TH_HIGH)
  {
    while (sparki.lineRight() > TH_HIGH){
      sparki.moveLeft();
    }
  }
  // if the right sensor hits an line or edge, turn right until perpendicular
  if (sparki.lineRight() < TH_HIGH)
  {
    while (sparki.lineLeft() > TH_HIGH){
      sparki.moveRight();
    }
  }
}

void moveline()
{
  while ((sparki.lineLeft() > TH_HIGH) && (sparki.lineRight() > TH_HIGH))
  {
    sparki.moveForward();
  }
}

void localize()
{
  int i = 0;
  int wallcount =0;
  // define an array where 0 is gray and 1 is black
  int bg[2] = {0, 0};
  while(i < 3)
  {
    moveline();
    correctPose();
    sparki.moveBackward(1);
    i++;
  }
  while (wallcount<2)
  {
    moveline();
    sparki.moveForward();
    delay(80);
  
    // grey line
    if ((sparki.lineLeft() < TH_HIGH) && (sparki.lineLeft() > TH_LOW) && (sparki.lineRight() < TH_HIGH) && (sparki.lineRight() > TH_LOW))
    {
      sparki.beep(240);
      sparki.moveBackward(10);
      if (wallcount == 0)
      {
        bg[0] = 0;
        sparki.moveLeft(90);
      }
      else
      {
        bg[1] = 0;
      }
      wallcount++;
    }
    
    // black line
    else if ((sparki.lineLeft() < TH_HIGH) && (sparki.lineLeft() < TH_LOW) && (sparki.lineRight() < TH_HIGH) && (sparki.lineRight() < TH_LOW))
    {
      sparki.beep(440);
      sparki.moveBackward(10);
      if (wallcount == 0)
      {
        bg[0] = 1;
        sparki.moveLeft(90);
      }
      else
      {
        bg[1] = 1;
      }
      wallcount++;
    }
  }
    
  // now turn a specific amount
  if (bg[0] == 0 && bg[1] == 0)
  {
    sparki.moveRight(90);
  }
  if (bg[0] == 0 && bg[1] == 1)
  {
    sparki.moveLeft(180);
  }
  if (bg[0] == 1 && bg[1] == 1)
  {
    sparki.moveLeft(90);
  }
  
  moveline();
  correctPose();
  sparki.moveBackward(20);
  sparki.moveLeft(90);
  moveline();
  correctPose();
  sparki.moveForward(2);
  moveline();
  correctPose();
  sparki.moveForward(15);
  sparki.moveLeft(180);

}

void drive_to(int target)
{
  
  //counter initialized as 1
  if(counter > target)
  {
   //sparki.moveLeft(180);
   direction=0;
  }
  else
  {
   direction = 1;
  }
 //Serial.print("Target: ");
 //Serial.println(target);
   while(counter != target+direction)
   {
    sparki.moveForward(); 
    if((sparki.lineLeft() < TH_HIGH) && (sparki.lineRight() < TH_HIGH)) //line found
      {
        //Serial.print("At line: ");
        //Serial.println(counter);
        sparki.moveForward(2);
        if (direction == 1)
        {
          counter++;
        }
        else
        {
          counter--;
        }
        //sparki.print(counter);
        //sparki.updateLCD();
        if(counter == 3 && direction == 0 && target <= 2)
        {
          sparki.moveForward(20);
          sparki.moveRight(90);
        }
        else if(counter == 3 && direction == 1 && target > 2)
        {
          sparki.moveForward(16);
          sparki.moveLeft(90);
        }       
      }
      correctPose();
    }
 sparki.moveStop();
}

void straddle(int dir)
{
  // 0 is turning right
  // 1 is turning left
  if (dir==0)
  {
    while (sparki.lineCenter() > TH_HIGH)
    {
      sparki.moveRight();
    }
    // now just turn a wee bit more to straighten
    sparki.moveRight(2);
  }
  if (dir==1)
  {
    while (sparki.lineCenter() > TH_HIGH)
    {
      sparki.moveLeft();
    }
    // now just turn a wee bit more to straighten
    sparki.moveLeft(6);
  }
  sparki.moveStop();
}

void line_follow()
{
  // you have to loop this to make it work
  if (sparki.lineLeft() < TH_HIGH)
  {
    // correct by moving left
    sparki.moveLeft();
  }
  if (sparki.lineRight() < TH_HIGH)
  {
    // correct by moving right
    sparki.moveRight();
  }
  if ((sparki.lineCenter() < TH_HIGH) && (sparki.lineLeft() > TH_HIGH) && (sparki.lineRight() > TH_HIGH))
  {
    //move forward if we're straddling it correctly
    sparki.moveForward();
  }
}
    
// heres a version using the ultrasonic sensor and line following
void pick_up_drink()
{
  // first, we move the robot forward so that it's center is roughly over the line
  sparki.println("Retrieving...");
  sparki.updateLCD();
  sparki.moveForward(2);
  sparki.gripperClose();
  delay(5000);
  sparki.gripperStop();
  sparki.gripperOpen(5);
  // make the servo move around like it's doing stuff
  sparki.servo(SERVO_RIGHT);
  //delay(1000);
  sparki.servo(SERVO_LEFT);
  //delay(1000);
  // now turn right until sparki is straddling the line!
  straddle(direction);
  // move the servo to the center so we can actually use it
  sparki.servo(SERVO_CENTER);
  while (sparki.ping() > 6)
  {
    line_follow();
    // since we're looping ultrasonic, we need to delay
    delay(50);
  }
  
  //now, stop and use the grippers
  sparki.moveForward(3.5);
  sparki.gripperClose(2);
  delay(1000);
  // start turning back around
  sparki.moveRight(90);
  // keep turning until the line is straddled
  straddle(0);
  sparki.moveRight(4);
  // now move back a set distance from the drink
  sparki.moveForward(15);
  sparki.moveLeft(90);
  if(direction == 0)
  {
    counter++;
  }
  if(counter == 3)
  {
      sparki.moveForward(12);
      sparki.moveLeft(90);
  }
  //sparki.print(counter);
  //sparki.updateLCD();
}

void deliver_drink()
{
  sparki.println("Delivering...");
  sparki.updateLCD();
  sparki.moveForward(2);
  straddle(0);
  while (sparki.edgeLeft() > TH_LOW && sparki.edgeRight() > TH_LOW)
  {
    line_follow();
  }
  
  sparki.moveBackward(5);
  sparki.gripperOpen(1);
  delay(1000);
  sparki.moveBackward(4);
  sparki.moveRight(90);
  straddle(0);
  // small correction
  sparki.moveRight(4);
  sparki.moveForward(14);
  sparki.moveLeft(90);
  counter--;
  if(counter == 3)
        {
          sparki.moveForward(20);
          sparki.moveRight(90);
        }
}

void bartend(int drink, int seat)
{
  
 drive_to(drink);
 pick_up_drink();
 drive_to(seat);
 deliver_drink();
 
}
 
void setup() // code inside these brackets runs first, and only once
{
  // play 99 bottles of beer
  song();
  // localize within the corner of the bar
  localize();
  sparki.clearLCD();
  sparki.println("Get some booze!!!");
  sparki.updateLCD();

}

void loop()
{
 // read in code from the remote 
 int code = sparki.readIR();
 if(code != -1 && code-4 > 2 && code-4 <= 6 && seatList.count() <= drinkList.count())
  { //if a seat location
    seat = code - 4;
    new_order[0]=seat;
    sparki.print("Seat: ");
    sparki.println(seat);
  }
  else if(code != -1 && code-4 <= 2 && seatList.count() >= drinkList.count())
  { //if a drink location
    drink = code - 4;
    new_order[1]=drink;
    sparki.print("Drink: ");
    sparki.println(drink);
  }
  sparki.updateLCD();
  if(new_order[0] != 0 && new_order[1] !=0)
  {
   sparki.println("Order Received!");
   sparki.updateLCD();
   seatList.push(new_order[0]);
   drinkList.push(new_order[1]);
   new_order[0] = 0;
   new_order[1] = 0;
  }
  if(!(seatList.isEmpty()) && !(drinkList.isEmpty()))
  {
   bartend(drinkList.pop(),seatList.pop());
  }
  delay(500);
}

