#include "ring_pipe.hpp"

uint8_t &ring_pipe::access(uint8_t *p)
{
    return _m_start[ (p - _m_start) % (_m_end - _m_start) ];
}

osStatus_t ring_pipe::init(size_t maxsize)
{
    this->_slots_data = osSemaphoreNew(maxsize, 0, NULL);
    this->_slots_empty = osSemaphoreNew(maxsize, maxsize, NULL);
    this->_mem = osMemoryPoolNew(1, maxsize, NULL);
    this->_m_start = (uint8_t*)osMemoryPoolAlloc(this->_mem, 0);
    this->_m_end = _m_start + maxsize;
    this->_d_start = this->_d_end = this->_m_start;
    this->_enable_put = true;
    return osOK;
}

osStatus_t ring_pipe::enaple_put(bool newstate)
{
    this->_enable_put = newstate;
    return osOK;
}

osStatus_t ring_pipe::put(uint8_t data, uint32_t timeout)
{
    if(!this->_enable_put) {return osErrorParameter;}
    osStatus_t status;
    // remove 1 empty slot, insert data, and add 1 data slot
    if( (status = osSemaphoreAcquire(this->_slots_empty, timeout)) != osOK) {return status;} // return on timeout
    this->access(this->_d_end++) = data;
    osSemaphoreRelease(this->_slots_data);
    return osOK;
}

osStatus_t ring_pipe::pop(uint8_t &data, uint32_t timeout)
{
    osStatus_t status;
    if( (status = osSemaphoreAcquire(this->_slots_data, timeout)) != osOK) {return status;} // return on timeout
    data = this->access(this->_d_start++);
    osSemaphoreRelease(this->_slots_empty);
    return osOK;
}
