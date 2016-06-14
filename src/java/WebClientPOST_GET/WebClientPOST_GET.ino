/*
  Repeating Web client

 This sketch connects to a a web server and makes a request
 using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
 the Adafruit Ethernet shield, either one will work, as long as it's got
 a Wiznet Ethernet module on board.

 This example uses DNS, by assigning the Ethernet client with a MAC address,
 IP address, and DNS address.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 19 Apr 2012
 by Tom Igoe
 modified 21 Jan 2014
 by Federico Vanzati
 modified 18 Mar 2016
 by Breno Beirigo

 http://www.arduino.cc/en/Tutorial/WebClientRepeating
 This code is in the public domain.

 */

#include <SPI.h>
#include <Ethernet.h>

/*ADDRESS OF ARDUINO ETHERNET SHIELD */
// assign a MAC address for the ethernet controller.
byte mac[] = { 0x00, 0x17, 0x13, 0x13, 0x19, 0x6b };// RESERVED MAC ADDRESS

// fill in an available IP address on your network here, for manual configuration:
IPAddress ip(192, 168, 0, 142);

/*ADDRESS OF SERVER WHERE DATABASE IS LOCATED*/
// fill in your Domain Name Server address here:
byte gateway[] = { 192, 168, 0, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };   

//char server[] = "www.arduino.cc";
IPAddress server(192,168,0,135);

//host address
String host = "192.168.0.135";

//Subfolders to locate resource in host
String resourcePath = "/ServerServlet/Receive";

//Connection port in server
int port = 8080;


// initialize the library instance:
EthernetClient client;

// last time you connected to the server, in milliseconds
unsigned long lastConnectionTime = 0;
// delay between updates, in milliseconds
const unsigned long postingInterval = 10L * 1000L; 
// the "L" is needed to use long type numbers

void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // give the ethernet module time to boot up:
  delay(1000);
  
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, gateway, subnet);
  
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  //Init random generated data (YOU SHOULD REPLACE FOR YOUR SENSORS' VALUES)
  randomSeed(analogRead(0));
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    String data = "temperatura="+String(random(40))+"&humidade="+String(random(100))+"&luz="+String(random(100));
    httpGETRequest(host,data,resourcePath);
    delay(2000);
    httpPOSTRequest(host,data,resourcePath);
  }
}

// this method makes a HTTP connection to the server:
void httpPOSTRequest(String host, String data, String resourcePath) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, port)) {
    Serial.println("connecting via POST...");
    // send the HTTP PUT request:
    client.println("POST "+resourcePath+" HTTP/1.1");
    client.println("Host: "+host);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


// this method makes a HTTP connection to the server:
void httpGETRequest(String host, String data, String resourcePath) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, port)) {
    Serial.println("connecting via GET...");
    // send the HTTP PUT request:
    client.println("GET "+resourcePath+"?"+data+" HTTP/1.1");
    client.println("Host: "+host);
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}
