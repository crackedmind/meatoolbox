#include "precompiled.hpp"
#include "uberbuffer.hpp"

int UberBuffer::readVarint() {
    int result = 0;
    char bitsRead = 0;

    while(true) {
        char value = 0;
        this->read((char*)&value, 1);
        result |= (value & 0x7f) << bitsRead;
        if ((value >> 7 ) == 0)
            return result;
        bitsRead += 7;
    }

}

bool UberBuffer::readBool()
{
    return readByte() == 1;
}

uint8_t UberBuffer::readByte() {
    uint8_t byte = 0;

    this->read((char*)&byte, 1);

    return byte;
}

int32_t UberBuffer::readInt32() {
    int32_t int32 = 0;

    this->read((char*)&int32, sizeof(int32_t));

    return int32;
}

int64_t UberBuffer::readInt64()
{
    int64_t int64 = 0;

    this->read((char*)&int64, sizeof(int64_t));

    return int64;
}

QString UberBuffer::readNullTerminatedString() {
    QString str;
    char ch = 0;
    this->read((char*)&ch, 1);
    while(ch > 0 && str.length() < this->pos()) {
        str.append(ch);
        this->read((char*)&ch, 1);
    }

    return str;
}

QString UberBuffer::readString(int size)
{
    return QString(this->read(size));
}
