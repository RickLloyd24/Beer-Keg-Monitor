/* Init Server Functions */
void InitServerFunctions( void ) {
   server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
     int args = request->args();
     Serialprintln("/get Request Received with " + String(args) +" arguments");
     BuildPage();
     request->send(200, "text/html", HTMLpage);
   });

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     int args = request->args();
     Serialprintln("Root Request Received with " + String(args) +" arguments");
     BuildPage();
     request->send(200, "text/html", HTMLpage);
   });

   server.on ("/favicon.ico", HTTP_GET, [] (AsyncWebServerRequest *request) {
      request->send(200, "image/png", imageString);
   });  
    
   server.onNotFound (notFound);
   server.begin();

}
 
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
  Serial.println(d);
}
