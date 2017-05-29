#pragma once

class UberBuffer : public QBuffer {
public:
    UberBuffer(QByteArray *buf) : QBuffer(buf) {
    }
    explicit UberBuffer() = default;

    int readVarint();

    bool    readBool();
    uint8_t readByte();
    int32_t readInt32();
    int64_t readInt64();

    QString readNullTerminatedString();
    QString readString(int size);
};
