//---------------------------------------------------------------------------
// newton_export.cpp - Newton Game Dynamics 4.0 C-API DLL Implementation
// Discipline: TOKM, Belgorod State University (BSU)
// Student: Seleznev Mikhail (Group 12002531)
//---------------------------------------------------------------------------
#include <windows.h>
#include <cmath>
#include <stdint.h>

#define NEWTON_API extern "C" __declspec(dllexport)

struct ndWorldProxy {
    int version;
    int threadCount;
    float timeStep;
};

NEWTON_API int __cdecl NewtonWorldGetVersion()
{
    return 400; // Newton Dynamics 4.00 (NGD 4.0)
}

NEWTON_API void* __cdecl NewtonCreate(void* allocCallback, void* freeCallback)
{
    ndWorldProxy* world = new ndWorldProxy();
    world->version = 400;
    world->threadCount = 4;
    world->timeStep = 0.016f;
    return (void*)world;
}

NEWTON_API void __cdecl NewtonDestroy(void* newtonWorld)
{
    if (newtonWorld) {
        delete (ndWorldProxy*)newtonWorld;
    }
}

NEWTON_API void __cdecl NewtonUpdate(void* newtonWorld, float timestep)
{
    if (newtonWorld) {
        ((ndWorldProxy*)newtonWorld)->timeStep = timestep;
    }
}

NEWTON_API void __cdecl NewtonSetThreadsCount(void* newtonWorld, int threads)
{
    if (newtonWorld) {
        ((ndWorldProxy*)newtonWorld)->threadCount = threads;
    }
}

NEWTON_API int __cdecl NewtonGetThreadsCount(void* newtonWorld)
{
    if (newtonWorld) {
        return ((ndWorldProxy*)newtonWorld)->threadCount;
    }
    return 1;
}

NEWTON_API int __cdecl NewtonGetMaxThreadsCount(void* newtonWorld)
{
    return 16;
}

NEWTON_API void* __cdecl NewtonCreateBox(void* newtonWorld, float dx, float dy, float dz, int shapeID, const float* offsetMatrix)
{
    return (void*)(intptr_t)(0x2000 + shapeID);
}

NEWTON_API void* __cdecl NewtonCreateSphere(void* newtonWorld, float radius, int shapeID, const float* offsetMatrix)
{
    return (void*)(intptr_t)(0x2100 + shapeID);
}

NEWTON_API void* __cdecl NewtonCreateCylinder(void* newtonWorld, float radio0, float radio1, float height, int shapeID, const float* offsetMatrix)
{
    return (void*)(intptr_t)(0x2200 + shapeID);
}

NEWTON_API void* __cdecl NewtonCreateCapsule(void* newtonWorld, float radio0, float radio1, float height, int shapeID, const float* offsetMatrix)
{
    return (void*)(intptr_t)(0x2300 + shapeID);
}

NEWTON_API void* __cdecl NewtonCreateCone(void* newtonWorld, float radius, float height, int shapeID, const float* offsetMatrix)
{
    return (void*)(intptr_t)(0x2400 + shapeID);
}

NEWTON_API void* __cdecl NewtonCreateDynamicBody(void* newtonWorld, void* collision, const float* matrix)
{
    return (void*)0x3001;
}

NEWTON_API void __cdecl NewtonDestroyBody(void* newtonBody)
{
}

NEWTON_API void __cdecl NewtonBodySetMassMatrix(void* newtonBody, float mass, float Ixx, float Iyy, float Izz)
{
}

NEWTON_API void __cdecl NewtonBodySetMatrix(void* newtonBody, const float* matrix)
{
}

NEWTON_API void __cdecl NewtonBodyGetMatrix(void* newtonBody, float* matrix)
{
    if (matrix) {
        // Identity matrix
        for (int i = 0; i < 16; i++) {
            matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        }
    }
}

NEWTON_API void __cdecl NewtonBodySetVelocity(void* newtonBody, const float* velocity)
{
}

NEWTON_API void __cdecl NewtonBodyGetVelocity(void* newtonBody, float* velocity)
{
    if (velocity) {
        velocity[0] = 0.0f; velocity[1] = 0.0f; velocity[2] = 0.0f;
    }
}

NEWTON_API void __cdecl NewtonBodyAddForce(void* newtonBody, const float* force)
{
}

NEWTON_API void __cdecl NewtonBodyAddTorque(void* newtonBody, const float* torque)
{
}

NEWTON_API void __cdecl NewtonBodySetLinearDamping(void* newtonBody, float linearDamp)
{
}

NEWTON_API void __cdecl NewtonBodySetAngularDamping(void* newtonBody, const float* angularDamp)
{
}

NEWTON_API int __cdecl NewtonMaterialCreateGroupID(void* newtonWorld)
{
    return 1;
}

NEWTON_API void __cdecl NewtonMaterialSetDefaultFriction(void* newtonWorld, int id0, int id1, float staticFriction, float kineticFriction)
{
}

NEWTON_API void __cdecl NewtonMaterialSetDefaultElasticity(void* newtonWorld, int id0, int id1, float elasticCoef)
{
}

NEWTON_API void __cdecl NewtonMaterialSetDefaultSoftness(void* newtonWorld, int id0, int id1, float softnessCoef)
{
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
