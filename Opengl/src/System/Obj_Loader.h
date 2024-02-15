#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<iostream>
#include<vector>

#include <fstream>
#include <sstream>
#include <string>
#include<string.h>

using namespace std;



class Obj_Loader
{
public:
	Obj_Loader();
	~Obj_Loader();

	std::vector<unsigned int> VAOs;
	std::vector<unsigned int> VBOs;
	std::vector<unsigned int> textures;

	int size;


	glm::vec3 read_vec3(std::vector<std::string> words,
		glm::mat4 preTransform, float w);

	void read_corner(std::string description,
		std::vector<glm::vec3>& v, std::vector<glm::vec2>& vt,
		std::vector<glm::vec3>& vn, std::vector<float>& vertices);

	void read_face(std::vector<std::string> words,
		std::vector<glm::vec3>& v, std::vector<glm::vec2>& vt,
		std::vector<glm::vec3>& vn, std::vector<float>& vertices);

	glm::vec2 read_vec2(std::vector<std::string> words);

	void make_obj_mesh(
		const char* filepath, glm::mat4 preTransform);


};