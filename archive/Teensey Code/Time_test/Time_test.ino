#include <time.h>
#include <TimeLib.h>

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

void setup()
{
  
}

void loop()
{
  time_t s_tm = tmConvert_t(2016,11,15,0,0,0);
  Serial.println((unsigned long) s_tm);
}
