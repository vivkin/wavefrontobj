#ifndef WAVEFRONTOBJ_H
#define WAVEFRONTOBJ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int ivec2[2];
typedef int ivec3[3];
typedef float vec3[3];

#define push_back(data, size, value)                        \
    do {                                                    \
        data = realloc(data, sizeof(data[0]) * (size + 1)); \
        memcpy(data + size++, value, sizeof(data[0]));      \
    } while (0)

struct wavefrontobj {
    ivec2 *facegroups;
    ivec3 *indices;
    vec3 *normals;
    vec3 *texcoords;
    vec3 *vertices;
    unsigned int num_facegroups, num_indices, num_vertices, num_texcoords, num_normals;
};

#endif // WAVEFRONTOBJ_H

#ifdef WAVEFRONTOBJ_IMPLEMENTATION

void wavefrontobj_parse(struct wavefrontobj *mesh, FILE *stream) {
    char line[256];
    while (fgets(line, sizeof(line), stream)) {
        if (line[0] == 'v') {
            vec3 v = {0, 0, 0};
            sscanf(line + 2, "%f %f %f", v + 0, v + 1, v + 2);
            if (line[1] == 'n')
                push_back(mesh->normals, mesh->num_normals, &v);
            else if (line[1] == 't')
                push_back(mesh->texcoords, mesh->num_texcoords, &v);
            else
                push_back(mesh->vertices, mesh->num_vertices, &v);
        } else if (line[0] == 'f') {
            unsigned int count = 0;
            char *saveptr, *token = strtok_r(line + 2, " ", &saveptr);
            while (token) {
                ivec3 v = {1, 1, 1};
                if (sscanf(token, "%d/%d/%d", v + 0, v + 1, v + 2) == 3 ||
                    sscanf(token, "%d/%d", v + 0, v + 1) == 2 ||
                    sscanf(token, "%d//%d", v + 0, v + 2) == 2 ||
                    sscanf(token, "%d", v + 0) == 1) {
                    if (count > 2) {
                        unsigned int start = mesh->num_indices - (count - 2) * 3;
                        unsigned int prev = mesh->num_indices - 1;
                        push_back(mesh->indices, mesh->num_indices, mesh->indices + start);
                        push_back(mesh->indices, mesh->num_indices, mesh->indices + prev);
                    }
                    v[0] = v[1] > 0 ? v[0] - 1 : v[0] + mesh->num_vertices;
                    v[1] = v[1] > 0 ? v[1] - 1 : v[1] + mesh->num_texcoords;
                    v[2] = v[2] > 0 ? v[2] - 1 : v[2] + mesh->num_normals;
                    push_back(mesh->indices, mesh->num_indices, &v);
                    ++count;
                }
                token = strtok_r(NULL, " ", &saveptr);
            }
        } else if (line[0] == 'u') {
            ivec2 v = {mesh->num_indices, 0x811C9DC5};
            for (const char *s = line + 7; *s && *s != '\n'; ++s)
                v[1] = ((unsigned int)v[1] ^ (unsigned char)*s) * 0x01000193;
            push_back(mesh->facegroups, mesh->num_facegroups, &v);
        }
    }
    push_back(mesh->facegroups, mesh->num_facegroups, (&(ivec2){(int)mesh->num_indices, 0}));
}

#endif // WAVEFRONTOBJ_IMPLEMENTATION
