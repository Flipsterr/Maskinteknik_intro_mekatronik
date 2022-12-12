
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
///////
char ssid[] = SECRET_SSID;        // Ditt nätverks SSID (namn)
char pass[] = SECRET_PASS;    // Ditt närverks lösenord
int keyIndex = 0;                 // Nätverkets index nummer

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // Påbörjar serial uppkopplingen (möjligör serialmonitor användning)
  pinMode(A0, INPUT);

  // Kontrollerar WiFi modulen:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication ");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Var vänlig att uppdatera din firmware");
  }

  // Försöker att koppla upp sig till nätverket:
  while (status != WL_CONNECTED) {
    Serial.print("Försöker att ansluta till nätverket: ");
    Serial.println(ssid);                   // Printar nätverksnamnet (SSID);

    // Kopplar upp sig till WPA och WPA2 nätverk:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // Öppnar webserver på porten 80
  printWifiStatus();                        // Printar WiFi statusen
}


void loop() {
  WiFiClient client = server.available();   // Kollar efter nya klienter


  if (client) {                             
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
    if (client.available()) {
    int analogValue = analogRead(A0);

    Serial.println("Analog reading: ");
    Serial.println(analogValue);   // the raw analog reading
    if (analogValue < 10) {
     server.println("Ljus Status");
    server.println("-----------------------");
    server.println(" - Det är natt");
    server.println("För att uppdatera sidan var vänlig och ladda om sidan");
  } else if (analogValue < 200) {
    server.println("Ljus Status");
    server.println("-----------------------");
    server.println(" - Det är gryning/skymning");
    server.println("För att uppdatera sidan var vänlig och ladda om sidan");
  } else if (analogValue < 500) {
    server.println("Ljus Status");
    server.println("-----------------------");
    server.println(" - Det är morgon/Kväll");
    server.println("För att uppdatera sidan var vänlig och ladda om sidan");
  } else if (analogValue < 800) {
    server.println("Ljus Status");
    server.println("-----------------------");
    server.println(" - Det är dag");
    server.println("För att uppdatera sidan var vänlig och ladda om sidan");
  } else {
    server.println("Ljus Status");
    server.println("-----------------------");
    server.println(" - Solen ligger på!");
    server.println(" - För att uppdatera sidan var vänlig och ladda om sidan");
  }
 
  delay(1000);
    }
    // close the connection:
    client.stop();
    }
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
