//
// Created by Ng Thuy Quynh on 11/1/2024.
//

#include "u_queue.h"
#include "sm_elapsed_timer.h"

int queue_push(data_queue_t* _queue, uint8_t _data){
	_queue->m_queue[_queue->m_head] = _data;
	_queue->m_head++;
	if(_queue->m_head >= QUEUE_SIZE){
		_queue->m_head = 0;
	}
	return 0;
}

int queue_get_byte_available(data_queue_t* _queue){
	uint16_t head = _queue->m_head;
	uint16_t tail = _queue->m_tail;
	uint16_t size = QUEUE_SIZE;
	return (head >= tail) ? head - tail : size - tail + head;
}

int queue_get_bytes(data_queue_t* _queue, uint8_t* _buff, uint8_t _len, uint32_t _timeout){

	elapsed_timer_t timeout;
	elapsed_timer_resetz(&timeout, _timeout);

	do{
		if(queue_get_byte_available(_queue) >= _len){
			for(int i = 0; i < _len; i++){
				_buff[i] = _queue->m_queue[_queue->m_tail];
				_queue->m_tail++;
				if(_queue->m_tail >= QUEUE_SIZE){
					_queue->m_tail = 0;
				}
			}
			return _len;
		}
	}
	while(elapsed_timer_get_remain(&timeout));
	return 0;
}

int queue_reset(data_queue_t* _queue){
	_queue->m_head = _queue->m_tail = 0;
	return 0;
}

