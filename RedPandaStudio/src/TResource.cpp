#include "TResource.h"
#include <stdlib.h>
#include <string.h>

void TResource::setName(const char * n) {
    char* copy = (char*) malloc( strlen(n) + 1);
    strcpy(copy, n);

    name = copy;

}