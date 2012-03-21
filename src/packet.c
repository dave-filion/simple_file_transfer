#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"packet.h"

/* PACKET FUNCTIONS */
Packet* initPacket(char* c, int f) {
   Packet* p = malloc(sizeof(Packet));
   p->fragment = f;

  // Check for dummy pointer
  if (c != NULL) {
    strcpy( p->contents, c );
  }
  return p;
}

Header* initHeader(char* filename, char* username) {
   Header* h = malloc(sizeof(Header));
   strcpy((char*) h->filename, filename);
   strcpy((char*) h->username, username);
   return h;
}

char* serializeHeader(Header* h) {
   char* s = malloc(HEADER_SIZE);   
   memset(s, 0, HEADER_SIZE);
   char* temp = s;

   int i = 0;
   while (h->filename[i] != 0) {
      (*temp) = h->filename[i];
      temp++;
      i++;
   }
   
   (*temp) = AND;
   temp++;

   i = 0;
   while (h->username[i] != 0) {
      (*temp) = h->username[i];
      temp++;
      i++;
   }
   (*temp) = 0;
   
   return s;
}

char* serializePacket(Packet* p) {
  char* buffer = malloc(TRANSMIT_SIZE);
  char* temp = buffer;
  int frag;
  char b[8];
  
  // convert fragment to string (ugh)
  frag = p->fragment;
  sprintf(b, "%d", frag);

  int i;
  for (i = 0; i < strlen(b); i++) {
     (*temp) = b[i];
     temp++;
  }
  
  (*temp) = AND;
  temp++;
    
  for (i = 0; i < PACKET_SIZE; i++) {
     (*temp) = p->contents[i];
     temp++;
  }

  return buffer;
}

Header* deserializeHeader(char* s) {
   char filename[32];
   char username[32];
   
   int i = 0;
   while ((*s) != AND) {
      filename[i] = (*s);
      i++;
      s++;
   }
   
   s++;
   i = 0;
   while ((*s) != 0) {
      username[i] = (*s);
      s++;
      i++;
   }
   
   Header* h = initHeader(filename, username);
   return h;
}

Packet* deserializePacket(char* s) {   
   char b[8];
   
   int i = 0;
   while ((*s) != AND) {
      b[i] = (*s);
      i++;
      s++;
   }
   
   int fragment;
   fragment = atoi(b);
   
   s++;
   i = 0;
   char contents[PACKET_SIZE];
   while ((*s) != 0) {
      contents[i] = (*s);
      s++;
      i++;
   }
   
   Packet* p = initPacket(contents, fragment);
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

int isLocked(PacketHolder* ph) {
   return ph->lock;
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
