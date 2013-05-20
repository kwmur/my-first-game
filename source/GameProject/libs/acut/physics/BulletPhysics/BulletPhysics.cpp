#include "BulletPhysics.h"


namespace acut {


	bool BulletPhysics::initialize()
	{
		/*
		m_collisionConfiguration.reset(new btDefaultCollisionConfiguration());

		m_dispatcher.reset(new btCollisionDispatcher(m_collisionConfiguration.get()));

		m_broadphase.reset(new btDbvtBroadphase());

		m_solver.reset(new btSequentialImpulseConstraintSolver);

		m_dynamicsWorld.reset(new btDiscreteDynamicsWorld(
			m_dispatcher.get(),
			m_broadphase.get(),
			m_solver.get(),
			m_collisionConfiguration.get()
		));

		//m_debugDrawer.reset(new BulletDebugDrawer);
		//m_dynamicsWorld->setDebugDrawer(m_debugDrawer.get());

		//m_dynamicsWorld->setInternalTickCallback(BulletInternalTickCallback);
		m_dynamicsWorld->setWorldUserInfo(this);

		*/

		return true;
	}


	BulletPhysics::~BulletPhysics()
	{
		/*
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; 0 <= i; --i) {
			btCollisionObject* const obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			//RemoveCollisionObject(obj);
		}

		for (ActorIDToBulletActorMap::iterator it = m_actorBodies.begin();
			it != m_actorBodies.end();
			++it
		) {
			BulletActor* pBulletActor = it->second;
			//delete pBulletActor;
		}
		m_actorBodies.clear();
		*/
	}


	void BulletPhysics::onUpdate(const float deltaSeconds)
	{
		/*
		m_dynamicsWorld->stepSimulation(deltaSeconds, 4);
		*/
	}


	void BulletPhysics::syncVisibleScene()
	{
		/*
		for (ActorIDToBulletActorMap::const_iterator it = m_actorBodies.begin();
			it != m_actorBodies.end();
			++it
		) {
			const ActorId id = it->first;

//			const ActorMotionState* const actorMotionState = 
//				static_cast<ActorMotionState*>(it->second->m_pRigidBody->getMotionState());
//			assert(actorMotionState);

//			boost::shared_ptr<IActor> gameActor = g_pApp->m_pGame->getActor(id);
//			if (gameActor) {
//				if (gameActor->getMat() != actorMotionState->m_worldToPositionTransform) {

//				}
//			}

		}

		*/
	}


};
