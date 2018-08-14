//---------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/14
// Brief: Dynamic world debug drawer
//---------------------------------------------------------
#include "dynamicdrawer.h"

#include "glb.h"

namespace dynamic {

void DynamicDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
    glb::render::Render::AddLine(glb::math::Vector(from.getX(), from.getY(), from.getZ()), glb::math::Vector(to.getX(), to.getY(), to.getZ()), glb::math::Vector(color.getX(), color.getY(), color.getZ()));
}

void DynamicDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) {
    // TODO:
}

void DynamicDebugDrawer::reportErrorWarning(const char* warningString) {
    // TODO:
}

void DynamicDebugDrawer::draw3dText(const btVector3& location,const char* textString) {
    // TODO:
}

void DynamicDebugDrawer::setDebugMode(int debugMode) {
    // TODO:
}

int DynamicDebugDrawer::getDebugMode() const {
    return DBG_DrawWireframe;
}

};  // namespace dynamic