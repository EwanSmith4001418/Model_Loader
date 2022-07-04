#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

struct vec4
{
	float x, y, z, w;
};

struct vec2
{
	float u, v;
};

struct OBJVertex
{
	vec4 vertex;
	vec4 normal;
	vec2 uvCoord;
};

vec4 processVectorString(const std::string a_data)
{
	//split the line data at each space character and store this as a float value within a glm::vec4
	std::stringstream iss(a_data);
	//create a zero vec4
	vec4 vecData = { 0.f,0.f,0.f,0.f };
	int i = 0;
	//for looop to loop until iss cannot stream data into val
	for (std::string val; iss >> val; ++i)
	{
		//use std::string to float function
		float fVal = std::stof(val);
		//cast vec4 to float* to allow iteration through elements of vec4
		((float*)(&vecData))[i] = fVal;
	}
	return vecData;
}
std::vector<std::string> spiltStringAtCharacter(std::string data, char a_character)
{
	std::vector<std::string> lineData;
	std::stringstream iss(data);
	std::string lineSegment;
	//providing a character to the getline function splits the line at occurences of that character
	while (std::getline(iss, lineSegment, a_character))
	{
		//push each line segment into a vector
		lineData.push_back(lineSegment);
	}
	return lineData;
}

OBJVertex processFaceData(std::string a_faceData, std::vector<vec4>& a_vertexArray,
	std::vector<vec4>& a_normalArray, std::vector<vec2>& a_uvArray)
{
	std::vector<std::string> vertexIndices = spiltStringAtCharacter(a_faceData, '/');
	//a simple local structire to hold face triplet data as integer values
	typedef struct objFaceTriplet { int32_t v, vn, vt; }objFaceTriplet;
	//instance of objFaceTriplet struct
	objFaceTriplet ft = { 0,0,0 };
	//parse vertex indices as integer values to look up in vertex/normal/uv array data
	ft.v = std::stoi(vertexIndices[0]);
	//if ise is >= 2 then there is additional information outside of vertex data
	if (vertexIndices.size() >= 2) {
		//if size of element 1 is greater than 0 then UV-coord information present
		if (vertexIndices[1].size() > 0)
		{
			ft.vt = std::stoi(vertexIndices[1]);
		}
		//if size is greater than 3 then there is normal data present
		if (vertexIndices.size() >= 3)
		{
			ft.vn = std::stoi(vertexIndices[2]);
		}
	}
	//now that face index values have been processed retrieve actual data from vertex arrays
	OBJVertex currentVertex;
	currentVertex.vertex = a_vertexArray[size_t(ft.v) - 1];
	if (ft.vn != 0)
	{
		currentVertex.normal = a_normalArray[size_t(ft.vn) - 1];
	}
	if (ft.vt != 0)
	{
		currentVertex.uvCoord = a_uvArray[size_t(ft.vt) - 1];
	}
	return currentVertex;
}

bool ProcessLine(const std::string& a_inLine, std::string& a_outKey, std::string& a_outValue)
{
	if (!a_inLine.empty())
	{
		//find the first character on the line that is not a space or a tab
		size_t keyStart = a_inLine.find_first_not_of(" \t\r\n");
		//if key start is not valid
		if (keyStart == std::string::npos)
			return false;
		//starting from the key find the first character that is not a space or a tab
		size_t keyEnd = a_inLine.find_first_of(" \t\r\n", keyStart);
		size_t valueStart = a_inLine.find_first_not_of("\t\r\n", keyEnd);
		//find the end position for the value
		//from the end of the line find the lsat character that isn't a space, tab, newline or return
		//+1 to include the last character itself
		size_t valueEnd = a_inLine.find_last_not_of(" \t\n\r") + 1;
		//now that we have the start and end positions for the data use substrong
		a_outKey = a_inLine.substr(keyStart, keyEnd - keyStart);
		if (valueStart == std::string::npos)
		{
			//If we get here then we had a line with a key and no data value
			//E.G "# \n"
			a_outValue = "";
			return true;
		}
		a_outValue = a_inLine.substr(valueStart, valueEnd - valueStart);
		return true;
	}
	return false;
}

int main(int argx, char* argv[])
{
	//Needs a directory/folder called obj_models 
	//In that directory/folder needs a file called basic_box.OBJ
	std::string filename = "obj_models/basic_box.OBJ";
	std::cout << "Attempting to open file: " << filename << std::endl;
	//use fstream to read file datat
	std::fstream file;
	file.open(filename, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		std::cout << "Successfully Opened!" << std::endl;
		// attempt to read the maximum number of bytes available from the file
		file.ignore(std::numeric_limits<std::streamsize>::max());
		//the fstream gcounter will now be at the end of the file, gcount is a byte offset from 0
		//0 is the start of the file, or how many bytes file.ignore just read
		std::streamsize fileSize = file.gcount();
		//clear the EDF marker from being read
		file.clear();
		//reset the seekg back to the start of the file
		file.seekg(0, std::ios_base::beg);
		//write out the files size to console if it contains data
		if (fileSize != 0)
		{
			std::cout << std::fixed;
			std::cout << std::setprecision(2);
			std::cout << "File Size: " << fileSize / (float)1024 << "KB" << std::endl;
			//File is open and contains data
			//read each line of the file and display to the console
			std::string fileLine;
			//while the end of file (EOF) token has not been read
			std::map< std::string, int32_t > faceIndexMap;
			std::vector<vec4> vertexData;
			std::vector<vec4> normalData;
			std::vector<vec2> textureData;
			std::vector<OBJVertex> meshData;

			while (!file.eof())
			{
				if (std::getline(file, fileLine))
				{
					//if we get here we managed to read a line from the file
					std::string key;
					std::string value;
					if (ProcessLine(fileLine, key, value))
					{
						if (key == "#") //this is a comment
						{
							std::cout << value << std::endl;
						}
						if (key == "v")
						{
							vec4 vertex = processVectorString(value);
							vertex.w = 1.f; //as this is positional data ensure that w component is set to 1
							vertexData.push_back(vertex);
						}
						if (key == "vn")
						{
							vec4 normal = processVectorString(value);
							normal.w = 0.f; //as this directional data ensure that w component is set to 0#
							normalData.push_back(normal);
						}
						if (key == "vt")
						{
							vec4 vec = processVectorString(value);
							vec2 uvCoord = { vec.x, vec.y };
							textureData.push_back(uvCoord);
						}
						if (key == "f")
						{
							std::vector<std::string> faceComponents = spiltStringAtCharacter(value, ' ');
							std::vector<uint32_t>faceIndices;
							std::vector<uint32_t>meshIndices;
							for (auto iter = faceComponents.begin(); iter != faceComponents.end(); ++iter)
							{
								//see if the face has already been processed
								auto searchKey = faceIndexMap.find(*iter);
								if (searchKey != faceIndexMap.end())
								{
									//we have already processed this vertex
									std::cout << "Processing repeat face data: " << (*iter) << std::endl;
								}
								else
								{
									OBJVertex vertex = processFaceData(*iter, vertexData, normalData, textureData);
									meshData.push_back(vertex);
									uint32_t index = ((uint32_t)meshData.size() - 1);
									faceIndexMap[*iter] = index;
									faceIndices.push_back(index);
								}
							}
							//now that all triplets have been processed process indexbuffer
							for (int i = 1; i < faceIndices.size() - 1; i++)
							{
								meshIndices.push_back(faceIndices[0]);
								meshIndices.push_back(faceIndices[i]);
								meshIndices.push_back(faceIndices[(size_t)i +1]);

							}
						}
						
					}
				}
			}
			std::cout << "Processed " << vertexData.size() << " vertices in OBJ File" << std::endl;
			vertexData.clear();
			normalData.clear();
			textureData.clear();

			std::cout << "Processed " << meshData.size() << " vertices in OBJ Mesh Data" << std::endl;
		}
		else
		{
			std::cout << "File contains no data, closing file" << std::endl;
		}
		file.close();

	}
	else
	{
		std::cout << "Unable to open file: " << filename << std::endl;
	}
	return EXIT_SUCCESS;
}
