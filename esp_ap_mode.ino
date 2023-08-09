#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Set the credentials for your access point
const char* ssid = "ig: thank_toanf";
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

// EEPROM addresses for username and password
int usernameAddr = 0;
int passwordAddr = 50;

// Timer variables
unsigned long previousMillis = 0;
const unsigned long interval = 1000;  // 1 second interval

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to the access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Print the IP address of the access point
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialize EEPROM
  EEPROM.begin(512);

  // Read username and password from EEPROM
  String savedUsername = "";
  String savedPassword = "";
  for (int i = 0; i < 50; ++i) {
    savedUsername += char(EEPROM.read(usernameAddr + i));
    savedPassword += char(EEPROM.read(passwordAddr + i));
  }

  // Handle root URL request
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  // Handle login form submission
  server.on("/login", []() {
    String username = server.arg("username");
    String password = server.arg("password");

    // Save the username and password in EEPROM
    for (int i = 0; i < username.length(); ++i) {
      EEPROM.write(usernameAddr + i, username[i]);
    }
    for (int i = 0; i < password.length(); ++i) {
      EEPROM.write(passwordAddr + i, password[i]);
    }
    EEPROM.commit();

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

  // Check if it's time to print EEPROM contents
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Print the EEPROM contents
    Serial.println("EEPROM Contents:");
    for (int i = 0; i < 50; ++i) {
      Serial.print(char(EEPROM.read(usernameAddr + i)));
    }
    Serial.println();
    for (int i = 0; i < 50; ++i) {
      Serial.print(char(EEPROM.read(passwordAddr + i)));
    }
    Serial.println();
  }
}
