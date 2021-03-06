/**
 * @file IReceiver.h
 * @author Jan Wielgus (jan.wielgus12@gmail.com)
 * @brief Interface for classes that enables receiving
 * arrays of bytes (uint8_t).
 * @date 2021-04-10
 * 
 */

#ifndef IRECEIVER_H
#define IRECEIVER_H

#include "DataBuffer.h"


/**
 * @brief Interface for classes that enables receiving
 * arrays of bytes (uint8_t).
 */
class IReceiver
{
public:
    virtual ~IReceiver() {}

    /**
     * @brief Receive next available data buffer. Call before getReceivedData() method
     * to get new data packet each time.
     * @return true if a data packet with size greater than zero was received;
     * false if there were no data (getReceivedData() method will return an empty buffer).
     */
    virtual bool receiveData() = 0;

    /**
     * @brief Call receiveData() method before to get the newest buffer. Can be called
     * multiple times in a row (to get the same buffer).
     * @return DataBuffer with received data or empty buffer if no data was received.
     * Make your own copy of data in the buffer or just use returned buffer.
     */
    virtual DataBuffer getReceivedData() = 0;
};


#endif
