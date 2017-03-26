#define WAVEFRONTOBJ_IMPLEMENTATION
#include "wavefrontobj.h"
#include <stdio.h>
#include <stdlib.h>

#define dump(f, v) fprintf(stderr, #v " = %" #f "\n", v)

int main(int argc, char **argv) {
    struct wavefrontobj foo;

    dump(zu, sizeof(foo.facegroups));
    dump(zu, sizeof(foo.facegroups[0]));
    dump(zu, sizeof(foo.indices));
    dump(zu, sizeof(foo.indices[0]));
    dump(zu, sizeof(foo.vertices));
    dump(zu, sizeof(foo.vertices[0]));

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
