#pragma once

#include "decrypt.hpp"

namespace Frostbite {
    class Bson
    {
        UberBuffer *device;
        QJsonValue bson;
    public:
        explicit Bson(UberBuffer *buf) : device(buf) {
            if(!device->isOpen()) {
                device->open(QIODevice::ReadOnly);
            }
        }

        ~Bson() {
            device->close();
        }

        void parse();

        QJsonValue getJson() {
            return bson;
        }

        QJsonValue readEntry();

        void merge(QJsonObject& out, const QJsonObject &in);
    };

    class TocParser {
        QString tocfilename;
        Bson *bson = nullptr;
        UberBuffer *buffer = nullptr;
        bool save_decrypted = true;
    public:
        TocParser(const QString &filename) :tocfilename(filename) {
        }

        ~TocParser() {
//            delete bson;
//            delete buffer;
        }

        void parse();

        Bson* getBson() { return bson; }
    };

    class SbParser {
        QString sbname;
        Bson *bson = nullptr;
        UberBuffer *buffer = nullptr;
    public:
        SbParser(const QString &filename): sbname(filename) {
        }
        Bson* getBson() { return bson; }
        void parse() {
            QFile infile(sbname);

            if(infile.open(QIODevice::ReadOnly)) {

                QByteArray data = infile.readAll();

                buffer = new UberBuffer;
                buffer->setData(data);
                buffer->open(QIODevice::ReadOnly);

                bson = new Bson(buffer);
                bson->parse();
            }
        }
    };
}
