#pragma once

namespace Frostbite {
    namespace Utils {
        namespace Decrypt {
            void xorDecrypt(QByteArray& buffer, uint8_t iv);
        }
    }
}
