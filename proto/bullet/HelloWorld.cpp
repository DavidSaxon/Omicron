#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

int main(int argc, char* argv[])
{
    //-------------------------------BULLET SETUP-------------------------------

    // the broad phase is algorithm used to reduce the number of collision
    // pairs that need checking
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    // the collision configuration allows you to fine tune the algorithms used
    // for the full (not broadphase) collision
    btDefaultCollisionConfiguration* collision_configuration =
        new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher =
        new btCollisionDispatcher(collision_configuration);
    // if we introduce different types of collision objects later (e.g. meshes
    // using btGImpactMeshShape) then you need to register a collision
    // algorithm to get the collision recognised.
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
    // a solver is what causes the objects to interact properly, taking into
    // account gravity, game logic, supplied forces, collisions, and hinge
    // constraints. (There are parallel versions of this available).
    btSequentialImpulseConstraintSolver* solver =
        new btSequentialImpulseConstraintSolver();

    // we can now instantiate the dynamics world
    btDiscreteDynamicsWorld* dynamics_world = new btDiscreteDynamicsWorld(
        dispatcher,
        broadphase,
        solver,
        collision_configuration
    );
    // and set gravity (we have chosen the Y axis to be up)
    dynamics_world->setGravity(btVector3(0, -10, 0));

    std::cout << "Hello world!" << std::endl;

    //-----------------------------COLLISION SETUP------------------------------

    // here we will create plane collision shape for the ground, and a sphere
    // collision shape for a falling ball.
    // It is a good to "instance" the collision shapes, e.g. if there were 5
    // balls they should all use the same collision shape.
    btCollisionShape* ground_shape =
        new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btCollisionShape* ball_shape = new btSphereShape(1);

    //-----------------------------RIGID BODY SETUP-----------------------------

    // create the motion state for the ground, no rotation, and -1 below the
    // origin to offset the 1 above the origin of collision plane
    btDefaultMotionState* ground_motion_state = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    // the first parameter is the mass of the ground, and the last parameter is
    // the inertia of the ground. Since the ground is static, we fill these
    // values with zero. Bullet considers a mass of 0 equivalent  to making a
    // body with infinite mass - it is immovable.
    btRigidBody::btRigidBodyConstructionInfo ground_rigid_body_info(
        0, ground_motion_state, ground_shape, btVector3(0, 0, 0));
    btRigidBody* ground_rigid_body = new btRigidBody(ground_rigid_body_info);
    // finally we add the ground to the world
    dynamics_world->addRigidBody(ground_rigid_body);

    // create the motion state for the sphere
    btDefaultMotionState* ball_motion_state = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    // since the sphere is dynamic, we will give it a mass of 1kg, and we can
    // calculate the inertia of the sphere
    btScalar ball_mass = 1;
    btVector3 ball_inertia(0, 0, 0);
    ball_shape->calculateLocalInertia(ball_mass, ball_inertia);
    // construct the rigid body for the ball
    btRigidBody::btRigidBodyConstructionInfo ball_rigid_body_info(
        ball_mass, ball_motion_state, ball_shape, ball_inertia);
    btRigidBody* ball_rigid_body = new btRigidBody(ball_rigid_body_info);
    // add the ball to the world
    dynamics_world->addRigidBody(ball_rigid_body);

    //---------------------------STEP THE SIMULATION----------------------------

    // we will step the world 300 times
    for(std::size_t i = 0; i < 300; ++i)
    {
        // step the world at 60hz
        dynamics_world->stepSimulation(1 / 60.0F, 10);

        // get the current transform of the sphere
        btTransform sphere_transform;
        ball_rigid_body->getMotionState()->getWorldTransform(sphere_transform);

        // print the height of the sphere
        std::cout << "sphere height: " << sphere_transform.getOrigin().getY()
                  << std::endl;
    }

    //---------------------------------CLEANUP----------------------------------

    // clean up
    delete ball_rigid_body;
    delete ball_motion_state;
    delete ground_rigid_body;
    delete ground_motion_state;
    delete ball_shape;
    delete ground_shape;
    delete dynamics_world;
    delete solver;
    delete dispatcher;
    delete collision_configuration;
    delete broadphase;

    return 0;
}
