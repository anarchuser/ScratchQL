#include "FileHandler.h"

FileHandler::FileHandler (std::string const & database, std::string const & table,
        std::vector <int> const & columnLen, std::vector <CellType> const & colType) :
    db_root{DATABASE_DIR},
    database{database},
    name{table},
    path{db_root + database + '/' + name + "/table.tsv"},       //purely for transition path coexists with the other three directory paths
    lineLength{calcLineLength(columnLen)},
    columnLength(columnLen),
    columnType(colType)
    {
    createDatabase();
    createTable();
}

void FileHandler::createDatabase(){
    try {
        cleanName(database);
    }
    catch(std::invalid_argument & error) {
        THROW(error);
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
        THROW(error);
    }
    std::filesystem::create_directory(db_root + database + '/' + name, db_root);
    LOG(INFO) << "successfully created table at " << path << std::endl;
}

void FileHandler::createLine (std::vector <Cell> const & content) {
    std::vector <int> extralength = surplusColumnLengths(content);
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
    int cellCount = 0;
    for (Cell cell : content) {
        out << cell << std::string(extralength[cellCount], ' ') << '\t';
        cellCount++;
    }
    out << std::endl;
    out.close();
}

std::vector <Cell> FileHandler::readLine (std::size_t index) const {
    std::vector <std::string> contentStringVector;
    std::vector <Cell> content;

    std::cout << "reading from file initialized.....";
    std::ifstream in (path, std::ios::in);
    if (!in.is_open ()) THROW (std::ios_base::failure (STR+
    "Could not open file " + path));

    in.seekg(+(lineLength + 1 ) * index);
    for (int const & cellLength : columnLength){
        char tmp_line[cellLength];
        in.read(tmp_line, cellLength);
        in.seekg(+1);
        contentStringVector.emplace_back(std::string(tmp_line));
    }
    in.close();
    std::cout << "reading into memory finished successfully" << std::endl << "proceding write to cells..." << std::endl;

    return (writeToCells(contentStringVector));
}

void FileHandler::updateLine (std::size_t index, std::vector <Cell> content) {
    std::fstream file (path, std::ios::in | std::ios::out);
    std::string tmpline;
    std::vector <int> extralength = surplusColumnLengths(content);

    file.seekp((lineLength + 1) * index);
    int cellCounter;
    for (Cell cell : content){
        file << cell << std::string(extralength[cellCounter], ' ') << '\t';
        cellCounter++;
    }
    file.close();
}

void FileHandler::deleteLine (std::size_t index) {
    std::fstream file (path, std::ios::in | std::ios::out);

    file.seekp((lineLength + 1) * index);
    for (int const & cellLength : columnLength){
        file << std::string(cellLength, ' ') << '\t';
    }
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

int FileHandler::checkLineLength(std::string const & content){
    int extralength = lineLength - content.length();
    if (extralength < 0){
        THROW (std::range_error ("Contents exceed maximum length " + content));
    }
    return extralength;
}

std::vector <int> const FileHandler::surplusColumnLengths(std::vector <Cell> const & contentVector) {
    int maxLen;
    int actualLen;
    std::vector <int> lengths;
    for (Cell celly : contentVector){
        if (celly.index() == TEXT){
            actualLen = std::get <std::string> (celly).size();
        }
        else{
            std::stringstream tmpContent;
            tmpContent << celly;
            actualLen = tmpContent.str().length();
        }
        maxLen = columnLength[celly.index()];
        if (maxLen - actualLen < 0) THROW(std::range_error ("Contents exceed maximum length"));
        lengths.emplace_back(maxLen - actualLen);
    }
    return (lengths);
}
std::vector <Cell> const FileHandler::writeToCells (std::vector <std::string> & inputVector) const{
    std::vector <Cell> targetVector;
    for (int colCounter = 0; colCounter < inputVector.size(); colCounter += 1){
        cutTailingSpaces(inputVector[colCounter]);
        if (columnType[colCounter] == TEXT){
            std::cout << "writing string to vector...";
            targetVector.emplace_back(inputVector[colCounter]);
            std::cout << "success" << std::endl;
        }
        else {
            std::cout << "converting string |"<< inputVector[colCounter] << "| to int...";
            int contentAsInt = std::stoi(inputVector[colCounter]);
            std::cout << "success...";
            if (columnType[colCounter] == SHORT) {
                targetVector.emplace_back(short(contentAsInt));
            } else if (columnType[colCounter] == LONG) {
                targetVector.emplace_back(long(contentAsInt));
            } else if (columnType[colCounter] == BINARY) {
                targetVector.emplace_back(bool(contentAsInt));
            }
            std::cout << "finished conversion" << std::endl;
        }
    }
    return (targetVector);
}

void FileHandler::cutTailingSpaces(std::string & content) {
    while (content.back() == ' ')   content.pop_back();
}
//shall iterate over all columns of one table and return a maximum line length for the fileHandler
int calcLineLength(std::vector <int> const & colLength) {
    int lineLength = 0;
    for (int length: colLength){  //how to iterate over all columns of a certain table?
        lineLength += length + 1;
    }
    return lineLength;
}
/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */