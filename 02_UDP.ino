void sendPacketUDP(char* payload)
{
  Udp.beginPacket(remoteUdpIP, remoteUdpOutPort );
  Udp.printf(payload,2);
  Udp.endPacket();  

  //Debug print  
  Serial.println("UDP Packet was sent to:");
  Serial.println("IP address: ");
  Serial.print(remoteUdpIP);
  Serial.print(":");
  Serial.println(remoteUdpOutPort);
  Serial.println("Payload: ");
  Serial.println(payload);
  delay(50);
}

void receivePacketUDP()
{
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    newPacketFlag = true;
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());
    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
}

void getQuidoOutputStates()
{
  sendPacketUDP("*B1OR1\r");
  sendPacketUDP("*B1OR2\r");
  sendPacketUDP("*B1OR3\r");
  sendPacketUDP("*B1OR4\r");
  sendPacketUDP("*B2OR1\r");
  sendPacketUDP("*B2OR2\r");
  sendPacketUDP("*B2OR3\r");
  sendPacketUDP("*B2OR4\r");
}

void killEmAll(){
  sendPacketUDP("*B1OS1L\r");
  sendPacketUDP("*B1OS2L\r");
  sendPacketUDP("*B1OS3L\r");
  sendPacketUDP("*B1OS4L\r");
  sendPacketUDP("*B2OS1L\r");
  sendPacketUDP("*B2OS2L\r");
  sendPacketUDP("*B2OS3L\r");
  sendPacketUDP("*B2OS4L\r");
}
