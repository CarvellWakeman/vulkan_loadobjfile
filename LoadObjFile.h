#pragma once

#include <vector>
#include "vertex.h"
#include "model.h"

struct model LoadObjFile(char *name);

char* ReadRestOfLine(FILE *fp);

void ReadObjVTN(char *str, int *v, int *t, int *n);