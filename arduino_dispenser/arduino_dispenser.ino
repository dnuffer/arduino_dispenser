/*
 DHCP Chat  Server
 
 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield. 
 
 THis version attempts to get an IP address using DHCP
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 21 May 2011
 modified 9 Apr 2012
 by Tom Igoe
 Based on ChatServer example by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0x2C, 0x8C, 0x51, 0x2E, 0x93, 0x6F };
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(23);
const int 
PWM_A   = 3,
DIR_A   = 12,
BRAKE_A = 9,
SNS_A   = A0;

void setup() {
  
  // Configure the A output
  //pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  //pinMode(DIR_A, OUTPUT);    // Direction pin on channel A

  // Open serial communications and wait for port to open:
  Serial.begin(9600);


  // start the Ethernet connection:
  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // initialize the ethernet device not using DHCP:
    Ethernet.begin(mac, ip, gateway, subnet);
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  // start listening for clients
  server.begin();
  
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

String readString(EthernetClient client, char stopChar)
{
  String result;
  while (client.connected() && client.available())
  {
    char c = client.read();
    Serial.print("Read char: ");
    Serial.println((int)c);
    if (c == stopChar)
      break;
    result += c;
  }
  Serial.print("Read out: ");
  Serial.println(result);
  return result;
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    Serial.println("We have a new client");

    //client.setTimeout(10);
    String inputLine = readString(client, '\n'); //client.readStringUntil('\n');
    
    if (inputLine == "dispense")
    {
      client.println("Dispensing");
      digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
      digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward
    
      Serial.println("Enabling");
      analogWrite(PWM_A, 255);     // Set the speed of the motor, 255 is the maximum value
    
      delay(3 * 1000);                 // hold the motor at full speed
    
    
      Serial.println("Disabling");
      // now stop the motor by inertia, the motor will stop slower than with the brake function
      analogWrite(PWM_A, 0);       // turn off power to the motor
      client.println("Dispensed");
      delay(1);
      client.stop();
    }
    else
    {
      client.println("No comprendo");
      delay(1);
      client.stop();
    }
  }
}




/*
const int 
PWM_A   = 3,
DIR_A   = 12,
BRAKE_A = 9,
SNS_A   = A0;


void setup() {
  // Configure the A output
  pinMode(BRAKE_A, OUTPUT);  // Brake pin on channel A
  pinMode(DIR_A, OUTPUT);    // Direction pin on channel A

  // Open Serial communication
  Serial.begin(9600);
  Serial.println("Motor shield DC motor Test:\n");
}

void loop() {

// Set the outputs to run the motor forward

  digitalWrite(BRAKE_A, LOW);  // setting brake LOW disable motor brake
  digitalWrite(DIR_A, HIGH);   // setting direction to HIGH the motor will spin forward

  Serial.println("Enabling");
  analogWrite(PWM_A, 255);     // Set the speed of the motor, 255 is the maximum value

  delay(3 * 1000);                 // hold the motor at full speed


  Serial.println("Disabling");
  // now stop the motor by inertia, the motor will stop slower than with the brake function
  analogWrite(PWM_A, 0);       // turn off power to the motor

  //Serial.println("End of the motor shield test with DC motors. Thank you!");


  delay(31 * 1000);
  delay(30 * 1000);
  
}
*/

