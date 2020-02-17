#include "FileHandler.h"

#include <utility>

FileHandler::FileHandler (std::string  path) : path{std::move(path)} {}

void FileHandler::createDatabase(std::string name){
    std::filesystem::create_directory(DATABASE_DIR);
    std::filesystem::create_directory(DATABASE_DIR + name);
}

void FileHandler::createTable(std::string database, std::string name){
    std::filesystem::create_directory(DATABASE_DIR + database + '/' + name);
}

void FileHandler::createLine (const std::string & content) {
    std::ofstream out (path, std::ios::app);
    if (!out.is_open ()) {
        LOG (ERROR) << "Could not open '" << path << "'";
        throw (std::ios_base::failure ("Could not open file"));
    }

    out << content << std::endl;
    out.close();
}

// TODO: rewrite
std::string FileHandler::readLine (long index) {
    std::ifstream in (path, std::ios::in);
    if (!in.is_open ()) {
        LOG (ERROR) << "Could not open '" << path << "'";
        throw (std::ios_base::failure ("Could not open file"));
    }

    std::string header, line;
//    getline (in, header);
    for (int i = 0; i <= index; i++) getline (in, line);
    in.close();
    return line;
}

void FileHandler::updateLine (const std::string & content, long index) {}
void FileHandler::deleteLine (long index) {
    std::cout << "hi, still working" << std::endl;
    std::fstream file (path, std::ios::in | std::ios::out);
    std::string tmpline;
    int tmplength;
    int i = 0;

    while (getline (file, tmpline)){
        if (i++ == index) {
            tmplength = tmpline.length();
            tmpline = std::string (tmplength, ' ');
        }
        file << tmpline;
    }
    file.close();
}

void FileHandler::deleteTable() {
    std::remove (path.data());
}

void FileHandler::removePadding () {}


/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */