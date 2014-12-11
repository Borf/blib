#pragma once

#include <string>

namespace blib
{
	class SkelAnimatedMesh
	{
	public:
		SkelAnimatedMesh(const std::string &meshFile, const std::string &boneFile);
		void loadAnimation(const std::string fileName);

		void setAnimation(const std::string& animation);

	};
}