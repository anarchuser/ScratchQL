#include "FileHandler.h"

FileHandler::FileHandler (std::string const & database, std::string const & table,
        std::vector <std::size_t> const & columnLen, std::vector <CellType> const & colType) :
    db_root{DATABASE_DIR},
    database{database},
    name{table},
    path{db_root + database + '/' + name + "/table.tsv"},       //purely for transition path coexists with the other three directory paths
    lineLength{calcLineLength(columnLen)},
    columnLength(columnLen),
    columnType(colType)
    { createTable(); }

FileHandler::FileHandler (Table const & table) :
    FileHandler (
            table.database,
            table.name,
            table.getMetaColLength(table.getMeta()),
            table.getMetaDataType (table.getMeta())) {}


void FileHandler::createDatabase() {
    try {
        cleanName(database);
    }
    catch(std::invalid_argument & error) {
        THROW(error);
    }
    std::filesystem::create_directory(db_root);
    std::filesystem::create_directory(db_root + database, db_root);
}

void FileHandler::createTable() {
    createDatabase();
    try {
        cleanName(name);
    }
    catch(std::invalid_argument & error) {
        THROW(error);
    }
    std::filesystem::create_directory(db_root + database + '/' + name, db_root);
    LOG(INFO) << "successfully created table at " << path << std::endl;
}

void FileHandler::createLine (std::vector <Cell> const & content) {
    std::vector <std::size_t> extralength = surplusColumnLengths(content);
    std::ofstream out;
    try {
        out = std::ofstream (path, std::ios::app);
    }
    catch(std::exception & e){
        std::cerr << e.what() << std::endl;
    }
    if (!out.is_open ()) {
        THROW (std::ios_base::failure ("Could not open file"));
    }
    std::size_t cellCount = 0;
    for (Cell const & cell : content) {
        out << cell << std::string(extralength[cellCount], ' ') << '\t';
        cellCount++;
    }
    out << std::endl;
    out.close();
}

std::vector <Cell> FileHandler::readLine (std::size_t index) const {
    std::vector <std::string> contentStringVector;
    std::vector <Cell> content;

    std::ifstream in (path, std::ios::in);
    if (!in.is_open ()) THROW (std::ios_base::failure (STR+
    "Could not open file " + path));

    in.seekg(+(lineLength + 1 ) * index);
    char waste[1];
    for (std::size_t const & cellLength : columnLength){
        char tmp_line[cellLength + 1];
        for (std::size_t i = 0; i < cellLength; i++) tmp_line[i] = ' '; tmp_line[cellLength] = 0;
        in.read(tmp_line, cellLength);
        contentStringVector.emplace_back(std::string(tmp_line));
        in.read(waste, 1);
    }
    in.close();
    std::size_t counter = 0;
    for (std::string element : contentStringVector){
        content.emplace_back(writeToCell(element, columnType[counter++]));
    }
    return content;
}

void FileHandler::updateLine (std::size_t index, std::vector <Cell> const & content) {
    std::ofstream file (path, std::ios::in | std::ios::out);
    std::string tmpline;
    std::vector <std::size_t> extralength = surplusColumnLengths(content);

    file.seekp((lineLength + 1) * index);
    std::size_t cellCounter {};
    for (Cell const & cell : content){
        file << cell << std::string(extralength[cellCounter], ' ') << '\t';
        cellCounter++;
    }
    file.close();
}

void FileHandler::deleteLine (std::size_t index) {
    std::fstream file (path, std::ios::in | std::ios::out);

    file.seekp((lineLength + 1) * index);
    for (std::size_t const & cellLength : columnLength){
        file << std::string(cellLength, ' ') << '\t';
    }
    file.close();
}

void FileHandler::deleteTable() const {
    std::filesystem::remove_all(db_root + '/' + database + '/' + name);
}

void FileHandler::deleteDatabase() const {
    std::filesystem::remove_all(db_root + '/' + database);
}

void FileHandler::clearLines () const {
    std::fstream file (path, std::ios::in | std::ios::out);
    std::fstream newfile (db_root + database + '/' + name + "/table_tmp.tsv", std::ios::out);
    std::string tmpline;

    while (getline (file, tmpline)){
        if (tmpline != std::string(lineLength, ' ')){
            newfile << tmpline << std::endl;
        }
    }
    file.close();
    newfile.close();
    std::filesystem::remove(path);
    std::filesystem::rename(db_root + database + '/' + name + "/table_tmp.tsv", path);
}

void FileHandler::cleanName(std::string & alnum_string){
    if (alnum_string.empty()){
        THROW (std::invalid_argument("String cannot be empty"));
    }
    for (auto const & letter : alnum_string){
        if (!std::isalnum(letter)){
            THROW (std::invalid_argument("String contains non-alphanumeric characters"));
        }
    }
}

std::size_t FileHandler::checkLineLength(std::string const & content) const {
    std::size_t extralength = lineLength - content.length();
    if (extralength < 0){
        THROW (std::range_error ("Contents exceed maximum length " + content));
    }
    return extralength;
}

std::vector <std::size_t> FileHandler::surplusColumnLengths(std::vector <Cell> const & contentVector) {
    std::size_t maxLen;
    std::size_t actualLen;
    std::size_t counter = 0;
    std::vector <std::size_t> lengths;
    for (Cell celly : contentVector){
        if (celly.index() == TEXT){
            actualLen = std::get <std::string> (celly).size();
        }
        else{
            actualLen = (-celly).length();
        }
        maxLen = columnLength[counter];
        if (maxLen - actualLen < 0) {
            std::stringstream errorMsg;
            errorMsg << "Contents exceed maximum length - maximum allowed Length: " << maxLen
                    << " -actual Length: " << actualLen << " -- content in question: " << +celly;
            THROW(std::range_error (errorMsg.str()));
        }
        lengths.emplace_back(maxLen - actualLen);
        counter++;
    }
    return lengths;
}

//shall iterate over all columns of one table and return a maximum line length for the fileHandler
std::size_t calcLineLength(std::vector <std::size_t> const & colLength) {
    std::size_t lineLength = 0;
    for (std::size_t length: colLength){  //how to iterate over all columns of a certain table?
        lineLength += length + 1;
    }
    return lineLength;
}
/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */