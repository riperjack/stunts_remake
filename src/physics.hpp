#ifndef physics_hpp
#define physics_hpp

#include <btBulletDynamicsCommon.h>

class Physics {

public:
	Physics();
	~Physics();
	btDiscreteDynamicsWorld* dynamicsWorld;

private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
};

#endif