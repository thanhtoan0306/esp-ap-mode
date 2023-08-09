#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set the credentials for your access point
const char* ssid = "Free-wifi/12345678";
const char* password = "12345678";

// Create an instance of the web server
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

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to the access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Print the IP address of the access point
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Handle root URL request
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  // Handle login form submission
  server.on("/login", []() {
    String username = server.arg("username");
    String password = server.arg("password");

    // Perform your login verification here
    // For demonstration purposes, let's just print the entered credentials
    Serial.print("Username: ");
    Serial.println(username);
    Serial.print("Password: ");
    Serial.println(password);

    // Redirect to a success page
    server.sendHeader("Location", "/success");
    server.send(302, "text/plain", "");
  });

  // Handle success page
  server.on("/success", []() {
    server.send(200, "text/html", "<h1>Login Successful!</h1>");
  });

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();
}
