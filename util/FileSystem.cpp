#include "FileSystem.h"

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

	}
}

