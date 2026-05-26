#include "Manipulator.h"

void Manipulator::RotateJoint(int jointIndex, float delta)
{
	switch (jointIndex) {
	case 0:
		BaseYaw += delta;
		BaseYaw = std::clamp(BaseYaw, baseMinYaw, baseMaxYaw);
		break;
	case 1:
		ShoulderPitch += delta;
		ShoulderPitch = std::clamp(ShoulderPitch, shoulderMinPitch, shoulderMaxPitch);
		break;
	case 2:
		ElbowPitch += delta;
		ElbowPitch = std::clamp(ElbowPitch, elbowMinPitch, elbowMaxPitch);
		break;
	case 3:
		WristPitch += delta;
		WristPitch = std::clamp(WristPitch, wristMinPitch, wristMaxPitch);
		break;
	case 4:
		WristYaw += delta;
		WristYaw = std::clamp(WristYaw, wristMinYaw, wristMaxYaw);
		break;
	case 5:
		WristRoll += delta;
		WristRoll = std::clamp(WristRoll, wristMinRoll, wristMaxRoll);
		break;
	}
}

dx::XMMATRIX Manipulator::GetBaseTransform() const
{
    return dx::XMMatrixRotationY(BaseYaw);
}
dx::XMMATRIX Manipulator::GetShoulderTransform() const
{
	return dx::XMMatrixTranslation(0.0f, shoulderJointRadius + upperArmLength*0.5f, 0.0f) *
        dx::XMMatrixRotationX(ShoulderPitch) *
        dx::XMMatrixTranslation(0.0f, baseHeight*0.5f + shoulderJointRadius, 0.0f) *
        GetBaseTransform();
}
dx::XMMATRIX Manipulator::GetElbowTransform() const
{
    return dx::XMMatrixTranslation(0.0f, elbowJointRadius + forearmLength*0.5f, 0.0f) *
        dx::XMMatrixRotationX(ElbowPitch) *
        dx::XMMatrixTranslation(0.0f, upperArmLength*0.5f + elbowJointRadius, 0.0f) *
        GetShoulderTransform();
}
dx::XMMATRIX Manipulator::GetWristTransform() const
{
    return dx::XMMatrixTranslation(0.0f, wristJointRadius + wristLength * 0.5f, 0.0f) *
        dx::XMMatrixRotationY(WristRoll) *
        dx::XMMatrixRotationZ(WristYaw) *
        dx::XMMatrixRotationX(WristPitch) *
        dx::XMMatrixTranslation(0.0f, forearmLength * 0.5f + wristJointRadius, 0.0f) *
        GetElbowTransform();
}