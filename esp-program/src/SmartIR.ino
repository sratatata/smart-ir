#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>

#include <FS.h>
#include <ESP8266WebServer.h>

#define DBG_OUTPUT_PORT Serial

const char* ssid = "wulkan";
const char* password = "wulkan123";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  setup_wifi_manager();
  setup_ota();
  setup_server();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_server(){
  // Server
  server.on("/health", handleHealth);
  server.on("/emmit", handleEmmit);
  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
  server.begin();
}

void setup_wifi_manager(){
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //reset settings - for testing
  //wifiManager.resetSettings();
  if (!wifiManager.autoConnect("IR_REMOTE_AP")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
}

void setup_ota(){
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void handleRoot() {
  SPIFFS.begin();
  File f = SPIFFS.open("/index.html", "r");

  if (f) {
    Serial.println("Serve: /index.html");

    size_t sent = server.streamFile(f, "text/html");
    f.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening index.html");
  }

  SPIFFS.end();
}

bool handleFileRead(String path){
  SPIFFS.begin();
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";

    DBG_OUTPUT_PORT.println("handleFileRead: " + path);
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    SPIFFS.end();
    return true;
  }
  SPIFFS.end();
  return false;
}

void handleHealth() {
  Serial.println("OK");
  server.send(200, "text/plain", "OK");
}

void handleEmmit(){
  if(server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");
  String code = server.arg(0);
  String type = server.arg(1);
  Serial.print("Emmit code: ");
  Serial.print(code);
  Serial.print(" on protocole: ");
  Serial.println(type);
  server.send(200, "text/plain", "OK");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
