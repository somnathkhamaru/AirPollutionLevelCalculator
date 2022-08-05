#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */
#include "MQ135.h"
BlynkTimer timer;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "e4bb1e7d382c4c209075fb9f01e22ea9";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "oneplus 5";
char pass[] = "gaurav2433";
WidgetLCD lcd(V6);
int air_quality;
int counter;
float basemq2;
float basemq4;
float basemq9;
float basemq135;
int notifytype=0;
void getReadings()
{
  counter++;
  int16_t adc0, adc1, adc2, adc3; 
  adc0 = ads.readADC_SingleEnded(0); 
  adc1 = ads.readADC_SingleEnded(1); 
  adc2 = ads.readADC_SingleEnded(2); 
  adc3 = ads.readADC_SingleEnded(3); 


  //
  Serial.print("AIN0:MQ135: ");Serial.println(adc0);
  Serial.print("AIN1:MQ9: "); Serial.println(adc1);
  Serial.print("AIN2:MQ2: "); Serial.println(adc2);
  Serial.print("AIN3:MQ4: "); Serial.println(adc3);

  MQ135 gasSensor = MQ135(adc0);
  //float rzero = gasSensor.getRZero();
  //Serial.print("ppm mq135 rzero: ");
  //Serial.println (rzero);
  float air_quality = gasSensor.getPPM();
  Serial.print("ppm mq135: "); Serial.println(air_quality);
   Serial.print("COUNTER: ");Serial.println(counter);
  if( counter == 120)
  {
    basemq2=adc2;
    basemq4=adc3;
    basemq9=adc1;
    basemq135=adc0;

    Serial.print("basemq2: ");Serial.println(basemq2);
    Serial.print("basemq4: ");Serial.println(basemq4);
    Serial.print("basemq9: ");Serial.println(basemq9);
    Serial.print("basemq135: ");Serial.println(basemq135);
   
    
    Blynk.virtualWrite(V1, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Blynk.virtualWrite(V1, "Air Pollution monitoring System (APMS) initiation completed");   
    Serial.println("System Initiation Completed");
    
    Blynk.virtualWrite(V1, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Blynk.virtualWrite(V1, "Air Pollution monitoring System (APMS) Ready");   
    Serial.println("Air Pollution monitoring System (APMS) Ready");
    lcd.clear();
    lcd.print(4,0,"System");
    lcd.print(4,1,"Ready");

     delay(10000);
  }
  else if( counter>120)
  {
    Serial.print("basemq2: ");Serial.println(basemq2);
    Serial.print("basemq4: ");Serial.println(basemq4);
    Serial.print("basemq9: ");Serial.println(basemq9);
    Serial.print("basemq135: ");Serial.println(basemq135);
    //MQ9 CO sensor - blow air to test
    float co2=0.0;
    if(basemq9>adc1)
    {
      
      co2=((basemq9-adc1)/basemq9)*100;
      
    }
    else
    {
      co2=((adc1-basemq9)/adc1)*100;
      if(co2<8.0)
      {
        Serial.print("corrected");
        basemq9=adc1;
      }
    }
    Serial.print("CO2: ");Serial.println(co2);
    Blynk.virtualWrite(V2,co2);
    //MQ4 Methane sensor - gas lighter to test
    float ch4=0.0;
    if(basemq4>adc3)
    {
      ch4=((basemq4-adc3)/basemq4)*100;
    }
    else
    {
      ch4=((adc3-basemq4)/adc3)*100;
    }
    Serial.print("CH4: ");Serial.println(ch4);
    Blynk.virtualWrite(V3, ch4);
    //MQ2 smoke sensor -use smoke
    
    float sm2=0.0;
    if(basemq2>adc2)
    {
      sm2=((basemq2-adc2)/basemq2)*100;
    }
    else
    {
      sm2=((adc2-basemq2)/adc2)*100;
    }
    Serial.print("SM2: ");Serial.println(sm2);
    Blynk.virtualWrite(V4, sm2);  
     
    Blynk.virtualWrite(V5, air_quality);
    if(air_quality>0 && air_quality<50)
    {
      Serial.println("Air Quality Good");
      lcd.clear();
      lcd.print(4,0,"Air Quality");
      lcd.print(4,1,"Good");
    }
    if(air_quality>50 && air_quality<100)
    {
      if(notifytype!=1)
      {
        Blynk.notify("Air Quality Moderate");
        notifytype=1;
      }
      Serial.println("Air Quality Moderate");
      lcd.clear();
      lcd.print(4,0,"Air Quality");
      lcd.print(4,1,"Moderate");
    }
    if(air_quality>100 && air_quality<150)
    {
      if(notifytype!=2)
      {
        Blynk.notify("Air Quality Unhealthy for Sensitive Groups");
        notifytype=2;
      }
      Serial.println("Air Quality Unhealthy for Sensitive Groups");
      lcd.clear();
      lcd.print(4,0,"Air Quality");
      lcd.print(4,1,"Unhealthy - FSG ");
    }
    if(air_quality>150 && air_quality<200)
    {
      if(notifytype!=3)
      {
        Blynk.notify("Air Quality Unhealthy");
         notifytype=3;
      }
      Serial.println("Air Quality Unhealthy");
      lcd.clear();
      lcd.print(4,0,"Air Quality");
      lcd.print(4,1,"Unhealthy");
    }
    if(air_quality>200 && air_quality<300)
    {
      if(notifytype!=4)
      {
        Blynk.notify("Air Quality Very Unhealthy");
        notifytype=4;
      }
      Serial.println("Air Quality Very Unhealthy");
      lcd.clear();
      lcd.print(4,0,"Air Quality");
      lcd.print(4,1,"Very Unhealthy");
    }
    if(air_quality>300 && air_quality<500)
    {
      if(notifytype!=5)
      {
        Blynk.notify(" Air Quality Hazardous");
        notifytype=5;
      }
      Serial.println(" Air Quality Hazardous");
      lcd.clear();
      lcd.print(4,0,"Air Quality");
      lcd.print(4,1,"Hazardous");
    }
    
  }
 
}
void setup()
{
  // Debug console
  Serial.begin(9600); 
  counter=0;
  
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V5, 0.0);
  Blynk.virtualWrite(V4, 0.0);
  Blynk.virtualWrite(V2, 0.0);
  Blynk.virtualWrite(V3, 0.0);
  ads.begin();
  lcd.clear();
  Blynk.virtualWrite(V1, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  Blynk.virtualWrite(V1, "Air Pollution monitoring System (APMS) initiation started");   
  Serial.println("System Initiation Started"); 
  lcd.clear();
  lcd.print(4,0,"Waiting for");
  lcd.print(4,1,"initiation");
  timer.setInterval(3000L,getReadings);
 
}
void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

}
