#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../math/mathlib.hpp"

enum class CameraDirection
{
	NONE = 0, FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
};


class Camera {
public:

	static Camera defaultCamera;

	vec2 camPos;
	vec2 camUp;
	vec2 camRight;

	float yaw;
	float pitch;
	float speed;
	float zoom;
	float sensitivity;

	Camera(vec2 position);
	
	void updateDirection(double dx, double dy);
	void updatePos(CameraDirection dir, double dt);
	void updateZoom(double dy);

private:
	void updateVectors();
};



#endif