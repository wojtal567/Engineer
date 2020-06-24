#include <PMS5003.hpp>

PMS5003::PMS5003()
{
    for(uint16_t i = 0; i < VALUES; i++)
        data.insert(std::pair<std::string, uint16_t>(std::string (labels[i]), i));
}

bool PMS5003::readData(Stream *reader, Stream *debugger)
{
    if (!debugger->available()) 
        return false;

    if (reader->peek() != 0x42) 
    {
        reader->read();
        return false;
    }

    if (reader->available() < 32) 
        return false;
    
    uint8_t buffer[BUFFERSIZE];    
    uint16_t checkSum {0};

    reader->readBytes(buffer, BUFFERSIZE);

    for(uint8_t i = 0; i < 30; i++)
    {
        checkSum += buffer[i];
        debugger->print("0x");
        debugger->print(buffer[i+2], HEX);
        debugger->print("\n");
    }

    debugger->println();

    uint16_t buffer_u16;
    for(uint8_t i = 0; i < VALUES; i++)
        data[labels[i]] = buffer[2 + i*2 + 1] + (buffer[2 + i*2] << 8);

    if (checkSum != data["checksum"]) {
        debugger->println("Checksum failure");
        return false;
    }

    return true;
}
