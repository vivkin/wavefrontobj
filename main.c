#define WAVEFRONTOBJ_IMPLEMENTATION
#include "wavefrontobj.h"
#include <stdio.h>
#include <stdlib.h>

#define dump(f, v) fprintf(stderr, #v " = %" #f "\n", v)

int main(int argc, char **argv) {
    struct wavefront_mesh mesh;

    dump(zu, sizeof(mesh.groups));
    dump(zu, sizeof(mesh.groups[0]));
    dump(zu, sizeof(mesh.indices));
    dump(zu, sizeof(mesh.indices[0]));
    dump(zu, sizeof(mesh.vertices));
    dump(zu, sizeof(mesh.vertices[0]));

    if (argc < 2) {
        fprintf(stderr, "usage: %s [file ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("%10s %10s %10s %10s %10s\n", "groups", "indices", "vertices", "texcoords", "normals");
    for (int argi = 1; argi < argc; ++argi) {
        const char *filename = argv[argi];
        FILE *fp = fopen(filename, "rb");
        if (fp) {
            struct wavefront_mesh mesh;
            memset(&mesh, 0, sizeof(mesh));
            wavefrontobj_load(&mesh, fp);
            printf("%10u %10u %10u %10u %10u %s\n", mesh.num_groups, mesh.num_indices, mesh.num_vertices, mesh.num_texcoords, mesh.num_normals, filename);
            wavefrontobj_free(&mesh);
            fclose(fp);
        }
    }

    return 0;
}
