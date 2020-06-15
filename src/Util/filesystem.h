#ifndef DATABASE_FILESYSTEM_H
#define DATABASE_FILESYSTEM_H

#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <experimental/filesystem>
#define std::filesystem std::experimental::filesystem
#endif

#endif //DATABASE_FILESYSTEM_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */