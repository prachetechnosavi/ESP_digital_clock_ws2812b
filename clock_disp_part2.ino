#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#define PIN 13
#define NUMPIXELS 95
 
const char *ssid     = "your wifi ssid";
const char *password = "your wifi password";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int count = 0;
int period = 500;
unsigned long time_now = 0;
int state = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const int digits[10][7] = {
  {1,2,3,4,5,6},        // 0
  {2,3},                // 1
  {1,2,7,5,4},          // 2
  {1,2,3,4,7},          // 3
  {2,3,6,7},            // 4
  {1,6,7,3,4},          // 5
  {3,4,5,6,7,1},        // 6
  {1,2,3},              // 7
  {1,2,3,4,5,6,7},      // 8
  {1,2,3,4,6,7}         // 9
};

// Length of each digit array
const int digitSize[10] = {
  6,  // 0
  2,  // 1
  5,  // 2
  5,  // 3
  4,  // 4
  5,  // 5
  6,  // 6
  3,  // 7
  7,  // 8
  6   // 9
};

void printDigit(int n, int d, int r, int g, int b) {
  if (n < 0 || n > 9) return;
  if (d < 0 || d > 3) return;
  for(int x=1;x<8;x++)
  {
    count = 0;
    for (int i = 0; i < digitSize[n]; i++) 
    {
      if(x != digits[n][i])
      {
       count++;  
      }
    }
    if(count!=digitSize[n])
    {
     for(int j=3*(x-1);j<(3*x);j++)
     {
      pixels.setPixelColor(j+(d*21), pixels.Color(r,g,b));
     // delay(1);  
     }
    }
    else
    {
     for(int j=3*(x-1);j<(3*x);j++)
     {
       pixels.setPixelColor(j+(d*21), pixels.Color(0,0,0));
       //delay(1);  
     }
    }
  }
// delay(10);  
 //pixels.show();
}

void clearDigit(int d)
{
  if (d < 0 || d > 3) return;
  for(int i=d*21;i<21+(d*21);i++)
  {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
   // delay(1);
  }
  //delay(10);
 // pixels.show();
}

void disp2digit(unsigned int num,bool sel,uint8_t r,uint8_t g,uint8_t b)
{
  if(num>99)return;
  if(num<10)
  {
    if(sel == false)
    {
    clearDigit(0);
    printDigit(num,1,r,g,b);
    }
    else if(sel == true)
    {
    clearDigit(2);
    printDigit(num,3,r,g,b);
    }
  }
  else
  {
    unsigned int tp = num/10; //get tens place digit
    unsigned int up = num - (tp*10);//get units place digit 
    if(sel == false)
    {
      printDigit(tp,0,r,g,b);
      printDigit(up,1,r,g,b);
    }
    else if(sel == true)
    {
      printDigit(tp,2,r,g,b);
      printDigit(up,3,r,g,b);
    }
  }
}

void secblink(int con,uint8_t r,uint8_t g,uint8_t b)
{ 
  for(int i=93;i<95;i++)
  {
     pixels.setPixelColor(i, pixels.Color(r*con,g*con,b*con));
  }
  pixels.show();
}

int setampm(int hr,uint8_t r,uint8_t g,uint8_t b)
{
  int fhr;
  if(hr>=12)
  {
    fhr = hr-12;
    if(fhr == 0)//to display 12 for 12PM
    {
      fhr = 12;
    }
    pixels.setPixelColor(91, pixels.Color(r,g,b));//pm led
    pixels.setPixelColor(92, pixels.Color(0,0,0));
  }
  else
  {
    pixels.setPixelColor(92, pixels.Color(r,g,b)); //am led
    pixels.setPixelColor(91, pixels.Color(0,0,0));
    if(hr==0)//to display 12 for 12AM
    {
      fhr = 12;
    }
    else
    {
      fhr = hr;
    }
  }
   return fhr;
}

void dispday(int day_no, uint8_t r,uint8_t g,uint8_t b)
{
  int wd = 90-day_no;
  for(int i=84;i<91;i++){
    if(i==wd)
    {
     pixels.setPixelColor(i, pixels.Color(r,g,b));
    }
    else
    {
     pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
  }
}

void setup() {
 pixels.begin();
 //Serial.begin(115200);
// Serial.print("Connecting to ");
 //Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
    secblink(1,0,0,50);
    delay(250);
    secblink(0,0,0,50);
    delay(250);
 }
 timeClient.begin();
 timeClient.setTimeOffset(19800);// gmtofset of india
}

void loop() 
{
  timeClient.update();
//  if(millis() >= time_now + period){
//     time_now += period;
//     state = !state;
//     secblink(state,50,0,0);   
//  }

  int h = timeClient.getHours();
  disp2digit(setampm(h,50,0,0),false,50,0,0);

  int m = timeClient.getMinutes();
  disp2digit(m,true,50,0,0);

  int d = timeClient.getDay();
  dispday(d,50,0,0);

  if(WiFi.status() != WL_CONNECTED) 
  {
    secblink(0,0,0,50);
    delay(500);
    secblink(1,0,0,50);
    delay(500);
  }
  else
  {
    secblink(0,50,0,0); 
    delay(500);
    secblink(1,50,0,0);   
    delay(500);
  } 
}
