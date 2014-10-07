#pragma once

#include "Widget.h"

#include <string>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class Textbox : public Widget
			{

				int scrollPosition;

				int selectionStart;
				int selectionEnd;
			public:
				std::string emptyText;

				std::string text;
				unsigned int cursor;


				Textbox();

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
			};
		}
	}
}
