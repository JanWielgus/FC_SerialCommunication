	// Author: Jan Wielgus
// Date: 29.01.2019
//

#include <FC_SerialCommBase.h>


FC_SerialCommBase::FC_SerialCommBase(Stream* serial, size_t bufSize): BufferSize(bufSize)
{
	this->serial = serial;

	decodedData.buffer = nullptr; // pointer to this array is set after successful receiving
	
	// allocate memory for receive and decode buffer
	receiveBuffer = new uint8_t[BufferSize];
	decodeBuffer = new uint8_t[BufferSize];
}


FC_SerialCommBase::~FC_SerialCommBase()
{
	delete [] receiveBuffer;
	delete [] decodeBuffer;
}


bool FC_SerialCommBase::send(uint8_t* buffer, size_t size)
{
	if (buffer==0 || size==0)
		return;

	size++; // create room for the checksum
	
	uint8_t* encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(size)];

	// copy all data + checksum at the end to the encodeBuffer array
	// to don't create separate array for data + checksum, because checksum cannot be added to the buffer directly
	for (int i = 0; i < size - 1; i++)
		encodeBuffer[i] = buffer[i];
	encodeBuffer[size - 1] = calcChecksum(buffer, size - 1);
	
	size_t numEncoded = COBS::encode(/*buffer*/ encodeBuffer, size, encodeBuffer);
	
	serial->write(encodeBuffer, numEncoded);
	serial->write(PacketMarker);
	
	delete [] encodeBuffer;
	return true;
}



DataBuffer FC_SerialCommBase::receiveNextData()
{
	while (serial->available() > 0)
	{
		uint8_t data = serial->read();
		
		if (data == PacketMarker)
		{
			size_t numDecoded = COBS::decode(receiveBuffer, receiveBufferIndex, decodeBuffer);
			
			decodedData.buffer = decodeBuffer;
			decodedData.size = numDecoded;

			// reset index in the received data buffer
			receiveBufferIndex = 0;


			// check the checksum
			if (checkChecksum(decodedData.buffer, decodedData.size))
				decodedData.size--; // remove checksum from the array
			else
				decodedData.size = 0; // checksum failed, received buffer is 0
			

			// Return decoded data packet (with zero size if checksum failed)
			return decodedData;
		}
		else
		{
			if ((receiveBufferIndex + 1) < BufferSize)
				receiveBuffer[receiveBufferIndex++] = data;
			else
			{
				// ERROR, buffer oberflow if we write. 
			}
		}
	}
	
	// Any complete data packet was received
	decodedData.size = 0;
	return decodedData;
}


size_t FC_SerialCommBase::available()
{
	return serial->available();
}



// size - all data + 1 for checksum (assuming that checksum value is after data in buffer)
bool FC_SerialCommBase::checkChecksum(const uint8_t* buffer, size_t size)
{
	uint8_t checksum = calcChecksum(buffer, size - 1);
	return checksum == buffer[size - 1]; // checksum is stored at the last place in buffer
}


// size - only size of the data included in checksum
uint8_t FC_SerialCommBase::calcChecksum(const uint8_t* buffer, size_t size)
{
	uint8_t checksum = buffer[0];
	for (int i = 1; i < size; i++)
		checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
	
	return checksum;
}

