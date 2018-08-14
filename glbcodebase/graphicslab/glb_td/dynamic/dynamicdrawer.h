//---------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/14
// Brief: Dynamic world debug drawer
//---------------------------------------------------------
#include "btBulletDynamicsCommon.h"

namespace dynamic {

class DynamicDebugDrawer : public btIDebugDraw {
public:
    virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
    virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
    virtual void	reportErrorWarning(const char* warningString);
    virtual void	draw3dText(const btVector3& location,const char* textString);
    virtual void	setDebugMode(int debugMode);
    virtual int		getDebugMode() const;
};

};  // namespace dynamic