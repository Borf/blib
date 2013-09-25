#pragma once


#include <map>
#include <string>
#include <glm/glm.hpp>

namespace blib
{
	class Shader
	{
	public:
		class State
		{
		public:
			std::map<std::string,	float>		floatValues;
			std::map<std::string,	int>			intValues;
			std::map<std::string,	glm::vec2>	vec2Values;
			std::map<std::string,	glm::vec3>	vec3Values;
			std::map<std::string,	glm::vec4>	vec4Values;
			std::map<std::string,	glm::mat3>	mat3Values;
			std::map<std::string,	glm::mat4>	mat4Values;

			inline void setValue(std::string name,			const float &value)		{	floatValues[name] = value;	};
			inline void setValue(std::string name,			const int &value)		{	intValues.insert(std::pair<std::string, const int>(name, value));	};
			inline void setValue(std::string name,			const glm::vec2 &value)	{	vec2Values.insert(std::pair<std::string, const glm::vec2>(name, value));	};
			inline void setValue(std::string name,			const glm::vec3 &value)	{	vec3Values.insert(std::pair<std::string, const glm::vec3>(name, value));	};
			inline void setValue(std::string name,			const glm::vec4 &value)	{	vec4Values.insert(std::pair<std::string, const glm::vec4>(name, value));	};
			inline void setValue(std::string name,			const glm::mat3 &value)	{	mat3Values.insert(std::pair<std::string, const glm::mat3>(name, value));	};
			inline void setValue(std::string name,			const glm::mat4 &value)	{	mat4Values[name] = value;	};
		}	state;


		std::map<std::string, int> attributes;
		std::string vertexShader;
		std::string fragmentShader;

		template <class T>
		inline void setUniform(std::string name,			const T& value)
		{
			state.setValue(name, value);
		//	doUniform(name, value);
		}
		virtual void use() = 0;
		virtual void initFromData(std::string vertexShader, std::string fragmentShader);
		virtual void bindAttributeLocation(std::string name, int index);


		virtual void setState(State& state);
	protected:
		virtual void doUniform(const std::string &name,			const glm::mat4& value) = 0;
		virtual void doUniform(const std::string &name,			const glm::mat3& value) = 0;
		virtual void doUniform(const std::string &name,			const float& value) = 0;
		virtual void doUniform(const std::string &name,			const int& value) = 0;
		virtual void doUniform(const std::string &name,			const glm::vec4& value) = 0;
		virtual void doUniform(const std::string &name,			const glm::vec3& value) = 0;
		virtual void doUniform(const std::string &name,			const glm::vec2& value) = 0;
	};
}