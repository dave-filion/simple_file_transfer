#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"packet.h"

/* PACKET FUNCTIONS */
Packet* initPacket(char* c, int f, Packet* p) {
  p->fragment = f;

  // Check for dummy pointer
  if (c != NULL) {
    strcpy( p->contents, c );
  }

  return p;
}


char* serialize(Packet* p, char* buffer) {
  buffer = malloc(((PACKET_SIZE) * 4) + 4);
  char* temp = buffer;
  
  *((int*) temp) = p->fragment;
  temp += 4;

  int i;
  for (i = 0; i < PACKET_SIZE; i++) {
    *((int*) temp) = (int) p->contents[i];
    temp += 4;
  }

  return buffer;
}


Packet* deserialize(char* s, Packet* p) {
  int* temp = (int*) s;
  char c[PACKET_SIZE];

  int fragment;
  fragment = *temp;
  temp++;

  int i;
  for (i = 0; i < PACKET_SIZE; i++) {
    c[i] = *((char*) temp);
    temp++;
  }

  p = malloc(sizeof(Packet));
  p = initPacket(c, fragment, p);

  return p;
}

/* PACKETHOLDER FUNCTIONS */
void initPacketHolder(PacketHolder* ph) {
  // Init packets to NULL
  int i;
  for (i = 0; i < NUM_PACKET_SLOTS; i++) {
    ph->packets[i] = NULL;
  }

  // Initially unlocked
  ph->lock = FALSE;
}

void lock(PacketHolder* ph) {
  ph->lock = TRUE;
}
void unlock(PacketHolder* ph) {
  ph->lock = FALSE;
}

int isEmpty(PacketHolder* ph) {
  int i;
  for (i = 0; i < NUM_PACKET_SLOTS; i++) {
    if (ph->packets[i] != NULL)
      return FALSE;
  }
  return TRUE;
}

int isFull(PacketHolder* ph) {
  int i;
  for (i = 0; i < NUM_PACKET_SLOTS; i++) {
    if (ph->packets[i] == NULL)
      return FALSE;
  }
  return TRUE;
}

Packet* addPacket(PacketHolder* ph, Packet* p) {
  int i;
  for (i = 0; i < NUM_PACKET_SLOTS; i++) {
    // Find an open spot
    if (ph->packets[i] == NULL) {
      printf("Adding packet with frag %d in slot %d\n", p->fragment, i);
      ph->packets[i] = p;
      return p;
    }
  }
  printf("ERROR: COULD NOT FIND ANY OPENINGS!\n");
}


Packet* removePacket(PacketHolder* ph, Packet* p) {
  int i;
  int minIndex = -1;

  // Find min index
  for (i = 0; i < NUM_PACKET_SLOTS; i++) {
    if (ph->packets[i] != NULL) {
      if (minIndex == -1){
	minIndex = i;
      } else {
	if (ph->packets[i] < ph->packets[minIndex]) {
	  minIndex = i;
	}
      }
    }
  }

  p = ph->packets[minIndex];
  ph->packets[minIndex] = NULL; // Remove from array
  //printf("REMOVING PACKET FROM SLOT %d\n", minIndex);
  return p;
}
