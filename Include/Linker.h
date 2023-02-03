#ifndef LINKER_H
#define LINKER_H

#include "LinkerFile.h"
#include "CommandLine.h"

#define ELF_STARTING_ADDRESS 1000

struct LinkerFile* Link(struct LinkerFileList* Sources);

#endif