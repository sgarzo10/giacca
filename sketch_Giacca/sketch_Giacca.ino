#include <SPI.h>  
#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  
#include <SoftwareSerial.h>
#include <dht11.h>

#define BTN 3
#define DHT11_PIN 5  
#define OLED_MOSI   11  
#define OLED_CLK   13  
#define OLED_DC    9  
#define OLED_CS    8  
#define OLED_RESET 10  
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

uint8_t indexPage = 1;
uint8_t lastIndexPage = 0;
boolean collegato = false;
String Data = "dd/mm/yyyy";
String Ora = "HH:MM";
String BattStatus = "";
String BattLevel = "";
String BLUETOOTH_BUFFER = "";
int temp = 0;
int hum = 0;
dht11 DHT;
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);  
SoftwareSerial bluetooth(6, 7); //BLUETOOTH: PIN TXD 6, PIN RXD 7

void setup()
{
  pinMode(BTN, INPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  PageStart();
  display.clearDisplay();
  Serial.println("Start!");
  delay(50);
}

void loop()
{
  BLUETOOTH_READ();
  if (BLUETOOTH_BUFFER != "")
    BLUETOOTH_COMMAND();
  DisplayPage();
  int ValueBTN = digitalRead(BTN);
  //Serial.println(ValueBTN);
  if (ValueBTN==0)
  {
    Serial.println("Premuto");
    if (indexPage==2)
      indexPage=1;
    else
      indexPage++;
    delay(300);
  }
}

void PageStart()
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Ver.0.2-Menoz/Sga");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Bt-Jacket");
  display.display();
  delay(5000);
}

void PageOra()
{
  display.clearDisplay();
  Serial.println(Data);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.print(Data);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(22, 10);
  display.print(Ora);
  display.display();
}

void PageDHT11()
{
  display.clearDisplay();
  int chk = DHT.read(DHT11_PIN);
  Serial.print("TEMP:_");
  Serial.println(DHT.temperature);
  temp = DHT.temperature;
  hum = DHT.humidity;
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temp: " + String(temp) + " C");
  display.setCursor(100, 0);
  display.setTextSize(1);
  display.print("o");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 15);
  display.print("Hum : " + String(hum) + " %");
  display.display();
}

void DisplayPage()
{
  if (indexPage != lastIndexPage)
  {
    lastIndexPage = indexPage;
    if (indexPage == 1)
      PageOra();
    if (indexPage == 2)
      PageDHT11();
  }
}
void BLUETOOTH_READ()
{
  BLUETOOTH_BUFFER = "";
  while (bluetooth.available())
  {
    Serial.println("Arrivano dati");
    BLUETOOTH_BUFFER += (char)bluetooth.read();
  }
  if (!bluetooth.available() && BLUETOOTH_BUFFER != "")
  {
    Serial.print("Messagio ricevuto: ");
    Serial.println(BLUETOOTH_BUFFER);
    bluetooth.println("Messaggio ricevuto!"+BLUETOOTH_BUFFER);
  }
}

void BLUETOOTH_COMMAND()
{
  if(BLUETOOTH_BUFFER.substring(0, 4) == "check"){
    Serial.println("Collegato!!");
    collegato = true;
  }
  if (BLUETOOTH_BUFFER.substring(0, 4) == "data"){
    Data = BLUETOOTH_BUFFER.substring(5, BLUETOOTH_BUFFER.length());
    Serial.println(Data);
  }
  if (BLUETOOTH_BUFFER.substring(0, 4) == "ora "){
    Ora = BLUETOOTH_BUFFER.substring(5, BLUETOOTH_BUFFER.length());
    Serial.println(Ora);
  }
}

