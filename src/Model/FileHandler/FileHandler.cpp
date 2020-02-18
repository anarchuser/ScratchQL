#include "FileHandler.h"

#include <utility>

FileHandler::FileHandler (std::string path) : path{std::move(path)} {}

void FileHandler::createDatabase(std::string name){
    std::filesystem::create_directory(DATABASE_DIR);
    std::filesystem::create_directory(DATABASE_DIR + name);
}

void FileHandler::createTable(std::string database, std::string name){
    std::filesystem::create_directory(DATABASE_DIR + database + '/' + name);
}

void FileHandler::createLine (std::string const & content) {
    std::ofstream out (path, std::ios::app);
    if (!out.is_open ()) {
        LOG (ERROR) << "Could not open '" << path << "'";
        throw (std::ios_base::failure ("Could not open file"));
    }

    out << content << std::endl;
    out.close();
}

// TODO: rewrite
std::string FileHandler::readLine (std::size_t index) {
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

void FileHandler::updateLine (std::string const & content, std::size_t index) {}
void FileHandler::deleteLine (std::size_t index) {}
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