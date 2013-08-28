#pragma once


namespace blib
{
	class VBO
	{
	public:
		virtual void bind() = 0;
	};

	template <class T>
	class VBO_ : public VBO
	{
	public:
		virtual void setData(int length, T* data) = 0;
	};
}