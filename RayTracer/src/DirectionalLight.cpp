#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: Light()
	, mLightDirection(0,-1,0)
{

}

DirectionalLight::DirectionalLight(Color LightColor, Vector3f LightDirection)
	: Light(LightColor)
	, mLightDirection(LightDirection)
{
	mLightDirection.Normalize();
}


Ray DirectionalLight::getRayToLight(const Vector3f& SurfacePoint)
{
	return Ray(SurfacePoint, -mLightDirection);
}


void DirectionalLight::setLightDirection(const Vector3f& LightDirection)
{
	mLightDirection = LightDirection;
	mLightDirection.Normalize();
}

Vector3f DirectionalLight::getLightDirection() const
{
	return mLightDirection;
}