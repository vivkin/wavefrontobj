#define WAVEFRONTOBJ_IMPLEMENTATION
#include "wavefrontobj.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [file ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("%10s %10s %10s %10s %10s\n", "facegroups", "indices", "vertices", "texcoords", "normals");
    for (int argi = 1; argi < argc; ++argi) {
        const char *filename = argv[argi];
        FILE *fp = fopen(filename, "rb");
        if (fp) {
            struct wavefrontobj mesh;
            memset(&mesh, 0, sizeof(mesh));
            wavefrontobj_parse(&mesh, fp);
            printf("%10u %10u %10u %10u %10u %s\n", mesh.num_facegroups, mesh.num_indices, mesh.num_vertices, mesh.num_texcoords, mesh.num_normals, filename);
            fclose(fp);
        }
    }

    return 0;
}
