#ifndef __USERPROG__
#define __USERPROG__
#include "lab2-api.h"

typedef struct buffer {
  int head;
  int tail;
  char content[BUFFERSIZE];
} buffer;

#define PRODUCER "producer.dlx.obj"
#define CONSUMER "consumer.dlx.obj"
#endif
