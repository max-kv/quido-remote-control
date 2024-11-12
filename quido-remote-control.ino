
/*********
@ MadMax
Portable controler for home automation lights. Other more advanced features are done in Home assistant.
The system is based on Quido modules which are connected through RS485 bus
Currently there are two QUIDO 4/4 modules in net connected to 8 light bulbs in different places in the house
Waveshare 485 gateway transmits UDP packets between RS 485 and LAN
Spinel protocol is standard for Papouch (we use serial commands to switch on/off outputs)
*********/

/* Includes */
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ezButton.h>

/* Settings for RS485waveshare server */
const char* remoteUdpIP = "192.168.0.143";
unsigned int remoteUdpInPort = 4195;  //  port to listen on
unsigned int remoteUdpOutPort = 4196; // port for sending
char packetBuffer[255]; //buffer for incoming packet
bool newPacketFlag = false;

/* Wifi credentials */
const char* ssid = "*******";
const char* password = "#########";

/* Wifi objects */
WiFiUDP Udp;

// Variable to store the HTTP request
String header;

/* IP address */
IPAddress ip;
String ipAddress;

/* Time variables */
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

bool RemoteStates[5] = {false};

// LED GPIO pin
#define LED_GPIO 16
#define KEY_NUM 4  // the number of keys

#define PIN_KEY_1 14  // The Arduino pin connected to the key 1
#define PIN_KEY_2 12  // The Arduino pin connected to the key 2
#define PIN_KEY_3 26  // The Arduino pin connected to the key 3
#define PIN_KEY_4 27  // The Arduino pin connected to the key 4

ezButton keypad_1x4[] = {
  ezButton(PIN_KEY_1),
  ezButton(PIN_KEY_2),
  ezButton(PIN_KEY_3),
  ezButton(PIN_KEY_4)
};

void setup() {
  Serial.begin(115200);
    
  // Init blue led
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, HIGH);

  for (byte i = 0; i < KEY_NUM; i++) {
    keypad_1x4[i].setDebounceTime(100);  // set debounce time to 100 milliseconds
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Udp.begin(remoteUdpInPort);
  String ip = IpAddress2String(WiFi.localIP());
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(ip);
  ipAddress = "IP: " + ip;
  digitalWrite(LED_GPIO, LOW); // Light the led if connected
}

void loop()
{
  int key = getKeyPressed();
  if (key) {
    Serial.print("The key ");
    Serial.print(key);
    Serial.println(" is pressed");
    buttonPushAction(key);
  }
  receivePacketUDP();
}
