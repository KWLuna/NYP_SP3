#pragma once
#include "Camera3.h"

class PlayerInformation
{
public:
	PlayerInformation();
	~PlayerInformation();

	Vector3 GetPos(void) const;
	Vector3 GetTarget(void) const;
	Vector3 GetUp(void) const;

	void SetPos(const Vector3& pos);
	void SetTarget(const Vector3& target);
	void SetUp(const Vector3& up);

	void Constrain(void);
	void AttachCamera(Camera3* _cameraPtr);

	void update(double dt);


private:
	Camera3 * attachedCamera;
};

