#include "textbox.h"

#include <blib/wm/WM.h>
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
    selectionPosition = 0;
    scrollPosition = 0;
    canHaveKeyboardFocus = true;
    shiftDown = false;
    blinkTime = blib::util::Profiler::getAppTime();


    addCharHandler([this](char key)
    {
        if (key >= ' ') //only printable
        {
			if (cursor != selectionPosition)
			{
				text = text.substr(0, glm::min(cursor, selectionPosition)) + text.substr(glm::max(cursor, selectionPosition));
				cursor = glm::min(cursor, selectionPosition);
				selectionPosition = cursor;
			}
			text = text.substr(0, cursor) + (char)key + text.substr(cursor);
            cursor++;
            selectionPosition = cursor;
			for (const auto& o : onChange)	o();
            return true;
        }
        return false;
    });

    addKeyUpHandler([this](blib::Key key)
    {
        if (key == blib::Key::SHIFT)
        {
            shiftDown = false;
            return true;
        }
        return false;
    });

    addKeyDownHandler([this](blib::Key key)
    {
        blinkTime = blib::util::Profiler::getAppTime();
        switch (key)
        {
        case blib::Key::SHIFT:
            shiftDown = true;
            break;
        case blib::Key::BACKSPACE:
            if (cursor > 0)
            {
				if (cursor != selectionPosition)
				{
					text = text.substr(0, glm::min(cursor, selectionPosition)) + text.substr(glm::max(cursor, selectionPosition));
					cursor = glm::min(cursor, selectionPosition);
					selectionPosition = cursor;
				}
				else
				{
					text = text.substr(0, cursor - 1) + text.substr(cursor);
					cursor--;
					selectionPosition = cursor;
				}
				for (const auto& o : onChange)	o();
			}
            return true;
        case blib::Key::TAB:
            return true;
        case blib::Key::DEL:
            if (cursor < text.size())
            {
				if (cursor != selectionPosition)
				{
					text = text.substr(0, glm::min(cursor, selectionPosition)) + text.substr(glm::max(cursor, selectionPosition));
					cursor = glm::min(cursor, selectionPosition);
					selectionPosition = cursor;
				}
				else
					text = text.substr(0, cursor) + text.substr(cursor + 1);
				for (const auto& o : onChange)	o();
			}
            return true;
        case blib::Key::HOME:
            cursor = 0;
            if (!shiftDown)
                selectionPosition = cursor;
            return true;
        case blib::Key::END:
            cursor = text.size();
            if (!shiftDown)
                selectionPosition = cursor;
            return true;
        case blib::Key::LEFT:
            if (cursor > 0)
                cursor--;
            if (!shiftDown)
                selectionPosition = cursor;
            return true;
        case blib::Key::RIGHT:
            cursor = glm::min((unsigned int)text.length(), ( cursor + 1));
            if (!shiftDown)
                selectionPosition = cursor;
            return true;
        default:
            return false;
        }
        return false;
    });


    addMouseDownHandler([this](int x, int y, int clickCount)
    {
        x -= this->x;//position
        x -= 1;//padding
        x += scrollPosition;

        for (unsigned int i = 0; i < text.size(); i++)
        {
            float adv = WM::getInstance()->font->textlen(text.substr(0, i).c_str());
            if (x < adv)
            {
                cursor = i;
                if (cursor < 0)
                    cursor = 0;
                if (cursor > text.size())
                    cursor = text.size();
                if (!shiftDown)
                    selectionPosition = cursor;
                return true;
            }
        }
        cursor = text.size();
        if (!shiftDown)
            selectionPosition = cursor;

		if (clickCount == 2)
		{
			cursor = 0;
			selectionPosition = text.size();
		}

        return true;
    });

    addDragHandler([this](int x, int y)
    {
        //x -= this->x;//position
        x -= 1;//padding
        x += scrollPosition;

        for (unsigned int i = 0; i < text.size(); i++)
        {
            float adv = WM::getInstance()->font->textlen(text.substr(0, i).c_str());
            if (x < adv)
            {
                cursor = i;
                if (cursor < 0)
                    cursor = 0;
                if (cursor > text.size())
                    cursor = text.size();
                return true;
            }
        }
        cursor = text.size();
        return true;
    });




}



void Textbox::draw(SpriteBatch& spriteBatch, glm::mat4 matrix, Renderer* renderer) const
{
    //ewww
    if (!selected)
        const_cast<Textbox*>(this)->selectionPosition = cursor;

	if (selectionPosition > text.size())
		const_cast<Textbox*>(this)->selectionPosition = text.size();

	if (cursor > text.size())
		const_cast<Textbox*>(this)->cursor = text.size();

    json skin = WM::getInstance()->skin["input"];
    Font* font = WM::getInstance()->font;
    Texture* texture = WM::getInstance()->skinTexture;


    spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x, y, 0)), skin, glm::vec2(width, height), glm::vec4(selected ? 1 : 0.9f, selected ? 1 : 0.9f, selected ? 1 : 0.9f, 1));



    if (text != "" || selected)
    {
        std::string preSelectionText = text.substr(0, glm::min(selectionPosition, cursor));
        std::string selectionText = text.substr(glm::min(selectionPosition, cursor), glm::max(selectionPosition, cursor) - glm::min(selectionPosition, cursor));
        std::string postSelectionText = text.substr(glm::max(selectionPosition, cursor));

        if (selectionPosition != cursor)
        {
            float posLeft = font->textlen(preSelectionText);
            float selectionWidth = font->textlen(selectionText);
            spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x + 2 + posLeft, y + 4, 0)), skin, glm::vec2(selectionWidth, 12), WM::getInstance()->convertHexColor4(skin["selectcolor"].get<std::string>()));
        }


        glm::vec2 renderPos(0, 0);
        renderPos = spriteBatch.draw(font, preSelectionText, glm::translate(matrix, glm::vec3(x + 1.0f, y + 3, 0)), WM::getInstance()->convertHexColor4(skin["fontcolor"].get<std::string>()), renderPos);
        renderPos = spriteBatch.draw(font, selectionText, glm::translate(matrix, glm::vec3(x + 1.0f, y + 3, 0)), WM::getInstance()->convertHexColor4(skin["selectfontcolor"].get<std::string>()), renderPos);
        renderPos = spriteBatch.draw(font, postSelectionText, glm::translate(matrix, glm::vec3(x + 1.0f, y + 3, 0)), WM::getInstance()->convertHexColor4(skin["fontcolor"].get<std::string>()), renderPos);



        if ((int)((blib::util::Profiler::getAppTime() - blinkTime)* 2) % 2 == 0 && selected)
            spriteBatch.draw(font, "|",  glm::translate(matrix, glm::vec3(x - 1.0f + font->textlen(text.substr(0, cursor)), y + 3, 0)), WM::getInstance()->convertHexColor4(skin["fontcolor"].get<std::string>()));

    }
    else
        spriteBatch.draw(font, emptyText, glm::translate(matrix, glm::vec3(x + 1.0f, y + 3, 0)), glm::vec4(0.6f, 0.6f, 0.6f, 1));



    /*
                glBindTexture(GL_TEXTURE_2D, WM::getInstance()->skinTexture->texid);
                shader->setColor(glm::vec4(selected ? 1 : 0.9f,selected ? 1 : 0.9f,selected ? 1 : 0.9f,1));
                GlHelper::drawStretchyRect(x, y, width, height, WM::getInstance()->skin["input"]);


                glScissor((int)shader->matrix[3][0]+1,0*//*shader->height-(int)shader->matrix[3][1]-height+1*//*,width-2,1999+height-2);
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


}
}
}
