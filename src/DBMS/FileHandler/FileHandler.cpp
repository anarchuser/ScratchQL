#include "FileHandler.h"

#include <utility>

FileHandler::FileHandler (std::string & database, std::string & table) :
    db_root{DATABASE_DIR},
    database{database},
    name{table},
    path{db_root + database + '/' + name + "/table.tsv"},       //purely for transition path coexists with the other three directory paths
    tmp_line_length{26}                                                     //for testing purposes until a meta file is created TODO: remove ASAP
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
    int extralength = checkLineLength(content);
    std::ofstream out;
    try {
        out = std::ofstream (path, std::ios::app);
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
    }
    if (!out.is_open ()) {
        LOG (ERROR) << "Could not open '" << path << "'";
        std::cerr << "The filesystem exists: " << std::filesystem::exists(path) << std::endl;
        throw (std::ios_base::failure ("Could not open file"));
    }
    out << content << std::string(extralength, ' ') << std::endl;
    out.close();
}

std::string FileHandler::readLine (std::size_t index) {
    std::ifstream in (path, std::ios::in);
    if (!in.is_open ()) {
        LOG (ERROR) << "Could not open '" << path << "'";
        throw (std::ios_base::failure ("Could not open file"));
    }

    char tmp_line[tmp_line_length + 1];
    tmp_line[tmp_line_length] = 0;
    in.seekg((tmp_line_length + 1 )  * index);              //try-catch sigsegv - SIGSEGV - Segmentation violation signal
    in.read(tmp_line, tmp_line_length);
    in.close();

    std::string line = std::string(tmp_line);
    cutTailingSpaces(line);
    return line;
}

void FileHandler::updateLine (std::size_t index, std::string content) {
    std::fstream file (path, std::ios::in | std::ios::out);
    std::string tmpline;
    int extralength = checkLineLength(content);

    file.seekp((tmp_line_length + 1) * index);
    file << content << std::string(extralength, ' ');
    file.close();
}

void FileHandler::deleteLine (std::size_t index) {
    std::fstream file (path, std::ios::in | std::ios::out);

    file.seekp((tmp_line_length + 1) * index);
    file << std::string(tmp_line_length, ' ');
    file.close();
}

void FileHandler::deleteTable() {
    std::filesystem::remove_all(db_root + '/' + database + '/' + name);
}

void FileHandler::deleteDatabase(){
    std::filesystem::remove_all(db_root + '/' + database);
}

void FileHandler::clearLines () {
    std::fstream file (path, std::ios::in | std::ios::out);
    std::fstream newfile (db_root + database + '/' + name + "/table_tmp.tsv", std::ios::out);
    std::string tmpline;

    while (getline (file, tmpline)){
        if (tmpline != std::string(tmp_line_length, ' ')){
            newfile << tmpline << std::endl;
        }
    }
    file.close();
    newfile.close();
    std::filesystem::remove(path);
    std::filesystem::rename(db_root + database + '/' + name + "/table_tmp.tsv", path);
}

void FileHandler::cleanName(std::string & alnum_string){
    for (auto const & letter : alnum_string){
        if (!std::isalnum(letter)){
            std::cerr << "name " << alnum_string << "is not valid, because it contains non-alphanumeric characters" << std::endl;
            throw (std::invalid_argument("String contains non-alphanumeric characters"));
        }
    }
}

int FileHandler::checkLineLength(std::string const & content){
    int extralength = tmp_line_length - content.length();
    if (extralength < 0){
        LOG (ERROR) << "Contents exceed maximum length";
        throw (std::range_error ("Contents exceed maximum length " + content));
    }
    return extralength;
}

void FileHandler::cutTailingSpaces(std::string & content){
    while (content.back() == ' ')   content.pop_back();
}
/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */