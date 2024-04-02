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
