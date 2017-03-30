//-----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 04
// Brief: Define some module used to comminucate
//-----------------------------------------------------
#ifndef GLB_EDITOR_COM_H_
#define GLB_EDITOR_COM_H_

#include <stdint.h>

namespace comm {

static const int32_t kUnkownPacket = 0;
static const int32_t kMaterialList = 1;

struct Packet {
    int32_t type;
    int32_t size_in_bytes;
    void* data;
};

class Comm {
public:
    static void Initialize(int16_t accept_port, int16_t post_port, bool listen);
    static void Terminate();

    static void Send(Packet* packet);
    static Packet* Recieve();
};

};  // namespace comm

#endif  // GLB_EDITOR_COM_H_