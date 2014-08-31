#pragma once
#include "Shape.h"
#include "Vector3.h"

class Triangle : public Shape
{
public:
	Triangle(Vector3f V0, Vector3f V1, Vector3f V2, const Material& LightingMaterial);
	
	/**
	* Checks if a ray intersects the triangle.
	* If the intersection succeeds, the intersection properties and t value are output through
	* an optional t value and intersection pointer.
	* @param Ray - the ray to check for intersection
	* @param tValueOut(optional) - the smallest t parameter will be output to this
	* @param IntersectionOut(optional) - intersection attributes will be assigned to this reference if
	*							if the interection returns true
	* @return True if the ray intersects the triangle.
	*/
	bool isIntersectingRay(Ray Ray, float* tValueOut = nullptr, Intersection* IntersectionOut = nullptr);


private:
	/**
	* Constructs intersection properties for a point on this triangle
	* @param IntersectionPoint - the point of the intersection
	* @param IntersectionOut - intersection properties will be output through
	*/
	void constructIntersection(Vector3f IntersectionPoint, Intersection& IntersectionOut);

	void constructAABB();

private:
	Vector3f mV0;
	Vector3f mV1;
	Vector3f mV2;
	Vector3f mNormal;
};

