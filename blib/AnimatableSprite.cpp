#include "AnimatableSprite.h"
#include <blib/SpriteBatch.h>
#include <blib/Math.h>

namespace blib
{

	AnimatableSprite::AnimatableSprite(blib::Texture* texture, const blib::math::Rectangle &rect) : rect(rect)
	{
		this->texture = texture;
	}
	AnimatableSprite::AnimatableSprite(blib::Texture* texture, const glm::vec2 &pos) : rect(pos, texture->originalWidth, texture->originalHeight)
	{
		this->texture = texture;
	}

	void AnimatableSprite::draw(SpriteBatch* spriteBatch)
	{
		spriteBatch->draw(texture, blib::math::easyMatrix(texture, rect));
	}



	void AnimatableSprite::update(float elapsedTime)
	{
		for (int i = 0; i < (int)animations.size(); i++)
		{
			Animation* animation = animations[i];
			animation->elapsedTime += elapsedTime;
			if (animation->elapsedTime > animation->duration)
				animation->elapsedTime = animation->duration;
			animation->apply(this);
			if (animation->elapsedTime >= animation->duration)
			{
				if (animation->onDone != nullptr)
					animation->onDone();
				delete animation;

				animations.erase(animations.begin() + i);
				i--;
			}
		}
	}


	void AnimatableSprite::moveTo(const glm::vec2 &targetPosition, float time, const std::function<void()> &onDone /* = nullptr */)
	{
		MoveToAnimation* animation = new MoveToAnimation(rect, blib::math::Rectangle(targetPosition, rect.width(), rect.height()));
		animation->onDone = onDone;
		animation->duration = time;
		animations.push_back(animation);
	}
	void AnimatableSprite::moveTo(const blib::math::Rectangle &targetRect, float time, const std::function<void()> &onDone /* = nullptr */)
	{
		MoveToAnimation* animation = new MoveToAnimation(rect, targetRect);
		animation->onDone = onDone;
		animation->duration = time;
		animations.push_back(animation);
	}

	void AnimatableSprite::shake()
	{
		animations.push_back(new ShakeAnimation(rect));
	}


	MoveToAnimation::MoveToAnimation(const blib::math::Rectangle& src, const blib::math::Rectangle& dest) : src(src), dest(dest)
	{
	}


	void MoveToAnimation::apply(AnimatableSprite* sprite)
	{
		float fac = elapsedTime / duration;
		sprite->rect = blib::math::Rectangle(glm::mix(src.topleft, dest.topleft, fac), glm::mix(src.bottomright, dest.bottomright, fac));
	}


	ShakeAnimation::ShakeAnimation(const blib::math::Rectangle& original) : original(original)
	{
		duration = 999999;
	}

	void ShakeAnimation::apply(AnimatableSprite* sprite)
	{
		duration = elapsedTime + 100;
		sprite->rect = original + glm::vec2(glm::sin(100 * elapsedTime), glm::cos(40 * elapsedTime));
	}

}