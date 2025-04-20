#include <meshInformation.h>

void loadModelFromFile(const std::streamsize buffer_size, const char* file_path, MeshData& result)
{

	auto start_time = std::chrono::high_resolution_clock::now();

	std::ifstream file(file_path, std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Failed to open file : " << file_path << std::endl;
		return;
	}

	char* buffer = new char[buffer_size];
	std::vector<Vector> positionsList;
	std::vector<Vector> normalsList;
	std::vector<Vector> texturesList;
	std::vector<Face> faces;

	positionsList.reserve(1000000);
	normalsList.reserve(10000);
	texturesList.reserve(10000);

	faces.reserve(100000);

	result.vertices.reserve(1000000);

    std::string leftover;  // to store incomplete lines across chunks

    while (file.read(buffer, buffer_size) || file.gcount() > 0)
    {
        std::streamsize bytesRead = file.gcount();

        // append the new chunk to any leftover partial line from before
        std::string chunk = leftover + std::string(buffer, bytesRead);

        size_t lineStart = 0;
        size_t newlinePos;

        while ((newlinePos = chunk.find('\n', lineStart)) != std::string::npos)
        {
            std::string line = chunk.substr(lineStart, newlinePos - lineStart);

            // Process the line here:
            if (line.rfind("v ", 0) == 0) {
                std::istringstream iss(line.substr(2));
                float x, y, z;
                iss >> x >> y >> z;
                positionsList.emplace_back(x, y, z);
            }
            else if (line.rfind("vn ", 0) == 0) {
                std::istringstream iss(line.substr(3));
                float x, y, z;
                iss >> x >> y >> z;
                normalsList.emplace_back(x, y, z);
            }
            else if (line.rfind("vt ", 0) == 0) {
                std::istringstream iss(line.substr(3));
                float x, y, z;
                iss >> x >> y >> z;
                texturesList.emplace_back(x, y, z);
            }
            else if (line.rfind("f ", 0) == 0) {
                Face face;
                std::istringstream lineStream(line.substr(2));
                std::string token;
                int idx = 0;

                while (std::getline(lineStream, token, ' ') && idx < 3) {
                    std::istringstream tokenStream(token);
                    std::string vertex, texture, normal;

                    if (std::getline(tokenStream, vertex, '/') &&
                        std::getline(tokenStream, texture, '/') &&
                        std::getline(tokenStream, normal, '/'))
                    {
                        face.vIndices[idx] = std::stoi(vertex);
                        face.vtIndices[idx] = texture.empty() ? 0 : std::stoi(texture);
                        face.nIndices[idx] = normal.empty() ? 0 : std::stoi(normal);
                    }
                    else
                    {
                        face.vIndices[idx] = std::stoi(vertex);
                        face.vtIndices[idx] = 0;
                        face.nIndices[idx] = 0;
                    }
                    idx++;
                }
                faces.emplace_back(face);
            }

            lineStart = newlinePos + 1;  // advance to start of next line
        }

        // Handle leftover partial line (no newline found)
        leftover = chunk.substr(lineStart);
    }

	for (size_t fc = 0; fc < faces.size(); fc++)
	{
		for (size_t idx = 0; idx < 3; idx++)
		{
			size_t vi = faces[fc].vIndices[idx];
			size_t ni = faces[fc].nIndices[idx];
			size_t ti = faces[fc].vtIndices[idx];

			if (vi == 0 || vi > positionsList.size()) continue;
			if (ni == 0 || ni > normalsList.size()) continue;
			if (ti == 0 || ti > texturesList.size()) continue;

			Vertex vert;
			vert.position = positionsList[vi - 1];
			vert.normal = normalsList[ni - 1];
			vert.texture = texturesList[ti - 1];
			result.vertices.emplace_back(vert);
		}
	}


	delete[] buffer;
	positionsList.clear();

	result.indicesSize = result.vertices.size();

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end_time - start_time;
	std::cout << "[ Model Load Finished ]" << std::endl;
	std::cout << "Elapsed time " << duration.count() << "s" << std::endl;
	std::cout << "Vertex Count: " << result.vertices.size() << std::endl;

	

}

