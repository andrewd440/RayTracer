#include "Scene.h"
#include "Sphere.h"
#include "Intersection.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Ray.h"
#include "Cube.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <limits>

namespace
{
	const Vector2i OUTPUT_RESOLUTION(2000, 1100);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void throwSceneConfigError(const std::string& objectType)
{
	std::cout << "...Error in scene config file for a " << objectType << std::endl;
	throw std::runtime_error("Scene config error.");
}

//////////////////////////////////////////////////////////////////////////////////////////////
FScene::FScene()
	: mOutputImage("RenderedScene2", OUTPUT_RESOLUTION)
	, mBackgroundColor(FColor::Black)
	, mGlobalAmbient(100, 100, 100)
	, mCamera(Vector3f(0, 6, 0), Vector3f(0, 0, -15.0f), Vector3f(0, 1, 0), 75, OUTPUT_RESOLUTION)
	, mPrimitives()
	, mLights()
	, mKDTree()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::buildScene(std::istream& in)
{
	std::string string;
	in >> string;

	while (in.good())
	{
		if (string == "BackgroundColor:")
			in >> mBackgroundColor.r >> mBackgroundColor.g >> mBackgroundColor.b;
		else if (string == "GlobalAmbientColor:")
			in >> mGlobalAmbient.r >> mGlobalAmbient.g >> mGlobalAmbient.b;
		else if(string == "Camera")
		{
			Vector3f position;
			float FOV;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("Camera");
			in >> position.x >> position.y >> position.z;

			in >> string;
			if (string != "FOV:")
				throwSceneConfigError("Camera");
			in >> FOV;

			//mCamera.SetPosition(position);
			mCamera.SetFOV(FOV);
		}
		else if (string == "DirectionalLight")
		{
			FColor color;
			Vector3f direction;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("DirectionalLight");
			in >> color.r >> color.g >> color.b;

			in >> string;
			if (string != "Direction:")
				throwSceneConfigError("DirectionalLight");
			in >> direction.x >> direction.y >> direction.z;

			mLights.push_back(LightPtr(new FDirectionalLight(color, direction)));
		}
		else if (string == "PointLight")
		{
			FColor color;
			Vector3f position;

			in >> string;
			if (string != "Color:")
				throwSceneConfigError("PointLight");
			in >> color.r >> color.g >> color.b;

			in >> string;
			if (string != "Position:")
				throwSceneConfigError("PointLight");
			in >> position.x >> position.y >> position.z;

			mLights.push_back(LightPtr(new FPointLight(color, position, 4, 10, 35))); ///////////////////////////////////////CHANGE TO FILE INPUT
		}
		else if (string == "Plane")
		{
			Vector3f direction;
			Vector3f point;

			in >> string;
			if (string != "Direction:")
				throwSceneConfigError("Plane");
			in >> direction.x >> direction.y >> direction.z;
			in >> string;
			if (string != "Point:")
				throwSceneConfigError("Plane");
			in >> point.x >> point.y >> point.z;

			FMaterial material(readMaterial(in));

			mPrimitives.push_back(PrimitivePtr(new FPlane(material, direction, point)));
		}
		else if (string == "Sphere")
		{
			Vector3f center;
			float radius;

			in >> string;
			if (string != "Center:")
				throwSceneConfigError("Sphere");
			in >> center.x >> center.y >> center.z;
			in >> string;
			if (string != "Radius:")
				throwSceneConfigError("Sphere");
			in >> radius;
			
			FMaterial material(readMaterial(in));

			mPrimitives.push_back(PrimitivePtr(new FSphere(center, radius, material)));
		}
		else if (string == "Model")
		{
			std::string filename;
			Vector3f translation;

			in >> string;
			if (string != "File:")
				throwSceneConfigError("Model");
			in >> filename;

			in >> string;
			if (string != "Translation:")
				throwSceneConfigError("Model");
			in >> translation.x >> translation.y >> translation.z;

			FMaterial material(readMaterial(in));

			readModel(filename, translation, material);
		}
		in >> string;
	}
	
	mLights.push_back(LightPtr(new FPointLight(FColor(1.0f, 1.0f, 1.0f), Vector3f(-5.0f, 10, -8), 4, 10, 35)));

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-5.0f, -3.5f, -22.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.2f, .8f, .8f), FColor(.1f, .1f, .1f), 64, .9f))));
	IPrimitive* cube = mPrimitives.back().get();
	cube->mTransform.Scale(0.5);

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(-5.0f, -3.5f, -13.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.5f, 1.0f, .5f), FColor(.1f, .1f, .1f), 64, .9f))));
	cube = mPrimitives.back().get();
	cube->mTransform.Scale(0.5f);

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(5.0f, -3.5f, -22.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.1f, .3f, .8f), FColor(.1f, .1f, .1f), 64, .9f))));
	cube = mPrimitives.back().get();
	cube->mTransform.Scale(0.5f);

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(5.0f, -3.5f, -13.0f), FMaterial(FColor(.2f, .7f, .7f), FColor(.8f, .2f, .8f), FColor(.1f, .1f, .1f), 64, .9f))));
	cube = mPrimitives.back().get();
	cube->mTransform.Scale(0.5f);

	mPrimitives.push_back(PrimitivePtr(new FCube(Vector3f(0.0f, -2.9f, -17.5f), FMaterial(FColor::White, FColor::White, FColor(.1f, .1f, .1f), 64, .9f))));
	cube = mPrimitives.back().get();
	cube->mTransform.Scale(Vector3f(5.0f, 0.05f, 4.5f));

	mPrimitives.push_back(PrimitivePtr(new FSphere(Vector3f(0.0f, -1.5f, -17.5f), 2.0f, FMaterial(FColor(.2f, .7f, .7f), FColor(1.0f, .4f, .1f), FColor(.1f, .1f, .1f), 64, .9f))));

	mKDTree.buildTree(mPrimitives, 10);
}

//////////////////////////////////////////////////////////////////////////////////////////////

FColor FScene::traceRay(const FRay& cameraRay, int32_t depth)
{
	if (depth < 1)
		return mBackgroundColor;

	float maxTValue(std::numeric_limits<float>::max());
	FIntersection closestIntersection;

	
	 //For performs difference tests
	for (const auto& primitive : mPrimitives)
		primitive->IsIntersectingRay(cameraRay, &maxTValue, &closestIntersection);
		
	//mKDTree.IsIntersectingRay(cameraRay, &maxTValue, &closestIntersection);

	// If an object was intersected
	if (closestIntersection.object)
	{
		FColor outputColor;

		// Get the surface material, point, and normal
		const FMaterial& surfaceMaterial(closestIntersection.object->getMaterial());
		const Vector3f& surfacePoint(closestIntersection.point);
		const Vector3f& surfaceNormal(closestIntersection.normal);

		for (const auto& light : mLights)
		{
			const FColor lightColor = light->GetIntesityAt(surfacePoint);

			// if the light intesity is 0, skip this light
			if (lightColor == FColor::Black)
				continue;

			// Get direction of light and compute h reflection
			const FRay& rayToLight(light->GetRayToLight(surfacePoint));
			const Vector3f& lightDirection(rayToLight.direction);
			const Vector3f& h = computeBlinnSpecularReflection(rayToLight.direction, -cameraRay.direction);

			// If an object is in the way of the light, skip lighting for that light
			/*if (isInShadow(rayToLight))
				continue;*/

			const float ShadeFactor = ComputeShadeFactor(*light, surfacePoint);
			if (ShadeFactor <= 0.0)
				continue;

			// Get dot product of surface normal and h for specular lighting
			float specularFactor = std::max(Vector3f::Dot(surfaceNormal, h), 0.f);

			// Add glossiness expononent
			specularFactor = pow(specularFactor, surfaceMaterial.glossiness);

			// Get dot product of surface normal and light direction for diffuse lighting
			float diffuseFactor = std::max(Vector3f::Dot(surfaceNormal, lightDirection), 0.f);

			// Combine material color and light color for diffuse and specular
			FColor specularColor(lightColor * surfaceMaterial.specularColor * specularFactor);
			FColor diffuseColor(lightColor  * surfaceMaterial.diffuseColor * diffuseFactor);

			// Add diffuse and specular contributions to total
			outputColor += specularColor + diffuseColor;
			outputColor *= ShadeFactor;

			// Add mirror reflection contributions
			Vector3f mirrorReflection = -cameraRay.direction.Reflect(surfaceNormal);
			FRay reflectionRay(surfacePoint, mirrorReflection);
			outputColor += traceRay(reflectionRay, depth - 1) * outputColor * surfaceMaterial.reflectivity;
		}

		// return computed color totals with ambient contribution
		return  outputColor + (mGlobalAmbient * surfaceMaterial.ambientColor);
	}
	else
		return mBackgroundColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void FScene::renderScene()
{
	for (int y = 0; y < OUTPUT_RESOLUTION.y; y++)
	{
		for (int x = 0; x < OUTPUT_RESOLUTION.x; x++)
		{
			FRay ray = mCamera.GenerateRay(x, y);
			FColor pixelColor = traceRay(ray,5);
			mOutputImage.setPixel(x, y, pixelColor);
		}
	}

	mOutputImage.writeImage();
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f FScene::computeBlinnSpecularReflection(const Vector3f& lightDirection, const Vector3f& viewerDirection)
{
	Vector3f reflection(lightDirection + viewerDirection);
	reflection.Normalize();
	return reflection;
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool FScene::isInShadow(const FRay& lightRay)
{
	//return mKDTree.IsIntersectingRay(lightRay);

	
	// For performance tests
	for (const auto& Primitive : mPrimitives)
	{
		// If the object is not the reference one and intersects the light
		if (Primitive->IsIntersectingRay(lightRay))
			return true;
	}

	return false;
	
}

float FScene::ComputeShadeFactor(const ILight& Light, const Vector3f& SurfacePoint) const
{
	const float FactorSize = 1.0f / 16.0f;
	float ShadeFactor = 1.0f;

	for (const FRay& ShadowSample : Light.GetRayToLightSamples(SurfacePoint, 16))
	{
		for (const auto& Primitive : mPrimitives)
		{
			// If the object is not the reference one and intersects the light
			if (Primitive->IsIntersectingRay(ShadowSample))
			{
				ShadeFactor -= FactorSize;
				break;
			}
		}
	}

	return ShadeFactor;
}

//////////////////////////////////////////////////////////////////////////////////////////////

Vector3f FScene::computeMirriorReflection(const Vector3f& viewerDirection, const Vector3f& surfaceNormal) const
{
	float viewerDotNormal = Vector3f::Dot(surfaceNormal, viewerDirection);
	Vector3f reflectionDirection(2 * (viewerDotNormal) * surfaceNormal - viewerDirection);
	reflectionDirection.Normalize();
	return reflectionDirection;
}

//////////////////////////////////////////////////////////////////////////////////////////////

FMaterial FScene::readMaterial(std::istream& input)
{
	FColor specular, diffuse, ambient;
	float specualarExponent, reflectivity;

	std::string materialProperty;
	
	input >> materialProperty;
	if (materialProperty != "Specular:")
		throwSceneConfigError("Material");
	input >> specular.r >> specular.g >> specular.b;

	input >> materialProperty;
	if (materialProperty != "Diffuse:")
		throwSceneConfigError("Material");
	input >> diffuse.r >> diffuse.g >> diffuse.b;

	input >> materialProperty;
	if (materialProperty != "Ambient:")
		throwSceneConfigError("Material");
	input >> ambient.r >> ambient.g >> ambient.b;

	input >> materialProperty;
	if (materialProperty != "SpecularExponent:")
		throwSceneConfigError("Material");
	input >> specualarExponent;

	input >> materialProperty;
	if (materialProperty != "Reflectivity:")
		throwSceneConfigError("Material");
	input >> reflectivity;

	return FMaterial(specular, diffuse, ambient, specualarExponent, reflectivity);
}

void FScene::readModel(std::string filename, Vector3f translation, FMaterial material)
{
	std::filebuf modelBuffer;
	if (modelBuffer.open(filename, std::ios::in))
	{
		std::istream in(&modelBuffer);

		std::string string;
		std::size_t vertexIndex;
		Vector3f vertex;
		std::vector<Vector3f> vertices;

		// Vertex indices for each face
		std::size_t faceV0, faceV1, faceV2;

		in >> string;

		while (in.good())
		{
			if (string == "Vertex")
			{
				in >> vertexIndex;
				in >> vertex.x >> vertex.y >> vertex.z;
				vertex *= 2;
				vertex += translation;
				vertices.push_back(vertex);
			}
			else if (string == "Face")
			{
				// pull out face number
				in >> string;

				// Get vertices' indices
				in >> faceV0;
				in >> faceV1;
				in >> faceV2;

				mPrimitives.push_back(PrimitivePtr(new FTriangle(vertices[faceV0-1], vertices[faceV1-1], vertices[faceV2-1], material)));
			}

			in >> string;
		}
	}

	modelBuffer.close();
}