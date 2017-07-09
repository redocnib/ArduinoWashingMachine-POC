
/*
   Author : Kiran Abraham Korah
   beep.exe@gmail.com
   http://bincoder.com/
   https://github.com/redocnib
   Fully Automatic Washing Machine Logic Board
*/


#include <Wire.h>
#include <EEPROM.h>


//-- I2C 16x2 LCD Display Support Added! 
//-- [LCD : http://ecx.images-amazon.com/images/I/51NJ2CGn63L.jpg , 
//-- I2C Interface : http://ecx.images-amazon.com/images/I/51QKT0ft6RL.jpg]

/////------------------------------------- LCD MODULE INITIALIZATION -----------------------//
#include "src/LiquidCrystal/LCD.h"
#include "src/LiquidCrystal/LiquidCrystal_I2C.h"

#define I2C_ADDR 0x3F // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
/////---------------------------------- END OF LCD MODULE INITIALIZATION -------------------//



//-------- Relay State Setups -------//
#define CLOCKWISE HIGH
#define ANTICLOCKWISE LOW
#define RELAY_ON LOW
#define RELAY_OFF HIGH
//----------------------------------//

// PORTS //
#define MotorPowerPort 2
#define MototrDirectionPort 6
#define WaterFillValvePort 4
#define WaterDrainValvePort 5
#define CONTROL_SWITCH 8

// GLOBAL PROCESS VARIABLES //
double process_start = 0L;
unsigned long process_duration = 0L;
int motor_status = CLOCKWISE;
int process = 0;
String process_name = "";
int process_max = 10;
long millis_padding = 0;
// ------------------------ //


void setup() {
  Serial.begin(9600);
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd_cleanPrint("Hello, WELCOME !!!", "--- v0.3 Alpha---");
  lcd_blinkScreen(3);
  delay(1000);
  processInit();
 
}

void loop() {
  // put your main code here, to run repeatedly:

  if (doProcess())
  {
    processReset();
    lcd_blinkScreen(3);
    lcd_cleanPrint("Washing Complete !", "--- v0.3 Alpha---");
    delay(3000);
  }
}




//-------------------------LCD Derived Methods----------------------------------//
void lcdProgressBar(double val,double outof)
{
  

}


void lcd_blinkScreen(int t)
{
  for (int i = 0; i < t; i++)
  {
    lcd.setBacklight(LOW);
    delay(500);
    lcd.setBacklight(HIGH);
    delay(500);
  }
}

void lcd_cleanPrint(String line1, String line2)
{
  delay(100);
  lcd.clear();
  lcd.home ();
  lcd.setCursor (0, 0);
  lcd.print(line1);
  lcd.setCursor (0, 1);
  lcd.print(line2);
  delay(100);
}

void lcd_Clean()
{
  delay(100);
  lcd.clear();
  lcd.home ();
}


//------------------PROCESS AND OPERATIONS ---------------------------------//


boolean doProcess() //Return is Completed !
{

  if (process == 0)drainWater(1);
  else if (process == 1)fillWater(5);
  //doSoak(2);
  else if (process == 2)doWash(4000, 10, "WASHING DRESS");
  else if (process == 3)drainWater(2);
  else if (process == 4)fillWater(5);
  else if (process == 5)doWash(5000, 8, "RINSING DRESS 1");
  else if (process == 6)drainWater(3);
  else if (process == 7)fillWater(5);
  else if (process == 8)doWash(6000, 7, "RINSING DRESS 2");
  else if (process == 9)drainWater(2);
  else if (process == 10)doSpin(3, CLOCKWISE);
  else return true;

  processUpdate();
  return false;
}




double process_pivot = 0;
void processUpdate()
{

  double process_current = millis_padded();


  if ((process_current - process_start) >= process_duration)
  {
    process++;
    processReset();
    delay(1000);
  }
  else
  {
    if ((process_current - process_pivot) > 1000) //If past a minimum of one sec
    {
      //double remain=(process_remain/1000)/60;
      process_pivot = process_current;


      lcd_Clean();
      lcd.setCursor (0, 0);
      lcd.print(process_name);
      lcd.setCursor (0, 1);


      process_current = (process_current - process_start);

      process_current = process_current / 1000;


      if (process_current >= 60)
      {
        process_current = process_current / 60;
        lcd.print((int)process_current);
        lcd.print("Min");
      }
      else
      {
        lcd.print((int)process_current);
        lcd.print("Sec");
      }

      lcd.print("/");

      process_current = process_duration / 1000;
      if (process_current >= 60)
      {
        process_current = process_current / 60;
        lcd.print((int)process_current);
        lcd.print("Min");
      }
      else
      {
        lcd.print((int)process_current);
        lcd.print("Sec");
      }


    }
  }
}

void processInit()
{
  pinMode(MotorPowerPort, OUTPUT);
  pinMode(MototrDirectionPort, OUTPUT);
  pinMode(WaterFillValvePort, OUTPUT);
  pinMode(WaterDrainValvePort, OUTPUT);

  pinMode(CONTROL_SWITCH, INPUT_PULLUP);


  process_duration = 0;
  process = 0;
  process_start = millis_padded();


  motor_status = CLOCKWISE;
  digitalWrite(MotorPowerPort, RELAY_OFF);
  digitalWrite(MototrDirectionPort, CLOCKWISE);
  digitalWrite(WaterFillValvePort, RELAY_OFF);
  digitalWrite(WaterDrainValvePort, RELAY_OFF);
}

void processReset()
{
 
  process_start = millis_padded();
  process_duration = 0;
  motor_status = CLOCKWISE;
  digitalWrite(MotorPowerPort, RELAY_OFF);
  digitalWrite(MototrDirectionPort, CLOCKWISE);
  digitalWrite(WaterFillValvePort, RELAY_OFF);
  digitalWrite(WaterDrainValvePort, RELAY_OFF);
}


void setProcessDuration(int mins)
{
  process_duration = mins2millis(mins);
}

//---------------------Process In Detail --------------------------------------//
void doWash(int cycle_delay, int time_mins, String title)
{
  process_name = title;
  setProcessDuration(time_mins);
  if (motor_status == CLOCKWISE)motor_status = ANTICLOCKWISE;
  else motor_status = CLOCKWISE;
  digitalWrite(MotorPowerPort, RELAY_OFF); //TURN OFF MAIN POWER TO MOTOR
  delay(1000);
  digitalWrite(MototrDirectionPort, motor_status); //SET MOTOR DIRECTION
  delay(1000);
  digitalWrite(MotorPowerPort, RELAY_ON); //TURN ON MAIN POWER TO MOTOR
  delay(cycle_delay);
}


void fillWater(int time_mins)
{
  process_name = "FILLING WATER";
  setProcessDuration(time_mins);
  digitalWrite(WaterFillValvePort, RELAY_ON);
}


void drainWater(int time_mins)
{
  process_name = "DRAINING WATER";
  setProcessDuration(time_mins);
  digitalWrite(WaterDrainValvePort, RELAY_ON);
}

void doSpin(int time_mins, int DIR)
{
  process_name = "DRY SPINNING";
  setProcessDuration(time_mins);
  digitalWrite(WaterDrainValvePort, RELAY_ON);
  delay(3000);
  digitalWrite(MototrDirectionPort, CLOCKWISE);
  delay(1000);
  digitalWrite(MotorPowerPort, RELAY_ON);
}

void doSoak(int time_mins)
{
  process_name = "SOAKING DRESS";
  setProcessDuration(time_mins);
  delay(1000);
  //JUST WAIT :)
}

//---------------------------------------------------------------------------------------//


unsigned long mins2millis(int mins)
{
  return (((unsigned long)mins) * 60 * 1000);
}


double millis_padded()
{
  return (millis() + millis_padding);
}




