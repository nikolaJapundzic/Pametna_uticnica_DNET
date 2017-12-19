#include <ESP8266WiFi.h>
#include <EEPROM.h>
boolean flagReset = false;
const int buttonRESET = 4;     // button za reset na pin-u 2
const int ledPin =  15;      // indikator na pin-u 8
int buttonState = 0;         // stanje butona
const char* host = "dnetteststorage.blob.core.windows.net";
boolean flagKONEKCIJA1 = true;

String set_wifiSTRING = "";
boolean set_wifi = false;

String ssidNEW = "";
String passwordNEW = "";

String on_offSTRING = "";
boolean on_off = false;

String dimerSTRING = "";
int dimer = 255;

//------
int br = 0;
//------

String id = "";
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);     //Setovanje pin-a da bude izlaznog tipa
  pinMode(buttonRESET, INPUT); //Setovanje pin-a da bude ulaznog tipa

  
  //PROVERA DA LI JE FABRICKA ADRESA TU, AKO NIJE UNESI JE DA POSTOJI
  if(proveriDaLiJeOK()){
    Serial.println("True"); //Ako je tu fabricki onda je ok
  }else{
    Serial.println("False");//Ako fabricki nije tu onda je ga unesi
    nulirajEEPROM();
    upisiSSIDiPASS_FABRICKI();
    delay(500);
    //void(* resetFunc) (void) = 0; //declare reset function @ address 0
  }
  //PROVERA DA LI JE FABRICKA ADRESA TU, AKO NIJE UNESI JE DA POSTOJI

  EEPROM.begin(256);
  char value = EEPROM.read(50);
  EEPROM.end();
  if(value == '~'){ // provera da li postoji u eeprom-u novi ssid i password
    konektovanjeNaWiFi_PRVIH_50();   //konektuj se na fabricki ssid i pass
  }else{
    konektovanjeNaWiFi_DRUGIH_50();
  }

}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void loop()
{

  procitaj_dejstvuj_JSON_SSID_PASS();//popuni varijable iz JSONA
  UPISI_novi_SSID_I_PASS_u_Eeprom(); //ako je set_wifi == true unesi novi ssid i pass iz json-a
  resetTaster();                     //taster drzati stisnuto dok LED ne poplavi
  procitajJSONizVARIABLI();          //isprintaj sve iz varijavli
  
  //---------------------------------------------------------
  procitajEEPROM();
  br++;
  if(br == 3){
    WiFi.disconnect();
    Serial.println("Diskonektovao sam se sa DunavNet-a. pripremam se za konektovanje na IoT");
    delay(1000);
    konektovanjeNaWiFi_DRUGIH_50();
  }

    
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void UPISI_novi_SSID_I_PASS_u_Eeprom(){
  if(set_wifi){
    EEPROM.begin(256);
    int duz1 = ssidNEW.length() +1;
    char duz1CHAR[duz1];
    ssidNEW.toCharArray(duz1CHAR, duz1);
    for(int i = 50; i < (50+duz1); i++){
      EEPROM.write(i, duz1CHAR[i - 50]);
      delay(20);
    }
    
    int duz2 = passwordNEW.length() +1;
    char duz2CHAR[duz2];
    passwordNEW.toCharArray(duz2CHAR, duz2);
    for(int i = 75; i < (75+duz2); i++){
      EEPROM.write(i, duz2CHAR[i - 75]);
      delay(20);
    }

    EEPROM.commit(); 
    EEPROM.end();   
  }
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void nulirajEEPROM(){
  EEPROM.begin(256);
    // write a 0 to all 512 bytes of the EEPROM
    for (int i = 0; i < 256; i++){
      EEPROM.write(i, '~');
      delay(10);
    }
    EEPROM.end();
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void procitajEEPROM(){
  byte value;
  EEPROM.begin(256);
  Serial.print("EEPROM vrednosti: ");
  for(int i = 0; i < 256; i++){
    value = EEPROM.read(i);
    Serial.print(value, DEC);
    Serial.print(" ");
    delay(10);
  }
  Serial.println();
  EEPROM.end();
  
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void upisiSSIDiPASS_FABRICKI(){
  const char* ssid     = "DunavNETTenda2";
  const char* password = "dNETheBEST2015";
    
  EEPROM.begin(256);
  for(int i = 0; i < strlen(ssid); i++){
    EEPROM.write(i, ssid[i]);
    delay(20);
  }
  int kontinue = 25 + strlen(password);
  for(int i = 25; i < kontinue; i++){
    EEPROM.write(i, password[i-25]);
    delay(20);
  }
  EEPROM.commit();
  EEPROM.end();
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
boolean proveriDaLiJeOK(){
  boolean flag = false;
  byte value;
  String provera1 = "";
  String provera2 = "";
  const char* ssid     = "DunavNETTenda2";
  const char* password = "dNETheBEST2015";
  char karakter;
  char karakter2;
  EEPROM.begin(256);
  for(int i = 0; i < strlen(ssid); i++){
    value = EEPROM.read(i);
    karakter = value;
    karakter2 = ssid[i];
    provera2 = provera2 + karakter2;
    provera1 = provera1 + karakter;
    delay(20);
  }
  if(provera1.equals(provera2)){
    provera1 = "";
    provera2 = "";
    int kont = 25 + strlen(password);
    for(int i = 25; i < kont; i++){
      value = EEPROM.read(i);
      karakter = value;
      karakter2 = password[i-25];
      provera2 = provera2 + karakter2;
      provera1 = provera1 + karakter;
      delay(20);
    }
    if(provera1.equals(provera2)){
      flag = true;
    }
  }
  //Serial.println(provera1);
  EEPROM.end();
  return flag;
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void konektovanjeNaWiFi_PRVIH_50(){
  String ssidSTR = "";
  String passwordSTR = "";
  byte value;
  char karakter;
  EEPROM.begin(256);

  //Izvlacenje iz eeproma ssid
  for(int i = 0; i < 25; i++){
    value = EEPROM.read(i);
    if(value == '~'){
      break;
    }
    karakter = value;
    ssidSTR = ssidSTR + karakter;
  }
  //Izvlacenje iz eeproma ssid

  //Izvlacenje iz eeproma password
  for(int i = 25; i < 50; i++){
    value = EEPROM.read(i);
    if(value == '~'){
      break;
    }
    karakter = value;
    passwordSTR = passwordSTR + karakter;
  }
  //Izvlacenje iz eeproma password

  Serial.println("ssid");
  Serial.println(ssidSTR);
  Serial.println("password");
  Serial.println(passwordSTR);
  int ssidLENGTH = (ssidSTR.length())+1;
  int passwordLENGTH = (passwordSTR.length())+1;
  
  char ssid[ssidLENGTH];
  ssidSTR.toCharArray(ssid, ssidLENGTH); 

  char password[passwordLENGTH];
  passwordSTR.toCharArray(password, passwordLENGTH); 
/*
  //Stampa 
  for(int i = 0; i <= ssidLENGTH; i++){
    Serial.print(ssid[i]);
  }
  Serial.println();
  //Stampa 
*/ 
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
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void konektovanjeNaWiFi_DRUGIH_50(){
  String ssidSTR = "";
  String passwordSTR = "";
  byte value;
  char karakter;

  //-------
  
  
  //-------
  EEPROM.begin(256);

  //Izvlacenje iz eeproma ssid
  for(int i = 50; i < 75; i++){
    value = EEPROM.read(i);
    if(value == '~'){
      break;
    }
    karakter = value;
    ssidSTR = ssidSTR + karakter;
  }
  //Izvlacenje iz eeproma ssid

  //Izvlacenje iz eeproma password
  for(int i = 75; i < 100; i++){
    value = EEPROM.read(i);
    if(value == '~'){
      break;
    }
    karakter = value;
    passwordSTR = passwordSTR + karakter;
  }
  //Izvlacenje iz eeproma password

  Serial.println("ssid");
  Serial.println(ssidSTR);
  Serial.println("password");
  Serial.println(passwordSTR);
  int ssidLENGTH = (ssidSTR.length())+1;
  int passwordLENGTH = (passwordSTR.length())+1;

  Serial.println(ssidLENGTH);
  Serial.println(passwordLENGTH);

  char ssid[ssidLENGTH];
  ssidSTR.toCharArray(ssid, ssidLENGTH); 

  char password[passwordLENGTH];
  passwordSTR.toCharArray(password, passwordLENGTH); 

 
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
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void resetTaster(){
  buttonState = digitalRead(buttonRESET);

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    WiFi.disconnect();
    delay(300);
    nulirajEEPROM();
    upisiSSIDiPASS_FABRICKI();
    delay(500);
    br = 0;
    konektovanjeNaWiFi_PRVIH_50();   //konektuj se na fabricki ssid i pass
  } else {
    digitalWrite(ledPin, LOW);
  }
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void procitaj_dejstvuj_JSON_SSID_PASS(){
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
  
  set_wifiSTRING = "";
  ssidNEW = "";
  passwordNEW = "";
  on_offSTRING = "";
  dimerSTRING = "";
  id = "";
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    //ZA PROMENLJIVE ZA CUPANJE JSONA
    int brojac = 0;
    
    for(int i = 0; i < line.length(); i++){
      if(line[i] == '"'){
        brojac++;
      }
      //-----------------------------------------------------------
      if(brojac == 3){
        if(line[i] != '"'){
          set_wifiSTRING = set_wifiSTRING + line[i];
        }
      }
      if(set_wifiSTRING == "false"){
        set_wifi = false;
      }
      if(set_wifiSTRING == "true"){
        set_wifi = true;
      }
      //-----------------------------------------------------------
      if(brojac == 7){
        if(line[i] != '"'){
          ssidNEW = ssidNEW + line[i];
        }
      }
      //-----------------------------------------------------------
      if(brojac == 11){
        if(line[i] != '"'){
          passwordNEW = passwordNEW + line[i];
        }
      }
      //-----------------------------------------------------------
      if(brojac == 15){
        if(line[i] != '"'){
          on_offSTRING = on_offSTRING + line[i];
        }
      }
      if(on_offSTRING == "false"){
        on_off = false;
      }
      if(on_offSTRING == "true"){
        on_off = true;
      }
      //-----------------------------------------------------------
      if(brojac == 19){
        if(line[i] != '"'){
          dimerSTRING = dimerSTRING + line[i];
        }
      }
      dimer = dimerSTRING.toInt();
      //-----------------------------------------------------------
      if(brojac == 23){
        if(line[i] != '"'){
          id = id + line[i];
        }
      }
      //-----------------------------------------------------------
    }
    //ZA PROMENLJIVE ZA CUPANJE JSONA
  }
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void procitajJSONizVARIABLI(){
  Serial.println("\nIspis trenutnih stanja varijabli.");
  Serial.print("\nset_wifi: ");
  Serial.println(set_wifi);
  Serial.print("ssidNEW: ");
  Serial.println(ssidNEW);
  Serial.print("passwordNEW: ");
  Serial.println(passwordNEW);
  Serial.print("on_off: ");
  Serial.println(on_off);
  Serial.print("dimer: ");
  Serial.println(dimer);
  Serial.print("id: ");
  Serial.println(id);
}


