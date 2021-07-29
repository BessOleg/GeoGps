//#include <splash.h>
//#include <Adafruit_SSD1306.h>

//#include <gfxfont.h>
//#include <Adafruit_SPITFT_Macros.h>
//#include <Adafruit_SPITFT.h>
//#include <Adafruit_GrayOLED.h>
//#include <Adafruit_GFX.h>

#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"    

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h>
//в скобках названия файла записи
static String fail ="GeoCoor3.txt";
//пин подключения sd карты

const int chipSelect = 15;
const int buttonPin = 16; 
//#define SSD1306_128_64
SSD1306Wire display(0x3c, SDA, SCL); 

static const int RXPin = 2, TXPin = 0;
static const uint32_t GPSBaud = 9600;
static double Ls =0,Ws=0,Dl=0,Dw=0,ifhdop=5.0 ;
static bool but =false;
static int k=0;

TinyGPSPlus gps;
static String flag;
SoftwareSerial ss(RXPin, TXPin);
static int ste=0,h,m,s;


void setup()
{
  Serial.begin(115200); 
  ss.begin(GPSBaud);
  display.init();
display.flipScreenVertically();
display.setFont(ArialMT_Plain_10);
pinMode(buttonPin, INPUT);


  Serial.println(F("FullExample.ino"));
  Serial.println(F("An extensive example of many interesting TinyGPS++ features"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  Serial.println(F("|Sats HDOP  Latitude   Longitude   speed   time  |"));
  Serial.println(F("|           (deg)      (deg)        (km)         |"));
  Serial.println(F("|------------------------------------------------|"));
  
if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
        flag = "Fal se";
        return;
    }
    else{
      Serial.println("SD card connected");
      flag="True";
      }
}

static int curentTime,sekund,minut=0;
static int buttonState = 0; 

void loop()
{
buttonState = digitalRead(buttonPin);
if (buttonState == HIGH&but==0){but=true; Serial.print("true");sekund=0;minut=0;h=gps.time.hour()+3,m=gps.time.minute(),s=gps.time.second();}
else if (buttonState == HIGH&but==1){but=false;Serial.print("false");}
String sput = String(gps.satellites.value());
String hdep = String(gps.hdop.hdop(),1);
display.clear(); // Очищаем экран от предыдущих данных
display.setTextAlignment(TEXT_ALIGN_CENTER); // Выравниваем текст по центру
//display.setFont(Chewy_24); // Подключаем созданный нами шрифт
display.drawString(32, 0, sput ); // Первая строка в координаты 64 по оси X и 8 по оси Y
display.drawString(64, 0,hdep);
display.drawString(32, 8, "SD card:");
display.drawString(76, 8,flag);
display.drawString(32, 16, "Save  coord:");
display.drawString(100, 16,String(ste));
display.drawString(32, 26,"Recording");
if(but==true & gps.time.isValid()){
if(gps.time.isValid())
sekund++;
if(sekund == 60){
sekund = 0;minut++;}
display.drawString(70, 26,String(minut));
display.drawString(80, 26,".");
display.drawString(90, 26,String(sekund));
}
else{
display.drawString(76, 26,"Fal se");
}
smartDelay(10);
display.drawString(32, 40,"Time:");
if(gps.time.isValid()){
display.drawString(52, 40,    String(gps.time.hour()+3));
display.drawString(66, 40,"."+String(gps.time.minute()));
display.drawString(82, 40,"."+String(gps.time.second()));}

display.drawString(60, 50,"File name: "+fail);

display.display(); // Выводим все на экран
//delay(2000);
  

  if(gps.location.isValid()==1&gps.hdop.isValid()==1&gps.hdop.hdop()<100& but==1){
  float lat1=gps.location.lat(),lng1=gps.location.lng();
  float alt1=gps.altitude.value();
  printDateTime(gps.date, gps.time);
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(lat1, gps.location.isValid(), 11, 6);
  printFloat(lng1, gps.location.isValid(), 12, 6);
  printInt(alt1, gps.altitude.isValid(),8);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 8, 2);
  //time
  File dataFile = SD.open(fail, FILE_WRITE);
  dataFile.println();
  if(dataFile)
  ste++;
  dataFile.close();
  
  }





/*if(gps.hdop.hdop()<=ifhdop && gps.hdop.isValid()==1 &&gps.location.isValid()==1){
 
  Ls = (Ls * k + lat1)/ (k+1);
  Ws = (Ws * k + lng1)/ (k+1);
  Dl = (Dl * k + abs(Ls-lat1))/ (k+1);
  Dw = (Dw * k + abs(Ws-lng1))/ (k+1); 
  
  Serial.print("|");
  Serial.print(Ls,6); 
  Serial.print(" ");
  Serial.print(Dl,6); 
  Serial.print("|");
  
  Serial.print(Ws,6);
  Serial.print(" ");
  Serial.print(Dw,6); 
  Serial.print("|");
  Serial.print(k);
  k = k + 1;

   if(gps.hdop.hdop() +0.3< ifhdop && gps.hdop.hdop() > 0.7){
    ifhdop=gps.hdop.hdop()+0.3;
    Ls=0;Ws=0;Dl=0;Dw=0;k=0;
    
    }
  }*/
  
  Serial.println();
  smartDelay(920);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
    
/*if(gps.hdop.isValid()==1&&gps.location.isValid()==1){
    // открываем файл, в который будет записана строка
    File dataFile = SD.open("test.txt", FILE_WRITE);

    if (dataFile) {
        // записываем строку в файл
        dataFile.print(dates);
        dataFile.print("|");
        dataFile.print(times);
        dataFile.print("|");
        dataFile.print(gps.satellites.value());
        dataFile.print("|");
        dataFile.print(gps.hdop.hdop());
        dataFile.print("|");
        dataFile.print(lng1,6);
        dataFile.print("|");
        dataFile.print(lat1,6);
        dataFile.print("|");
        dataFile.println(alt1);
        dataFile.close();
        Serial.println("Success!");
    } else {
        // выводим ошибку если не удалось открыть файл
        Serial.println("error opening file");
    }
}*/
}
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  smartDelay(10);
  File dataFile = SD.open(fail, FILE_WRITE);
  
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    int god = d.year() % 100;
    sprintf(sz, "%02d%02d%02d ", god, d.month(), d.day());
    Serial.print(sz);
    dataFile.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    if(s==60){
    s=0;m++;if(m==60){m=0;h++;if(h==24)h=0;}}
    char sz[32];
    sprintf(sz, "%02d%02d%02d ", h, m, s);
    Serial.print(sz);
    dataFile.print(sz);
    s++;
  }
  dataFile.close();
  //printInt(d.age(), d.isValid(), 5);
  
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  smartDelay(10);
  File dataFile = SD.open(fail, FILE_WRITE);
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    dataFile.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i){
      Serial.print(' ');
      dataFile.print(' ');
      }
  }
  dataFile.close();
  
}

static void printInt(unsigned long val, bool valid, int len)
{
  smartDelay(10);
   File dataFile = SD.open(fail, FILE_WRITE);
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  dataFile.print(sz);
  dataFile.close();
  
}
