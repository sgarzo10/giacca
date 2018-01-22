#include <SPI.h> 
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
String data = "dd/mm/yyyy";
String ora = "HH:MM";
String BLUETOOTH_BUFFER = "";
String latitude="xx.xxxxx";
String longitude="xx.xxxxx";
String city="Wonderland";
String Nota="PROVA";
uint16_t interval_DHT11=5000;
unsigned long last_sent=0;
uint8_t temp = 0;
uint8_t hum = 0;
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
  unsigned long now = millis();
  if ( now - last_sent >= interval_DHT11  ){
    read_DHT11();
    last_sent=now;
  }
  AGGIORNA_DATA();
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
    delay(200);
  }
}

void AGGIORNA_DATA(){
  if (day()<10)
    data="0"+String(day());
  else
    data=String(day());
  data=data+"/";
  if (month()<10)
    data=data+"0"+String(month());
  else
    data=data+String(month());
  data=data+"/";
  data=data+String(year());
  if (hour()<10)
    ora="0"+String(hour());
  else
    ora=String(hour());
  ora=ora+":";
  if (minute()<10)
    ora=ora+"0"+String(minute());
  else
    ora=ora+String(minute());
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
  display.setCursor(0, 10);
  display.print("Bt-Jacket");
  display.display();
  delay(2000);
}

void PageOra(){
  initPage(36, 0);
  display.print(data);
  display.setTextSize(3);
  display.setCursor(22, 10);
  display.print(ora);
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
  display.print("Lat.: "+latitude);
  display.setCursor(0, 16);
  display.print("Lon.: "+longitude);
  display.setCursor(0, 24);
  display.print(city);
  display.display();
}

void PageNota(){
  initPage(50, 0);
  display.print("NOTA:");
  display.fillRect(0,7,display.width(),1,1%2);  
  display.setTextSize(2);
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

uint8_t castMonth(String s){
  uint8_t r=0;
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
  latitude = s.substring(3,s.indexOf("LO:"));
  longitude = s.substring(s.indexOf("LO:")+3,s.indexOf("CITY:"));
  city = s.substring(s.indexOf("CITY:")+5,s.length());
  Serial.println("SET POS:");
  Serial.println("LATITUDINE: "+latitude);
  Serial.println("LONGITUDINE: "+longitude);
  Serial.println("CITY: "+city);
}

void set_DataOra(String s){
  data=s.substring(8,10)+"/"+String(castMonth(s.substring(4,7)))+"/"+s.substring(s.length()-4);
  ora=s.substring(11,13)+":"+s.substring(14,16);
  setTime(s.substring(11,13).toInt(),s.substring(14,16).toInt(),s.substring(17,19).toInt(),data.substring(0,2).toInt(),data.substring(3,5).toInt(),data.substring(6,10).toInt());
  Serial.println("SET DATA: "+data+" ORA: "+ora);
}

void BLUETOOTH_COMMAND(){
  if (BLUETOOTH_BUFFER.substring(0, 4) == "data")
    set_DataOra(BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length()));
  if (BLUETOOTH_BUFFER.substring(0, 4) == "nota"){
    Nota=BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length());
    Serial.println("SET NOTA: "+Nota);
  }  
  if (BLUETOOTH_BUFFER.substring(0, 4) == "pos ")
    set_Posizione(BLUETOOTH_BUFFER.substring(4, BLUETOOTH_BUFFER.length()));
  BLUETOOTH_BUFFER = ""; 
}
