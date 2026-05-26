#pragma once

#include <DirectXMath.h>
#include <algorithm>

namespace dx = DirectX;

class Manipulator {
public:
	void RotateJoint(int jointIndex, float delta);

	dx::XMMATRIX GetBaseTransform() const;
	dx::XMMATRIX GetShoulderTransform() const;
	dx::XMMATRIX GetElbowTransform() const;
	dx::XMMATRIX GetWristTransform() const;

	float GetBaseHeight()     const { return baseHeight; }
	float GetUpperArmLength() const { return upperArmLength; }
	float GetForearmLength()  const { return forearmLength; }
	float GetWristLength()    const { return wristLength; }
private:
	float BaseYaw       = 0.0f;
	float ShoulderPitch = -0.5f;
	float ElbowPitch    = 2.1f;
	float WristPitch    = 0.0f;
	float WristYaw      = 0.0f;
	float WristRoll     = 0.0f;

	const float baseMinYaw       = -3.1f;
	const float baseMaxYaw       =  3.1f;
	const float shoulderMinPitch = -1.7f;
	const float shoulderMaxPitch =  1.7f;
	const float elbowMinPitch    = -3.1f;
	const float elbowMaxPitch    =  3.1f;
	const float wristMinPitch    = -1.58f;
	const float wristMaxPitch    =  1.58f;
	const float wristMinYaw      = -1.58f;
	const float wristMaxYaw      =  1.58f;
	const float wristMinRoll     = -3.1f;
	const float wristMaxRoll     =  3.1f;

	const float baseHeight          = 0.5f;
	const float shoulderJointRadius = 0.2f;
	const float upperArmLength      = 3.0f;
	const float elbowJointRadius    = 0.2f;
	const float forearmLength       = 3.0f;
	const float wristJointRadius    = 0.1f;
	const float wristLength         = 0.2f;
};