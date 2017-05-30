//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/29
// Brief: Data component hold some tempoary data of this entity
//-------------------------------------------------------------
#ifndef ENTITY_DATACOM_H_
#define ENTITY_DATACOM_H_

#include "component.h"

#include <map>
#include <string>

namespace entity {

//----------------------------------------------------------------
// Type Declaration

//----------------------------------------------------------------
class DataPack {
public:
    DataPack(int i, float f);
    virtual ~DataPack();

public:
    void SetInt(int data);
    void SetFloat(float data);
    int GetInt() const;
    float GetFloat() const;

protected:
    int m_Int;
    float m_Float;
};


//----------------------------------------------------------------

class DataCom : public Component {
public:
    DataCom(Entity* entity);
    virtual ~DataCom();

public:
    void AddData(const char* name, DataPack data);
    DataPack* GetData(const char* name);

protected:
    std::map<std::string, DataPack>        m_Datas;
};

};  // namespace entity

#endif  // ENTITY_DATACOM_H_