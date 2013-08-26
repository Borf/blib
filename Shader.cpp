#include "Shader.h"

namespace blib
{
	void Shader::setState( const State& state )
	{
		for(std::map<std::string,	const float>::const_iterator it = state.floatValues.cbegin(); it != state.floatValues.cend(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	const int>::const_iterator it = state.intValues.cbegin(); it != state.intValues.cend(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	const glm::vec2>::const_iterator it = state.vec2Values.cbegin(); it != state.vec2Values.cend(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	const glm::vec3>::const_iterator it = state.vec3Values.cbegin(); it != state.vec3Values.cend(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	const glm::vec4>::const_iterator it = state.vec4Values.cbegin(); it != state.vec4Values.cend(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	const glm::mat3>::const_iterator it = state.mat3Values.cbegin(); it != state.mat3Values.cend(); it++)
			setUniform(it->first, it->second);
		for(std::map<std::string,	const glm::mat4>::const_iterator it = state.mat4Values.cbegin(); it != state.mat4Values.cend(); it++)
			setUniform(it->first, it->second);
	}


}