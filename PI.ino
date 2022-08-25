/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLgE8D0EEK"
#define BLYNK_DEVICE_NAME "Penulisan ilmiah"
#define BLYNK_AUTH_TOKEN "TIfFaYyYHLj5aR3N82H8PX6RvIpi7rvM"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
// Suhu
#include <OneWire.h>              //Library komunikasi one wire
#include <DallasTemperature.h>    //Library DS18B20
#define ONE_WIRE_BUS D5           // Inisialisasi pin komunikasi di D2
#define sensor D5                 // Inisialisasi sensor di D2
OneWire oneWire(ONE_WIRE_BUS);    // Mengenalkan pin komunikasi
DallasTemperature Suhu(&oneWire); // Memasukkan pin sensor
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;

const int sensorPh = A0;
float Po = 0;
float PH_step;
int nilai_analogPH;
float teganganPH;
const int buzz = D10;

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

// suhu
float temp;

const int R = D13;
const int G = D12;
const int B = D11;
float PH4 = 5.0;
float PH7 = 3.9;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(sensorPh, INPUT);
  Wire.begin();
  lcd.begin();
  //  pinMode(buzz, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  lcd.print("Jenis : ");
  lcd.setCursor(0, 1);
  lcd.print("Ph:");
  lcd.setCursor(8, 1);
  lcd.print("temp:");
}

void loop()
{
  Blynk.run();

  Suhu.requestTemperatures();
  temp = Suhu.getTempCByIndex(0);
  Blynk.virtualWrite(V5, temp);
  Serial.print("Suhu : ");
  Serial.println(temp);
  nilai_analogPH = analogRead(sensorPh);
  Serial.print("Nilai ADC PH :");
  Serial.println(nilai_analogPH);
  teganganPH = 5 / 1024.0 * nilai_analogPH;
  //  teganganPH = (5 / 1023.0) * nilai_analogPH;
  Serial.print("TeganganPH :");
  Serial.println(teganganPH, 2);
  //  lcd.setCursor(8, 0);
  //  lcd.print(teganganPH);

  PH_step = (PH4 - PH7) / 3;
  Po = 7.00 + ((PH7 - teganganPH) / PH_step);
  //  Po = 20.00 + (PH7 - teganganPH / PH_step);
  Serial.print("Nilai PH :");
  Serial.println(Po, 1);
  //  lcd.setCursor(8, 1);
  //  lcd.print(Po);

  Blynk.virtualWrite(V0, teganganPH);
  Blynk.virtualWrite(V1, Po);
  //  condition
  if (Po >= 0 && Po <= 6.4)
  {
    //    LED
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
    delay(10000);
    digitalWrite(R, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(B, LOW);
    Blynk.virtualWrite(V2, R);
    lcd.setCursor(8, 0);
    lcd.print("ASAM  ");
    lcd.setCursor(3, 1);
    lcd.print(Po);
    lcd.setCursor(13, 1);
    int tempNew = temp;
    lcd.print(tempNew);
  }
  else if (Po >= 6.5 && Po <= 7.9)
  {
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
    delay(1000);
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
    delay(10000);
    digitalWrite(G, HIGH);
    digitalWrite(R, LOW);
    digitalWrite(B, LOW);
    Blynk.virtualWrite(V3, G);
    lcd.setCursor(8, 0);
    lcd.print("Netral");
    lcd.setCursor(3, 1);
    lcd.print(Po);
    lcd.setCursor(13, 1);
    int tempNew = temp;
    lcd.print(tempNew);
  }
  else if (Po >= 8 && Po <= 14)
  {
    digitalWrite(buzz, HIGH);
    delay(1000);
    digitalWrite(buzz, LOW);
    delay(1000);
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(1000);
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(10000);
    digitalWrite(B, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(R, LOW);
    Blynk.virtualWrite(V4, B);

    lcd.setCursor(8, 0);
    lcd.print("Basa  ");
    lcd.setCursor(3, 1);
    lcd.print(Po);
    lcd.setCursor(13, 1);
    int tempNew = temp;
    lcd.print(tempNew);
  }

  else if (Po > 15)
  {
    lcd.setCursor(8, 0);
    lcd.print("Error...");
    lcd.setCursor(8, 1);
    lcd.print("Error...");
    lcd.clear();
  }
}
