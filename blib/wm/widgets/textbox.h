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
				std::list<std::function<void()>> onChange;
				int scrollPosition;
				unsigned int selectionPosition;

				bool shiftDown;
				bool ctrlDown;

				double blinkTime;

			public:
				std::string emptyText;

				std::string text;
				unsigned int cursor;


				std::string getSelectedText();

				Textbox();

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
				inline void addChangeHandler(const std::function<void()> &callback) { onChange.push_back(callback); }
			};
		}
	}
}
