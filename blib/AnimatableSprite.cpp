#include "AnimatableSprite.h"
#include <blib/SpriteBatch.h>
#include <blib/Math.h>
#include <blib/Color.h>
#include <blib/util/Profiler.h>
#include <blib/Util.h>

#include <glm/gtc/matrix_transform.hpp>

namespace blib
{

	AnimatableSprite::AnimatableSprite(blib::Texture* texture, const blib::math::Rectangle &rect) : rect(rect)
	{
		this->texture = texture;
		color = blib::Color::white;
		rotation = 0;
	}
	AnimatableSprite::AnimatableSprite(blib::Texture* texture, const glm::vec2 &pos) : rect(pos, texture->originalWidth, texture->originalHeight)
	{
		this->texture = texture;
		color = blib::Color::white;
		rotation = 0;
	}

	void AnimatableSprite::draw(SpriteBatch* spriteBatch)
	{
		spriteBatch->draw(texture, blib::math::easyMatrix(texture, rect, rect.center(), rotation), color);
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

	void AnimatableSprite::alphaTo(const float target, float time, const std::function<void()> &onDone /* = nullptr */)
	{
		AlphaAnimation* animation = new AlphaAnimation(color.a, target);
		animation->onDone = onDone;
		animation->duration = time;
		animations.push_back(animation);
	}

	void AnimatableSprite::rotateTo(const float target, float time, const std::function<void()> &onDone /* = nullptr */)
	{
		RotateAnimation* animation = new RotateAnimation(rotation, target);
		animation->onDone = onDone;
		animation->duration = time;
		animations.push_back(animation);
	}

	void AnimatableSprite::curveTo(const glm::vec2 &targetPosition, float direction, float incomingDirection, float time, const std::function<void()> &onDone /*= nullptr*/)
	{
		CurveToAnimation* animation = new CurveToAnimation(rect, blib::math::Rectangle(targetPosition, rect.width(), rect.height()), direction, incomingDirection);
		animation->onDone = onDone;
		animation->duration = time;
		animations.push_back(animation);
	}

	void AnimatableSprite::shake()
	{
		animations.push_back(new ShakeAnimation(rect));
	}

	void AnimatableSprite::clearAnimations()
	{
		for (size_t i = 0; i < animations.size(); i++)
		{
			animations[i]->finish(this);
			animations[i]->elapsedTime = 10;
			animations[i]->duration = 10;
		}
	}

	bool AnimatableSprite::contains(glm::vec2 point)
	{
		glm::mat4 mat;
		mat = glm::translate(mat, glm::vec3(rect.center(), 0));
		mat = glm::rotate(mat, rotation, glm::vec3(0, 0, 1));
		mat = glm::translate(mat, glm::vec3(-rect.center(), 0));
		mat = glm::inverse(mat);
		point = glm::vec2(mat * glm::vec4(point, 0, 1));
		return rect.contains(point);
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
		done = false;
		this->elapsedTime = rand() / (float)RAND_MAX;
	}

	void ShakeAnimation::apply(AnimatableSprite* sprite)
	{
		if (!done)
			duration = elapsedTime + 100;
		sprite->rect = original + 2.0f * glm::vec2(glm::sin(30 * elapsedTime), glm::cos(20 * elapsedTime));
	}

	void ShakeAnimation::finish(AnimatableSprite* sprite)
	{
		done = true;
		duration = elapsedTime;
		sprite->rect = original;
	}


	AlphaAnimation::AlphaAnimation(const float src, const float dest)
	{
		this->src = src;
		this->dest = dest;
	}

	void AlphaAnimation::apply(AnimatableSprite* sprite)
	{
		float fac = elapsedTime / duration;
		sprite->color.a = glm::mix(src, dest, fac);
	}
	RotateAnimation::RotateAnimation(const float src, const float dest)
	{
		this->src = src;
		this->dest = dest;
	}

	void RotateAnimation::apply(AnimatableSprite* sprite)
	{
		float fac = elapsedTime / duration;
		sprite->rotation = glm::mix(src, dest, fac);
	}



	CurveToAnimation::CurveToAnimation(const blib::math::Rectangle& src, const blib::math::Rectangle& dest, float direction, float incomingDirection) : src(src), dest(dest)
	{
		path = math::BiArc(src.center(), blib::util::fromAngle(direction), dest.center(), blib::util::fromAngle(incomingDirection));
	}

	void CurveToAnimation::apply(AnimatableSprite* sprite)
	{
		float fac = elapsedTime / duration;

		glm::vec2 size = glm::mix(src.size(), dest.size(), fac);
		glm::vec2 pos = path.getPoint(fac);

		sprite->rect = blib::math::Rectangle(pos-size/2.0f, pos+size/2.0f);
	}

}