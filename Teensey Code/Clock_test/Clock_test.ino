//This sketch  triggers daily alarms at 8:30 am and 17:45 pm.
//A Timer is triggered every 15 seconds, another timer triggers once only after 10 seconds.
#include <TimeLib.h>
#include <TimeAlarms.h>

int experimental_date = 0;
void setup()
{
  setTime(8,29,0,8,12,2021);  // hr,min,sec,day,mnth,yr
  Alarm.triggerOnce(tmConvert_t(2021,12,8,8,29,5), day_1); //  YYYY,  MM,  DD,  hh,  mm,  ss
  


  
  Alarm.timerRepeat(3, Taking_Sensor_Data);         
}

void  loop(){    
  digitalClockDisplay();
  Alarm.delay(1000);
 
}

void day_1(){
  experimental_date = experimental_date + 1;
  Serial.println("Experimental_date);    
}

void Taking_Sensor_Data(){
  Serial.println("Taking Sensor Data");    
}


void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet); 
}
