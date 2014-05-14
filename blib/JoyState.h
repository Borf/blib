#pragma once

#include <glm/glm.hpp>

namespace blib
{
	struct JoyState
	{
	public:
		enum class ControllerType
		{
			Xbox,
			GameCube,
			Other,
		};

		JoyState()
		{
			leftTrigger = 0;
			rightTrigger = 0;
			button = 0;
			connected = false;
			type = ControllerType::Other;
		}

		ControllerType type;

		glm::vec2 leftStick;
		glm::vec2 rightStick;

		float leftTrigger;
		float rightTrigger;

		bool connected;
		union
		{
			int button;
			struct 
			{
				unsigned a : 1;
				unsigned b : 1;
				unsigned x : 1;
				unsigned y : 1;

				unsigned l : 1;
				unsigned r : 1;
				unsigned select : 1;
				unsigned start : 1;
				unsigned leftStickButton : 1;
				unsigned rightStickButton : 1;
			};
		};
	};
}