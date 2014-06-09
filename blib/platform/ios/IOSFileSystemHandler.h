//
//  IOSFileHandler.h
//  Memorable Knights
//
//  Created by Borf on 04/06/14.
//  Copyright (c) 2014 Borf. All rights reserved.
//


#include <blib/util/FileSystem.h>

class IOSFileSystemHandler : public blib::util::FileSystemHandler
{
    std::string directory;
public:
    IOSFileSystemHandler(std::string directory = "") : FileSystemHandler("ios"), directory(directory) {}
    ~IOSFileSystemHandler() {}
    virtual blib::util::StreamInFile* openRead( const std::string &fileName );
    virtual blib::util::StreamOut* openWrite( const std::string &fileName );
    virtual void getFileList(const std::string &path, std::vector<std::string> &files);
    
};