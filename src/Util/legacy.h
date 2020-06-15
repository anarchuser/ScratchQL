#ifndef DATABASE_LEGACY_H
#define DATABASE_LEGACY_H

#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <experimental/filesystem>
#include <experimental/vector>

#define std::filesystem std::experimental::filesystem
#define std::erase std::experimental::erase
#endif

#endif //DATABASE_LEGACY_H

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */