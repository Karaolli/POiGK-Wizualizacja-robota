#pragma once

#include <DirectXMath.h>
#include <algorithm>

namespace dx = DirectX;

class Manipulator {
public:
	void RotateJoint(int jointIndex, float delta);
	bool SetTarget(float x, float y, float z, float wristPitch, float wristYaw, float wristRoll);

	bool IsMoving() const { return moving; }
	void MoveTowardsTarget(float deltaTime);

	dx::XMMATRIX GetBaseTransform()     const;
	dx::XMMATRIX GetShoulderTransform() const;
	dx::XMMATRIX GetElbowTransform()    const;
	dx::XMMATRIX GetWristTransform()    const;

	float GetBaseYaw()       const { return baseYaw; }
	float GetShoulderPitch() const { return shoulderPitch; }
	float GetElbowPitch()    const { return elbowPitch; }
	float GetWristPitch()    const { return wristPitch; }
	float GetWristYaw()      const { return wristYaw; }
	float GetWristRoll()     const { return wristRoll; }

	float GetBaseHeight()     const { return baseHeight; }
	float GetUpperArmLength() const { return upperArmLength; }
	float GetForearmLength()  const { return forearmLength; }
	float GetWristLength()    const { return wristLength; }

	float GetBaseMinYaw()       const { return baseMinYaw; }
	float GetBaseMaxYaw()       const { return baseMaxYaw; }
	float GetShoulderMinPitch() const { return shoulderMinPitch; }
	float GetShoulderMaxPitch() const { return shoulderMaxPitch; }
	float GetElbowMinPitch()    const { return elbowMinPitch; }
	float GetElbowMaxPitch()    const { return elbowMaxPitch; }
	float GetWristMinPitch()    const { return wristMinPitch; }
	float GetWristMaxPitch()    const { return wristMaxPitch; }
	float GetWristMinYaw()      const { return wristMinYaw; }
	float GetWristMaxYaw()      const { return wristMaxYaw; }
	float GetWristMinRoll()     const { return wristMinRoll; }
	float GetWristMaxRoll()     const { return wristMaxRoll; }

	float baseYawTarget       =  0.0f;
	float shoulderPitchTarget = -0.5f;
	float elbowPitchTarget    =  2.1f;
	float wristPitchTarget    =  0.0f;
	float wristYawTarget      =  0.0f;
	float wristRollTarget     =  0.0f;

	bool moving = false;
private:
	float baseYaw       =  0.0f;
	float shoulderPitch = -0.5f;
	float elbowPitch    =  2.1f;
	float wristPitch    =  0.0f;
	float wristYaw      =  0.0f;
	float wristRoll     =  0.0f;

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