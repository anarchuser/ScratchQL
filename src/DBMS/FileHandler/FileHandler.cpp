#include "FileHandler.h"

#include <utility>

FileHandler::FileHandler (std::string & database, std::string & table) :
    db_root{DATABASE_DIR},
    database{database},
    name{table},
    path{db_root + database + '/' + name + "/table.tsv"}       //purely for transition path coexists with the other three directory paths
    {
    createDatabase();
    createTable();
}

void FileHandler::createDatabase(){
    try {
        cleanName(database);
    }
    catch(std::invalid_argument & error) {
        LOG(ERROR) << "Database name is not valid";
        throw;
    }
    std::filesystem::create_directory(db_root);
    std::filesystem::create_directory(db_root + database, db_root);
}

void FileHandler::createTable(){
    createDatabase();
    try {
        cleanName(name);
    }
    catch(std::invalid_argument & error) {
        LOG(ERROR) << "Table name is not valid";
        throw;
    }
    std::filesystem::create_directory(db_root + database + '/' + name, db_root);
    LOG(INFO) << "successfully created table at " << path << std::endl;
}

void FileHandler::createLine (std::string const & content) {
    std::ofstream out;
    try {
        out = std::ofstream (path, std::ios::app);
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
    }
    if (!out.is_open ()) {
        std::cerr << "WAAAAAA" << std::endl;
        LOG (ERROR) << "Could not open '" << path << "'";
        std::cerr << "The filesystem exists: " << std::filesystem::exists(path) << std::endl;
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

void FileHandler::deleteLine (std::size_t index) {
    std::fstream file (path, std::ios::in | std::ios::out);
    std::string tmpline;

    int write_pos;
    int i = 0;
    int iterating_pos = write_pos = file.tellg();

    while (getline (file, tmpline)){
        if (i++ == index) {
            tmpline = std::string (tmpline.length(), ' ');
            write_pos = iterating_pos;
            break;
        }
        iterating_pos = file.tellg();
    }
    file.seekp(write_pos);
    file << tmpline;
    file.close();
}

void FileHandler::deleteTable() {
    std::filesystem::remove_all(db_root + '/' + database + '/' + name);
}

void FileHandler::deleteDatabase(){
    std::filesystem::remove_all(db_root + '/' + database);
}

void FileHandler::removePadding () {}

void FileHandler::cleanName(std::string & alnum_string){
    for (auto const & letter : alnum_string){
        if (!std::isalnum(letter)){
            std::cerr << "name " << alnum_string << "is not valid, because it contains non-alphanumeric charaters" << std::endl;
            throw (std::invalid_argument("String contains non-alphanumeric characters"));
        }
    }
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */