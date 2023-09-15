#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) //6 cube faces, 2 triangles per face
extern face_t cube_faces[N_CUBE_FACES];

/// <summary>
/// Define a struct for dynamic size meshes, with array of vertices and faces.
/// </summary>
typedef struct {
	vec3_t* vertices; //Dynamic array of vertices
	face_t* faces; //Dynamic array of faces
    vec3_t rotation; //Rotation vectors for the mesh (yaw, pitch, roll)
} mesh_t; 

extern mesh_t mesh;

//Loads the cube mesh data into our global mesh variable.
void load_cube_mesh_data(void);

//Reads content of .obj file and loads vertices and faces into our mesh.
void load_obj_file_data(char* filename);

#endif