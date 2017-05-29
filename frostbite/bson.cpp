#include "precompiled.hpp"
#include "uberbuffer.hpp"
#include "bson.hpp"

namespace Frostbite {

void Bson::parse() {
    while(device->pos() < device->size()) {
        bson = readEntry();
//        qDebug() << entry;
//        qDebug() << bson;
    }
}

QJsonValue Bson::readEntry() {
    QJsonValue root;

    uint8_t byte = device->readByte();

    if (byte == 0)
        return root;
    uint8_t flags = byte >> 5;
    uint8_t typecode = byte & 0x1f;

    QString key;

    switch(flags){
    case 0x0:
        key = device->readNullTerminatedString();
        break;
    case 0x4:
        // do nothing for now
        break;
    default:
        qDebug("Unknown flag 0x%X with typecode 0x%X (original 0x%X) at 0x%X", flags, typecode, byte, device->pos() - 1) ;
        break;
    }


    switch(typecode) {
    case 0x01: // LIST
    {
        auto listSize = device->readVarint();
        auto listOffset = device->pos();

        QJsonArray list;

        while(device->pos() < listOffset + listSize) {
            auto entry = readEntry();
            if(!entry.isNull())
                list << entry;
        }

        if(!list.empty()) {
            if(flags == 0) {
                QJsonObject obj{{key,list}};
                root = QJsonValue(obj);
            }
            else if(flags == 0x4) {
                root = QJsonValue(list);
            }
        }
        break;
    }

    case 0x02: // Dict
    {
        QJsonObject dict;
        auto dictSize = device->readVarint();
        auto dictOffset = device->pos();

        while(device->pos() - dictOffset < dictSize) {
            auto value = readEntry();
            merge(dict, value.toObject());
        }
        if(flags == 0) {
            QJsonObject obj{{key,dict}};
            root = QJsonValue(obj);
        }
        else if(flags == 0x4) {
            root = QJsonValue(dict);
        }
    }
        break;
    case 0x06: // Bool
    {
        auto value = device->readBool();
        QJsonObject obj{{key,value}};
        root = QJsonValue(obj);
    }
        break;
    case 0x07: // AZCIIZ string
    {
        auto valueSize = device->readVarint();
        auto value = device->readString(valueSize);
        if(flags == 0) {
            QJsonObject obj{{key,value}};
            root = QJsonValue(obj);
        }
        else if(flags == 0x4) {
            root = QJsonValue(value);
        }
    }
        break;
    case 0x08: // int32
    {
        auto value = device->readInt32();
        if(flags == 0) {
            QJsonObject obj{{key,value}};
            root = QJsonValue(obj);
        }
        else if(flags == 0x4) {
            root = QJsonValue(value);
        }
    }
        break;
    case 0x09: // int64
    {
        auto value = device->readInt64();
        QJsonObject obj{{key,value}};
        root = QJsonValue(obj);
    }
        break;
    case 0x0F: // UUID
    {
        auto value = QString(device->read(0x10).toHex());
        if(flags == 0) {
            QJsonObject obj{{key,value}};
            root = QJsonValue(obj);
        }
        else if(flags == 0x4) {
            root = QJsonValue(value);
        }
    }
        break;
    case 0x10: // SHA1
    {
        auto value = QString(device->read(0x14).toHex());
        QJsonObject obj{{key,value}};
        root = QJsonValue(obj);
    }
        break;
    case 0x13: // blob
    {
        auto varSize = device->readVarint();
        auto value = device->readString(varSize);
        QJsonObject obj{{key,value}};
        root = QJsonValue(obj);
    }
        break;
    default:
        qDebug("Unknown type: 0x%x at 0x%x ", typecode, device->pos()) ;
        break;
    }

    return root;
}

void Bson::merge(QJsonObject &out, const QJsonObject &in) {
    auto keys = in.keys();

    for(auto key : keys) {
        out[key] = in[key];
    }
}

void TocParser::parse() {
    QFile infile(tocfilename);
    uint32_t headersize;
    uint8_t signature[0x20];

    if(infile.open(QIODevice::ReadOnly)) {
        infile.seek(infile.size() - 0x24);

        infile.read((char*)&headersize, sizeof(headersize));
        infile.read((char*)signature, sizeof(signature));

        infile.seek(0);

        QByteArray data = infile.read(infile.size() - headersize);

        Utils::Decrypt::xorDecrypt(data, data[0]);

        if(save_decrypted) {
            QFile output(tocfilename + ".out");
            if (output.open(QIODevice::WriteOnly)) {
                output.write(data);
            }

        }

        buffer = new UberBuffer;
        buffer->setData(data);
        buffer->open(QIODevice::ReadOnly);
        qint32 magic = buffer->readInt32();

        if (magic == 0x01CED100) {
            buffer->seek(0x22C); // magic

            bson = new Bson(buffer);
            bson->parse();
        }
    }
}

}
