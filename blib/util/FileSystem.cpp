#include "FileSystem.h"
#include <blib/util/Log.h>
#include <blib/json.h>
#include <blib/linq.h>
#include <fstream>
#include <iostream>


#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#else
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

namespace blib
{
	namespace util
	{
		StreamInFile* PhysicalFileSystemHandler::openRead(const std::string &fileName)
		{
			std::ifstream* stream = NULL;
			if (directory != "")
				stream = new std::ifstream((directory + "/" + fileName).c_str(), std::ios_base::binary);
			else
				stream = new std::ifstream(fileName.c_str(), std::ios_base::binary);
			if (stream->is_open())
			{
				return new StreamInFilePhysical(stream);
			}
			delete stream;
			return NULL;

		}

		StreamOut* PhysicalFileSystemHandler::openWrite(const std::string &fileName)
		{
			throw "The method or operation is not implemented.";
		}

		void PhysicalFileSystemHandler::getFileList(const std::string &path, std::vector<std::string> &files)
		{
#ifndef WIN32
			DIR *dp;
			struct dirent *ep;
			dp = opendir(path.c_str());
			if(dp)
			{
				while((ep = readdir(dp)))
				{
					if(strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
						continue;

					struct stat stFileInfo;
					stat((directory + "/" + ep->d_name).c_str(), &stFileInfo);

					if((stFileInfo.st_mode & S_IFDIR))// && recursive)
					{
						/*vector<string> dirContents = getFiles(dir + "/" + ep->d_name, filter, recursive);
						for(unsigned int i = 0; i < dirContents.size(); i++)
						files.push_back(dirContents[i]);*/
						files.push_back(std::string(ep->d_name) + "/");
					}
					else
					{
						//						if(fnmatch(filter.c_str(), ep->d_name,0) == 0)
						files.push_back(ep->d_name);
					}
				}
				closedir(dp);
			}
			else
				Log::out<<"Could not open directory '"<<directory<<"'"<<Log::newline;
#else
			WIN32_FIND_DATA FileData;													// thingy for searching through a directory
			HANDLE hSearch;
			if (directory != "")
				hSearch = FindFirstFile(std::string(directory + "/" + path + "/*.*").c_str(), &FileData);
			else
				hSearch = FindFirstFile(std::string(path + "/*.*").c_str(), &FileData);
			if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
			{
				while (true)														// loop through all the files
				{
					if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
					{
						files.push_back(std::string(FileData.cFileName) + "/");
					}
					else
						files.push_back(FileData.cFileName);

					if (!FindNextFile(hSearch, &FileData))								// find next file in the resultset
					{
						if (GetLastError() == ERROR_NO_MORE_FILES)						// we're finished when there are no more files
							break;
						else
							return;													// wow, something really weird happened
					}
				}
				FindClose(hSearch);
			}
#endif			

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

		unsigned int PhysicalFileSystemHandler::StreamInFilePhysical::read(char* data, int count)
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

		void PhysicalFileSystemHandler::StreamInFilePhysical::seek(int offset, StreamOffset offsetTo)
		{
			stream->clear();
			if (offsetTo == StreamSeekable::BEGIN)
				stream->seekg(offset, std::ios_base::beg);
			else if (offsetTo == StreamSeekable::CURRENT)
				stream->seekg(offset, std::ios_base::cur);
			else if (offsetTo == StreamSeekable::END)
				stream->seekg(offset, std::ios_base::end);
			stream->clear();
		}

		unsigned int PhysicalFileSystemHandler::StreamInFilePhysical::tell()
		{
			return (unsigned int)stream->tellg();
		}

		bool PhysicalFileSystemHandler::StreamInFilePhysical::opened()
		{
			if (!stream)
				return false;
			return stream->is_open();
		}





















		std::list<FileSystemHandler*> FileSystem::handlers;

		StreamInFile* FileSystem::openRead(const std::string &fileName)
		{
			for (std::list<FileSystemHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				StreamInFile* stream = (*it)->openRead(fileName);
				if (stream)
					return stream;
			}
			return NULL;
		}

		void FileSystem::registerHandler(FileSystemHandler* handler)
		{
			handlers.push_back(handler);
		}

		int FileSystem::getData(const std::string &fileName, char* &data)
		{
			StreamInFile file(fileName);
			if (!file.opened())
				return 0;
			file.seek(0, StreamSeekable::END);
			unsigned int size = file.tell();
			data = new char[size];
			file.seek(0, StreamSeekable::BEGIN);
			file.read(data, size);
			return size;
		}
		std::string FileSystem::getData(const std::string &fileName)
		{
			StreamInFile file(fileName);
			if (!file.opened())
				return "";
			std::string ret;
			char buf[1024];

			while (!file.eof())
			{
				int rc = file.read(buf, 1024);
				ret += std::string(buf, rc);
			}
			return ret;
		}

		json::Value FileSystem::getJson(const std::string &fileName)
		{
			std::string data = getData(fileName);
			if (data == "")
			{
				Log::out << "Could not open " << fileName << Log::newline;
				return json::Value();
			}
			return json::readJson(data);
		}

		std::vector<std::string> FileSystem::getFileList(const std::string &path)
		{
			std::vector<std::string> ret;
			for (std::list<FileSystemHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
				(*it)->getFileList(path, ret);


			return ret;
		}

		const std::list<FileSystemHandler*> FileSystem::getHandlers()
		{
			return handlers;
		}

		bool FileSystem::exists(const std::string &fileName)
		{
			StreamInFile* file = openRead(fileName);
			bool e = file != NULL;
			if (file)
				delete file;
			return e;
		}

		void FileSystem::dispose()
		{
			blib::linq::deleteall(handlers);
		}


		MemoryFile::MemoryFile(char* data, int length, bool copy)
		{
			this->data = data;
			this->length = length;
			this->copied = copy;
			this->index = 0;
			if (copy)
			{
				this->data = new char[length];
				memcpy(this->data, data, length);
			}

		}
		MemoryFile::~MemoryFile()
		{
			if (copied && data)
			{
				delete[] data;
				data = NULL;
			}
		}

		unsigned int MemoryFile::read(char* data, int count)
		{
			int remaining = length - index;
			if (count > remaining)
				count = remaining;

			memcpy(data, this->data + index, count);
			index += count;

			return count;
		}

		char MemoryFile::get()
		{
			index++;
			return this->data[index - 1];
		}

		bool MemoryFile::eof()
		{
			return index >= length;
		}

		void MemoryFile::seek(int offset, StreamOffset offsetTo)
		{
			if (offsetTo == StreamSeekable::BEGIN)
				index = offset;
			else if (offsetTo == StreamSeekable::CURRENT)
				index += offset;
			else if (offsetTo == StreamSeekable::END)
				index = length + offset;
		}

		unsigned int MemoryFile::tell()
		{
			return index;
		}

		bool MemoryFile::opened()
		{
			return true;
		}


		PhysicalFileSystemHandler::StreamOutFilePhysical::StreamOutFilePhysical(std::string filename)
		{
			this->fileName = filename;
			deleteOnDestruct = true;
			index = 0;
			stream = new std::ofstream(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
		}
		PhysicalFileSystemHandler::StreamOutFilePhysical::~StreamOutFilePhysical()
		{
			if (deleteOnDestruct)
				delete stream;
		}

		void PhysicalFileSystemHandler::StreamOutFilePhysical::write(char* data, int count)
		{
			stream->write(data, count);
			index += count;
		}

		bool PhysicalFileSystemHandler::StreamOutFilePhysical::opened()
		{
			return stream->is_open();
		}

	}
}

