#pragma once

#include <string>
#include <fstream>
#include <list>
#include <vector>

#include <blib/util/StreamInFile.h>
#include <blib/util/StreamOutFile.h>
#include <blib/util/StreamSeekable.h>
#include <blib/json.hpp>

namespace blib
{

	namespace util
	{
		class StreamOut;


		class FileSystemHandler
		{
		public:
			std::string name;
			FileSystemHandler(const std::string &name) : name(name) {};
            virtual ~FileSystemHandler() {};

			virtual StreamInFile* openRead(const std::string &fileName) { return NULL; };
			virtual StreamOut* openWrite(const std::string &fileName) { return NULL; };
			virtual void getFileList(const std::string &path, std::vector<std::string> &files) {};
			virtual void getFileList(const std::function<bool(const std::string&)> &filter, std::vector<std::string> &files) {};
		};

		class PhysicalFileSystemHandler : public FileSystemHandler
		{
		public:
			class StreamInFilePhysical : public StreamInFile
			{
				std::ifstream* stream;
			public:
				StreamInFilePhysical(std::ifstream* stream);
				~StreamInFilePhysical();
				virtual unsigned int read( char* data, int count );
				virtual char get();
				virtual bool eof();
				virtual void seek( int offset, StreamOffset offsetTo );
				virtual unsigned int tell();
				virtual bool opened();
			};
			class StreamOutFilePhysical : public StreamOutFile
			{
				std::ofstream* stream;
				bool deleteOnDestruct;
				int index;
			public:
				StreamOutFilePhysical(std::string filename);
				StreamOutFilePhysical(std::ofstream* stream);
				~StreamOutFilePhysical();
				virtual void write(char* data, int count);
				virtual bool opened();
			};


			std::string directory;

			PhysicalFileSystemHandler(std::string directory = ".") : FileSystemHandler("physical"), directory(directory) {}
			virtual StreamInFile* openRead( const std::string &fileName );
			virtual StreamOut* openWrite( const std::string &fileName );
			virtual void getFileList(const std::string &path, std::vector<std::string> &files);
			virtual void getFileList(const std::function<bool(const std::string&)> &filter, std::vector<std::string> &files);
		};


		class MemoryFile : public StreamInFile
		{
			char* data;
			int length;
			bool copied;
			int index;
		public:
			MemoryFile(char* data, int lenght, bool copy);
			~MemoryFile();
			virtual unsigned int read( char* data, int count );
			virtual char get();
			virtual bool eof();
			virtual void seek( int offset, StreamOffset offsetTo );
			virtual unsigned int tell();
			virtual bool opened();
		};

		class FileSystem
		{
		private:
			static std::list<FileSystemHandler*> handlers;
		public:
			static StreamInFile* openRead(const std::string &fileName);
			static StreamOut* openWrite(const std::string &fileName);
			static bool exists(const std::string &fileName);
			static void registerHandler(FileSystemHandler* handler);
			static const std::list<FileSystemHandler*> getHandlers();
			static int getData(const std::string &fileName, char* &data);
			static std::string getData(const std::string &fileName);
			static json getJson(const std::string &fileName);
			static json getCbor(const std::string &fileName);
			static std::vector<std::string> getFileList(const std::string &path);
			static std::vector<std::string> getFileList(const std::function<bool(const std::string&)> &filter);
			static void dispose();
		};
	}
}
