#include "Manipulator.h"

void Manipulator::RotateJoint(int jointIndex, float delta)
{
	switch (jointIndex) {
	case 0:
		baseYaw += delta;
		baseYaw = std::clamp(baseYaw, baseMinYaw, baseMaxYaw);
		break;
	case 1:
		shoulderPitch += delta;
		shoulderPitch = std::clamp(shoulderPitch, shoulderMinPitch, shoulderMaxPitch);
		break;
	case 2:
		elbowPitch += delta;
		elbowPitch = std::clamp(elbowPitch, elbowMinPitch, elbowMaxPitch);
		break;
	case 3:
		wristPitch += delta;
		wristPitch = std::clamp(wristPitch, wristMinPitch, wristMaxPitch);
		break;
	case 4:
		wristYaw += delta;
		wristYaw = std::clamp(wristYaw, wristMinYaw, wristMaxYaw);
		break;
	case 5:
		wristRoll += delta;
		wristRoll = std::clamp(wristRoll, wristMinRoll, wristMaxRoll);
		break;
	}
}

bool Manipulator::SetTarget(float x, float y, float z, float wristPitch, float wristYaw, float wristRoll)
{
	baseYawTarget = atan2f(x, z);
	if (baseYawTarget < baseMinYaw || baseYawTarget > baseMaxYaw) return false;

	float rr = x * x + z * z;
	float s = y - baseHeight - shoulderJointRadius;
	float l1 = shoulderJointRadius + upperArmLength + elbowJointRadius;
	float l2 = elbowJointRadius + forearmLength + wristJointRadius * 2.0f + wristLength * 0.5f + 0.8f;
	float D = (rr + s * s - l1 * l1 - l2 * l2)
		/ (2.0f * l1 * l2);
	if (D < -1.0f || D > 1.0f) return false;
	elbowPitchTarget = atan2f(sqrtf(1.0f - D * D), D);
	if (elbowPitchTarget < elbowMinPitch || elbowPitchTarget > elbowMaxPitch) return false;

	shoulderPitchTarget = -atan2f(s, sqrtf(rr)) -
		atan2f(l2 * sinf(elbowPitchTarget), l1 + l2 * cosf(elbowPitchTarget))
		+ dx::XM_PIDIV2;
	if (shoulderPitchTarget < shoulderMinPitch || shoulderPitchTarget > shoulderMaxPitch) return false;

	wristPitchTarget = wristPitch;
	wristYawTarget   = wristYaw;
	wristRollTarget  = wristRoll;
	moving = true;


	return true;
}

void Manipulator::MoveTowardsTarget(float deltaTime)
{
	float baseYawDelta       = baseYawTarget       - baseYaw;
	float shoulderPitchDelta = shoulderPitchTarget - shoulderPitch;
	float elbowPitchDelta    = elbowPitchTarget    - elbowPitch;
	float wristPitchDelta    = wristPitchTarget    - wristPitch;
	float wristYawDelta      = wristYawTarget      - wristYaw;
	float wristRollDelta     = wristRollTarget     - wristRoll;

	float maxDelta = 0;
	maxDelta = std::max(maxDelta, fabsf(baseYawDelta));
	maxDelta = std::max(maxDelta, fabsf(shoulderPitchDelta));
	maxDelta = std::max(maxDelta, fabsf(elbowPitchDelta));
	maxDelta = std::max(maxDelta, fabsf(wristPitchDelta));
	maxDelta = std::max(maxDelta, fabsf(wristYawDelta));
	maxDelta = std::max(maxDelta, fabsf(wristRollDelta));

	if (maxDelta > 0.0001f) {
		RotateJoint(0, baseYawDelta       / maxDelta * deltaTime);
		RotateJoint(1, shoulderPitchDelta / maxDelta * deltaTime);
		RotateJoint(2, elbowPitchDelta    / maxDelta * deltaTime);
		RotateJoint(3, wristPitchDelta    / maxDelta * deltaTime);
		RotateJoint(4, wristYawDelta      / maxDelta * deltaTime);
		RotateJoint(5, wristRollDelta     / maxDelta * deltaTime);
	}
	else moving = false;
}

dx::XMMATRIX Manipulator::GetBaseTransform() const
{
    return dx::XMMatrixRotationY(baseYaw) *
		dx::XMMatrixTranslation(0.0f, baseHeight * 0.5f, 0.0f);
}
dx::XMMATRIX Manipulator::GetShoulderTransform() const
{
	return dx::XMMatrixTranslation(0.0f, shoulderJointRadius + upperArmLength*0.5f, 0.0f) *
        dx::XMMatrixRotationX(shoulderPitch) *
        dx::XMMatrixTranslation(0.0f, baseHeight*0.5f + shoulderJointRadius, 0.0f) *
        GetBaseTransform();
}
dx::XMMATRIX Manipulator::GetElbowTransform() const
{
    return dx::XMMatrixTranslation(0.0f, elbowJointRadius + forearmLength*0.5f, 0.0f) *
        dx::XMMatrixRotationX(elbowPitch) *
        dx::XMMatrixTranslation(0.0f, upperArmLength*0.5f + elbowJointRadius, 0.0f) *
        GetShoulderTransform();
}
dx::XMMATRIX Manipulator::GetWristTransform() const
{
    return dx::XMMatrixTranslation(0.0f, wristJointRadius + wristLength*0.5f, 0.0f) *
        dx::XMMatrixRotationY(wristRoll) *
        dx::XMMatrixRotationZ(wristYaw) *
        dx::XMMatrixRotationX(wristPitch) *
        dx::XMMatrixTranslation(0.0f, forearmLength*0.5f + wristJointRadius, 0.0f) *
        GetElbowTransform();
}