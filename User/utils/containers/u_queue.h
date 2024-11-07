//
// Created by Ng Thuy Quynh on 11/1/2024.
//

#ifndef U_QUEUE_H
#define U_QUEUE_H
#include "stdint.h"


#define QUEUE_SIZE 256

typedef struct{
	uint8_t m_queue[QUEUE_SIZE];
	uint16_t m_head;
	uint16_t m_tail;
}data_queue_t;

int queue_push(data_queue_t* _queue, uint8_t _data);

int queue_get_byte_available(data_queue_t* _queue);

int queue_get_bytes(data_queue_t* _queue, uint8_t* _buff, uint8_t _len, uint32_t _timeout);

int queue_reset(data_queue_t* _queue);

#endif //U_QUEUE_H
