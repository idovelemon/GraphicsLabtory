//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/29
// Brief: Data component hold some tempoary data of this entity
//-------------------------------------------------------------
#include "datacom.h"

#include <assert.h>

namespace entity {

//----------------------------------------------------------------
DataPack::DataPack(int i, float f)
: m_Int(i)
, m_Float(f) {
}

DataPack::~DataPack() {
}

void DataPack::SetInt(int data) {
    m_Int = data;
}

void DataPack::SetFloat(float data) {
    m_Float = data;
}

int DataPack::GetInt() const {
    return m_Int;
}

float DataPack::GetFloat() const {
    return m_Float;
}

//----------------------------------------------------------------
DataCom::DataCom(Entity* entity)
: Component(CT_DATA, entity) {
}

DataCom::~DataCom() {
}

void DataCom::AddData(const char* name, DataPack data) {
    if (name != NULL) {
        m_Datas.insert(std::pair<std::string, DataPack>(std::string(const_cast<char*>(name)), data));
    }
}

DataPack* DataCom::GetData(const char* name) {
    DataPack* data = NULL;

    if (name != NULL) {
        std::map<std::string, DataPack>::iterator it = m_Datas.find(std::string(const_cast<char*>(name)));
        if (it != m_Datas.end()) {
            data = &(it->second);
        } else {
            assert(false && "Can not find data pack");
        }
    }

    return data;
}

};  // namespace entity