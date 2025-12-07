#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>








#define USE_DHT11  1

#ifdef USE_DHT11

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN D5
#define DHTTYPE    DHT11
DHT_Unified dht(DHT_PIN, DHTTYPE);

uint32_t delayMSDHT11;
uint32_t lastDHTRead;

struct{
  double temp = 0.0;
  double humidity = 0.0;
  int error = 0;
} DHT11Data;


void setUpDHT11(){
dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMSDHT11 = (sensor.min_delay / 1000)+200;
  lastDHTRead = millis();
}



boolean isDHTReady(){
  if( ((lastDHTRead + delayMSDHT11) < millis()) &&  ((lastDHTRead + 1000) < millis())){
    return true;
  }
  else{
    return false;
  }
}

void readDHT11(boolean printdebug){
  DHT11Data.error = 0;
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    if(printdebug){
      Serial.println(F("Error reading temperature!"));
    }
    DHT11Data.error = 1;
  }
  else {
    if(printdebug){
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }
    DHT11Data.temp =  event.temperature;//*1.8 + 32;
    Serial.println(sizeof(event.temperature));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    if(printdebug){
      Serial.println(F("Error reading humidity!"));
    }
    DHT11Data.error = 1;
  }
  else {
    if(printdebug){
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
    DHT11Data.humidity =  event.relative_humidity;
  }
  lastDHTRead = millis();
}
#endif //USE_DHT11











///*******************************************************///
///*******************************************************///
///              SERVER CODE START FROM HERE              ///
///*******************************************************///
///*******************************************************///
#define LOLIN_LED D4
#define PRODUCTION  1
//#define DEBUG_SERVER  1
String HOME = "/"; 

/* Global vareiables are here */
unsigned long timestamp = 0;


/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);


//@auto generated code
//QR Format
//WIFI:S:MySSID;T:WPA;P:MyPassW0rd;;
//hotspot config
const char* hotspot_name = "iota1112-mlirg";
const char* hotspot_password = "iota1112";

struct{
	String message;
	String message_class = "hide";

	float temperature;
	String temperature_class = "success";

	int humidity;
	String humidity_class = "success";

	String soil;
	String soil_class = "success";

	int aqi;
	String aqi_class = "success";

	String pump;
	String pump_class = "success";

} dataPacket;

struct{
	 const int BTN_NONE = -1;
} btnAction;
int userBtnAction = btnAction.BTN_NONE;

#ifdef PRODUCTION 
String getDataJson(){
	return "{\"message\":\""+dataPacket.message+"\",\"message_class\":\""+dataPacket.message_class+"\", "
	"\"temperature\":\""+String(dataPacket.temperature)+"\",\"temperature_class\":\""+dataPacket.temperature_class+"\", "
	"\"humidity\":\""+String(dataPacket.humidity)+"\",\"humidity_class\":\""+dataPacket.humidity_class+"\", "
	"\"soil\":\""+dataPacket.soil+"\",\"soil_class\":\""+dataPacket.soil_class+"\", "
	"\"aqi\":\""+String(dataPacket.aqi)+"\",\"aqi_class\":\""+dataPacket.aqi_class+"\", "
	"\"pump\":\""+dataPacket.pump+"\",\"pump_class\":\""+dataPacket.pump_class+"\"}";
}
#endif

void handel_UserAction(){
	for (uint8_t i = 0; i < server.args(); i++) {
	}
	server.send(200, "text/json", getDataJson());
}

#ifdef DEBUG_SERVER
String getTestClass(){
	int r = random(0,4);
	switch (r){
		case 0: return "primary";
		case 1: return "secondary";
		case 2: return "success";
		case 3: return "danger";
		case 4: return "warning";
	}
}
String getDataJson(){
	return "{\"message\":\""+String(random(10,99))+"\",\"message_class\":\""+getTestClass()+"\", "
	"\"temperature\":\""+String(random(10,99))+"\",\"temperature_class\":\""+getTestClass()+"\", "
	"\"humidity\":\""+String(random(10,99))+"\",\"humidity_class\":\""+getTestClass()+"\", "
	"\"soil\":\""+String(random(10,99))+"\",\"soil_class\":\""+getTestClass()+"\", "
	"\"aqi\":\""+String(random(10,99))+"\",\"aqi_class\":\""+getTestClass()+"\", "
	"\"pump\":\""+String(random(10,99))+"\",\"pump_class\":\""+getTestClass()+"\"}";
}
#endif



// auto generated code
void forwardTo(String location){
  server.sendHeader("Location", location, true);
  server.send( 302, "text/plain", "");
}

void handle_Home() {
  server.send( 200, "text/html", getTemplate());
}

void handle_DataRequest(){ 
  server.send( 200, "text/json", getDataJson());
}


void handle_NotFound(){
  forwardTo(HOME);
}


void setUpServer(){
    delay(500);
    WiFi.softAP(hotspot_name, hotspot_password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    server.on("/", handle_Home);
    server.on("/act", handel_UserAction);
    server.on("/data.json", handle_DataRequest);
    server.onNotFound(handle_NotFound);
    server.begin();
    delay(300);
    Serial.println("server started.");

}



#define AQI_PIN A0
#define SOIL_PIN D6
#define PUMP_PIN D7


String in_str = "";
boolean in_complete = false;
int pump_state = 0;

void setUpGPIO(void);

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("\n\nstartng...");
  setUpServer();
  setUpGPIO();
#ifdef USE_DHT11
  setUpDHT11();
#endif //USE_DHT11
  timestamp = millis();
}

void setUpGPIO(){
	pinMode(LOLIN_LED,OUTPUT);
  pinMode(PUMP_PIN,OUTPUT);
	in_str.reserve(50);
}



uint32_t led_time_stamp = 0;

void blinkLed(int mil){
  if(led_time_stamp+mil < millis()){
      led_time_stamp = millis();
      if(digitalRead(LOLIN_LED) == 0){
          digitalWrite(LOLIN_LED, HIGH);
      }
      else{
        digitalWrite(LOLIN_LED, LOW);
      } 
  }
}


uint32_t lcd_update_time = 0;
void loop() {
  server.handleClient();
  blinkLed(500);
  if(userBtnAction != btnAction.BTN_NONE){
    userBtnAction = btnAction.BTN_NONE;
    Serial.println("Button clicked");
    
  }

  while(Serial.available() > 0){
    char inC = Serial.read();
    if(inC == '\n'){
      in_complete = true;
    }
    in_str+=inC;

  }

  if(in_complete){

    if(in_str.indexOf("PUMP_ON")){
      pump_state = 1;
    }
    else if(in_str.indexOf("PUMP_OFF")){
      pump_state = 0;
    }


    in_str = "";
    in_complete = false;
  }


    if(pump_state == 1){
      digitalWrite(PUMP_PIN, HIGH);
      dataPacket.pump =  "ON";
      dataPacket.pump_class =  "primary";
    }
    else{
      digitalWrite(PUMP_PIN, LOW);
      dataPacket.pump =  "OFF";
      dataPacket.pump_class =  "warning";
    }
  


    if(lcd_update_time+500 < millis()){
        lcd_update_time = millis();

        
        dataPacket.message_class = "hide";
        dataPacket.message = "";
        

        int soil_wet = digitalRead(SOIL_PIN) == 0 ? 1 : 0;
        int aqi =  analogRead(AQI_PIN);
        dataPacket.temperature =  DHT11Data.temp;
        dataPacket.humidity =  DHT11Data.humidity;
        dataPacket.aqi = aqi;

        if(soil_wet == 1){
          dataPacket.soil = "WET";
          dataPacket.soil_class = "primary";
        }
        else{
          dataPacket.soil = "DRY";
          dataPacket.soil_class = "warning";
        }

        if(aqi > 200){
          dataPacket.aqi_class = "danger";
        }
        else if(aqi > 100){
          dataPacket.aqi_class = "warning";
        }
        else{
          dataPacket.aqi_class = "success";
        }

        if(dataPacket.temperature > 45){
          dataPacket.temperature_class = "warning";
        }
        

        String data  =  "<data>{"
                        "\"soil\":\""+String(soil_wet)+"\","
                        "\"aqi\":\""+String(aqi)+"\","
                        "\"temp\":\""+String(dataPacket.temperature)+"\","
                        "\"hum\":\""+String(dataPacket.humidity)+"\""
                        "}</data>";

        Serial.println(data);
  
        

    }

	
  


  #ifdef USE_DHT11
    if(isDHTReady()){
      readDHT11(false /*debug*/);
      if(DHT11Data.error == 1){
        Serial.println("DHT READ ERROR!");
      }else{
         Serial.print("Temp:");
         Serial.println(DHT11Data.temp);
         Serial.print("Hum:");
         Serial.println(DHT11Data.humidity);
      }
    }
  #endif //USE_DHT11
}



String getTemplate(){
	return 	"<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"<style>"
	"body{\n"
	"background-color: #F1FCFF;\n"
	"padding:0px;\n"
	"margin:0px;\n"
	"text-align: center;\n"
	"}\n"
	"header{\n"
	"height:35px;\n"
	"padding:10px;\n"
	"text-align:left;\n"
	"display: flex;\n"
	"background-color: #0093E9;\n"
	"position:fixed;\n"
	"width:100%;\n"
	"z-index:100;\n"
	"top:0;\n"
	"}\n"
	"footer{\n"
	"padding:20px;\n"
	"}\n"
	"\n"
	"form{\n"
	"margin:15px auto 0px auto;\n"
	"max-width:90%;\n"
	"background-color: #AAAAAA;\n"
	"padding: 15px 0 15px 0;\n"
	"border-radius: 5px;\n"
	"}\n"
	"\n"
	"button{\n"
	"margin:8px auto 0px auto;\n"
	"width:90%;\n"
	"background-color: #AAAAAA;\n"
	"padding: 10px 0 10px 0;\n"
	"border-radius: 5px;\n"
	"font-size: 24px;\n"
	"font-weight: bold;\n"
	"color:white;\n"
	"border: none;\n"
	"}\n"
	"button:active {\n"
	"width:89%;\n"
	"padding: 10px 0 10px 0;\n"
	"color:black;\n"
	"}\n"
	"input{\n"
	"margin:8px auto 0px auto;\n"
	"width:90%;\n"
	"padding: 10px 0 10px 0;\n"
	"border-radius: 5px;\n"
	"font-size: 22px;\n"
	"color:black;\n"
	"border: none;\n"
	"}\n"
	"\n"
	"label{\n"
	"margin:15px auto 0px auto;\n"
	"width:90%;\n"
	"padding: 0px 0 0px 0;\n"
	"font-size: 22px;\n"
	"display:block;\n"
	"}\n"
	"\n"
	".radio-group{\n"
	"margin:15px auto 15px auto;\n"
	"font-size: 24px;\n"
	"width:100%;\n"
	"display:flex;\n"
	"flex-direction: row;\n"
	"text-align:left;\n"
	"}\n"
	".radio-label{\n"
	"margin:0px;\n"
	"padding:0px;\n"
	"}\n"
	".radio{\n"
	"width:32px;\n"
	"margin:0px 10px 0px 15px;\n"
	"}\n"
	"\n"
	".content{\n"
	"margin-top:70px;\n"
	"}\n"
	".connection{\n"
	"margin-left:20px;\n"
	"color: white;\n"
	"}\n"
	".online{\n"
	"margin: 8px 0 0 -8px;\n"
	"font-size: 16px;\n"
	"color:white;\n"
	"}\n"
	".card{\n"
	"margin:15px auto 0px auto;\n"
	"max-width:90%;\n"
	"padding: 15px 0 15px 0;\n"
	"border-radius: 5px;\n"
	"}\n"
	".primary{\n"
	"background-color: #8BC6EC;\n"
	"visibility: visible;\n"
	"}\n"
	".secondary{\n"
	"background-color: #AAAAAA;\n"
	"visibility: visible;\n"
	"}\n"
	".success{\n"
	"background-color: #82c063;\n"
	"visibility: visible;\n"
	"}\n"
	".danger{\n"
	"background-color: #F76666;\n"
	"visibility: visible;\n"
	"}\n"
	".warning{\n"
	"background-color: #E3D377;\n"
	"visibility: visible;\n"
	"}\n"
	".hide{\n"
	"visibility: hidden;\n"
	"}\n"
	"@media only screen and (min-width: 500px) {\n"
	".card {\n"
	"max-width:400px;\n"
	"}\n"
	"button{\n"
	"max-width:400px;\n"
	"}\n"
	"form{\n"
	"max-width:400px;\n"
	"}\n"
	"label{\n"
	"max-width:400px;\n"
	"}\n"
	"}\n"
	"\n"
	"h1 {\n"
	"margin: 2px;\n"
	"color: white;\n"
	"}\n"
	"h2 {\n"
	"margin: 2px;\n"
	"color: black;\n"
	"}\n"
	"</style>\n"
	"<meta charset='utf-8'>\n"
	"<meta http-equiv='X-UA-Compatible' content='IE=edge'>\n"
	"<title>Iot</title>\n"
	"<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
	"<link rel='stylesheet' type='text/css' media='screen' href='main.css'>\n"
	"\n"
	"</head>\n"
	"<body onload=\"liveDataAjax()\">\n"
	"<header>\n"
	"<span class=\"connection\" id=\"connected\">\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"32\" height=\"32\" fill=\"currentColor\" class=\"connected\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M15.384 6.115a.485.485 0 0 0-.047-.736A12.444 12.444 0 0 0 8 3C5.259 3 2.723 3.882.663 5.379a.485.485 0 0 0-.048.736.518.518 0 0 0 .668.05A11.448 11.448 0 0 1 8 4c2.507 0 4.827.802 6.716 2.164.205.148.49.13.668-.049z\"/>\n"
	"<path d=\"M13.229 8.271a.482.482 0 0 0-.063-.745A9.455 9.455 0 0 0 8 6c-1.905 0-3.68.56-5.166 1.526a.48.48 0 0 0-.063.745.525.525 0 0 0 .652.065A8.46 8.46 0 0 1 8 7a8.46 8.46 0 0 1 4.576 1.336c.206.132.48.108.653-.065zm-2.183 2.183c.226-.226.185-.605-.1-.75A6.473 6.473 0 0 0 8 9c-1.06 0-2.062.254-2.946.704-.285.145-.326.524-.1.75l.015.015c.16.16.407.19.611.09A5.478 5.478 0 0 1 8 10c.868 0 1.69.201 2.42.56.203.1.45.07.61-.091l.016-.015zM9.06 12.44c.196-.196.198-.52-.04-.66A1.99 1.99 0 0 0 8 11.5a1.99 1.99 0 0 0-1.02.28c-.238.14-.236.464-.04.66l.706.706a.5.5 0 0 0 .707 0l.707-.707z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<span class=\"connection\" id=\"disconnected\">\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"32\" height=\"32\" fill=\"currentColor\" class=\"connected\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M10.706 3.294A12.545 12.545 0 0 0 8 3C5.259 3 2.723 3.882.663 5.379a.485.485 0 0 0-.048.736.518.518 0 0 0 .668.05A11.448 11.448 0 0 1 8 4c.63 0 1.249.05 1.852.148l.854-.854zM8 6c-1.905 0-3.68.56-5.166 1.526a.48.48 0 0 0-.063.745.525.525 0 0 0 .652.065 8.448 8.448 0 0 1 3.51-1.27L8 6zm2.596 1.404.785-.785c.63.24 1.227.545 1.785.907a.482.482 0 0 1 .063.745.525.525 0 0 1-.652.065 8.462 8.462 0 0 0-1.98-.932zM8 10l.933-.933a6.455 6.455 0 0 1 2.013.637c.285.145.326.524.1.75l-.015.015a.532.532 0 0 1-.611.09A5.478 5.478 0 0 0 8 10zm4.905-4.905.747-.747c.59.3 1.153.645 1.685 1.03a.485.485 0 0 1 .047.737.518.518 0 0 1-.668.05 11.493 11.493 0 0 0-1.811-1.07zM9.02 11.78c.238.14.236.464.04.66l-.707.706a.5.5 0 0 1-.707 0l-.707-.707c-.195-.195-.197-.518.04-.66A1.99 1.99 0 0 1 8 11.5c.374 0 .723.102 1.021.28zm4.355-9.905a.53.53 0 0 1 .75.75l-10.75 10.75a.53.53 0 0 1-.75-.75l10.75-10.75z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<span class=\"connection\">\n"
	"<p class=\"online\" id=\"online\" >Online</p>\n"
	"</span>\n"
	"</header>\n"
	"\n"
	"<div class=\"content\">\n"
	"<div class=\"card hide\" id=\"message\">\n"
	"<span></span>\n"
	"<h1></h1>\n"
	"</div>\n"
	"<div class=\"card primary\" id=\"pump\">\n"
	"<span>\n"
	"<h3>Pump</h3>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"48\" height=\"48\" fill=\"red\" class=\"bi bi-water\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M10 3c0 1.313-.304 2.508-.8 3.4a2 2 0 0 0-1.484-.38c-.28-.982-.91-2.04-1.838-2.969a8 8 0 0 0-.491-.454A6 6 0 0 1 8 2c.691 0 1.355.117 1.973.332Q10 2.661 10 3m0 5q0 .11-.012.217c1.018-.019 2.2-.353 3.331-1.006a8 8 0 0 0 .57-.361 6 6 0 0 0-2.53-3.823 9 9 0 0 1-.145.64c-.34 1.269-.944 2.346-1.656 3.079.277.343.442.78.442 1.254m-.137.728a2 2 0 0 1-1.07 1.109c.525.87 1.405 1.725 2.535 2.377q.3.174.605.317a6 6 0 0 0 2.053-4.111q-.311.11-.641.199c-1.264.339-2.493.356-3.482.11ZM8 10c-.45 0-.866-.149-1.2-.4-.494.89-.796 2.082-.796 3.391q0 .346.027.678A6 6 0 0 0 8 14c.94 0 1.83-.216 2.623-.602a8 8 0 0 1-.497-.458c-.925-.926-1.555-1.981-1.836-2.96Q8.149 10 8 10M6 8q0-.12.014-.239c-1.02.017-2.205.351-3.34 1.007a8 8 0 0 0-.568.359 6 6 0 0 0 2.525 3.839 8 8 0 0 1 .148-.653c.34-1.267.94-2.342 1.65-3.075A2 2 0 0 1 6 8m-3.347-.632c1.267-.34 2.498-.355 3.488-.107.196-.494.583-.89 1.07-1.1-.524-.874-1.406-1.733-2.541-2.388a8 8 0 0 0-.594-.312 6 6 0 0 0-2.06 4.106q.309-.11.637-.199M8 9a1 1 0 1 0 0-2 1 1 0 0 0 0 2\"/>\n"
	"<path d=\"M8 15A7 7 0 1 1 8 1a7 7 0 0 1 0 14m0 1A8 8 0 1 0 8 0a8 8 0 0 0 0 16\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<h1>Not Detected</h1>\n"
	"</div>\n"
	"<div class=\"card primary\" id=\"soil\">\n"
	"<span>\n"
	"<h3>Soil</h3>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"48\" height=\"48\" fill=\"red\" class=\"bi bi-water\" viewBox=\"0 0 16 16\">\n"
	"<path fill-rule=\"evenodd\" d=\"M7.21.8C7.69.295 8 0 8 0q.164.544.371 1.038c.812 1.946 2.073 3.35 3.197 4.6C12.878 7.096 14 8.345 14 10a6 6 0 0 1-12 0C2 6.668 5.58 2.517 7.21.8m.413 1.021A31 31 0 0 0 5.794 3.99c-.726.95-1.436 2.008-1.96 3.07C3.304 8.133 3 9.138 3 10c0 0 2.5 1.5 5 .5s5-.5 5-.5c0-1.201-.796-2.157-2.181-3.7l-.03-.032C9.75 5.11 8.5 3.72 7.623 1.82z\"/>\n"
	"<path fill-rule=\"evenodd\" d=\"M4.553 7.776c.82-1.641 1.717-2.753 2.093-3.13l.708.708c-.29.29-1.128 1.311-1.907 2.87z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<h1>Not Detected</h1>\n"
	"</div>\n"
	"<div class=\"card primary\" id=\"humidity\">\n"
	"<span>\n"
	"<h3>Humidity</h3>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"48\" height=\"48\" fill=\"white\" class=\"bi bi-thermometer-half\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M13.5 0a.5.5 0 0 0 0 1H15v2.75h-.5a.5.5 0 0 0 0 1h.5V7.5h-1.5a.5.5 0 0 0 0 1H15v2.75h-.5a.5.5 0 0 0 0 1h.5V15h-1.5a.5.5 0 0 0 0 1h2a.5.5 0 0 0 .5-.5V.5a.5.5 0 0 0-.5-.5h-2zM7 1.5l.364-.343a.5.5 0 0 0-.728 0l-.002.002-.006.007-.022.023-.08.088a28.458 28.458 0 0 0-1.274 1.517c-.769.983-1.714 2.325-2.385 3.727C2.368 7.564 2 8.682 2 9.733 2 12.614 4.212 15 7 15s5-2.386 5-5.267c0-1.05-.368-2.169-.867-3.212-.671-1.402-1.616-2.744-2.385-3.727a28.458 28.458 0 0 0-1.354-1.605l-.022-.023-.006-.007-.002-.001L7 1.5zm0 0-.364-.343L7 1.5zm-.016.766L7 2.247l.016.019c.24.274.572.667.944 1.144.611.781 1.32 1.776 1.901 2.827H4.14c.58-1.051 1.29-2.046 1.9-2.827.373-.477.706-.87.945-1.144zM3 9.733c0-.755.244-1.612.638-2.496h6.724c.395.884.638 1.741.638 2.496C11 12.117 9.182 14 7 14s-4-1.883-4-4.267z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<h1>30 &deg; C</h1>\n"
	"</div>\n"
	"\n"
	"<div class=\"card primary\" id=\"temperature\">\n"
	"<span>\n"
	"<h3>Temperature</h3>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"48\" height=\"48\" fill=\"yellow\" class=\"bi bi-thermometer-half\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M9.5 12.5a1.5 1.5 0 1 1-2-1.415V6.5a.5.5 0 0 1 1 0v4.585a1.5 1.5 0 0 1 1 1.415z\"/>\n"
	"<path d=\"M5.5 2.5a2.5 2.5 0 0 1 5 0v7.55a3.5 3.5 0 1 1-5 0V2.5zM8 1a1.5 1.5 0 0 0-1.5 1.5v7.987l-.167.15a2.5 2.5 0 1 0 3.333 0l-.166-.15V2.5A1.5 1.5 0 0 0 8 1z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<h1>30%</h1>\n"
	"</div>\n"
	"\n"
	"<div class=\"card primary\" id=\"aqi\">\n"
	"<span>\n"
	"<h3>AQI</h3>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"48\" height=\"48\" fill=\"yellow\" class=\"bi bi-thermometer-half\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M12.5 2A2.5 2.5 0 0 0 10 4.5a.5.5 0 0 1-1 0A3.5 3.5 0 1 1 12.5 8H.5a.5.5 0 0 1 0-1h12a2.5 2.5 0 0 0 0-5m-7 1a1 1 0 0 0-1 1 .5.5 0 0 1-1 0 2 2 0 1 1 2 2h-5a.5.5 0 0 1 0-1h5a1 1 0 0 0 0-2M0 9.5A.5.5 0 0 1 .5 9h10.042a3 3 0 1 1-3 3 .5.5 0 0 1 1 0 2 2 0 1 0 2-2H.5a.5.5 0 0 1-.5-.5\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<h1>30%</h1>\n"
	"</div>\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"</div>\n"
	"<footer>\n"
	"\n"
	"</footer>\n"
	"\n"
	"<script>"
	"var DRT = 500;\n"
	"function updateCSSClass(element, css){\n"
	"    if(css != 'primary')\n"
	"        element.classList.remove('primary');\n"
	"    if(css != 'secondary')\n"
	"        element.classList.remove('secondary');\n"
	"    if(css != 'success')\n"
	"        element.classList.remove('success');\n"
	"    if(css != 'danger')\n"
	"        element.classList.remove('danger');\n"
	"    if(css != 'warning')\n"
	"        element.classList.remove('warning');\n"
	"    if(css != 'hide')\n"
	"        element.classList.remove('hide');\n"
	"    element.classList.add(css);\n"
	"}\n"
	"\n"
	"function updateData(data){\n"
	"\tdocument.getElementById(\"message\").children[1].innerHTML = \"\"+data.message+\"\";\n"
	"\tupdateCSSClass(document.getElementById(\"message\"), data.message_class);\n"
	"\tdocument.getElementById(\"temperature\").children[1].innerHTML = \"\"+data.temperature+\" &deg;C\";\n"
	"\tupdateCSSClass(document.getElementById(\"temperature\"), data.temperature_class);\n"
	"\tdocument.getElementById(\"humidity\").children[1].innerHTML = \"\"+data.humidity+\" %\";\n"
	"\tupdateCSSClass(document.getElementById(\"humidity\"), data.humidity_class);\n"
	"\tdocument.getElementById(\"soil\").children[1].innerHTML = \"\"+data.soil+\"\";\n"
	"\tupdateCSSClass(document.getElementById(\"soil\"), data.soil_class);\n"
	"\tdocument.getElementById(\"aqi\").children[1].innerHTML = \"\"+data.aqi+\"\";\n"
	"\tupdateCSSClass(document.getElementById(\"aqi\"), data.aqi_class);\n"
	"\tdocument.getElementById(\"pump\").children[1].innerHTML = \"\"+data.pump+\"\";\n"
	"\tupdateCSSClass(document.getElementById(\"pump\"), data.pump_class);\n"
	"}\n"
	"\n"
	"function getCommand(btn_id, value){\n"
	"    \n"
	"}\n"
	"\n"
	"function onClickBtn(btn_id){\n"
	"\tvar val = document.getElementById(btn_id).innerHTML;\n"
	"\tvar cmd = getCommand(btn_id,val);\n"
	"    console.log(cmd)\n"
	"\tsendButtonClick('/act?'+btn_id+'='+cmd)\n"
	"}\n"
	"\n"
	"\n"
	"\n"
	"function updateNetwork(connected){\n"
	"    if(connected){\n"
	"        document.getElementById('disconnected').style.display = 'none';\n"
	"        document.getElementById('connected').style.display = 'block';\n"
	"        document.getElementById('online').innerHTML = 'Online';\n"
	"    }\n"
	"    else{\n"
	"        document.getElementById('connected').style.display = 'none';\n"
	"        document.getElementById('disconnected').style.display = 'block';\n"
	"        document.getElementById('online').innerHTML = 'Offline';\n"
	"    }\n"
	"}\n"
	"\n"
	"\n"
	"function sendButtonClick(url){\n"
	"\t\n"
	"    const xhr = new XMLHttpRequest();\n"
	"    xhr.open('GET', url, true);\n"
	"    xhr.onload = () => {\n"
	"        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {\n"
	"            var data= JSON.parse(xhr.responseText);\n"
	"            updateData(data);\n"
	"            updateNetwork(true);\n"
	"        }\n"
	"    }\n"
	"    xhr.onerror = function() {\n"
	"        updateNetwork(false);\n"
	"    };\n"
	"    xhr.send();\n"
	"}\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"var netcount = 0;\n"
	"function reconnect(){\n"
	"    if(netcount == 0){\n"
	"        console.log(\"Retrying\");\n"
	"        document.getElementById('online').innerHTML = 'Retrying..';\n"
	"        setTimeout(liveDataAjax,1000);\n"
	"        return\n"
	"    }\n"
	"    netcount -= 1;\n"
	"    console.log(\"count\",netcount);\n"
	"    document.getElementById('online').innerHTML = 'Offline ('+netcount+')';\n"
	"    setTimeout(reconnect, 1000);\n"
	"}\n"
	"function liveDataAjax(){\n"
	"    const xhr = new XMLHttpRequest();\n"
	"    xhr.open('GET', '/data.json', true);\n"
	"    xhr.onload = () => {\n"
	"        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {\n"
	"            var data= JSON.parse(xhr.responseText);\n"
	"            updateData(data);\n"
	"            updateNetwork(true);\n"
	"            setTimeout(liveDataAjax, DRT);\n"
	"        }\n"
	"        else if (xhr.readyState === XMLHttpRequest.DONE){\n"
	"            updateNetwork(false);\n"
	"            netcount = 5;\n"
	"            reconnect();\n"
	"        }\n"
	"    };\n"
	"    xhr.onerror = function() {\n"
	"        updateNetwork(false);\n"
	"        netcount = 5;\n"
	"        reconnect();\n"
	"    };\n"
	"    xhr.send();\t\n"
	"}\n"
	"\n"
	"</script>\n"
	"</body>\n"
	"</html>";
}