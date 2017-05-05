#pragma once

#include "Resource.h"
#include <map>
#include <string>
#include <vector>
#include <type_traits>

#include <glm/glm.hpp>
#include <blib/config.h>
#include <blib/Util.h>

namespace blib
{
	class Shader : public Resource
	{
	public:
		std::map<std::string, int> attributes;
		std::vector<std::string> vertexShaders;
		std::vector<std::string> fragmentShaders;
		enum UniformType
		{
			Float,
			Int,
			Vec2,
			Vec3,
			Vec4,
			Mat3,
			Mat4,
			Bool,
			Struct,
			Array,
		};
		
		class Uniform
		{
		public:
			std::string name;
			int index;
			int size;
			UniformType type;
			int id;

			Uniform(const std::string &name, int size, UniformType type)
			{
				id = -1;
				this->name = name;
				this->size = size;
				this->type = type;
				index = 0;
			}

			template<class T>
			void set(char* data, const T& value)
			{ // quite magical that this works :D
				data[index - 1]++;
				(T&)(data[index]) = value;
			}

			template<class T>
			T get(char* data)
			{
				return (T)(data[index]);
			}
		};
		class StructUniform : public Uniform
		{
		public:
			StructUniform(const std::string &name) : Uniform(name, 0, Struct) {}

			std::vector<Uniform*> members;
		};


		class ArrayUniform : public Uniform
		{
		public:
			ArrayUniform(const std::string &name) : Uniform(name, 0, Array) {};

			std::vector<Uniform*> members;
		};


		
		class UniformStructBase
		{
		public:
			class UniformStructMemberBase
			{
			public:
				std::string name;
				int size;
				UniformType type;
			public:
				UniformStructMemberBase(const std::string &name, int size, UniformType type)
				{
					this->name = name;
					this->size = size;
					this->type = type;
				}

				virtual void set(char* data, Uniform* uniform) { throw "oops"; }

			};
			template<class T>
			class UniformStructMember : public UniformStructMemberBase
			{
				T& value;
			public:
				UniformStructMember(T& value, const std::string &name, int size, UniformType type) : UniformStructMemberBase(name, size, type), value(value)
				{
				}
				virtual void set(char* data, Uniform* uniform)
				{
					uniform->set(data, value);
				}
			};




			std::vector<UniformStructMemberBase*> members;

			void reg(glm::vec3& value,	const std::string &name)		{				members.push_back(new UniformStructMember<glm::vec3>(value, name, sizeof(float) * 3, Vec3));			}
			void reg(float& value,		const std::string &name)		{				members.push_back(new UniformStructMember<float>(value, name, sizeof(float) * 1, Float));				}
		public:
			UniformStructBase()
			{};
		};

		template<class SubClass>
		class UniformStruct : public UniformStructBase
		{
		public:
			template<class T>
			static void regStatic(const std::string &name)
			{
				if (std::is_same<T, float>::value)
					staticMembers.push_back(new UniformStructMemberBase(name, sizeof(float) * 1, Float));
				else if (std::is_same<T, glm::vec3>::value)
					staticMembers.push_back(new UniformStructMemberBase(name, sizeof(float) * 3, Vec3));
			}


			static std::vector<UniformStructMemberBase*> staticMembers;
			static bool staticFilled;
		};


	


		int uniformCount;
		char* uniformData;
		char* activeUniformData;
		int uniformSize;
		Uniform* uniforms[32];
		Shader();
		~Shader();


		Uniform* buildUniform(const std::string &name, UniformType type)
		{
			Uniform* uniform = NULL;
			switch (type)
			{
			case Float:
				uniform = new Uniform(name, sizeof(float), type);
				break;
			case Int:
				uniform = new Uniform(name, sizeof(int), type);
				break;
			case Bool:
				uniform = new Uniform(name, sizeof(bool), type);
				break;
			case Vec2:
				uniform = new Uniform(name, sizeof(float)* 2, type);
				break;
			case Vec3:
				uniform = new Uniform(name, sizeof(float)* 3, type);
				break;
			case Vec4:
				uniform = new Uniform(name, sizeof(float)* 4, type);
				break;
			case Mat3:
				uniform = new Uniform(name, sizeof(float)* 3 * 3, type);
				break;
			case Mat4:
				uniform = new Uniform(name, sizeof(float)* 4 * 4, type);
				break;
			default:
				break;
			}
			assert(uniform);
			return uniform;
		}

		template<class T>
		void setUniformName(T value, const std::string& name, UniformType type)
		{
			Uniform* uniform = buildUniform(name, type);
			uniforms[(int)value] = uniform;
			uniformCount = glm::max<int>(uniformCount, (int)value + 1);
			uniform->index = uniformSize+1;
			uniformSize += uniform->size+1;
		}

		template<class StructType, class T>
		void setUniformName(T value, const std::string &name)
		{
			StructUniform* u = new StructUniform(name);
			uniforms[(int)value] = u;
			uniformCount = glm::max(uniformCount, (int)value + 1);

			if(!StructType::staticFilled)
				StructType::init();
			StructType::staticFilled = true;

			for (auto m : StructType::staticMembers)
			{
				Uniform* uniform = new Uniform(name + "." + m->name, m->size, m->type);
				uniform->index = uniformSize + 1;
				uniformSize += uniform->size + 1;

				u->members.push_back(uniform);
			}

			/*for (size_t i = 0; i < StructType::members.size(); i++)
			{
				Uniform* uniform = new Uniform(name + "." + StructType::members[i]->name, StructType::members[i]->size, StructType::members[i]->type);
				StructType::members[i]->uniform = uniform;
			}*/
		}

		template<class T>
		void setUniformArray(T value, const std::string &name, int size, UniformType type)
		{
			ArrayUniform* u = new ArrayUniform(name);
			uniforms[(int)value] = u;
			uniformCount = glm::max<int>(uniformCount, (int)value + 1);

			for (int i = 0; i < size; i++)
			{
                Uniform* uniform = buildUniform(name + "[" + blib::util::toString(i) + "]", type);
				u->members.push_back(uniform);
				uniformCount = glm::max(uniformCount, (int)value + 1);
				uniform->index = uniformSize + 1;
				uniformSize += uniform->size + 1;
			}
		}


		template <class Enum>
		inline void setUniformStruct(Enum name,		const UniformStructBase& value)
		{
			assert(uniformData);

			StructUniform* structUniform = (StructUniform*)uniforms[(int)name];

			for (size_t i = 0; i < structUniform->members.size(); i++)
			{
				value.members[i]->set(uniformData, structUniform->members[i]);
			}


		}


		template <class T, class Enum>
		inline void setUniform(Enum name,			const T& value)
		{
			assert(uniformData);
			if (uniforms[(int)name])
				uniforms[(int)name]->set(uniformData, value);
		}
		template <class T, class Enum>
		inline T getUniform(Enum name)
		{
			assert(uniformData);
			return uniforms[(int)name]->get<T>(uniformData);
		}


		template <class T, class Enum>
		inline void setUniform(Enum name, int index, const T& value)
		{
			assert(uniformData);
			assert(uniforms[(int)name]->type == Array);
			return ((ArrayUniform*)uniforms[(int)name])->members[index]->set(uniformData, value);
		}

		virtual void use() = 0;
		virtual void initFromData(std::string vertexShader, std::string fragmentShader);
		virtual void bindAttributeLocation(std::string name, int index);
		void finishUniformSetup();
		virtual void addVertexShader(const std::string &fileName);

		virtual void setState(char* state);
	protected:
	};


//	template<class SubClass>
//	std::vector<Shader::UniformStructBase::UniformStructMemberBase*> Shader::UniformStruct<SubClass>::members;


	template<class SubClass>
	std::vector<Shader::UniformStructBase::UniformStructMemberBase*> Shader::UniformStruct<SubClass>::staticMembers;
	template<class SubClass>
	bool Shader::UniformStruct<SubClass>::staticFilled = false;


#ifdef BLIB_IOS    
#define STUPIDIPHONE
#endif

#ifdef STUPIDIPHONE
	template<>
	void Shader::Uniform::set<glm::mat4>(char* data, const glm::mat4& value);
	template<>
	void Shader::Uniform::set<glm::mat3>(char* data, const glm::mat3& value);
	template<>
	void Shader::Uniform::set<glm::vec2>(char* data, const glm::vec2& value);
	template<>
	void Shader::Uniform::set<glm::vec3>(char* data, const glm::vec3& value);
	template<>
	void Shader::Uniform::set<glm::vec4>(char* data, const glm::vec4& value);

	template<>
	glm::mat4 Shader::Uniform::get<glm::mat4>(char* data);
	template<>
	glm::mat3 Shader::Uniform::get<glm::mat3>(char* data);
	template<>
	glm::vec2 Shader::Uniform::get<glm::vec2>(char* data);
	template<>
	glm::vec3 Shader::Uniform::get<glm::vec3>(char* data);
	template<>
	glm::vec4 Shader::Uniform::get<glm::vec4>(char* data);
#endif

}

