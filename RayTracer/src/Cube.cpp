#include "Cube.h"
#include "Intersection.h"
#include <limits>


FCube::FCube(Vector3f Center, FMaterial LightingMaterial)
	: IPrimitive(LightingMaterial)
{
	mTransform.SetOrigin(Center);
	ConstructAABB();
}

bool FCube::IsIntersectingRay(FRay Ray, float* tValueOut, FIntersection* IntersectionOut)
{
	/**
	* Ray-AABB intersection test from Real-Time Collision Detection by Christer Ericson p. 181
	* Test checks AABB slabs against ray for concurrent intersection with each slab
	**/

	float tMin = 0.0f;
	float tMax = (tValueOut) ? *tValueOut : std::numeric_limits<float>::max();

	const Vector3f BoxMin(mTransform.TransformPosition(Vector3f(-1, -1, 1)));
	const Vector3f BoxMax(mTransform.TransformPosition(Vector3f(1, 1, -1)));

	const Vector3f& RayD = Ray.direction;
	const Vector3f& RayO = Ray.origin;
	
	for (int i = 0; i < 3; i++)
	{
		if (abs(RayD[i]) < _EPSILON)
		{
			//Ray is parallel to this slab, no hit unless origin is within slab
			if (RayO[i] < BoxMin[i] || RayO[i] > BoxMax[i])
				return false;
		}
		else
		{
			// Get intersection of t value with near and far planes of slab
			float ood = 1.0f / RayD[i];
			float t1 = (BoxMin[i] - RayO[i]) * ood;
			float t2 = (BoxMax[i] - RayO[i]) * ood;

			// Make t1 be the near plane
			if (t1 > t2)
				std::swap(t1, t2);

			// Compute intersection of slab intersection intervals
			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			// Exit if no collision when slab intersection becomes empty
			if (tMin > tMax)
				return false;
		}
	}

	// Ray intersects all slabs, return nearest t value if less than tValueOut
	if (tValueOut && tMin < *tValueOut)
	{
		*tValueOut = tMin;
		ConstructIntersection(RayO + tMin * RayD, IntersectionOut);
	}

	return true;
}

void FCube::ConstructIntersection(const Vector3f& IntersectionPoint, FIntersection* IntersectionOut)
{
	float LargestSide = 0;
	int IntersectionAxis = 0;
	int IntersectionSide = 0;

	const FMatrix4& ModelInverse = mTransform.GetInverse();

	const Vector3f IntersectionInObjectSpace = ModelInverse.TransformPosition(IntersectionPoint);

	// get the intersection plane
	for (int i = 0; i < 3; i++)
	{
		// get axis with largest length
		if (abs(IntersectionInObjectSpace[i]) > LargestSide)
		{
			LargestSide = abs(IntersectionInObjectSpace[i]);
			IntersectionSide = (IntersectionInObjectSpace[i] < 0.0f) ? -1 : 1;
			IntersectionAxis = i;
		}
	}

	Vector3f Normal;
	Normal[IntersectionAxis] = (float)IntersectionSide;
	Normal = ModelInverse.TransformDirection(Normal).Normalize();

	IntersectionOut->normal = Normal;
	IntersectionOut->object = this;

	// make sure point not inside of the object
	IntersectionOut->point = IntersectionPoint + (Normal * _EPSILON);
}

void FCube::ConstructAABB()
{
	Vector3f max(1, 1, -1);
	Vector3f min(-1, -1, 1);

	setBoundingBox(AABB{min, max});
}

FCube::~FCube()
{
}
