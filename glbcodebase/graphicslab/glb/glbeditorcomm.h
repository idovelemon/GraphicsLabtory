//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 05
// Brief: Deal with communication with glb editor
//-----------------------------------------------------------
#ifndef GLB_GLBEDITOR_H_
#define GLB_GLBEDITOR_H_

namespace glb {

class EditorComm {
 public:
    static void Initialize();
    static void Destroy();
    static void PostData();
    static void AcceptData();
};  // EditorComm

};  // namespace glb

#endif  // GLB_EDITOR_H_