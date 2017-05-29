#include "precompiled.hpp"
#include "decrypt.hpp"

namespace Frostbite {
    namespace Utils {
        namespace Decrypt {
            void xorDecrypt(QByteArray &buffer, uint8_t iv)
            {
                uint8_t key = iv;
                for (int i=0;i<buffer.length(); i++)
                {
                    uint8_t key1 = iv;
                    key1 ^= buffer[i];
                    key1 -= (uint8_t)i;
                    buffer[i] = buffer[i] ^ key;
                    key = key1;
                }
            }
        }
    }
}
