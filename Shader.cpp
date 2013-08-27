#include "Shader.h"

namespace blib
{
	void Shader::setState( State& state )
	{
		for(std::map<std::string,	float>::iterator it = state.floatValues.begin(); it != state.floatValues.end(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	int>::iterator it = state.intValues.begin(); it != state.intValues.end(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	glm::vec2>::iterator it = state.vec2Values.begin(); it != state.vec2Values.end(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	glm::vec3>::iterator it = state.vec3Values.begin(); it != state.vec3Values.end(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	glm::vec4>::iterator it = state.vec4Values.begin(); it != state.vec4Values.end(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	glm::mat3>::iterator it = state.mat3Values.begin(); it != state.mat3Values.end(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	glm::mat4>::iterator it = state.mat4Values.begin(); it != state.mat4Values.end(); it++)
			setUniform(it->first, it->second);
	}


}