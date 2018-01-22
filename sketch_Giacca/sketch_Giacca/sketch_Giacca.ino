#include <SPI.h>  
#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#include <SoftwareSerial.h>
#include <dht11.h>
#include <TimeLib.h>

#define BTN 3
#define DHT11_PIN 5  
#define OLED_MOSI   11  
#define OLED_CLK   13  
#define OLED_DC    9  
#define OLED_CS    8  
#define OLED_RESET 10  

uint8_t indexPage = 1;
uint8_t lastIndexPage = 0;
String Data = "dd/mm/yyyy";
String Ora = "HH:MM";
String sData = "dd/mm/yyyy";
String sOra = "HH:MM";
String BLUETOOTH_BUFFER = "";
String Latitude="xx.xxxxx";
String Longitude="xx.xxxxx";
String Geocode="Wonderland";
String Nota="PROVA";
int interval_DHT11=5000;
long last_sent=0;
int temp = 0;
int hum = 0;
dht11 DHT;
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);  
SoftwareSerial bluetooth(6, 7); //BLUETOOTH: PIN TXD 6, PIN RXD 7

void setup(){
  pinMode(BTN, INPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  PageStart();
  Serial.println("Start!");
}

void loop(){
  processSyncMessage();
  unsigned long now = millis();
  if ( now - last_sent >= interval_DHT11  ){
    read_DHT11();
    last_sent=now;
  }
  BLUETOOTH_READ();
  if (BLUETOOTH_BUFFER != "")
    BLUETOOTH_COMMAND();
  DisplayPage();
  if (digitalRead(BTN)==0){
    Serial.println("Premuto");
    if (indexPage==5)
      indexPage=1;
    else
      indexPage++;
  }
}

void read_DHT11(){
  int chk = DHT.read(DHT11_PIN);
  temp = DHT.temperature;
  hum = DHT.humidity;
  Serial.print("TEMP:_");
  Serial.print(temp);
  Serial.print("HUM:_");
  Serial.println(hum);
}

void processSyncMessage() {
  
  unsigned long pctime;
  int HH=0;
  int MM=0;
  int SS=0;
  int dd=0;
  int mm=0;
  int yyyy=0;
  String s;
 
  if (sData!="dd/mm/yyyy" && sOra != "HH:MM")
  {
    //DATA
    s=sData.substring(0,3);
    dd=s.toInt();
    s=sData.substring(3,6);
    mm=s.toInt();
    s=sData.substring(6);
    yyyy=s.toInt();
    //ORA
    s=sOra.substring(0,3);
    HH=s.toInt();
    s=sOra.substring(3,6);
    MM=s.toInt();
    s=sOra.substring(6);
    SS=s.toInt();

    Serial.print("SYNC:");
    Serial.print(HH);
    Serial.print(":");
    Serial.print(MM);
    Serial.print(":");
    Serial.print(SS);
    Serial.print(" ");
    Serial.print(dd);
    Serial.print("/");
    Serial.print(mm);
    Serial.print("/");
    Serial.println(yyyy);    
    setTime(HH,MM,SS,dd,mm,yyyy); // Sync Arduino clock to the time received on the serial port    
    Data=String(dd)+"/"+String(mm)+"/"+String(yyyy);
    Ora=String(HH)+":"+String(MM);
    sData="dd/mm/yyyy";
    sOra="HH:MM";
  }
  if (day()<10)
    Data="0"+String(day());
  else
    Data=String(day());
  Data=Data+"/";
  if (month()<10)
    Data=Data+"0"+String(month());
  else
    Data=Data+String(month());
  Data=Data+"/";  
  if (year()<10)
    Data=Data+"0"+String(year());
  else
    Data=Data+String(year());
  if (hour()<10)
    Ora="0"+String(hour());
  else
    Ora=String(hour());
  Ora=Ora+":";
  if (minute()<10)
    Ora=Ora+"0"+String(minute());
  else
    Ora=Ora+String(minute());
}

void initPage(int cursorX, int cursorY){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(cursorX, cursorY);
}

void PageStart(){
  initPage(0, 0);
  display.print("Ver.0.2-Menoz/Sga");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Bt-Jacket");
  display.display();
  delay(2000);
}

void PageOra(){
  initPage(36, 0);
  display.print(Data);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(22, 10);
  display.print(Ora);
  display.display();
}

void PageHum(){
  initPage(40, 0);
  display.print("UMIDITA':");
  display.fillRect(0,7,display.width(),1,1%2);
  display.setTextSize(2);
  display.setCursor(40, 12);
  display.print(String(hum) + " %");
  display.display();
}


void PageTemp(){
  initPage(30, 0);
  display.print("TEMPERATURA:");
  display.fillRect(0,7,display.width(),1,1%2);
  display.setTextSize(2);
  display.setCursor(40, 12);
  display.print(String(temp) +char(167)+ "C");
  display.display();
}

void PageGPS(){
  initPage(35, 0);
  display.print("POSIZIONE:");
  display.fillRect(0,7,display.width(),1,1%2);
  display.setCursor(0, 9);
  display.print("Lat.: "+Latitude);
  display.setCursor(0, 16);
  display.print("Lon.: "+Longitude);
  display.setCursor(0, 24);
  display.print(Geocode);
  display.display();
}

void PageNota(){
  initPage(50, 0);
  display.print("NOTA:");
  display.fillRect(0,7,display.width(),1,1%2);  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print(Nota);
  display.display();
}

void DisplayPage(){
  switch(indexPage){
    case 1:
      PageOra();
    break;
    case 2:
      PageTemp();
    break;
    case 3:
      PageHum();
    break;
    case 4:
      PageGPS();
    break;
    case 5:
      PageNota(); 
    break;
   }    
}

void BLUETOOTH_READ(){
  BLUETOOTH_BUFFER = "";
  while (bluetooth.available())
    BLUETOOTH_BUFFER += (char)bluetooth.read();
  if (!bluetooth.available() && BLUETOOTH_BUFFER != ""){
    Serial.print("RX: ");
    Serial.println(BLUETOOTH_BUFFER);
    bluetooth.println("chk_"+BLUETOOTH_BUFFER);
  }
}

int castMonth(String s){
  int r=0;
  if (s=="Jan")
    r=1;
  if (s=="Feb")    
    r=2;
  if (s=="Mar")    
    r=3;
  if (s=="Apr")
    r=4;
  if (s=="May")
    r=5;
  if (s=="Jun")
    r=6;
  if (s=="Jul")
    r=7;
  if (s=="Aug")
    r=8;
  if (s=="Sep")
    r=9;
  if (s=="Oct")
    r=10;
  if (s=="Nov")
    r=11;
  if (s=="Dec")
    r=12;
  return r;
}

void set_Posizione(String s){
  int a_1=0;
  int a_2=0;
  int o_1=0;
  int o_2=0;
  int c_1=0;
  int c_2=0;
  a_1=s.indexOf('a');
  a_2=s.indexOf('a',a_1+1);
  Latitude=s.substring(a_1+1,a_2);
  o_1=s.indexOf('o');
  o_2=s.indexOf('o',o_1+1);
  Longitude=s.substring(o_1+1,o_2);
  c_1=s.indexOf('*');
  c_2=s.indexOf('*',c_1+1);
  Geocode=s.substring(c_1+1,c_2);
  Serial.println("SET POS:");
  Serial.println("LATITUDINE: "+Latitude);
  Serial.println("LONGITUDINE: "+Longitude);
  Serial.println("CITY: "+Geocode);
}

void set_DataOra(String s){
  sData=s.substring(8,10)+" "+String(castMonth(s.substring(4,7)))+" "+s.substring(s.length()-4);
  sOra=s.substring(11,19);
  Serial.println("SET DATA: "+sData+" ORA: "+sOra);
}

void BLUETOOTH_COMMAND(){
  if(BLUETOOTH_BUFFER.substring(0, 4) == "check")
    Serial.println("Collegato!!");
  if (BLUETOOTH_BUFFER.substring(0, 4) == "data")
    set_DataOra(BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length()));
  if (BLUETOOTH_BUFFER.substring(0, 4) == "ora "){
    sOra = BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length());
    Serial.print("syncOra:");
    Serial.println(Ora);
  }
  if (BLUETOOTH_BUFFER.substring(0, 4) == "nota"){
    Nota=BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length());
    Serial.println("SET NOTA: "+Nota);
  }  
  if (BLUETOOTH_BUFFER.substring(0, 4) == "pos ")
    set_Posizione(BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length()));
  BLUETOOTH_BUFFER = ""; 
}
