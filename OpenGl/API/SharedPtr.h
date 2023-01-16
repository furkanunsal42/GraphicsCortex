#pragma once
#include "Config.h"

#include <memory>

template<typename T>
class SharedPtr {
public:

	SharedPtr() : 
		obj(std::make_shared<T>()) {}

	SharedPtr(std::shared_ptr<T> other_obj) :
		obj(other_obj) {}

	explicit SharedPtr(T& other) :
		obj(std::make_shared<T>(other)) {}

	explicit SharedPtr(T&& other) :
		obj(std::make_shared<T>(other)) {}

	T* operator->() const { return obj.get();  }

	std::shared_ptr<T> obj;
};

class UnorderedMaterial;
class Material;
class Mesh;
class Program;
class Light;
class AmbiantLight;
class DirectionalLight;
class PointLight;
class SpotLight;
class Graphic;
class Object;
class Vehicle;
class PhysicsObject;
class PhysicsVehicle;
class PxRigidActor;		// physx

typedef SharedPtr<UnorderedMaterial> UnorderedMaterial_s;
typedef SharedPtr<Material> Material_s;
typedef SharedPtr<Mesh> Mesh_s;
typedef SharedPtr<Program> Program_s;
typedef SharedPtr<Light> Light_s;
typedef SharedPtr<AmbiantLight> AmbiantLight_s;
typedef SharedPtr<DirectionalLight> DirectionalLight_s;
typedef SharedPtr<PointLight> PointLight_s;
typedef SharedPtr<SpotLight> SpotLight_s;
typedef SharedPtr<Graphic> Graphic_s;
typedef SharedPtr<Object> Object_s;
typedef SharedPtr<Vehicle> Vehicle_s;
typedef SharedPtr<PhysicsObject> PhysicsObject_s;
typedef SharedPtr<PhysicsVehicle> PhysicsVehicle_s;
typedef SharedPtr<PxRigidActor> PxRigidActor_s;		// physx

class ArrayBuffer;
class IndexBuffer;
