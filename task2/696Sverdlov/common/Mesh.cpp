#include "Mesh.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

MeshPtr makeSphere(float radius, unsigned int N)
{
    unsigned int M = N / 2;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    for (unsigned int i = 0; i < M; i++)
    {
        float theta = (float)glm::pi<float>() * i / M;
        float theta1 = (float)glm::pi<float>() * (i + 1) / M;

        for (unsigned int j = 0; j < N; j++)
        {
            float phi = 2.0f * (float)glm::pi<float>() * j / N + (float)glm::pi<float>();
            float phi1 = 2.0f * (float)glm::pi<float>() * (j + 1) / N + (float)glm::pi<float>();

            //Первый треугольник, образующий квад
            vertices.push_back(glm::vec3(cos(phi) * sin(theta) * radius, sin(phi) * sin(theta) * radius, cos(theta) * radius));
            vertices.push_back(glm::vec3(cos(phi1) * sin(theta1) * radius, sin(phi1) * sin(theta1) * radius, cos(theta1) * radius));
            vertices.push_back(glm::vec3(cos(phi1) * sin(theta) * radius, sin(phi1) * sin(theta) * radius, cos(theta) * radius));

            normals.push_back(glm::vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta)));
            normals.push_back(glm::vec3(cos(phi1) * sin(theta1), sin(phi1) * sin(theta1), cos(theta1)));
            normals.push_back(glm::vec3(cos(phi1) * sin(theta), sin(phi1) * sin(theta), cos(theta)));

            texcoords.push_back(glm::vec2((float)j / N, 1.0f - (float)i / M));
            texcoords.push_back(glm::vec2((float)(j + 1) / N, 1.0f - (float)(i + 1) / M));
            texcoords.push_back(glm::vec2((float)(j + 1) / N, 1.0f - (float)i / M));

            //Второй треугольник, образующий квад
            vertices.push_back(glm::vec3(cos(phi) * sin(theta) * radius, sin(phi) * sin(theta) * radius, cos(theta) * radius));
            vertices.push_back(glm::vec3(cos(phi) * sin(theta1) * radius, sin(phi) * sin(theta1) * radius, cos(theta1) * radius));
            vertices.push_back(glm::vec3(cos(phi1) * sin(theta1) * radius, sin(phi1) * sin(theta1) * radius, cos(theta1) * radius));

            normals.push_back(glm::vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta)));
            normals.push_back(glm::vec3(cos(phi) * sin(theta1), sin(phi) * sin(theta1), cos(theta1)));
            normals.push_back(glm::vec3(cos(phi1) * sin(theta1), sin(phi1) * sin(theta1), cos(theta1)));

            texcoords.push_back(glm::vec2((float)j / N, 1.0f - (float)i / M));
            texcoords.push_back(glm::vec2((float)j / N, 1.0f - (float)(i + 1) / M));
            texcoords.push_back(glm::vec2((float)(j + 1) / N, 1.0f - (float)(i + 1) / M));
        }
    }

    //----------------------------------------

    DataBufferPtr buf0 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf0->setData(vertices.size() * sizeof(float) * 3, vertices.data());

    DataBufferPtr buf1 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf1->setData(normals.size() * sizeof(float) * 3, normals.data());

    DataBufferPtr buf2 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf2->setData(texcoords.size() * sizeof(float) * 2, texcoords.data());

    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0);
    mesh->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0, buf1);
    mesh->setAttribute(2, 2, GL_FLOAT, GL_FALSE, 0, 0, buf2);
    mesh->setPrimitiveType(GL_TRIANGLES);
    mesh->setVertexCount(vertices.size());

    std::cout << "Sphere is created with " << vertices.size() << " vertices\n";

    return mesh;
}

MeshPtr makeCube(float size)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    //front 1
    vertices.push_back(glm::vec3(size, -size, size));
    vertices.push_back(glm::vec3(size, size, -size));
    vertices.push_back(glm::vec3(size, size, size));

    normals.push_back(glm::vec3(1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(1.0, 0.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));
    texcoords.push_back(glm::vec2(1.0, 1.0));

    //front 2
    vertices.push_back(glm::vec3(size, -size, size));
    vertices.push_back(glm::vec3(size, -size, -size));
    vertices.push_back(glm::vec3(size, size, -size));

    normals.push_back(glm::vec3(1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(1.0, 0.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(0.0, 0.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));

    //left 1
    vertices.push_back(glm::vec3(-size, -size, size));
    vertices.push_back(glm::vec3(size, -size, -size));
    vertices.push_back(glm::vec3(size, -size, size));

    normals.push_back(glm::vec3(0.0, -1.0, 0.0));
    normals.push_back(glm::vec3(0.0, -1.0, 0.0));
    normals.push_back(glm::vec3(0.0, -1.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));
    texcoords.push_back(glm::vec2(1.0, 1.0));

    //left 2
    vertices.push_back(glm::vec3(-size, -size, size));
    vertices.push_back(glm::vec3(-size, -size, -size));
    vertices.push_back(glm::vec3(size, -size, -size));

    normals.push_back(glm::vec3(0.0, -1.0, 0.0));
    normals.push_back(glm::vec3(0.0, -1.0, 0.0));
    normals.push_back(glm::vec3(0.0, -1.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(0.0, 0.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));

    //top 1
    vertices.push_back(glm::vec3(-size, size, size));
    vertices.push_back(glm::vec3(size, -size, size));
    vertices.push_back(glm::vec3(size, size, size));

    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));
    texcoords.push_back(glm::vec2(1.0, 1.0));

    //top 2
    vertices.push_back(glm::vec3(-size, size, size));
    vertices.push_back(glm::vec3(-size, -size, size));
    vertices.push_back(glm::vec3(size, -size, size));

    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(0.0, 0.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));

    //back 1
    vertices.push_back(glm::vec3(-size, -size, size));
    vertices.push_back(glm::vec3(-size, size, size));
    vertices.push_back(glm::vec3(-size, size, -size));

    normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(-1.0, 0.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));

    //back 2
    vertices.push_back(glm::vec3(-size, -size, size));
    vertices.push_back(glm::vec3(-size, size, -size));
    vertices.push_back(glm::vec3(-size, -size, -size));

    normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
    normals.push_back(glm::vec3(-1.0, 0.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));
    texcoords.push_back(glm::vec2(0.0, 0.0));

    //right 1
    vertices.push_back(glm::vec3(-size, size, size));
    vertices.push_back(glm::vec3(size, size, size));
    vertices.push_back(glm::vec3(size, size, -size));

    normals.push_back(glm::vec3(0.0, 1.0, 0.0));
    normals.push_back(glm::vec3(0.0, 1.0, 0.0));
    normals.push_back(glm::vec3(0.0, 1.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));

    //right 2
    vertices.push_back(glm::vec3(-size, size, size));
    vertices.push_back(glm::vec3(+size, size, -size));
    vertices.push_back(glm::vec3(-size, size, -size));

    normals.push_back(glm::vec3(0.0, 1.0, 0.0));
    normals.push_back(glm::vec3(0.0, 1.0, 0.0));
    normals.push_back(glm::vec3(0.0, 1.0, 0.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));
    texcoords.push_back(glm::vec2(0.0, 0.0));

    //bottom 1
    vertices.push_back(glm::vec3(-size, size, -size));
    vertices.push_back(glm::vec3(size, size, -size));
    vertices.push_back(glm::vec3(size, -size, -size));

    normals.push_back(glm::vec3(0.0, 0.0, -1.0));
    normals.push_back(glm::vec3(0.0, 0.0, -1.0));
    normals.push_back(glm::vec3(0.0, 0.0, -1.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));

    //bottom 2
    vertices.push_back(glm::vec3(-size, size, -size));
    vertices.push_back(glm::vec3(size, -size, -size));
    vertices.push_back(glm::vec3(-size, -size, -size));

    normals.push_back(glm::vec3(0.0, 0.0, -1.0));
    normals.push_back(glm::vec3(0.0, 0.0, -1.0));
    normals.push_back(glm::vec3(0.0, 0.0, -1.0));

    texcoords.push_back(glm::vec2(0.0, 1.0));
    texcoords.push_back(glm::vec2(1.0, 0.0));
    texcoords.push_back(glm::vec2(0.0, 0.0));

    //----------------------------------------

    DataBufferPtr buf0 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf0->setData(vertices.size() * sizeof(float) * 3, vertices.data());

    DataBufferPtr buf1 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf1->setData(normals.size() * sizeof(float) * 3, normals.data());

    DataBufferPtr buf2 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf2->setData(texcoords.size() * sizeof(float) * 2, texcoords.data());

    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0);
    mesh->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0, buf1);
    mesh->setAttribute(2, 2, GL_FLOAT, GL_FALSE, 0, 0, buf2);
    mesh->setPrimitiveType(GL_TRIANGLES);
    mesh->setVertexCount(vertices.size());

    std::cout << "Cube is created with " << vertices.size() << " vertices\n";

    return mesh;
}

MeshPtr makeScreenAlignedQuad()
{
    std::vector<glm::vec3> vertices;

    //front 1
    vertices.push_back(glm::vec3(-1.0, 1.0, 0.0));
    vertices.push_back(glm::vec3(1.0, -1.0, 0.0));
    vertices.push_back(glm::vec3(1.0, 1.0, 0.0));

    //front 2
    vertices.push_back(glm::vec3(-1.0, 1.0, 0.0));
    vertices.push_back(glm::vec3(-1.0, -1.0, 0.0));
    vertices.push_back(glm::vec3(1.0, -1.0, 0.0));

    //----------------------------------------

    DataBufferPtr buf0 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf0->setData(vertices.size() * sizeof(float) * 3, vertices.data());

    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0);
    mesh->setPrimitiveType(GL_TRIANGLES);
    mesh->setVertexCount(vertices.size());

    return mesh;
}

MeshPtr makeGroundPlane(float size, float numTiles)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    //front 1
    vertices.push_back(glm::vec3(-size, size, 0.0));
    vertices.push_back(glm::vec3(size, -size, 0.0));
    vertices.push_back(glm::vec3(size, size, 0.0));

    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));

    texcoords.push_back(glm::vec2(-numTiles, numTiles));
    texcoords.push_back(glm::vec2(numTiles, -numTiles));
    texcoords.push_back(glm::vec2(numTiles, numTiles));

    //front 2
    vertices.push_back(glm::vec3(-size, size, 0.0));
    vertices.push_back(glm::vec3(-size, -size, 0.0));
    vertices.push_back(glm::vec3(size, -size, 0.0));

    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    normals.push_back(glm::vec3(0.0, 0.0, 1.0));

    texcoords.push_back(glm::vec2(-numTiles, numTiles));
    texcoords.push_back(glm::vec2(-numTiles, -numTiles));
    texcoords.push_back(glm::vec2(numTiles, -numTiles));

    //----------------------------------------

    DataBufferPtr buf0 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf0->setData(vertices.size() * sizeof(float) * 3, vertices.data());

    DataBufferPtr buf1 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf1->setData(normals.size() * sizeof(float) * 3, normals.data());

    DataBufferPtr buf2 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf2->setData(texcoords.size() * sizeof(float) * 2, texcoords.data());

    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0);
    mesh->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0, buf1);
    mesh->setAttribute(2, 2, GL_FLOAT, GL_FALSE, 0, 0, buf2);
    mesh->setPrimitiveType(GL_TRIANGLES);
    mesh->setVertexCount(vertices.size());

    return mesh;
}

MeshPtr loadFromAIMesh(const aiMesh &assimpMesh) {
	if (!assimpMesh.HasPositions())
	{
		std::cerr << "This demo does not support meshes without positions\n";
		return std::make_shared<Mesh>();
	}

	if (!assimpMesh.HasNormals())
	{
		std::cerr << "This demo does not support meshes without normals\n";
		return std::make_shared<Mesh>();
	}

	if (!assimpMesh.HasTextureCoords(0))
	{
		std::cerr << "Mesh with no texture coords for texture unit 0 can cause strange visualization\n";
	}

	DataBufferPtr buf0 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
	buf0->setData(assimpMesh.mNumVertices * sizeof(float) * 3, assimpMesh.mVertices);

	DataBufferPtr buf1 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
	buf1->setData(assimpMesh.mNumVertices * sizeof(float) * 3, assimpMesh.mNormals);


	MeshPtr mesh = std::make_shared<Mesh>();
	mesh->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0);
	mesh->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0, buf1);
	mesh->setPrimitiveType(GL_TRIANGLES);
	mesh->setVertexCount(assimpMesh.mNumVertices);

	// Upload indices.
	DataBufferPtr indexBuf = std::make_shared<DataBuffer>(GL_ELEMENT_ARRAY_BUFFER);

	std::vector<unsigned int> indices(3 * assimpMesh.mNumFaces);
	for (int i = 0; i < assimpMesh.mNumFaces; i++) {
		const aiFace &face = assimpMesh.mFaces[i];
		if (face.mNumIndices != 3)
			continue;
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
	indexBuf->setData(indices.size() * sizeof(unsigned int), indices.data());
	mesh->setIndices(indices.size(), indexBuf);

	// Optional upload texcoords0.
	if (assimpMesh.HasTextureCoords(0)) {
		DataBufferPtr buf2 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
		buf2->setData(assimpMesh.mNumVertices * sizeof(float) * 3, assimpMesh.mTextureCoords[0]);
		mesh->setAttribute(2, 3, GL_FLOAT, GL_FALSE, 0, 0, buf2);
	}

	std::cout << "Mesh " << assimpMesh.mName.data << " is loaded with " << assimpMesh.mNumVertices << " vertices\n";
	return mesh;
}

MeshPtr loadFromFile(const std::string& filename, int meshIndex)
{
    aiEnableVerboseLogging(true);
    auto stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, nullptr);
    aiAttachLogStream(&stream);

    const struct aiScene* assimpScene = aiImportFile(filename.c_str(),
		    aiProcessPreset_TargetRealtime_MaxQuality);

    if (!assimpScene)
    {
        std::cerr << aiGetErrorString() << std::endl;
        return std::make_shared<Mesh>();
    }

    if (assimpScene->mNumMeshes <= meshIndex)
    {
        std::cerr << "Wrong mesh index " << meshIndex << " for file " << filename << std::endl;
        return std::make_shared<Mesh>();
    }

    auto mesh = loadFromAIMesh(*assimpScene->mMeshes[meshIndex]);

    aiReleaseImport(assimpScene);
    aiDetachAllLogStreams();

    return mesh;
}