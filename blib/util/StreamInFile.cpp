#include "StreamInFile.h"

#include <blib/util/FileSystem.h>

namespace blib
{
	namespace util
	{


		StreamInFile::StreamInFile( std::string fileName ) : fileName(fileName)
		{
			innerStream = FileSystem::openRead(fileName);
		}

	}
}

