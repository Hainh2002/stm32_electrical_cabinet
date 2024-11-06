
#include "sm_hal_flash.h"
#include "stdlib.h"
#include "sm_hal_porting.h"
#include "sm_logger.h"

#define TAG "hal_flash"

typedef struct{
    flash_instance_t* m_instance;
    uint32_t m_block_size;
    uint32_t m_block_count;
    uint32_t m_start_addr;
    uint32_t m_end_addr;
}sm_hal_flash_impl_t;

#define _impl(x) (sm_hal_flash_impl_t*)(x)

static bool sm_flash_string_cmp(char const str1[], char const str2[], int32_t lenght){
    for(uint32_t i = 0; i < lenght; i++){
        if(str1[i] != str2[i]) return  0;
    }
    return 1;
}



sm_hal_flash_t* sm_hal_flash_init(void* _instance){
    sm_hal_flash_impl_t* this = malloc(sizeof(sm_hal_flash_impl_t));

    if(!this){
        return NULL;
    }

    this->m_instance = _instance;


    // R_FLASH_HP_Open(this->m_instance->p_ctrl, this->m_instance->p_cfg);
    // R_FLASH_HP_StartUpAreaSelect(this->m_instance, FLASH_STARTUP_AREA_BLOCK0, true);

    flash_info_t flash_info;
    // R_FLASH_HP_InfoGet(this->m_instance->p_ctrl, &flash_info);

    this->m_start_addr = flash_info.data_flash.p_block_array->block_section_st_addr;
    this->m_end_addr   = flash_info.data_flash.p_block_array->block_section_end_addr;
    this->m_block_size  = flash_info.data_flash.p_block_array->block_size;
    this->m_block_count = (this->m_end_addr - this->m_start_addr + 1) / this->m_block_size;
    return this;
}

void sm_hal_flash_deinit(sm_hal_flash_t *_this){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return;
    }

    // R_FLASH_HP_Close(this->m_instance->p_ctrl);
}

int32_t sm_hal_flash_get_block_size(sm_hal_flash_t *_this){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    return this->m_block_size;
}

int32_t sm_hal_flash_write_block(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    if(sm_hal_flash_erase_block(this, _addr, _size) < 0){
        return -1;
    }

    if(R_FLASH_HP_Write(this->m_instance->p_ctrl, (uint32_t)_data, _addr, _size) != FSP_SUCCESS){
        return (-1);
    }

    uint8_t read_buff[_size];
    memcpy(read_buff,(uint8_t*)_addr, _size);
    if(sm_flash_string_cmp((char*)read_buff, (char*)_data, _size) == 0 ){
        LOG_ERR(TAG, "Verify write %d byte to addr 0x%x FAILED", _size, _addr);
        return (-1);
    }

    return 0;
}

int32_t sm_hal_flash_write_page(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    return sm_hal_flash_write_block(_this, _addr, _data, _size);
}

int32_t sm_hal_flash_read(sm_hal_flash_t *_this, uint32_t _addr, void *_data, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this || _addr < this->m_start_addr ||(_addr + _size) > this->m_end_addr){
        return -1;
    }

    memcpy(_data, (uint8_t*)_addr, _size);
    return 0;
}

int32_t sm_hal_flash_erase_block(sm_hal_flash_t *_this, uint32_t _addr, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }
    // uint8_t extra_block = (_size % this->m_block_size) ? 1: 0;
    // uint32_t block_num = (uint32_t)(_size / this->m_block_size) + extra_block;

    // if(R_FLASH_HP_Erase(this->m_instance->p_ctrl, _addr, block_num) != FSP_SUCCESS){
    //     return (-1);
    // }

    // flash_result_t blank_check_result = FLASH_RESULT_BLANK;
    // uint32_t ret = R_FLASH_HP_BlankCheck(this->m_instance->p_ctrl, _addr, _size, &blank_check_result);

    // if(ret != FSP_SUCCESS || blank_check_result == FLASH_RESULT_NOT_BLANK){
    //     return -1;
    // }

    return 0;
}

int32_t sm_hal_flash_erase_page(sm_hal_flash_t *_this, uint32_t _addr, int32_t _size){
    sm_hal_flash_impl_t* this = _impl(_this);
    if(!this){
        return -1;
    }

    return sm_hal_flash_erase_block(_this, _addr, _size);
}
