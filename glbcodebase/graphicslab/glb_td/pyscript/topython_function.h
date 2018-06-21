//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: Interface that will be used in python file
// Note: When you add a new function, you must run the topython.py script
//------------------------------------------------------------------
#ifndef PYSCRIPT_TOPYTHON_FUNCTION_H_
#define PYSCRIPT_TOPYTHON_FUNCTION_H_

// Debug
void DebugPrint(const char* s);

// Entity
int EntityCreate();
void EntitySetDead(int id, int dead);
int EntityIsDead(int id);
void EntityAddRefCount(int id);
void EntityDecRefCount(int id);
void EntityAddTransformCom(int id,
                           float px, float py, float pz,
                           float rx, float ry, float rz,
                           float sx, float sy, float sz);
void EntityAddRenderCom(int id, const char* name,
                        float px, float py, float pz,
                        float rx, float ry, float rz,
                        float sx, float sy, float sz);
void EntityAddCameraCom(int id, float px, float py, float pz, float tx, float ty, float tz);
void EntityAddScriptCom(int id, const char* script); 
void EntityAddRoleCom(int id, int mt, int st);
void EntityAddArsenalCom(int id);
void EntityAddDataCom(int id);
void EntityAddCollisionCom(int id);
float EntityGetPosX(int id);
float EntityGetPosY(int id);
float EntityGetPosZ(int id);
void EntitySetPos(int id, float x, float y, float z);
float EntityGetRotX(int id);
float EntityGetRotY(int id);
float EntityGetRotZ(int id);
void EntitySetRot(int id, float x, float y, float z);
float EntityGetScaleX(int id);
float EntityGetScaleY(int id);
float EntityGetScaleZ(int id);
void EntitySetScale(int id, float x, float y, float z);
void EntityAddWeapon(int id, int wt, const char* script);
void EntityActiveWeapon(int id, int wt);
int EntityGetWeaponBulletNum(int id);
void EntitySetWeaponBulletNum(int id, int num);
void EntityMoveToTarget(int id, float speed, float tx, float ty, float tz);
int EntityFindCloestEnemy(int id);
void EntityShoot(int id);
void EntityAddIntData(int id, const char* name, int data);
void EntityAddFloatData(int id, const char* name, float data);
int EntityGetIntData(int id, const char* name);
float EntityGetFloatData(int id, const char* name);
void EntitySetIntData(int id, const char* name, int data);
void EntitySetFloatData(int id, const char* name, float data);
void EntitySetRotateByLookVec(int id, float x, float y, float z);
void EntitySetScaleByLookVec(int id, float x, float y, float z);
float EntityCalcDistBetweenEntities(int e0, int e1);
void EntitySetDrawEnable(int id, int display);
void EntityBeginIterate();
int EntityIterate();
void EntityEndIterate();
int EntityIsMainType(int id, int main_type);
int EntityIsSubType(int id, int sub_type);
void EntityUpdateCollision(int id);
void EntityCheckCollision(int id);
void EntityCollisionBeginIterate(int id);
int EntityCollisionIterate(int id);
void EntityCollisionEndIterate(int id);
float EntityGetCollisionWidth(int id);
float EntityGetCollisionLength(int id);
int EntityFindEntity(int main, int sub);

// Time
float TimeGetPrevGameTime();
float TimeGetCurGameTime();
float TimeGetGameFrameSpeed();

// Input
int InputGetPointAtEntity();
float InputGetCursorPosX();
float InputGetCursorPosY();
int InputHasKeyPressed(int key);
int InputHasMousePressed(int mouse);

#endif  // PYSCRIPT_TOPYTHON_FUNCTION_H_