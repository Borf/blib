#pragma once


#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <blib/config.h>

namespace blib
{
	class Shader
	{
	public:
		std::map<std::string, int> attributes;
		std::string vertexShader;
		std::string fragmentShader;
		enum UniformType
		{
			Float,
			Int,
			Vec2,
			Vec3,
			Vec4,
			Mat3,
			Mat4,
			Struct,
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
		class UniformStruct : public Uniform
		{
		public:
			std::vector<Uniform*> members;
		};



		/**/
		class UniformStructBase
		{
		public:
			class UniformInfoBase
			{
			public:
				std::string name;
				int size;
				UniformType type;
				Uniform* uniform;

				UniformInfoBase(int size, UniformType type, const std::string &name)
				{
					this->size = size;
					this->type = type;
					this->name = name;
				}
				virtual void set(char* data) = 0;

			};
			template<class T>
			class UniformInfo : public UniformInfoBase
			{
			public:
				T& value;

				UniformInfo(T& value, int size, UniformType type, const std::string &name) : value(value), UniformInfoBase(size, type, name)
				{
				}
				virtual void set(char* data)
				{
					uniform->set(data, value);
				}
			};

		public:
			UniformStructBase() : Uniform("", 0, Struct)
			{};

			virtual std::vector<UniformInfoBase*> &getMembers() = 0;
			virtual bool canReg() = 0;

			void reg(glm::vec4& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<glm::vec4>(value, sizeof(float) * 4, Vec4, name));
			};
			void reg(glm::vec3& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<glm::vec3>(value, sizeof(float) * 3, Vec3, name));
			};
			void reg(glm::vec2& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<glm::vec2>(value, sizeof(float) * 2, Vec2, name));
			};
			void reg(float& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<float>(value, sizeof(float) * 1, Float, name));
			};
			void reg(glm::mat3& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<glm::mat3>(value, sizeof(float) * 3 * 3, Mat3, name));
			};
			void reg(glm::mat4& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<glm::mat4>(value, sizeof(float) * 4 * 4, Mat4, name));
			};
			void reg(int& value, const std::string &name) {
				if (canReg())
					getMembers().push_back(new UniformInfo<int>(value, sizeof(int), Int, name));
			};
		};

		template<class SubClass>
		class UniformStruct : public UniformStructBase
		{
		public:
			static std::vector<UniformInfoBase*> members;
			static bool doneReg;
			void finishReg()
			{
				doneReg = true;
			}

			virtual bool canReg() { return doneReg == false; }
			virtual std::vector<UniformInfoBase*> &getMembers() { return members; };
		};


	


		int uniformCount;
		char* uniformData;
		char* activeUniformData;
		int uniformSize;
		Uniform* uniforms[32];
		Shader();


		template<class T>
		void setUniformName(T value, const std::string& name, UniformType type)
		{
			Uniform* uniform = NULL;
			switch (type)
			{
			case Float:				uniform = new Uniform(name,	sizeof(float), type);			break;
			case Int:				uniform = new Uniform(name, sizeof(int), type);				break;
			case Vec2:				uniform = new Uniform(name, sizeof(float)* 2, type);		break;
			case Vec3:				uniform = new Uniform(name, sizeof(float)* 3, type);		break;
			case Vec4:				uniform = new Uniform(name, sizeof(float)* 4, type);		break;
			case Mat3:				uniform = new Uniform(name, sizeof(float)* 3 * 3, type);	break;
			case Mat4:				uniform = new Uniform(name, sizeof(float)* 4 * 4, type);	break;
			}
			assert(uniform);

			uniforms[(int)value] = uniform;
			uniformCount = glm::max(uniformCount, (int)value + 1);
			uniform->index = uniformSize+1;
			uniformSize += uniform->size+1;
		}

		template<class StructType, class T>
		void setUniformName(T value, const std::string name)
		{
			UniformStruct* u = new UniformStruct();
			uniforms[(int)value] = u;
			uniformCount = glm::max(uniformCount, (int)value + 1);

			for (auto m : StructType::members)
			{

			}

			StructType::init();

			/*for (size_t i = 0; i < StructType::members.size(); i++)
			{
				Uniform* uniform = new Uniform(name + "." + StructType::members[i]->name, StructType::members[i]->size, StructType::members[i]->type);
				uniform->index = uniformSize + 1;
				uniformSize += uniform->size + 1;
				StructType::members[i]->uniform = uniform;
			}*/
		}


		template <class Enum>
		inline void setUniformStruct(Enum name,		const UniformStructBase& value)
		{
			assert(uniformData);
			for (size_t i = 0; i < value.getMembers().size(); i++)
			{
				value.getMembers()[i]->set(uniformData);
			}


		}


		template <class T, class Enum>
		inline void setUniform(Enum name,			const T& value)
		{
			assert(uniformData);
			uniforms[(int)name]->set(uniformData, value);
		}
		template <class T, class Enum>
		inline T getUniform(Enum name)
		{
			assert(uniformData);
			return uniforms[(int)name]->get<T>(uniformData);
		}

		virtual void use() = 0;
		virtual void initFromData(std::string vertexShader, std::string fragmentShader);
		virtual void bindAttributeLocation(std::string name, int index);
		void finishUniformSetup();


		virtual void setState(char* state);
	protected:
	};

	template<class SubClass>
	bool Shader::UniformStruct<SubClass>::doneReg = false;

	template<class SubClass>
	std::vector<Shader::UniformStructBase::UniformInfoBase*> Shader::UniformStruct<SubClass>::members;


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

