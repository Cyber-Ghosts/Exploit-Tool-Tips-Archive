#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>
#include <TextFinder.h>
// @thelinuxchoice

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(9, 8, 5, 4, 3, 2); //without i2C module
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "www.folha.uol.com.br";  
char tweet[140];

IPAddress ip(192,168,1,77);

EthernetClient client;
TextFinder  finder( client );  

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Feed");
//  Serial.begin(9600);
  Ethernet.begin(mac, ip);

  Serial.begin(9600);
   while (!Serial) {
    ;
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  
}

void loop()
{
  if (client.connect(server, 80)) {
    client.println("GET /emcimadahora/rss091.xml HTTP/1.1");
    client.println("Host: feeds.folha.uol.com.br");
    client.println("Connection: close");
    client.println();
  } 
  else {
  }
  lcd.clear();
  if (client.connected()) {
   if((finder.find("<item>")&&(finder.getString("<title>","</title>",tweet,144)<64)))
     {  
         Serial.println(tweet);
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
     }
  }
  
  if (!client.connected()) {
    Serial.println();
    client.stop();

  }

}

