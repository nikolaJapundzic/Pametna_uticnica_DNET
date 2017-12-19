    /* DHTServer - ESP8266 Webserver with a DHT sensor as an input
     
       Based on ESP8266Webserver, DHTexample, and BlinkWithoutDelay (thank you)
     
       Version 1.0  5/3/2014  Version 1.0   Mike Barela for Adafruit Industries
    */
    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <ESP8266WebServer.h>
    
    const char *ssid = "DunavNETTenda2";
    const char* password  = "dNETheBEST2015";
   
     
    ESP8266WebServer server(1001);
     
    // Initialize DHT sensor 
    // NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
    // you need to increase the threshold for cycle counts considered a 1 or 0.
    // You can do this by passing a 3rd parameter for this threshold.  It's a bit
    // of fiddling to find the right value, but in general the faster the CPU the
    // higher the value.  The default for a 16mhz AVR is a value of 6.  For an
    // Arduino Due that runs at 84mhz a value of 30 works.
    // This is for the ESP8266 processor on ESP-01 

    String webString="";     // String to display
    // Generally, you should use "unsigned long" for variables that hold time
    unsigned long previousMillis = 0; 
    String status = "";
     
    void handle_root() {
      server.send(200, "text/plain", "Hello from the weather esp8266, read from /temp or /humidity");
      delay(100);
    }
     
    void setup(void)
    {
      pinMode(LED_BUILTIN, OUTPUT);
      // You can open the Arduino IDE Serial Monitor window to see what the code is doing
      Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
     
      // Connect to WiFi network
      WiFi.begin(ssid, password);
      Serial.print("\n\r \n\rWorking to connect");
     
      // Wait for connection
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("DHT Weather Reading Server");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
       
      server.on("/", handle_root);
  //---------------------------------------------------------------------------------------------------------------------------------------------------------    
           
      server.on("/temperatura=stanje", [](){  // if you add this subdirectory to your webserver call, you get text below :)
        webString="Temperature: milion ";
        webString+=" *C";
       
        // Arduino has a hard time with float to string
        server.send(200, "text/plain", webString);            // send to someones browser when asked
      });
  //---------------------------------------------------------------------------------------------------------------------------------------------------------   

  //---------------------------------------------------------------------------------------------------------------------------------------------------------    
           
      server.on("/ON", [](){  // if you add this subdirectory to your webserver call, you get text below :)
        digitalWrite(LED_BUILTIN, LOW);
        // Arduino has a hard time with float to string
        server.send(200, "text/plain", "Iskljuceno");            // send to someones browser when asked
      });
  //--------------------------------------------------------------------------------------------------------------------------------------------------------- 

  //---------------------------------------------------------------------------------------------------------------------------------------------------------    
           
      server.on("/OFF", [](){  // if you add this subdirectory to your webserver call, you get text below :)
        digitalWrite(LED_BUILTIN, HIGH);
        // Arduino has a hard time with float to string
        server.send(200, "text/plain", "Ukljuceno");            // send to someones browser when asked
      });
  //--------------------------------------------------------------------------------------------------------------------------------------------------------- 
      server.begin();
      Serial.println("HTTP server started");
    }
     
    void loop(void)
    {
      server.handleClient();
    } 
     
    
