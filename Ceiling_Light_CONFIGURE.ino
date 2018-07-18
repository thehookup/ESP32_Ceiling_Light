

/******************  LIBRARY SECTION *************************************/
#include <SimpleTimer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


//USER CONFIGURED SECTION START//
const char* ssid = "YOUR_WIRELESS_SSID";
const char* password = "YOUR_WIRELESS_PASSWORD";
const char* mqtt_server = "YOUR_MQTT_SERVER_ADDRESS";
const int mqtt_port = 1883;
const char *mqtt_user = "YOUR_MQTT_USERNAME";
const char *mqtt_pass = "YOUR_MQTT_PASSWORD";
const char *mqtt_client_name = "CeilingLightMCU"; // Client connections can't have the same connection name
//USER CONFIGURED SECTION END//

/*****************  DECLARATIONS  ****************************************/

WiFiClient espClient;
PubSubClient client(espClient);
SimpleTimer timer;

/*****************  GLOBAL VARIABLES  ************************************/


const int FrontLeft = 4; 
const int FrontMiddle = 16; 
const int FrontRight = 17; 
const int MiddleLeft = 13; 
const int MiddleRight = 5; 
const int BackLeft = 14; 
const int BackMiddle = 27; 
const int BackRight = 26; 
const int ambientPin = 15; 

int FrontLeft_Bright = 100;
int FrontMiddle_Bright = 100;
int FrontRight_Bright = 100;
int MiddleLeft_Bright = 100;
int MiddleRight_Bright = 100;
int BackLeft_Bright = 100;
int BackMiddle_Bright = 100;
int BackRight_Bright = 100;

int freq = 600;
int resolution = 8;

bool boot = true;


/*****************  SYSTEM FUNCTIONS  *************************************/

void setup_wifi() 
{
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  // Loop until we're reconnected
  int retries = 0;
  while (!client.connected()) {
    if(retries < 150)
    {
        Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(mqtt_client_name, mqtt_user, mqtt_pass)) 
      {
        Serial.println("connected");
        // Once connected, publish an announcement...
        if(boot == true)
        {
          client.publish("checkIn/FamilyRoomMCU","Rebooted");
          boot = false;
        }
        if(boot == false)
        {
          client.publish("checkIn/FamilyRoomMCU","Reconnected"); 
        }
        client.subscribe("FamilyRoom/FrontLeft");
        client.subscribe("FamilyRoom/FrontMiddle");
        client.subscribe("FamilyRoom/FrontRight");
        client.subscribe("FamilyRoom/MiddleLeft");
        client.subscribe("FamilyRoom/MiddleRight");
        client.subscribe("FamilyRoom/BackLeft");
        client.subscribe("FamilyRoom/BackMiddle");
        client.subscribe("FamilyRoom/BackRight");
      } 
      else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        retries++;
        // Wait 5 seconds before retrying
        delay(5000);
      }
    }
    if(retries > 1500)
    {
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  String newTopic = topic;
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  String newPayload = String((char *)payload);
  int intPayload = newPayload.toInt();
  Serial.println(newPayload);
  Serial.println();
  if (newTopic == "FamilyRoom/FrontLeft") 
  {
    if(newPayload == "OFF")
    { 
      ledcWrite(1, 0);
      client.publish("FamilyRoom/FrontLeft/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(1, 0);
      client.publish("FamilyRoom/FrontLeft/Status","OFF");
    }
    else
    {
      FrontLeft_Bright = intPayload;
      ledcWrite(1, FrontLeft_Bright);
      client.publish("FamilyRoom/FrontLeft/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/FrontMiddle") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(0, 0);
      client.publish("FamilyRoom/FrontMiddle/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(0, 0);
      client.publish("FamilyRoom/FrontMiddle/Status","OFF");
    }
    else
    {
      FrontMiddle_Bright = intPayload;
      ledcWrite(0, FrontMiddle_Bright);
      client.publish("FamilyRoom/FrontMiddle/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/FrontRight") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(2, 0);
      client.publish("FamilyRoom/FrontRight/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(2, 0);
      client.publish("FamilyRoom/FrontRight/Status","OFF");
    }
    else
    {
      FrontRight_Bright = intPayload;
      ledcWrite(2, FrontRight_Bright);
      client.publish("FamilyRoom/FrontRight/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/MiddleLeft") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(3, 0);
      client.publish("FamilyRoom/MiddleLeft/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(3, 0);
      client.publish("FamilyRoom/MiddleLeft/Status","OFF");
    }
    else
    {
      MiddleLeft_Bright = intPayload;
      ledcWrite(3, MiddleLeft_Bright);
      client.publish("FamilyRoom/MiddleLeft/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/MiddleRight") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(4, 0);
      client.publish("FamilyRoom/MiddleRight/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(4, 0);
      client.publish("FamilyRoom/MiddleRight/Status","OFF");
    }
    else
    {
      MiddleRight_Bright = intPayload;
      ledcWrite(4, MiddleRight_Bright);
      client.publish("FamilyRoom/MiddleRight/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/BackLeft") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(5, 0);
      client.publish("FamilyRoom/BackLeft/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(5, 0);
      client.publish("FamilyRoom/BackLeft/Status","OFF");
    }
    else
    {
      BackLeft_Bright = intPayload;
      ledcWrite(5, BackLeft_Bright);
      client.publish("FamilyRoom/BackLeft/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/BackMiddle") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(6, 0);
      client.publish("FamilyRoom/BackMiddle/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(6, 0);
      client.publish("FamilyRoom/BackMiddle/Status","OFF");
    }
    else
    {
      BackMiddle_Bright = intPayload;
      ledcWrite(6, BackMiddle_Bright);
      client.publish("FamilyRoom/BackMiddle/Status","ON");
    }
  }
  if (newTopic == "FamilyRoom/BackRight") 
  {
    if(newPayload == "OFF")
    {
      ledcWrite(7, 0);
      client.publish("FamilyRoom/BackRight/Status","OFF");
    }
    else if(newPayload == "0")
    {
      ledcWrite(7, 0);
      client.publish("FamilyRoom/BackRight/Status","OFF");
    }
    else
    {
      BackRight_Bright = intPayload;
      ledcWrite(7, BackRight_Bright);
      client.publish("FamilyRoom/BackRight/Status","ON");
    }
  }
}

/*****************  GLOBAL LIGHT FUNCTIONS  *******************************/

void checkIn()
{
  client.publish("checkIn/CeilingLightMCU","OK");
}

/*****************  SETUP FUNCTIONS  ****************************************/


void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
    
  //LEDC Setup
  ledcSetup(1, freq, resolution);
  ledcAttachPin(FrontLeft, 1);
  ledcSetup(0, freq, resolution);
  ledcAttachPin(FrontMiddle, 0);
  ledcSetup(2, freq, resolution);
  ledcAttachPin(FrontRight, 2);
  ledcSetup(3, freq, resolution);
  ledcAttachPin(MiddleLeft, 3);
  ledcSetup(4, freq, resolution);
  ledcAttachPin(MiddleRight, 4);
  ledcSetup(5, freq, resolution);
  ledcAttachPin(BackLeft, 5);
  ledcSetup(6, freq, resolution);
  ledcAttachPin(BackMiddle, 6);
  ledcSetup(7, freq, resolution);
  ledcAttachPin(BackRight, 7);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  timer.setInterval(120000, checkIn);
  
  ArduinoOTA.setHostname("CeilingLightMCU");
  ArduinoOTA.begin(); 
}


/*****************  MAIN LOOP  ****************************************/


void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();
  timer.run();
}




