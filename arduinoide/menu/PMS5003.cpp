#include "PMS5003.hpp"

PMS5003::PMS5003(HardwareSerial *reader, HardwareSerial *debugger)
{
    _reader = reader;
    _debugger = debugger;
    for(uint16_t i = 0; i < VALUES; i++)
        data.insert(std::pair<std::string, uint16_t>(std::string (labels[i]), i));
}

bool PMS5003::readData()
{
    /*
    if (!_debugger->available()) 
    {
        _reader->println("Debugger unavailable");
        return false;
    }*/
    _debugger->println("ELO");
    if (_reader->peek() != 0x42) 
    {
        _reader->read();
        _debugger->println("Reader done his job");
        return false;
    }

    if (_reader->available() < 32) 
    {
        _debugger->println("Reader unavailable");
        return false;
    }
        
    
    uint8_t buffer[BUFFERSIZE];    
    uint16_t checkSum {0};

    _reader->readBytes(buffer, BUFFERSIZE);

    for(uint8_t i = 0; i < 30; i++)
    {
        checkSum += buffer[i];
        _debugger->print("0x");
        _debugger->print(buffer[i+2], HEX);
        _debugger->print("\n");
    }

    _debugger->println();

    uint16_t buffer_u16[15];
    for(uint8_t i = 0; i < VALUES; i++)
    {
        buffer_u16[i] = buffer[2 + i*2 + 1];
        buffer_u16[i] += (buffer[2 + i*2] << 8);
        data[labels[i]] = buffer_u16[i];
    }
        

    if (checkSum != data["checksum"]) {
        _debugger->println("Checksum failure");
        return false;
    }
    return true;
}

std::map<std::string, uint16_t> PMS5003::returnData()
{
    return data;
}

void PMS5003::dumpSamples()
{
    for(uint16_t i = 0; i < VALUES; i++)
        _debugger->println((String)labels[i] + " " +(String)data[labels[i]]);
}
