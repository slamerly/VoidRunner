#include "Camera.h"
#include "Game.h"

Camera::Camera(Actor* owner) : Component(owner)
{
	up = Vector3::unitZ;
	//setPosition(Vector3(0.0f, 0.0f, -100.0f));
	pitchSpeed = 0.0f;
	rollSpeed = 0.0f;
	
	// --- FPS camera ---
	maxPitch = Maths::pi / 3.0f;
	pitch = 0.0f;
	roll = 0.0f;
}

void Camera::update(float deltaTime)
{
	Component::update(deltaTime);

	//cpt+= shakeSpeed;

	// --- Default values ---
	cameraPos = owner.getPosition();
	Vector3 target = owner.getPosition();

	// --- FPS camera ---
	if (fpsCam)
	{
		/*
		pitch += pitchSpeed * deltaTime;
		Quaternion pitchQuat{ owner.getRight(), pitch };

		roll += rollSpeed * deltaTime;
		Quaternion rollQuat{ owner.getForward(), roll };

		Quaternion cameraRotation = Quaternion::concatenate(pitchQuat, rollQuat);


		Vector3 viewForward = Vector3::transform(owner.getForward(), cameraRotation);

		// Movement shake
		//cameraPos += Vector3(owner.getRight().x * cos(cpt) * 0.1f, owner.getRight().y * cos(cpt) * 0.25f, sin(cpt) * 0.66f);

		//std::cout << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;

		target = cameraPos + viewForward * 100.0f;
		up = Vector3::transform(Vector3::unitZ, cameraRotation);
		*/
		Quaternion characterRotation = owner.getRotation();
		
		Vector3 viewForward = Vector3::transform(Vector3::unitX, characterRotation);

		target = cameraPos + viewForward * 100.0f;

		up = Vector3::transform(Vector3::unitZ, characterRotation);
	}
	// --- End FPS camera ---

	// View Matrix
	Matrix4 view = Matrix4::createLookAt(cameraPos, target, up);
	setViewMatrix(view);
}

void Camera::setPitchSpeed(float newPitchSpeed)
{
	pitchSpeed = newPitchSpeed;
}

void Camera::setRollSpeed(float newRollSpeed)
{
	rollSpeed = newRollSpeed;
}

void Camera::setMaxPitch(float pitch)
{
	maxPitch = pitch;
}

void Camera::setFPSCam(bool fpsCamP)
{
	fpsCam = fpsCamP;
}

void Camera::setShakeSpeed(float isActive)
{
	shakeSpeed = isActive;
}

void Camera::setViewMatrix(const Matrix4& view)
{
	Game& game = owner.getGame();
	game.getRenderer().setViewMatrix(view);
}
