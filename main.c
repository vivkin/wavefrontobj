#define WAVEFRONTOBJ_IMPLEMENTATION
#include "wavefrontobj.h"
#include <stdio.h>
#include <stdlib.h>

#define dump(f, v) fprintf(stderr, #v " = %" #f "\n", v)

void wavefrontobj_dump(FILE *stream, const struct wavefront_mesh *mesh) {
    for (size_t i = 0; i < mesh->num_vertices; ++i)
        fprintf(stream, "v %f %f %f\n", mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
    for (size_t i = 0; i < mesh->num_texcoords; ++i)
        fprintf(stream, "vt %f %f %f\n", mesh->texcoords[i].x, mesh->texcoords[i].y, mesh->texcoords[i].z);
    for (size_t i = 0; i < mesh->num_normals; ++i)
        fprintf(stream, "vn %f %f %f\n", mesh->normals[i].x, mesh->normals[i].y, mesh->normals[i].z);
    for (size_t i = 0; i < mesh->num_groups; ++i) {
        fprintf(stream, "o %s\n", mesh->groups[i].name);
        for (size_t j = mesh->groups[i].first; j < mesh->groups[i].first + mesh->groups[i].count; ++j)
            fprintf(stream, "f %u/%u/%u %u/%u/%u %u/%u/%u\n",
                    mesh->indices[j + 0].vertex + 1, mesh->indices[j + 0].texcoord + 1, mesh->indices[j + 0].normal + 1,
                    mesh->indices[j + 1].vertex + 1, mesh->indices[j + 1].texcoord + 1, mesh->indices[j + 1].normal + 1,
                    mesh->indices[j + 2].vertex + 1, mesh->indices[j + 2].texcoord + 1, mesh->indices[j + 2].normal + 1);
    }
}

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
