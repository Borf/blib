#include "Textbox.h"

//#include <gl/glew.h>

#include <blib/wm/wm.h>
#include <blib/Texture.h>
#include <blib/Shader.h>
#include <blib/Font.h>
#include <blib/SpriteBatch.h>
#include <blib/util/Profiler.h>

#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32
#include <windows.h>
#endif
#include <blib/KeyListener.h>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			Textbox::Textbox( )
			{
				this->text = "";
				this->width = 100;
				this->height = 25;

				cursor = 0;
				selectionStart = 0;
				selectionEnd = 0;
				scrollPosition = 0;
			}



			void Textbox::draw( SpriteBatch& spriteBatch, glm::mat4 matrix)
			{
				Json::Value skin = WM::getInstance()->skin["input"];

				spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x, y, 0)), skin, glm::vec2(width, height), glm::vec4(selected ? 1 : 0.9f, selected ? 1 : 0.9f, selected ? 1 : 0.9f, 1));


				Font* font = WM::getInstance()->font;

				if (text != "" || selected)
				{
					spriteBatch.draw(font, text, glm::translate(matrix, glm::vec3(x + 1.0f, y + 3, 0)), WM::getInstance()->convertHexColor4(skin["fontcolor"].asString()));
					if ((int)(blib::util::Profiler::getAppTime()*2) % 2 == 0)
						spriteBatch.draw(font, "|",  glm::translate(matrix, glm::vec3(x - 1.0f + font->textlen(text.substr(0, cursor)), y + 3, 0)), WM::getInstance()->convertHexColor4(skin["fontcolor"].asString()));

				}
				else
					spriteBatch.draw(font, emptyText, glm::translate(matrix, glm::vec3(x + 1.0f, y + 3, 0)), glm::vec4(0.6f, 0.6f, 0.6f, 1));


				
				/*
				glBindTexture(GL_TEXTURE_2D, WM::getInstance()->skinTexture->texid);
				shader->setColor(glm::vec4(selected ? 1 : 0.9f,selected ? 1 : 0.9f,selected ? 1 : 0.9f,1));
				GlHelper::drawStretchyRect(x, y, width, height, WM::getInstance()->skin["input"]);


				glScissor((int)shader->matrix[3][0]+1,0/*shader->height-(int)shader->matrix[3][1]-height+1*//*,width-2,1999+height-2);
				glEnable(GL_SCISSOR_TEST);
	
				if(selected)
				{
					int time = glutGet(GLUT_ELAPSED_TIME);
					if(time/500%2 == 0)
					{
						shader->setColor(glm::vec4(0,0,0,1));
						float adv = WM::getInstance()->font->Advance(text.substr(0, cursor).c_str());
						shader->useTexture(false);
						glBegin(GL_LINES);
						glVertexAttrib2f(0, x+2+adv, y+5.0f);
						glVertexAttrib2f(0, x+2+adv, y+height-5.0f);
						glEnd();
						shader->useTexture(true);
					}
				}

				if(text != "" || selected)
					WM::getInstance()->font->Render(text.c_str(), -1, FTPoint(x + 1.0f,-y-WM::getInstance()->font->LineHeight()-3));
				else
				{
					shader->setColor(glm::vec4(0.6f, 0.6f, 0.6f, 1));
					WM::getInstance()->font->Render(emptyText.c_str(), -1, FTPoint(x + 1.0f,-y-WM::getInstance()->font->LineHeight()-3));
					shader->setColor(glm::vec4(1,1,1, 1));
				}
				glDisable(GL_SCISSOR_TEST);*/
			}

			void Textbox::keyboard( char key )
			{
				if(key == 8)//backspace
				{
					if(cursor > 0)
					{
						//TODO: erase selection
						text = text.substr(0, cursor-1) + text.substr(cursor);
						cursor--;
					}
				}
				else if(key == 9)//tab
				{}
				else if(key == 127) //delete
				{
					if(cursor < text.size())
					{
						//TODO: erase selection
						text = text.substr(0, cursor) + text.substr(cursor+1);
					}
				}
				else if(key >= 32)
				{
					text = text.substr(0, cursor) + key + text.substr(cursor);
					cursor++;
					selectionStart = cursor;
					selectionEnd = cursor;
				}
	


				Widget::keyboard(key);
			}

			void Textbox::keyboardSpecial( int key )
			{
				if(key == blib::KEY_HOME)
					cursor = 0;
				else if(key == blib::KEY_HOME)
					cursor = text.size();
				Widget::keyboardSpecial(key);
			}


			void Textbox::mousedown(int x, int y)
			{
				Widget::mousedown(x, y);
				x-=this->x;//position
				x-=1;//padding
				x+=scrollPosition;

				for(unsigned int i = 0; i < text.size(); i++)
				{
					float adv = WM::getInstance()->font->textlen(text.substr(0, i).c_str());
					if(x < adv)
					{
						cursor = i;
						if(cursor < 0)
							cursor = 0;
						if(cursor > text.size())
							cursor = text.size();
						return;
					}
				}
				cursor = text.size();
			}
		}
	}
}
