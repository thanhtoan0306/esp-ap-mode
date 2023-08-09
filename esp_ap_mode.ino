#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char *ssid = "MyESP8266AP";
const char *password = "testpassword";
 
ESP8266WebServer server(80);

// HTML login page
const char* htmlPage = "<html><body>"
                       "<h1>Login</h1>"
                       "<form action='/login' method='POST'>"
                       "<label for='username'>Username:</label>"
                       "<input type='text' id='username' name='username'><br><br>"
                       "<label for='password'>Password:</label>"
                       "<input type='password' id='password' name='password'><br><br>"
                       "<input type='submit' value='Submit'>"
                       "</form>"
                       "</body></html>";
                       
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.softAP(ssid, password);
 
  Serial.println();
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Server MAC address: ");
  Serial.println(WiFi.softAPmacAddress());
 
  server.on("/", handleRoot);
  server.begin();
 
  Serial.println("Server listening");
}
 
void loop() {
  server.handleClient();
}
