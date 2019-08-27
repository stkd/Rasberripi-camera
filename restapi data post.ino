#include <Bridge.h>
#include <Console.h>
#include <Process.h>
#include <DHT.h>
#define DHTPIN 6
#define dust_pin 8
#define DHTTYPE    DHT11
String deviceId = "18351756411";
String projectKey = "PKS5WTB3X72R2MPCA1";
String sensorId1 = "Humidity_sensor";
String sensorId2 = "tempeture";
String sensorId3 = "powder_sensor";
String sensorId4 = "UV";
String Temporary;

String BACKEND_URL = "http://iot.cht.com.tw/iot/v1/device/" + deviceId + "/rawdata";
char buffer[80];
String sensorvalue1 = "{\"id\":humidity cameraId, \"value\":[20]}";
/***dust setup***/
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 10000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/***UV setup***/
int UV_sensorValue;
long UV_sum = 0;

/***dht setup***/
DHT dht(DHTPIN, DHTTYPE);

Process process;

void setup() {
 Bridge.begin();
 Serial.begin(9600);
 while (!Serial);
 pinMode(8,INPUT);
 pinMode(A0,INPUT);
 dht.begin();
 starttime = millis();//get the current time;
  // Serial.begin starts the serial connection between computer and Arduino
 Serial.println("OK");
}

void loop() {
  dust_sensor();
}

void doCurl(float _value,String sensor) {
  Serial.println(_value);

  process.begin("curl");
  process.addParameter("-k"); // allow insecure (not https)
  process.addParameter("-X"); // use POST instead of default GET
  process.addParameter("POST");
  process.addParameter(BACKEND_URL);
  process.addParameter("-H"); // Any headers go after -H
  process.addParameter("CK:"+projectKey);
  process.addParameter("-H");
  process.addParameter( "Content-Type:application/json" );

  
  /* Add data */
  process.addParameter("-d");
  Temporary = "[{\"id\":\"" + sensor + "\",\"value\":[\"" + String(_value) + "\"]}]";
  Serial.println(Temporary);
  process.addParameter(Temporary);
  
  process.run();
  Temporary = '/0';
}
void dht11_sensor(){
  float h = dht.readHumidity();//讀取濕度
  float t = dht.readTemperature();//讀取攝氏溫度
  float f = dht.readTemperature(true);//讀取華氏溫度
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("DHT can't found");
    return;
  }
  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.println(f);
  //Serial.println(",");
  doCurl(h,sensorId1);
  doCurl(t,sensorId2);
  }


void UV_sensor(){

  
  for(int i=0; i<1024; i++)
    {
        UV_sensorValue=analogRead(A0);
        UV_sum=UV_sensorValue+UV_sum;
        delay(1);
    }
    
    UV_sum = UV_sum >> 10;

    Serial.print("The voltage value:");
    float UV = UV_sum*4980.0/1023.0;
    Serial.print(UV);
    //Serial.println("mV\n");
    doCurl(UV,sensorId4);
}

void dust_sensor(){
  duration = pulseIn(dust_pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    //Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.print(",");
    //Serial.println(" pcs/0.01cf");
    //Serial.println("\n");
    lowpulseoccupancy = 0;

    dht11_sensor();                                      // same time
    
    starttime = millis();
    doCurl(concentration,sensorId3);
  }
}
