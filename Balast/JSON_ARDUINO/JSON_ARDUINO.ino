#include <ESP8266WiFi.h>

const char* ssid     = "DunavNETTenda2";
const char* password = "dNETheBEST2015";

const char* ssidB     = "IoT";
const char* passwordB = "kodirano";


const char* host = "dnetteststorage.blob.core.windows.net";
//------------------------------------------------------------------------------------  
boolean flag1 = false;
int brojac = 0;

//------------------------------------------------------------------------------------  

void setup() {
  //------------------------------------------------------------------------------------  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);
  //------------------------------------------------------------------------------------  
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
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


void loop() {
  brojac++;
  Serial.println("brojac iznosi: ");
  Serial.println(brojac);
  if(brojac == 5){
    WiFi.disconnect();
    Serial.println("WiFi je ugasen.");

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssidB);
  
    WiFi.begin(ssidB, passwordB);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
  }else if(brojac < 5){
    delay(15000);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/nikola/status.json";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(2000);

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    
    
  }

  Serial.println();
  Serial.println("closing connection");
  }else{
    delay(15000);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/nikola/status.json";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(2000);

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    
    
  }

  Serial.println();
  Serial.println("closing connection");
  }

  
}
