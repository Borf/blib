#include "FileSystem.h"
#include <blib/util/Log.h>
#include <blib/json.hpp>
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
			std::ofstream* stream = NULL;
			if (directory != "")
				stream = new std::ofstream((directory + "/" + fileName).c_str(), std::ios_base::binary);
			else
				stream = new std::ofstream(fileName.c_str(), std::ios_base::binary);
			if (stream->is_open())
			{
				return new StreamOutFilePhysical(stream);
			}
			delete stream;
			return NULL;
		}

		void PhysicalFileSystemHandler::getFileList(const std::string &path, std::vector<std::string> &files)
		{
#if !defined(WIN32) && !defined(BLIB_NX)
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
#elif defined(BLIB_NX)

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

		void PhysicalFileSystemHandler::getFileList(const std::function<bool(const std::string&)> &filter, std::vector<std::string> &files)
		{
#if !defined(WIN32) && !defined(BLIB_NX)
			DIR *dp;
			struct dirent *ep;
			dp = opendir(directory.c_str());
			if (dp)
			{
				while ((ep = readdir(dp)))
				{
					if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
						continue;

					struct stat stFileInfo;
					stat((directory + "/" + ep->d_name).c_str(), &stFileInfo);

					if ((stFileInfo.st_mode & S_IFDIR))// && recursive)
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
				Log::out << "Could not open directory '" << directory << "'" << Log::newline;
#elif defined(BLIB_NX)

#else
			WIN32_FIND_DATA FileData;													// thingy for searching through a directory
			HANDLE hSearch;
			if (directory != "")
				hSearch = FindFirstFile(std::string(directory + "/" + "*.*").c_str(), &FileData);
			else
				hSearch = FindFirstFile(std::string("*.*").c_str(), &FileData);
			if (hSearch != INVALID_HANDLE_VALUE)										// if there are results...
			{
				while (true)														// loop through all the files
				{
					if ((FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
					{
						if(filter(FileData.cFileName))
							files.push_back(std::string(FileData.cFileName) + "/");
					}
					else
						if (filter(FileData.cFileName))
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

		StreamOut* FileSystem::openWrite(const std::string &fileName)
		{
			for (std::list<FileSystemHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				StreamOut* stream = (*it)->openWrite(fileName);
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

		json FileSystem::getJson(const std::string &fileName)
		{
			std::string data = getData(fileName);
			if (data == "")
			{
				Log::out << "Could not open " << fileName << Log::newline;
				return json();
			}
			if (data.find("\0"))
				data = data.substr(0, data.find("\0"));

			bool inString = false;
			bool escape = false;
			for (size_t i = 0; i < data.size()-1; i++)
			{
				if (data[i] == '/' && data[i + 1] == '*' && !inString)
					data = data.substr(0, i) + data.substr(data.find("*/", i) + 2);
				if (data[i] == '/' && data[i + 1] == '/' && !inString)
					data = data.substr(0, i) + data.substr(data.find("\n", i));

				if (inString)
					if (data[i] == '\n')
						data = data.substr(0, i-1) + "\\n" + data.substr(i+1);
				if (inString)
					if (data[i] == '\t')
						data = data.substr(0, i) + "    " + data.substr(i + 1);
				if (data[i] == '"' && !escape)
					inString = !inString;
				if (data[i] == '\\')
					escape = !escape;
				else
					escape = false;
			}


			try
			{
				return json::parse(data);
			}
			catch (json::parse_error &exception)
			{
				Log::out << "Can not read json file: " << fileName << ", " << exception.what() << " at " << exception.byte << Log::newline;
				printf("\n\n%s\n\n",  data.c_str());
			}
			return nullptr;
		}

		json FileSystem::getCbor(const std::string &fileName)
		{
			char* data;
			int len = getData(fileName, data);
			if (len <= 0)
			{
				Log::out << "Could not open " << fileName << Log::newline;
				return json();
			}


			try
			{
				
				json jdata = json::from_cbor(data, len);
				delete[] data;
				return jdata;

			}
			catch (json::parse_error &exception)
			{
				Log::out << "Can not read cbor file: " << fileName << ", " << exception.what() << " at " << exception.byte << Log::newline;
			}
			return nullptr;
		}


		std::vector<std::string> FileSystem::getFileList(const std::string &path)
		{
			std::vector<std::string> ret;
			for (std::list<FileSystemHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
				(*it)->getFileList(path, ret);
			return ret;
		}

		std::vector<std::string> FileSystem::getFileList(const std::function<bool(const std::string&)> &filter)
		{
			std::vector<std::string> ret;
			for (std::list<FileSystemHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++)
				(*it)->getFileList(filter, ret);
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



		PhysicalFileSystemHandler::StreamOutFilePhysical::StreamOutFilePhysical(std::ofstream* stream)
		{
			this->stream = stream;
			this->deleteOnDestruct = true;
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

