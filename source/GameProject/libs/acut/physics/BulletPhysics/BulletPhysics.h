#pragma once

#include "btBulletDynamicsCommon.h"
#include <boost/optional.hpp>
#include <boost/utility.hpp>
#include <map>
#include <set>
#include <memory>


class BulletActor;
typedef unsigned ActorId;


namespace acut {


	class BulletPhysics : boost::noncopyable {

	public:
		~BulletPhysics();

		// 物理世界の初期化とメンテナンス
		bool initialize();
		void syncVisibleScene();
		void onUpdate(float deltaSeconds);


		/*
		std::auto_ptr<btDynamicsWorld> m_dynamicsWorld;
		std::auto_ptr<btBroadphaseInterface> m_broadphase;
		std::auto_ptr<btCollisionDispatcher> m_dispatcher;
		std::auto_ptr<btConstraintSolver> m_solver;
		std::auto_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		//std::auto_ptr<BulletDebugDrawer> m_debugDrawer;

		typedef std::map<ActorId, BulletActor*> ActorIDToBulletActorMap;
		ActorIDToBulletActorMap m_actorBodies;

		typedef std::map<btRigidBody const *, ActorId> RigidBodyToActorIDMap;
		RigidBodyToActorIDMap m_rigidBodyToActorId;

		typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
		typedef std::set<CollisionPair> CollisionPairs;
		CollisionPairs m_previousTickCollisionPairs;	

		void sendCollisionPairAddEvent(
			btPersistentManifold const* manifold,
			btRigidBody const* body0,
			btRigidBody const* body1
		);
		void sendCollisionPairRemoveEvent(
			btRigidBody const* body0,
			btRigidBody const* body1
		);

		//void addShape(IActor* actor, btCollisionShape* shape, btScalar mass, enum PhysicsMaterial material);

		void removeCollisionObject(btCollisionObject* removeMe);

		btRigidBody* findActorBody(ActorId id) const;

//		BulletActor* findBulletActor(ActorId id) const;

		boost::optional<ActorId> findActorID(btRigidBody const*) const;

		static void bulletInternalTickCallback(btDynamicsWorld* const world, btScalar const timeStep);

	public:
		BulletPhysics();
		~BulletPhysics();

		// 物理世界の初期化とメンテナンス
		bool initialize();
		void syncVisibleScene();
		void onUpdate(float deltaSeconds);

		// 物理オブジェクトの初期化
		void addSphere(
			float radius,
			//IActor* actor,
			float specificGravity,
			enum PhysicsMaterial material
		);
		void addBox(
//			const Vec3& dimensions,
			//IActor* gameActor,
			float specificGravity,
			enum PhysicsMaterial material
		);
		void addPointCloud(
//			Vec3* verts,
			int numPoints,
//			IActor* gameActor,
			float specificGravity,
			enum PhysicsMaterial material
		);
		void removeActor(ActorId id);

		// デバッグ
		void renderDiagnostics();

		// 物理世界モディファイア
		void createTrigger(
			//const Vec3& pos,
			const float dim,
			int triggerID
		);
		void applyForce(
//			const Vec3& dir,
			float newtons,
			ActorId actorId
		);
		void applyTorque(
//			const Vec3& dir,
			float newtons,
			ActorId actorId
		);
//		bool kinematicMove(const Mat4x4 &mat, ActorId actorId);

		void rotateY(ActorId actorId, float angleRadians, float time);
*/
	};

};

