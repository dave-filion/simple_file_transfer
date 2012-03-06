#include<stdio.h>

#include"packet.h"


int main() {

  Packet* p = initPacket("hey there man", 1);
  char* s = serialize(p);
  Packet* p2 = deserialize(s);
  Packet* p3 = initPacket("how is it going\n", 2);

  PacketHolder* ph = initPacketHolder();
  
  if (isEmpty(ph)) {
    printf("ITS EMPTY!\n");
  }

  addPacket(ph, p3);
  addPacket(ph, p2);

  if (isEmpty(ph)) {
    printf("ITS EMPTY!\n");
  }

  Packet* first = removePacket(ph);
  Packet* second = removePacket(ph);

  printf("%s%s\n", first->contents, second->contents);

  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);
  addPacket(ph, p);

  if (isFull(ph)) {
    printf("ITS FULL!\n");
  }

  removePacket(ph);

  if (isFull(ph)) {
    printf("ITS FULL!\n");
  }

  addPacket(ph, p);
}
