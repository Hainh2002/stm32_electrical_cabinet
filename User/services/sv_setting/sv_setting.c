/*
 * sv_setting.c
 *
 *  Created on: Nov 9, 2024
 *      Author: vuonglk
 */

#include "sv_setting.h"
#include "sm_mb_slave_impl.h"
#include "sm_modbus_define.h"
#include "sm_logger.h"
#include "string.h"
#include "u_queue.h"

#define TAG "sv_setting"

#define FLASH_SETTING_ADDR 	(FLASH_BASE + 100 + 1024)
#define FLASH_SETTING_SIZE 	FLASH_PAGE_SIZE
#define SETTING_BUFF_SIZE 	124

const uint32_t MAGIC_VALUE = 0x5AA55AA5;

uint8_t* storage_buff = NULL;
const uint8_t* sm_flash_read(uint32_t _addr){
	storage_buff = (uint8_t*)_addr;
    return storage_buff;
}


int32_t flash_write_btyes(uint32_t _addr, const uint8_t* _data, uint32_t _size){
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct; // struct cấu hình xóa dữ liệu
    EraseInitStruct.Banks = 1;
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = _addr; // địa chỉ page
    EraseInitStruct.NbPages     = 1; //(diff_)/PAGESIZE;
    uint32_t PAGEError;
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
    	PAGEError = HAL_FLASH_GetError();
    	LOG_ERR(TAG, "Erase %d page %d flash error code %d\n",EraseInitStruct.NbPages, HAL_FLASH_GetError());
    }else{
        LOG_INF(TAG, "Erase full success with page with address 0x%x\n",EraseInitStruct.PageAddress);
    }
	HAL_FLASH_Lock();
    LOG_INF(TAG, "request write flash %d byte at address 0x%x\n",_size,EraseInitStruct.PageAddress);
	uint32_t buff_data = 0;
	uint32_t temp_size = _size;
    for(uint32_t i = 0; i < _size; i = i + 4){
    	if (temp_size > 4) buff_data = (uint32_t)(_data[i+3]<<24) + (uint32_t)(_data[i+2]<<16) + (uint32_t)(_data[i+1]<<8) + (uint32_t)(_data[i]);
    	else {
    		switch (_size% 4) {
    		case 0:
    			buff_data = (_data[i+3]<<24) + (_data[i+2]<<16) + (_data[i+1]<<8) + (_data[i]);
    			break;
    		case 1:
    			buff_data = (0xFF<<24) + (0xFF<<16) + (0xFF<<8) + (_data[i]);
    			break;
    		case 2:
    			buff_data = (0xFF<<24) + (0xFF<<16) + (_data[i+1]<<8) + (_data[i]);
    			break;
    		case 3:
    			buff_data = (0xFF<<24) + (_data[i+2]<<16) + (_data[i+1]<<8) + (_data[i]);
    			break;
    		default:
    			break;
    	}
    	}
        HAL_FLASH_Unlock();
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, _addr + i, buff_data) == HAL_OK)
        {
//        	LOG_INF(TAG, "write flash success value 0x%x at addr 0x%x\n",buff_data,EraseInitStruct.PageAddress + i);
        }else{
        	PAGEError = HAL_FLASH_GetError();
        	LOG_ERR(TAG, "write flash error code %d\n", HAL_FLASH_GetError());
        	LOG_ERR(TAG, "write flash error value 0x%x at addr 0x%x\n",buff_data, EraseInitStruct.PageAddress + i);
        	return -1;
        }
        HAL_FLASH_Lock();
    	temp_size = temp_size - 4;
    }
	LOG_INF(TAG, "write flash success %d byte at addr 0x%x\n",_size, EraseInitStruct.PageAddress);
    return _size;
}

typedef struct{
	data_queue_t m_data_queue;
	USART_TypeDef* m_port;
	sm_mb_slave_t* m_mb_slave;
	setting_info_t m_info;
	sv_setting_new_setting_cb m_cb;
	void* m_cb_arg;
}sv_setting_impl_t;

static sv_setting_impl_t g_setting;


static void reset_default_setting(setting_info_t* _info){
	_info->m_auto = 0;
	_info->m_cur_thread_hold = 5000;
	_info->m_start_time = 0;
	_info->m_stop_time = 0;
	memset(_info->m_sn, '\0', 32);
	memcpy(_info->m_sn, "Chua_khoi_tao", strlen("Chua_khoi_tao"));
}


MODBUS_ERROR_CODE modbus_cb_read_holding_registers(uint16_t _address,
                                                   uint16_t _quantity,
                                                   uint16_t *_registers_out,
                                                   uint8_t _unit_id,
                                                   void *_arg) {
    LOG_DBG(TAG, "Master read holding reg %d quantity %d", _address, _quantity);

    if(_quantity != SV_SETTING_REG_NUMBER || _address != SV_SETTING_REG_INDEX){
    	return MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE;
    }

    memcpy(_registers_out, &g_setting.m_info, sizeof(setting_info_t));
    return MODBUS_ERROR_NONE;
}

MODBUS_ERROR_CODE modbus_cb_write_multiple_registers_cb(uint16_t _address,
                                                        uint16_t _quantity,
                                                        const uint16_t* _registers,
                                                        uint8_t _unit_id,
                                                        void* _arg){
    LOG_DBG(TAG, "Master write multi reg %d quantity %d", _address, _quantity);

    if(_quantity != SV_SETTING_REG_NUMBER || _address != SV_SETTING_REG_INDEX){
    	return MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE;
    }

    memcpy(&g_setting.m_info, _registers, sizeof(setting_info_t));

    sv_setting_storage_info(g_setting.m_info);

    if(g_setting.m_cb){
    	g_setting.m_cb(&g_setting.m_info, g_setting.m_cb_arg);
    }
    return MODBUS_ERROR_NONE;
}

static const sm_mb_slave_cb_t g_mb_slave_callback = {
        .read_holding_registers = modbus_cb_read_holding_registers,
        .write_multiple_registers = modbus_cb_write_multiple_registers_cb,
};

static int32_t mb_slv_send_if(uint8_t _addr, const uint8_t* _data, uint16_t _len, int32_t _timeout, void* _arg){
	return 	HAL_UART_Transmit(g_setting.m_port, _data, _len, _timeout);
}

static int32_t mb_slv_rcv_if(uint8_t _addr, uint8_t *_buf, uint16_t _max_len, int32_t _timeout, void *_arg) {
    return queue_get_bytes(&g_setting.m_data_queue, _buf, _max_len, _timeout);
}

void sv_setting_init(USART_TypeDef* _port){
	g_setting.m_port = _port;
	g_setting.m_mb_slave = sm_mb_slave_create_default(0x01, mb_slv_send_if, mb_slv_rcv_if, &g_setting);
	g_setting.m_mb_slave->m_proc->init(g_setting.m_mb_slave);
	g_setting.m_mb_slave->m_proc->reg_function_cb(g_setting.m_mb_slave, &g_mb_slave_callback);

	sv_setting_load_info(&g_setting.m_info);
}

setting_info_t sv_setting_get_info(){
	return g_setting.m_info;
}

int sv_setting_storage_info(setting_info_t info){
	g_setting.m_info = info;

	uint8_t buff[SETTING_BUFF_SIZE] = {0,};

	memcpy(buff, &MAGIC_VALUE, 4);
	memcpy(buff + 4, &info, sizeof(setting_info_t));

	if (flash_write_btyes(FLASH_SETTING_ADDR, buff, SETTING_BUFF_SIZE < 0)){
		LOG_ERR(TAG, "Could't not save setting to flash");
		return -1;
	}
	return 0;
}

int sv_setting_load_info(setting_info_t* info){
	const uint8_t* buff = sm_flash_read(FLASH_SETTING_ADDR);

	uint32_t* maigic = (uint32_t*)buff;

	if(*maigic != MAGIC_VALUE){
		LOG_WRN(TAG, "Setting is not init yet, restore default");
		reset_default_setting(info);
	}else{
		memcpy(info, buff + 4, sizeof(setting_info_t));
		LOG_INF(TAG, "Setting loaded!!!");
		LOG_INF(TAG, "Cabinet name is %s", info->m_sn);
		LOG_INF(TAG, "Cabinet mode is %s", info->m_auto?"Auto":"Manual");
		LOG_INF(TAG, "Cabinet sync time is %d", info->m_sync_time);
	}
	return 0;
}

void sm_sv_setting_feed_byte(uint8_t _byte){
	queue_push(&g_setting.m_data_queue, _byte);
}

void sv_setting_set_new_setting_cb(sv_setting_new_setting_cb _cb, void* _arg){
	g_setting.m_cb_arg = _arg;
	g_setting.m_cb = _cb;
}

void sv_setting_process(){
	sm_mb_slave_t* mb = g_setting.m_mb_slave;
	if(mb){
		mb->m_proc->polling(mb);
	}
}

