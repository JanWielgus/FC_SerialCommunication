/**
 * @file ITransceiver.h
 * @author Jan Wielgus
 * @brief Interface for all classes that can send and receive buffer of data.
 * @date 2020-08-04
 * 
 */

#ifndef ITRANSCEIVER_H
#define ITRANSCEIVER_H

#include <DataBuffer.h>


class ITransceiver
{
public:
    virtual ~ITransceiver() {}

    /**
     * @brief Initialize the communication.
     */
    virtual void begin() = 0;

    /**
     * @brief Send data from the buffer with provided size.
     * 
     * @param buffer Pointer to the data buffer.
     * @param size Amount of data to send (size of the buffer).
     * @return false if somethind went wrong and data were not sent, true otherwise.
     */
    virtual bool send(const uint8_t* buffer, size_t size) = 0;

    /**
     * @brief Send data from provided data buffer.
     * 
     * @param buffer Data buffer with data to send.
     * @return false if somethind went wrong and data were not sent, true otherwise.
     */
    virtual bool send(const DataBuffer& buffer) = 0;

    /**
     * @return amount of bytes that are waiting to receive
     * (or just positive value indicate, that there are some data to receive).
     */
    virtual size_t available() = 0;

    /**
     * @return DataBuffer with received data or empty buffer if no data were received.
     * Make your own copy of data in the buffer or just use it.
     * After calling this method, some data could still wait for receiving.
     */
    virtual DataBuffer receiveNextData() = 0;
};


#endif
