#include <WiFi.h>

// Replace with your WiFi credentials
const char* ssid = "1";       // Your WiFi SSID
const char* password = ""; // Your WiFi password

WiFiServer server(80); // Create a web server on port 80

// Define GPIO pins
#define GPIO_1 2
#define GPIO_2 4
#define GPIO_3 5
#define GPIO_4 18

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Server started.");

  // Initialize GPIO pins
  pinMode(GPIO_1, OUTPUT);
  pinMode(GPIO_2, OUTPUT);
  pinMode(GPIO_3, OUTPUT);
  pinMode(GPIO_4, OUTPUT);
  
  // Ensure all pins are LOW initially
  digitalWrite(GPIO_1, LOW);
  digitalWrite(GPIO_2, LOW);
  digitalWrite(GPIO_3, LOW);
  digitalWrite(GPIO_4, LOW);
}

void controlGPIO(int option) {
  // Reset all pins to LOW
  digitalWrite(GPIO_1, LOW);
  digitalWrite(GPIO_2, LOW);
  digitalWrite(GPIO_3, LOW);
  digitalWrite(GPIO_4, LOW);

  // Set pins based on option
  if (option == 1) {
    digitalWrite(GPIO_1, HIGH);
    digitalWrite(GPIO_2, HIGH);
  } else if (option == 2) {
    digitalWrite(GPIO_3, HIGH);
    digitalWrite(GPIO_4, HIGH);
  }
}

void loop() {
  // Check if a client is connected
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected.");
    String request = "";

    // Read the client request
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        // Break out of loop once the request ends
        if (request.endsWith("\r\n\r\n")) break;
      }
    }

    // Debug: Print the request to the Serial Monitor
    Serial.println(request);

    // Parse the request for GPIO control
    if (request.indexOf("/GPIO=1") != -1) {
      controlGPIO(1); // Activate GPIO_1 and GPIO_2
      Serial.println("GPIO_1 and GPIO_2 HIGH");
    } else if (request.indexOf("/GPIO=2") != -1) {
      controlGPIO(2); // Activate GPIO_3 and GPIO_4
      Serial.println("GPIO_3 and GPIO_4 HIGH");
    } else if (request.indexOf("/GPIO=0") != -1) {
      controlGPIO(0); // Set all GPIOs to LOW
      Serial.println("All GPIOs LOW");
    }

    // Send an HTTP response to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>ESP32 GPIO Control</h1>");
    client.println("<p><a href=\"/GPIO=1\">Set GPIO_1 & GPIO_2 HIGH</a></p>");
    client.println("<p><a href=\"/GPIO=2\">Set GPIO_3 & GPIO_4 HIGH</a></p>");
    client.println("<p><a href=\"/GPIO=0\">Set All GPIOs LOW</a></p>");
    client.println("</html>");

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }
}
