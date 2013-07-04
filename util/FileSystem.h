#pragma once

#include <string>
#include <fstream>
#include <list>

#include <blib/util/StreamInFile.h>
#include <blib/util/StreamSeekable.h>


namespace blib
{
	namespace util
	{
		class StreamOut;


		class FileSystemHandler
		{
		public:
			std::string name;
			FileSystemHandler(std::string name) : name(name) {};

			virtual StreamInFile* openRead(std::string fileName) { return NULL; };
			virtual StreamOut* openWrite(std::string fileName) { return NULL; };
		};

		class PhysicalFileSystemHandler : public FileSystemHandler
		{
			class StreamInFilePhysical : public StreamInFile
			{
				std::ifstream* stream;
			public:
				StreamInFilePhysical(std::ifstream* stream);
				virtual unsigned int read( char* data, int count );
				virtual char get();
				virtual bool eof();
				virtual void seek( int offset, StreamOffset offsetTo );
				virtual unsigned int tell();
			};
			std::string directory;
		public:
			PhysicalFileSystemHandler(std::string directory = ".") : FileSystemHandler("physical"), directory(directory) {}
			virtual StreamInFile* openRead( std::string fileName );
			virtual StreamOut* openWrite( std::string fileName );
		};

		class FileSystem
		{
		private:
			static std::list<FileSystemHandler*> handlers;
		public:
			static StreamInFile* openRead(std::string fileName);
			static StreamOut* openWrite(std::string fileName);
			static void registerHandler(FileSystemHandler* handler);
			static int getData(std::string fileName, char* &data);
			static std::string getData(std::string fileName);
		};
	}
}