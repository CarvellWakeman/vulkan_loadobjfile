#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <map>

#include "./Utils.h"
#include "./loadobjfile.h"


// delimiters for parsing the obj file:
const char* OBJDELIMS = " \t";


// Holds indexes for vertex, texture, normal
struct fancy_vertex
{
	int v, n, t;
};


struct model LoadObjFile(char *name)
{
	char *cmd;		// the command string
	char *str;		// argument string

	std::vector <glm::vec3> Vertices;
	std::vector <glm::vec3> Normals;
	std::vector <glm::vec3> TextureCoords;

	std::map<int, vertex> uniqueVertices = {};
	std::vector<struct vertex> vkVertices;

	struct model objModel;

	// Temp vectors
	glm::vec3 sv;
	glm::vec3 sn;
	glm::vec3 st;


	// open the input file:
	FILE *fp;
	errno_t err;

	if ((err = fopen_s(&fp, name, "r")) != 0)
	{
		fprintf(stderr, "Cannot open .obj file '%s'\n", name);
		return objModel;
	}


	// Process lines
	while(true)
	{
		// Read line
		char *line = ReadRestOfLine(fp);
		if (line == NULL)
			break;


		// skip this line if it is a comment:
		if (line[0] == '#')
			continue;


		// skip this line if it is something we don't feel like handling today:
		if (line[0] == 'g')
			continue;

		if (line[0] == 'm')
			continue;

		if (line[0] == 's')
			continue;

		if (line[0] == 'u')
			continue;


		// get the command string:
		rsize_t linesize = sizeof line;
		char *next_token;
		char *token;

		cmd = strtok_s(line, OBJDELIMS, &next_token);


		// skip this line if it is empty:
		if (cmd == NULL)
			continue;

		if (strcmp(cmd, "s") == 0)
			continue;


		// Vertex line
		if (strcmp(cmd, "v") == 0)
		{
			// X
			str = strtok_s(NULL, OBJDELIMS, &next_token);
			sv.x = atof(str);
			// Y
			str = strtok_s(NULL, OBJDELIMS, &next_token);
			sv.y = atof(str);
			// Z
			str = strtok_s(NULL, OBJDELIMS, &next_token);
			sv.z = atof(str);
			// Record
			Vertices.push_back(sv);

			continue;
		}


		// Normal line
		if (strcmp(cmd, "vn") == 0)
		{
			str = strtok_s(NULL, OBJDELIMS, &next_token);
			sn.x = atof(str);

			str = strtok_s(NULL, OBJDELIMS, &next_token);
			sn.y = atof(str);

			str = strtok_s(NULL, OBJDELIMS, &next_token);
			sn.z = atof(str);

			Normals.push_back(sn);

			continue;
		}


		// Texture coordinate line
		if (strcmp(cmd, "vt") == 0)
		{
			st.s = st.t = st.p = 0.;

			str = strtok_s(NULL, OBJDELIMS, &next_token);
			st.s = atof(str);

			str = strtok_s(NULL, OBJDELIMS, &next_token);
			if (str != NULL)
				st.t = atof(str);

			str = strtok_s(NULL, OBJDELIMS, &next_token);
			if (str != NULL)
				st.p = atof(str);

			TextureCoords.push_back(st);

			continue;
		}


		// Face line
		if (strcmp(cmd, "f") == 0)
		{
			// Get indices for each face
			std::vector<struct fancy_vertex> face;

			int sizev = (int)Vertices.size();
			int sizen = (int)Normals.size();
			int sizet = (int)TextureCoords.size();

			char *str;
			while ((str = strtok_s(NULL, OBJDELIMS, &next_token)) != NULL)
			{
				int v, t, n;
				ReadObjVTN(str, &v, &t, &n);

				// if v, n, or t are negative, they are wrt the end of their respective list:
				if (v < 0)
					v += (sizev + 1);

				if (n < 0)
					n += (sizen + 1);

				if (t < 0)
					t += (sizet + 1);


				// be sure we are not out-of-bounds (<vector> will abort):
				if (t > sizet)
				{
					if (t != 0)
						fprintf(stderr, "Read texture coord %d, but only have %d so far\n", t, sizet);
					t = 0;
				}

				if (n > sizen)
				{
					if (n != 0)
						fprintf(stderr, "Read normal %d, but only have %d so far\n", n, sizen);
					n = 0;
				}

				if (v > sizev)
				{
					if (v != 0)
						fprintf(stderr, "Read vertex coord %d, but only have %d so far\n", v, sizev);
					v = 0;
					continue;
				}

				// Build 
				struct fancy_vertex fv;
				fv.n = n;
				fv.t = t;
				fv.v = v;
				face.push_back(fv);
			}


			// if vertices are invalid, don't draw anything this time:
			if (face.size() != 3)
				continue;

			// Process indices
			struct fancy_vertex fv;
			int fv_1, fn_1, ft_1;

			// Every vertex in the face
			for (int i = 0; i < face.size(); i++) {
				// OBJ indices are one based
				fv = face[i];
				fv_1 = fv.v - 1;
				fn_1 = fv.n - 1;
				ft_1 = fv.t - 1;

				// Build vertex from indices
				struct vertex v;
				v.position = Vertices[fv_1];
				v.normal= Normals[fn_1];
				v.texCoord = glm::vec2(TextureCoords[ft_1].x, TextureCoords[ft_1].y);


				// DRAW BY VERTEX
				//v.color = glm::vec3((v.position.x+1)/2, (v.position.y + 1) / 2, (v.position.z + 1) / 2); // Rainbow colors
				v.color = glm::vec3(1., 1., 1.);
				vkVertices.push_back(v);


				// DRAW BY INDEX
				//Check if vertex encountered before
				//if (uniqueVertices.count(fv_1) == 0) {
				//	uniqueVertices[fv_1] = v;
				//}
				//objModel.indices.push_back(fv_1);
			}
		}
	}


	// Cleanup
	fclose(fp);

	fprintf(stderr, "Loaded OBJ file with %d vertices, %d normals, and %d texture coordinates. \n", Vertices.size(), Normals.size(), TextureCoords.size());
	
	
	// DRAW BY VERTEX
	objModel.vertices = vkVertices;

	// DRAW BY INDEX
	//Turn unique vertices into objModel.vertices
	//for (auto const& element : uniqueVertices) {
	//	objModel.vertices.push_back(element.second);
	//}

	return objModel;
}


char* ReadRestOfLine(FILE *fp)
{
	static char *line;
	std::vector<char> tmp(1000);
	tmp.clear();

	while(true)
	{
		int c = getc(fp);

		if (c == EOF && tmp.size() == 0)
		{
			return NULL;
		}

		if (c == EOF || c == '\n')
		{
			delete[] line;
			line = new char[tmp.size() + 1];
			for (int i = 0; i < (int)tmp.size(); i++)
			{
				line[i] = tmp[i];
			}
			line[tmp.size()] = '\0';	// terminating null
			return line;
		}
		else
		{
			tmp.push_back(c);
		}
	}

	return "";
}


void ReadObjVTN(char *str, int *v, int *t, int *n)
{
	// can be one of v, v//n, v/t, v/t/n:

	if (strstr(str, "//"))				// v//n
	{
		*t = 0;
		sscanf_s(str, "%d//%d", v, n);
		return;
	}
	else if (sscanf_s(str, "%d/%d/%d", v, t, n) == 3)	// v/t/n
	{
		return;
	}
	else
	{
		*n = 0;
		if (sscanf_s(str, "%d/%d", v, t) == 2)		// v/t
		{
			return;
		}
		else						// v
		{
			*n = *t = 0;
			sscanf_s(str, "%d", v);
		}
	}
}