#include "FileSystem.h"
#include <json/json.h>
#include <fstream>

namespace blib
{
	namespace util
	{
		StreamInFile* PhysicalFileSystemHandler::openRead( std::string fileName )
		{
			std::ifstream* stream = new std::ifstream(directory + "/" + fileName, std::ios_base::binary);
			if(stream->is_open())
			{
				return new StreamInFilePhysical(stream);
			}
			delete stream;
			return NULL;

		}

		StreamOut* PhysicalFileSystemHandler::openWrite( std::string fileName )
		{
			throw std::exception("The method or operation is not implemented.");
		}


		PhysicalFileSystemHandler::StreamInFilePhysical::StreamInFilePhysical(std::ifstream* stream)
		{
			this->stream = stream;
		}

		PhysicalFileSystemHandler::StreamInFilePhysical::~StreamInFilePhysical()
		{
			stream->close();
			delete stream;
			stream = NULL;
		}

		unsigned int PhysicalFileSystemHandler::StreamInFilePhysical::read( char* data, int count )
		{
			stream->read(data, count);
			return (unsigned int)stream->gcount();
		}

		char PhysicalFileSystemHandler::StreamInFilePhysical::get()
		{
			return stream->get();
		}

		bool PhysicalFileSystemHandler::StreamInFilePhysical::eof()
		{
			return stream->eof();
		}

		void PhysicalFileSystemHandler::StreamInFilePhysical::seek( int offset, StreamOffset offsetTo )
		{
			if(offsetTo == StreamSeekable::BEGIN)
				stream->seekg(offset, std::ios_base::beg);
			else if(offsetTo == StreamSeekable::CURRENT)
				stream->seekg(offset, std::ios_base::cur);
			else if(offsetTo == StreamSeekable::END)
				stream->seekg(offset, std::ios_base::end);
		}

		unsigned int PhysicalFileSystemHandler::StreamInFilePhysical::tell()
		{
			return (unsigned int)stream->tellg();
		}

		bool PhysicalFileSystemHandler::StreamInFilePhysical::opened()
		{
			if(!stream)
				return false;
			return stream->is_open();
		}





















		std::list<FileSystemHandler*> FileSystem::handlers;

		StreamInFile* FileSystem::openRead( std::string fileName )
		{
			for(std::list<FileSystemHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				StreamInFile* stream = (*it)->openRead(fileName);
				if(stream)
					return stream;
			}
			return NULL;
		}

		void FileSystem::registerHandler( FileSystemHandler* handler )
		{
			handlers.push_back(handler);
		}

		int FileSystem::getData(std::string fileName, char* &data)
		{
			StreamInFile file(fileName);
			file.seek(0, StreamSeekable::END);
			unsigned int size = file.tell();
			data = new char[size];
			file.seek(0, StreamSeekable::BEGIN);
			file.read(data, size);
			return size;
		}
		std::string FileSystem::getData(std::string fileName)
		{
			StreamInFile file(fileName);
			if(!file.opened())
				return "";
			std::string ret;
			char buf[1024];

			while(!file.eof())
			{
				int rc = file.read(buf, 1024);
				ret += std::string(buf, rc);
			}
			return ret;
		}

		Json::Value FileSystem::getJson( std::string fileName )
		{
			std::string data = getData(fileName);
			Json::Value ret;
			Json::Reader reader;
			reader.parse(data, ret);

			return ret;
		}

	}
}

