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
DataPack::DataPack(int i, float f, const char* str)
: m_Int(i)
, m_Float(f)
, m_String(str) {
}

DataPack::~DataPack() {
}

void DataPack::SetInt(int data) {
    m_Int = data;
}

void DataPack::SetFloat(float data) {
    m_Float = data;
}

void DataPack::SetString(const char* data) {
    m_String = data;
}

int DataPack::GetInt() const {
    return m_Int;
}

float DataPack::GetFloat() const {
    return m_Float;
}

std::string DataPack::GetString() const {
    return m_String;
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
        }
    }

    return data;
}

void DataCom::SetDataFloat(const char* name, float value) {
    DataPack* pack = GetData(name);
    if (pack) {
        pack->SetFloat(value);
    } else {
        assert(false);  // Invalid data
    }
}

void DataCom::SetDataInt(const char* name, int value) {
    DataPack* pack = GetData(name);
    if (pack) {
        pack->SetInt(value);
    } else {
        assert(false);  // Invalid data
    }
}

void DataCom::SetDataString(const char* name, const char* value) {
    DataPack* pack = GetData(name);
    if (pack) {
        pack->SetString(value);
    } else {
        assert(false);  // Invalid data
    }
}

float DataCom::GetDataFloat(const char* name) {
    float result = 0.0f;

    DataPack* pack = GetData(name);
    if (pack) {
        result = pack->GetFloat();
    } else {
        assert(false);  // Invalid data
    }

    return result;
}

int DataCom::GetDataInt(const char* name) {
    int result = 0;

    DataPack* pack = GetData(name);
    if (pack) {
        result = pack->GetInt();
    } else {
        assert(false);  // Invalid data
    }

    return result;
}

std::string DataCom::GetDataString(const char* name) {
    std::string result = "";

    DataPack* pack = GetData(name);
    if (pack) {
        result = pack->GetString();
    } else {
        assert(false);  // Invalid data
    }

    return result;
}

};  // namespace entity