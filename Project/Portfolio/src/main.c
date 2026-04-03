#include <stdio.h>

#define KDBS_USE                    \
"\n:: NOT ENOUGH ARGUMENTS ::"      \
"\n   ./K-DBS [-config.*-]\n"

int main(int argc, char** argv[]) {
    argc < 2 ? puts(KDBS_USE) : 0;

    return 0;
}