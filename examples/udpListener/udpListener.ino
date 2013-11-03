// Demonstrates usage of the new udpServer feature.
// You can register the same function to multiple ports, and multiple functions to the same port.
//
// 2013-4-7 Brian Lee <cybexsoft@hotmail.com>
// 2013-11-03 Arno Moonen <info@arnom.nl>

#include <EtherCard.h>
#include <IPAddress.h>

#define ETHER_CS_PIN 8 // Chip Select pin for ENC28J60
#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// Ethernet interface ip address
static byte myip[] = { 192,168,0,200 };
// Gateway ip address
static byte gwip[] = { 192,168,0,1 };
#endif

// Ethernet mac address - must be unique on your network
static byte mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

// Callback that prints received packets to the serial port
void udpSerialPrint(byte dst_ip[4], word port, byte src_ip[4], const char *data, word len) {
  IPAddress dst(dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3]);
  IPAddress src(src_ip[0], src_ip[1], src_ip[2], src_ip[3]);
  Serial.print("\nDEST:\t");
  Serial.println(dst);
  Serial.print("SRC:\t");
  Serial.println(src);
  Serial.print("PORT:\t");
  Serial.println(port);
  Serial.print("DATA:\t");
  Serial.println(data);
  Serial.print("LEN:\t");
  Serial.println(len);
}

void setup() {
  Serial.begin(57600);
  Serial.println("\n[udpListener]");

  if (ether.begin(sizeof Ethernet::buffer, mymac, ETHER_CS_PIN) == 0)
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("Local IP:\t", ether.myip);
  ether.printIp("Gateway:\t", ether.gwip);
  ether.printIp("DNS serv:\t", ether.dnsip);

  // Register udpSerialPrint() to port 1337 on local IP only
  ether.udpServerListenOnPort(&udpSerialPrint, 1337);

  // Register udpSerialPrint() to port 42 on all IPs
  ether.udpServerListen(&udpSerialPrint, 42, true);
}

void loop() {
  // This must be called for ethercard functions to work.
  ether.packetLoop(ether.packetReceive());
}


/*
//Processing sketch to send test UDP packets.

import hypermedia.net.*;

 UDP udp;  // define the UDP object


 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true );     // <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message
 }

 void draw()
 {
 }

 void keyPressed() {
 String ip       = "192.168.0.200";  // the remote IP address
 int port        = 1337;    // the destination port

 udp.send("Greetings via UDP!", ip, port );   // the message to send

 }

 void receive( byte[] data ) {       // <-- default handler
 //void receive( byte[] data, String ip, int port ) {  // <-- extended handler

 for(int i=0; i < data.length; i++)
 print(char(data[i]));
 println();
 }
*/
