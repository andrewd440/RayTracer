#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Ray.h"

#include <cstdint>

/**
* Represents a camera in the scene.
* Used to generate rays from a viewpoint through pixels on the screen.
*/
class Camera
{
public:
	/**
	* Constructs a scene camera from it's world position, direction, distance from the screen, and 
	* screen size.
	* @param EyePosition - world position of the camera
	* @param LookDirection - Normalized direction vector of the camera's direction
	* @param FOV - the distance of the camera from the screen
	* @param OutputResolution - size of the screen (in pixels)
	*/
	Camera(Vector3f EyePosition, Vector3f LookDirection, float FOV, Vector2i OutputResolution);

	/**
	* Generates a ray from the viewpoint through a screen pixel.
	* @param X - x coordinate of the pixel
	* @param Y - y coordinate of the pixel
	* @return The generated ray
	*/
	Ray generateRay(int32_t X, int32_t Y) const;

	/**
	* Retrieves the horizontal FOV of the camera.
	* @returns The horizontal field of view in degrees
	*/
	float getFOV() const;

	/**
	* Sets the horizontal FOV of the camera.
	* @param FOV - The horizontal field of view in degrees
	*/
	void setFOV(float FOV);

	/**
	* Retrieves the direction of the camera.
	* @returns The direction
	*/
	Vector3f getDirection() const;

	/**
	* Sets the direction of the camera.
	* @param Direction - The direction
	*/
	void setDirection(Vector3f Direction);

	/**
	* Retrieves the world postion of the camera.
	* @returns The world postion
	*/
	Vector3f getPosition() const;

	/**
	* Sets the world postion of the camera.
	* @param Position - The world postion
	*/
	void setPosition(Vector3f Position);

private:
	float mFieldOfView; /* Horizontal FOV of the camera */
	float mAspectRatio; /* Output resolution height/width */
	float mDistanceFromScreenPlane; /* Distance between viewpoint and screen */
	Vector2i mOutputResolution; /* Size, in pixels, of the output image */
	Vector3f mDirection; /* Normalized direction of the camera */
	Vector3f mPosition; /* World position of the camera */
};

