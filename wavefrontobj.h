#ifndef WAVEFRONTOBJ_H
#define WAVEFRONTOBJ_H

#include <stdio.h>

struct wavefront_coord {
    float x, y, z;
};

struct wavefront_index {
    unsigned int vertex, normal, texcoord;
};

struct wavefront_group {
    unsigned int first, count;
    char name[64];
};

struct wavefront_mesh {
    struct wavefront_coord *vertices, *normals, *texcoords;
    struct wavefront_index *indices;
    struct wavefront_group *groups;
    unsigned int num_groups, num_indices, num_vertices, num_normals, num_texcoords;
};

void wavefrontobj_fixup(struct wavefront_mesh *mesh);
void wavefrontobj_free(struct wavefront_mesh *mesh);
void wavefrontobj_line(struct wavefront_mesh *mesh, const char *line);
void wavefrontobj_load(struct wavefront_mesh *mesh, FILE *stream);
void wavefrontobj_parse(struct wavefront_mesh *mesh, const char *str);

#endif // WAVEFRONTOBJ_H

#ifdef WAVEFRONTOBJ_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>

#define push_back(data, size, value)                                     \
    do {                                                                 \
        *((void **)&data) = realloc(data, sizeof(data[0]) * (size + 1)); \
        memcpy(data + size++, value, sizeof(data[0]));                   \
    } while (0)

void wavefrontobj_fixup(struct wavefront_mesh *mesh) {
    if (mesh->num_groups) {
        mesh->groups[mesh->num_groups - 1].count = mesh->num_indices - mesh->groups[mesh->num_groups - 1].first;
    } else if (mesh->num_vertices && mesh->num_indices) {
        struct wavefront_group group = {0, mesh->num_indices, ""};
        push_back(mesh->groups, mesh->num_groups, &group);
    }
}

void wavefrontobj_free(struct wavefront_mesh *mesh) {
    free(mesh->vertices);
    free(mesh->normals);
    free(mesh->texcoords);
    free(mesh->indices);
    free(mesh->groups);
}

void wavefrontobj_line(struct wavefront_mesh *mesh, const char *line) {
    if (line[0] == 'v') {
        struct wavefront_coord coord = {0, 0, 0};
        sscanf(line + 2, "%f %f %f", &coord.x, &coord.y, &coord.z);
        if (line[1] == 'n')
            push_back(mesh->normals, mesh->num_normals, &coord);
        else if (line[1] == 't')
            push_back(mesh->texcoords, mesh->num_texcoords, &coord);
        else
            push_back(mesh->vertices, mesh->num_vertices, &coord);
    } else if (line[0] == 'f') {
        unsigned int count = 0;
        int v[3] = {0, 0, 0};
        int n;
        line += 2;
        while (sscanf(line, "%d/%d/%d%n", v + 0, v + 1, v + 2, &n) == 3 ||
               sscanf(line, "%d/%d%n", v + 0, v + 1, &n) == 2 ||
               sscanf(line, "%d//%d%n", v + 0, v + 2, &n) == 2 ||
               sscanf(line, "%d%n", v + 0, &n) == 1) {
            if (count > 2) {
                unsigned int start = mesh->num_indices - (count - 2) * 3;
                unsigned int prev = mesh->num_indices - 1;
                push_back(mesh->indices, mesh->num_indices, mesh->indices + start);
                push_back(mesh->indices, mesh->num_indices, mesh->indices + prev);
            }
            struct wavefront_index index = {
                v[0] > 0 ? (unsigned int)v[0] - 1 : v[0] + mesh->num_vertices,
                v[1] > 0 ? (unsigned int)v[1] - 1 : v[1] + mesh->num_texcoords,
                v[2] > 0 ? (unsigned int)v[2] - 1 : v[2] + mesh->num_normals};
            push_back(mesh->indices, mesh->num_indices, &index);
            count++;
            v[0] = v[1] = v[2] = 0;
            line += n;
        }
    } else if (line[0] == 'u' || line[0] == 'o' || line[0] == 'g') {
        if (mesh->num_groups)
            mesh->groups[mesh->num_groups - 1].count = mesh->num_indices - mesh->groups[mesh->num_groups - 1].first;
        struct wavefront_group group = {mesh->num_indices, 0, ""};
        sscanf(line, "%*s %63s", group.name);
        push_back(mesh->groups, mesh->num_groups, &group);
    }
}

void wavefrontobj_load(struct wavefront_mesh *mesh, FILE *stream) {
    char line[256];
    while (fgets(line, sizeof(line), stream))
        wavefrontobj_line(mesh, line);
    wavefrontobj_fixup(mesh);
}

void wavefrontobj_parse(struct wavefront_mesh *mesh, const char *str) {
    while (str) {
        wavefrontobj_line(mesh, str);
        if ((str = strchr(str, '\n')))
            str++;
    }
    wavefrontobj_fixup(mesh);
}

#undef push_back

#endif // WAVEFRONTOBJ_IMPLEMENTATION
