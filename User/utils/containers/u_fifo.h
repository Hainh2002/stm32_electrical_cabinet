#ifndef U_FIFO_H
#define U_FIFO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void *buffer;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    uint32_t size;
#ifdef _FREERTOS
    SemaphoreHandle_t mutex;
#endif
} FIFO_t;

bool fifo_init(FIFO_t *fifo, uint32_t item_size, uint32_t item_num) ;
bool fifo_enqueue(FIFO_t *fifo, void *data) ;
bool fifo_dequeue(FIFO_t *fifo, void *data) ;
void fifo_free(FIFO_t *fifo) ;

#endif //U_FIFO_H
