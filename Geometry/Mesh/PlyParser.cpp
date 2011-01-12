#include "PlyParser.h"

typedef struct Vertex {
   float x,y,z;      // space coordinates       
} Vertex;

typedef struct Shape {
   unsigned char nverts;    // number of vertex indices in list
   int* verts;              // vertex index list 
} Shape;

PlyParser::PlyParser() : Mesh() {
}

void PlyParser::loadModel(string fname) {
   // list of property information for a vertex
	// this varies depending on what you are reading from the file
   PlyProperty vertProps[] = {
      {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
      {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
      {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0}
	};
   
   PlyProperty faceProps[] = { 
	  	{"vertex_indices", PLY_INT, PLY_INT, offsetof(Shape,verts),
      1, PLY_UCHAR, PLY_UCHAR, offsetof(Shape,nverts)}
	};

   int nelems;
   char** elist;
   int file_type;
   float version;

   PlyFile* ply = ply_open_for_reading(fname.c_str(), &nelems, &elist, &file_type, &version);
   printf ("version %f\n", version);
  	printf ("type %d\n", file_type);
   
   // go through each kind of element that we learned is in the file and read them 
   int num_elems, nprops;
   for (int i = 0; i < nelems; i++) {
      ply_get_element_description (ply, elist[i], &num_elems, &nprops);
      printf("element name = %s num elements = %u\n", elist[i], num_elems);
      
      // if we're on vertex elements, read in the properties
      if(strcmp("vertex", elist[i]) == 0) {
         // set up for getting vertex elements
         // the three properties are the vertex coordinates
         ply_get_property(ply, elist[i], &vertProps[0]);      
		  	ply_get_property(ply, elist[i], &vertProps[2]);
         
         pointsReserve(num_elems);
         
         for(int j = 0; j < num_elems; j++) {
            Vertex* vptr = new Vertex;
            ply_get_element(ply, (void*) vptr);
            addPoint(new Point3D(vptr->x, vptr->y, vptr->z));
            delete vptr;
         }
      }
      
      // if we're on face elements, read them in 
      if(strcmp("face", elist[i]) == 0) {
         ply_get_property (ply, elist[i], &faceProps[0]);
         
         facesReserve(num_elems);
         
         for(int j = 0; j < num_elems; j++) {
            Shape* sptr = new Shape;
            ply_get_element (ply, (void *) sptr);
            addFace(new Face(sptr->verts[0], sptr->verts[1], sptr->verts[2]));
            delete sptr;
         }
      }
   }
   
   ply_close(ply);
   printf("done loading");

   calculateNormals();
   printf("normals calculated\n");
}
