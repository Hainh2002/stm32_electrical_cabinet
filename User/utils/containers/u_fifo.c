#include "u_fifo.h"

#ifndef _RTOS

// Initialize the FIFO with dynamic allocation
bool fifo_init(FIFO_t *fifo, uint32_t item_size, uint32_t item_num) {
    fifo->buffer = NULL;
    fifo->buffer = calloc(item_num, item_size);
    if (fifo->buffer == NULL) {
        return false;  // Allocation failed
    }
    fifo->head = 0;
    fifo->tail = 0;
    fifo->count = 0;
    fifo->size = item_size;
    return true;
}

// Add data to the FIFO
bool fifo_enqueue(FIFO_t *fifo, void* data) {
    if (fifo->count == fifo->size) {
        // FIFO is full
        return false;
    }
    memcpy(fifo->buffer + fifo->head * fifo->size, data, fifo->size);
    fifo->head = (fifo->head + 1) % fifo->size;
    fifo->count++;
    return true;
}

// Remove data from the FIFO
bool fifo_dequeue(FIFO_t *fifo, void *data) {
    if (fifo->count == 0) {
        // FIFO is empty
        return false;
    }
    memcpy(fifo->buffer + fifo->tail * fifo->size, data, fifo->size);
    fifo->tail = (fifo->tail + 1) % fifo->size;
    fifo->count--;
    return true;
}

// Free the FIFO buffer
void fifo_free(FIFO_t *fifo) {
    free(fifo->buffer);
    fifo->buffer = NULL;
    fifo->size = 0;
    fifo->count = 0;
}

#else

#include "FreeRTOS.h"
#include "semphr.h"

// Initialize the FIFO with dynamic allocation and a mutex
bool fifo_init(FIFO_t *fifo, uint32_t item_size, uint32_t item_num) {
    fifo->buffer = pvPortMalloc(item_size * item_num);
    if (fifo->buffer == NULL) {
        return false;  // Allocation failed
    }
    memset(fifo->buffer, 0, item_size * item_num);
    fifo->head = 0;
    fifo->tail = 0;
    fifo->count = 0;
    fifo->size = size;
    fifo->mutex = xSemaphoreCreateMutex();
    return fifo->mutex != NULL;
}

// Add data to the FIFO in a thread-safe way
bool fifo_enqueue(FIFO_t *fifo, void *data) {
    bool result = false;
    if (xSemaphoreTake(fifo->mutex, portMAX_DELAY) == pdTRUE) {
        if (fifo->count < fifo->size) {
            memcpy(fifo->buffer + fifo->head * fifo->size, data, fifo->size);
            fifo->head = (fifo->head + 1) % fifo->size;
            fifo->count++;
            result = true;
        }
        xSemaphoreGive(fifo->mutex);
    }
    return result;
}

// Remove data from the FIFO in a thread-safe way
bool fifo_dequeue(FIFO_t *fifo, void *data) {
    bool result = false;
    if (xSemaphoreTake(fifo->mutex, portMAX_DELAY) == pdTRUE) {
        if (fifo->count > 0) {
            memcpy(fifo->buffer + fifo->tail * fifo->size, data, fifo->size);
            fifo->tail = (fifo->tail + 1) % fifo->size;
            fifo->count--;
            result = true;
        }
        xSemaphoreGive(fifo->mutex);
    }
    return result;
}

// Free the FIFO buffer and mutex
void fifo_free(FIFO_t *fifo) {
    if (fifo->buffer != NULL) {
        free(fifo->buffer);
        fifo->buffer = NULL;
    }
    if (fifo->mutex != NULL) {
        vSemaphoreDelete(fifo->mutex);
    }
    fifo->size = 0;
    fifo->count = 0;
}

#endif