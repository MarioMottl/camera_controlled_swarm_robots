/**
 * @file ring_pipe.hpp
 * @author Pruggmayer Clemens
 * @brief Definition of a RT-capable byte-wise data pipe
 * @version 0.1
 * @date 2021-03-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cmsis_os2.h>

#define nullptr NULL

class ring_pipe
{
    osMemoryPoolId_t _mem;
    osSemaphoreId_t  _slots_data, _slots_empty;
    uint8_t *_m_start, *_m_end,
            *_d_start, *_d_end;
    bool _enable_put;
protected:
    /**
     * @brief access into buffer and shadow everything to the buffer
     * 
     * @param p pointer, range anything
     * @return uint8_t& reference to the shadowed data
     */
    uint8_t &access(uint8_t *p);
public:
    /**
     * @brief initialise a pipe
     * 
     * @param maxsize maximum size of the pipe
     * @return osStatus_t 
     */
    osStatus_t init(size_t maxsize);
    /**
     * @brief enable/disable putting into the pipe
     * 
     * @param newstate true: enable, flase: disbale
     * @return osStatus_t 
     */
    osStatus_t enable_put(bool newstate);
    /**
     * @brief put one byte into the queue
     * 
     * @param data to put into the queue
     * @param timeout 
     * @return osStatus_t 
     */
    osStatus_t put(uint8_t data, uint32_t timeout);
    /**
     * @brief pop and remove one byte from the pipe
     * 
     * @param data to pop from the queue
     * @param timeout 
     * @return osStatus_t 
     */
    osStatus_t pop(uint8_t &data, uint32_t timeout);
    /**
     * @brief query if the pipe is empty
     * 
     * @return true the pipe is empty
     * @return false the pipe is not empty
     */
    bool       is_empty(void);
};

struct ring_pipe_pair
{
    ring_pipe *src, *dst;
};
