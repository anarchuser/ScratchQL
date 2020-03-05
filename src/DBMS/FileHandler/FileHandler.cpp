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
    cutTailSpaces(line);
    return line;
}

void FileHandler::updateLine (std::size_t index, std::string content) {
    std::fstream file (path, std::ios::in | std::ios::out);
    std::string tmpline;

    int extralength = checkLineLength(content);
    int tmp_len;
    int i = 0;
    int content_len = content.length();
    int iterating_pos = file.tellg();
    int len_diff = 0;

    while (getline (file, tmpline)){
        if (i++ == index) {
            tmp_len = tmpline.length();
            if (content_len <= tmp_len){
                len_diff = tmp_len - content_len;
                content.append(std::string (extralength, ' '));
            }
            else if (content_len > tmp_len){
                createLine(content);
                deleteLine(index);
                break;
            }
            tmpline = content;
            file.seekp(iterating_pos);
            file << tmpline;
            break;
        }
        iterating_pos = file.tellg();
    }
    file.close();
}

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
            file.seekp(write_pos);
            file << tmpline;
            break;
        }
        iterating_pos = file.tellg();
    }
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

int FileHandler::checkLineLength(std::string const & content){
    int extralength = tmp_line_length - content.length();
    if (extralength < 0){
        LOG (ERROR) << "Contents exceed maximum length";
        throw (std::range_error ("Contents exceed maximum length " + content));
    }
    return extralength;
}

void FileHandler::cutTailSpaces(std::string & content){
    while(content.back() == ' ')   content.pop_back();
}
/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */