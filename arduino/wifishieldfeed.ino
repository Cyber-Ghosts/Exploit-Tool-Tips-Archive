#include <SoftwareSerial.h> 
#include <SparkFunESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TextFinder.h>
// @thelinuxchoice
// sparkfun wifi shield (Feed RSS)

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);


const char mySSID[] = "";
const char myPSK[] = "";
//char server[] = "www.folha.uol.com.br";
char tweet[140];
ESP8266Client client;
TextFinder  finder( client );  

                            

void setup() 
{
  lcd.begin (16,2);
  //lcd.setBacklight(LOW);
  Serial.begin(9600);


  initializeESP8266();

  // connectESP8266() connects to the defined WiFi network.
  connectESP8266();
  displayConnectInfo();

//server.begin();

}

void loop() 
{
  
 clientfeed();
}

void initializeESP8266()
{

  int test = esp8266.begin();
  if (test != true)
  {
   // Serial.println(("Error talking to ESP8266."));
   // errorLoop(test);
  }
  //Serial.println(("ESP8266 Shield Present"));
}

void connectESP8266()
{
  // The ESP8266 can be set to one of three modes:
  //  1 - ESP8266_MODE_STA - Station only
  //  2 - ESP8266_MODE_AP - Access point only
  //  3 - ESP8266_MODE_STAAP - Station/AP combo
  // Use esp8266.getMode() to check which mode it's in:
  int retVal = esp8266.getMode();
  if (retVal != ESP8266_MODE_STA)
  { // If it's not in station mode.
    // Use esp8266.setMode([mode]) to set it to a specified
    // mode.
    retVal = esp8266.setMode(ESP8266_MODE_STA);
    if (retVal < 0)
    {
     // Serial.println(("Error setting mode."));
    //  errorLoop(retVal);
    }
  }
  //Serial.println(("Mode set to station"));

  // esp8266.status() indicates the ESP8266's WiFi connect
  // status.
  // A return value of 1 indicates the device is already
  // connected. 0 indicates disconnected. (Negative values
  // equate to communication errors.)
  retVal = esp8266.status();
  if (retVal <= 0)
  {
   /// Serial.print(("Connecting to "));
   /// Serial.println(mySSID);
    // esp8266.connect([ssid], [psk]) connects the ESP8266
    // to a network.
    // On success the connect function returns a value >0
    // On fail, the function will either return:
    //  -1: TIMEOUT - The library has a set 30s timeout
    //  -3: FAIL - Couldn't connect to network.
    retVal = esp8266.connect(mySSID, myPSK);
    if (retVal < 0)
    {
      Serial.println(("Error connecting"));
     // errorLoop(retVal);
    }
  }
}

void displayConnectInfo()
{
  char connectedSSID[24];
  memset(connectedSSID, 0, 24);
  // esp8266.getAP() can be used to check which AP the
  // ESP8266 is connected to. It returns an error code.
  // The connected AP is returned by reference as a parameter.
  int retVal = esp8266.getAP(connectedSSID);
  if (retVal > 0)
  {
  //  Serial.print(("Connected to: "));
  //  Serial.println(connectedSSID);
  
  }

  // esp8266.localIP returns an IPAddress variable with the
  // ESP8266's current local IP address.
  IPAddress myIP = esp8266.localIP();
 // Serial.print(("My IP: ")); Serial.println(myIP);
 // lcd.setBacklight(LOW);
  lcd.setCursor(0,0);
    lcd.print(myIP);
//lcd.setCursor(0,1);
//lcd.print(connectedSSID);
lcd.setBacklight(HIGH);
delay(2000);
lcd.setBacklight(LOW);
}


void clientfeed ()
{
  
if (client.connect("www.folha.uol.com.br", 80)); // Connect to sparkfun (HTTP port)
{    

  if (client.connected()) 
    {
      client.print("GET //emcimadahora/rss091.xml HTTP/1.1\nHost: feeds.folha.uol.com.br\nConnection: close\n\n");

 if((finder.find("<item>")&&(finder.getString("<title>","</title>",tweet,144)<64)))
     {  
         Serial.println(tweet);
           lcd.setBacklight(HIGH);
           lcd.clear();
           lcd.setCursor(0,0);
           for (int i=0; i<16; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,1);
           for (int i=16; i<32; i++)
             lcd.print(tweet[i]);
       
           delay(2000);

           lcd.clear();
           lcd.setCursor(0,0);
           for (int i=32; i<48; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,1);
           for (int i=48; i<64; i++)
             lcd.print(tweet[i]);
           delay(2000);
           lcd.clear();

         lcd.setBacklight(LOW);
         }

  if (!client.connected()) {
    Serial.println();
   client.stop();
   client.flush(); 

  }

}
}  
}








