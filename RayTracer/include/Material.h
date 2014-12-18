#pragma once

#include "Color.h"
#include "Texture.h"
#include <cassert>

/**
* Holds information for using a texture.
*/
struct FTextureInfo
{
	FTexture* Texture;
	float UAxisScale;
	float VAxisScale;
};

/**
* Represents the lighting material for a specific surface
*/
class FMaterial
{
public:
	/**
	* Constructs a Material with a specular, diffuse, ambient, and glossiness.
	* @param Specular - The specular color (default: (0.7, 0.7, 0.7))
	* @param Diffuse - The diffuse color (default: (0.7, 0.7, 0.7))
	* @param Ambient - The ambient color (default: (0.1, 0.1, 0.1))
	* @param Glossiness - The glossiness of the surface (default: 1.0)
	* @param Reflectivity - The reflectiveness of the surface [0-1] (default: 0.0)
	*/
	FMaterial(	FColor Specular = FColor(0.0f, 0.0f, 0.0f), 
				FColor Diffuse = FColor(0.7f, 0.7f, 0.7f), 
				FColor Ambient = FColor(0.1f, 0.1f, 0.1f), 
				float Glossiness = 0.0f, 
				float Reflectivity = 0.0f)
		: mSpecularColor(Specular)
		, mGlossiness(Glossiness)
		, mDiffuseColor(Diffuse)
		, mReflectivity(Reflectivity)
		, mAmbientColor(Ambient)
		, mDiffuseTextureInfo()
	{
		// Reflectivity must be between 0 and 1
		assert(Reflectivity <= 1 && Reflectivity >= 0);
	}

	void SetSpecular(const FColor& Specular)
	{
		mSpecularColor = Specular;
	}

	FColor GetSpecular() const
	{
		return mSpecularColor;
	}

	void SetDiffuse(const FColor& Diffuse)
	{
		mDiffuseColor = Diffuse;
	}

	FColor GetDiffuse() const
	{
		return mDiffuseColor;
	}

	void SetAmbient(const FColor& Ambient)
	{
		mAmbientColor = Ambient;
	}

	FColor GetAmbient() const
	{
		return mAmbientColor;
	}

	void SetGlossiness(const float Glossiness)
	{
		mGlossiness = Glossiness;
	}

	float GetGlossiness() const
	{
		return mGlossiness;
	}

	void SetReflectivity(const float Reflectivity)
	{
		mReflectivity = Reflectivity;
	}

	float GetReflectivity() const
	{
		return mReflectivity;
	}

	void SetDiffuseTexture(const FTextureInfo& DiffuseInfo)
	{
		mDiffuseTextureInfo = DiffuseInfo;
	}

	FTextureInfo GetDiffuseTexture() const
	{
		return mDiffuseTextureInfo;
	}

private:
	FColor mSpecularColor;
	float mGlossiness;
	FColor mDiffuseColor;
	float mReflectivity;
	FColor mAmbientColor;
	FTextureInfo mDiffuseTextureInfo;
};

