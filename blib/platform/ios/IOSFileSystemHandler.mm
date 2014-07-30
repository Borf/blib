//
//  IOSFileHandler.m
//  Memorable Knights
//
//  Created by Borf on 04/06/14.
//  Copyright (c) 2014 Borf. All rights reserved.
//

#include <Foundation/Foundation.h>
#import "IOSFileSystemHandler.h"

blib::util::StreamInFile* IOSFileSystemHandler::openRead( const std::string &fileName )
{
    std::string file = fileName;
    std::string ext = fileName;
    
    ext = ext.substr(ext.rfind(".")+1);
    file = file.substr(0, file.rfind("."));
    
//    file = file.substr(file.rfind("/")+1);
    
    NSString* path = [[NSBundle mainBundle] pathForResource:[NSString stringWithCString:file.c_str()] ofType:[NSString stringWithCString:ext.c_str()]];
    const char* cpath = [path cStringUsingEncoding:1 ];

  //  printf("%s\n", cpath);
    
    std::ifstream* stream = new std::ifstream(cpath, std::ios_base::binary);
    if(stream->is_open())
    {
        return new blib::util::PhysicalFileSystemHandler::StreamInFilePhysical(stream);
    }
    delete stream;
    return NULL;
}


blib::util::StreamOut* IOSFileSystemHandler::openWrite( const std::string &fileName )
{
    return NULL;
}


void IOSFileSystemHandler::getFileList(const std::string &path, std::vector<std::string> &files)
{
    
}
