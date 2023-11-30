#include "Mesh.h"
#include <Windows.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <exception>
#include <string>
#include <sstream>
//#include <locale>
//#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"
Mesh::Mesh(const wchar_t* full_path) : Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;
	//std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	//bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str());

#pragma region Converting wstring to string
	std::string converted_str;
	std::wstring wstr = full_path;
	for (wchar_t UTF16 : wstr)
	{
		char UTF8 = static_cast<char>(UTF16);

		converted_str.push_back(UTF8);
	}
#pragma endregion


	std::string mtldir = converted_str.substr(0, converted_str.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, converted_str.c_str(), mtldir.c_str());

	try
	{
		if (!err.empty())
		{
			throw std::exception("Mesh Not created Succesfully");
		}
		if (!res)
		{
			throw std::exception("Mesh was not created successfully");
		}
		std::vector<VertexMesh> list_vertices;//Stores the Vertex Layout
		std::vector<unsigned int> list_indices;//Stores the list of vertices indices, to help map out which modle show be draw first

		size_t size_vertex_index_lists = 0;//Is that some of the size of all the indeis list of all the shapes
		
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			size_vertex_index_lists += shapes[s].mesh.indices.size();//Finding out how many Indices there are in all
		}

		list_vertices.reserve(size_vertex_index_lists);//Expaneding the campacity of the vector, so more memory dosn't have to be allocated and moved some where else in memory every time a new element is added.
		list_indices.reserve(size_vertex_index_lists);//Expaneding the campacity of the vector, so more memory dosn't have to be allocated and moved some where else in memory every time a new element is added.

		m_material_slots.resize(materials.size());//Resize the vectory with new memory allocated to sotre information of the materials in the file

		size_t index_global_offset = 0;//

		for (size_t m = 0; m < materials.size(); m++)
		{
			m_material_slots[m].start_index = index_global_offset;
			m_material_slots[m].material_id = m;

			for (size_t s = 0; s < shapes.size(); s++)
			{
				size_t index_offset = 0;


				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					if (shapes[s].mesh.material_ids[f] != m) continue;

					unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

					for (unsigned char v = 0; v < num_face_verts; v++)
					{
						tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

						tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
						tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
						tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

						tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

						tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
						tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
						tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

						VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));
						list_vertices.push_back(vertex);

						list_indices.push_back((unsigned int)index_global_offset + v);
					}

					index_offset += num_face_verts;
					index_global_offset += num_face_verts;
				}
			}

			m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;

		}

		void* shader_byte_code = nullptr;
		size_t size_shader = 0;
		GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
		m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh),
			(UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
		m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
	}
	catch (const std::exception& e)
	{
		std::wstringstream msg;
		msg << L"Exception: " << e.what();
		OutputDebugStringW(msg.str().c_str());
	}


}


Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::getMaterialSlot(UINT slot)
{
	if (slot >= m_material_slots.size()) return MaterialSlot();

	return m_material_slots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return m_material_slots.size();
}