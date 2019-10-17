#pragma once

struct Vector3
{
	float x;
	float y;
	float z;

	std::string getValues() const
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
};
