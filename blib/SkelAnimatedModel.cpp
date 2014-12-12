#include "SkelAnimatedModel.h"
#include <blib/util/FileSystem.h>
#include <blib/json.h>

namespace blib
{

	SkelAnimatedModel::SkelAnimatedModel(const std::string &meshFile, const std::string &boneFile)
	{
		const blib::json::Value meshData = blib::util::FileSystem::getJson(meshFile);
		const blib::json::Value boneData = blib::util::FileSystem::getJson(boneFile);




	}

}