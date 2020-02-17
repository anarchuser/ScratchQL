#include "Logger.h"

void THROW (const std::exception & e) {
    LOG (ERROR) << e.what ();
    throw e;
}

/* Copyright (C) 2020 Aaron Alef & Felix Bachstein */