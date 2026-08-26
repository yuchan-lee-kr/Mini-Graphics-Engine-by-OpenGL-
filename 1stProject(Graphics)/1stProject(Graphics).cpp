#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cmath>
#include <cfloat>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Shader.h> //Shader header
#include <mathoperation.h> //Custom math header

using namespace std;
using namespace Mmt;

Mathmethod math;

int SCR_WIDTH = 1920; int SCR_HEIGHT = 1080;

Mathmethod::Vec3 position;
Mathmethod::Vec3 color;
float scaleFactor = 0;

Mathmethod::Vec3 SceneCenter;
Mathmethod::Vec3 SceneSize;
Mathmethod::Vec3 telePos;
Mathmethod::Vec3 CameraEye = Mathmethod::Vec3(0.0f, 5.0f, 150.0f);
Mathmethod::Vec3 CameraTarget = Mathmethod::Vec3(0.0f, 0.0f, -1.0f);
Mathmethod::Vec3 CameraUp= Mathmethod::Vec3(0.0f, 1.0f, 0.0f);
Mathmethod::Vec3 MaxVertex = Mathmethod::Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
Mathmethod::Vec3 MinVertex = Mathmethod::Vec3(FLT_MAX, FLT_MAX, FLT_MAX);

float movespeed = 0.3f;
float lightColor[3] = { 1.0f, 1.0f, 1.0f };
float Elevation = 45.0f;
float Azimuth = 30.0f;
float Ambient = 0.2f;
float Normal = 0.5f;
float Shadow = 1.0f;
float yaw = -90.0f; 
float pitch = 0.0f; 
float BackgroundColor[4] = { 0.45f, 0.65f, 0.85f, 1.0f };
bool isWalkMode = false;      
float humanEyeHeight = 0;
bool isDragging = false;
bool Tapkeypressedlast = false;
bool firstMouse;
double lastX = 0.0, lastY = 0.0;
float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationSpeed = 0.5f;
float fov = 90.0f;
float SceneRadius;
int currentMode = 0;

struct Material
{
	Mathmethod::Vec3 ambientColor = Mathmethod::Vec3(0.1f, 0.1f, 0.1f);
	Mathmethod::Vec3 diffuseColor = Mathmethod::Vec3(0.8f, 0.8f, 0.8f);
	Mathmethod::Vec3 specularColor = Mathmethod::Vec3(0.2f, 0.2f, 0.2f);
	float Shininess = 32.0f;
	float SpecularStrength = 1.0f;
};
struct SubMesh 
{
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int vertexCount = 0;
	unsigned int textureID = 0;
	unsigned int normalmapID = 0;
	bool hasNormalmap = false;
	Material material;
};
vector<SubMesh> g_meshes;
vector<unsigned int> g_materialTextures;
vector<unsigned int> g_materialNormalTextures;  
vector<bool> g_materialHasNormalMap;

enum class SelectedItem
{
	Model,
	DirectionalLight,
	Camera
};
SelectedItem selectedItem = SelectedItem::Model;
void CameraModeChange(GLFWwindow* window,int newmode)
{
	currentMode = newmode;
	if (currentMode == 1)
	{
		telePos = Mathmethod::Vec3(SceneCenter.x, MinVertex.y + SceneRadius * 0.2f, SceneCenter.z);
		CameraEye = telePos;
		fov = 60.0f;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
void UIDesign()
{
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 10.0f;
	style.ChildRounding = 8.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 6.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.18f, 0.22f, 1.00f);
}
void UIset(GLFWwindow* window,float* lightColor,float& elevation,float& azimuth,float* backgroundColor,Mathmethod::Mat4 viewMat) //ImGui Interface set
{
	UIDesign();
	ImGuiIO& io = ImGui::GetIO();
	float screenW = io.DisplaySize.x;
	float screenH = io.DisplaySize.y;
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenW * 0.16f, screenH * 0.75f), ImGuiCond_Always);
	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::Selectable("Model",selectedItem == SelectedItem::Model))
			selectedItem = SelectedItem::Model;
		if (ImGui::Selectable("Directional Light",selectedItem == SelectedItem::DirectionalLight))
			selectedItem = SelectedItem::DirectionalLight;
		if (ImGui::Selectable("Main Camera", selectedItem == SelectedItem::Camera))
			selectedItem = SelectedItem::Camera;
	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(screenW * 0.85f, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenW * 0.15f, screenH * 0.75f), ImGuiCond_Always);
	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);
	{
		switch (selectedItem)
		{
		case SelectedItem::Model:
			if (ImGui::CollapsingHeader("Model Options", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Total SubMesh: %d", g_meshes.size());
				ImGui::SliderFloat("NormalStrength", &Normal, 0.0f, 5.0f);
			}
			break;

		case SelectedItem::DirectionalLight:
			if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
			{

				ImGui::SliderFloat("Elevation", &elevation, 0.0f, 90.0f);
				ImGui::SliderFloat("Azimuth", &azimuth, 0.0f, 360.0f);
				ImGui::SliderFloat("AmbientStrength", &Ambient, 0.0f, 1.0f);
				ImGui::SliderFloat("ShadowStrength", &Shadow, 0.0f, 1.0f);
				ImGui::ColorEdit3("Light Color", lightColor);
				ImGui::ColorEdit3("Background Color", backgroundColor);
			}
			break;
		case SelectedItem::Camera:
			if (ImGui::CollapsingHeader("Camera Controller", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const char* modes[] = { "Free Fly Mode", "Human Walk Mode" };
				if (ImGui::Combo("Mode", &currentMode, modes, IM_ARRAYSIZE(modes)))
				{

				}
				ImGui::Text("Press Tab to switch modes.");
				ImGui::Separator();
				float pos[3] = { CameraEye.x, CameraEye.y, CameraEye.z };
				if (ImGui::DragFloat3("Camera Pos", pos, 0.1f))
				{
					CameraEye.x = pos[0];
					CameraEye.y = pos[1];
					CameraEye.z = pos[2];
				}
				ImGui::SliderFloat("FOV", &fov, 30.0f, 90.0f);
				ImGui::SliderFloat("Move Speed", &movespeed, 0.05f, 5.0f);
			}
			break;
		}
	}
	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(0, screenH * 0.75f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(screenW, screenH * 0.25f), ImGuiCond_Always);
	ImGui::Begin("Console / Statistics", nullptr, ImGuiWindowFlags_NoCollapse);
	{
		ImGui::Text("FPS: %.1f (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
		ImGui::Text("Total Draw Calls: %d", g_meshes.size());
		ImGui::Separator();

		if (ImGui::TreeNode("View Matrix"))
		{
			ImGui::Text("[ %.2f, %.2f, %.2f, %.2f ]", viewMat.col[0].x, viewMat.col[0].y, viewMat.col[0].z, viewMat.col[0].w);
			ImGui::Text("[ %.2f, %.2f, %.2f, %.2f ]", viewMat.col[1].x, viewMat.col[1].y, viewMat.col[1].z, viewMat.col[1].w);
			ImGui::Text("[ %.2f, %.2f, %.2f, %.2f ]", viewMat.col[2].x, viewMat.col[2].y, viewMat.col[2].z, viewMat.col[2].w);
			ImGui::Text("[ %.2f, %.2f, %.2f, %.2f ]", viewMat.col[3].x, viewMat.col[3].y, viewMat.col[3].z, viewMat.col[3].w);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
int loadTexture(char const* path)
{
	unsigned int texID;
	glGenTextures(1, &texID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1)      format = GL_RED;
		else if (nrComponents == 3) format = GL_RGB;
		else if (nrComponents == 4) format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
		std::cout << path << " Load Success! (" << width << "x" << height << ")" << std::endl;
	}
	else
	{
		std::cout << "Load failed: " << path << std::endl;
		stbi_image_free(data);
		return 0;
	}
	return texID;
}
void ProcessMaterials(const aiScene* scene, const string& modelPath )
{
	g_materialTextures.clear();
	g_materialNormalTextures.clear();
	g_materialHasNormalMap.clear();
	string directory;
	size_t lastSlash = modelPath.find_last_of("/\\");
	if (lastSlash != string::npos) {
		directory = modelPath.substr(0, lastSlash);
	}
	else {
		directory = "";
	}
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		unsigned int texID = 0; 
		unsigned int normalID = 0;
		bool hasnormal = false;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			string rawFileName = str.C_Str();
			size_t lastSpace = rawFileName.find_last_of(" \t");
			if (lastSpace != string::npos)
			{
				rawFileName = rawFileName.substr(lastSpace + 1);
			}
			string texturePath;
			if (!directory.empty()) {
				texturePath = directory + "/" + str.C_Str();
			}
			else {
				texturePath = rawFileName;
			}
			if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0) texID = loadTexture(texturePath.c_str());
			cout << "material " << i << " texture linked: " << texturePath << std::endl;
			string NormalFileName;
			size_t dot = rawFileName.find_last_of('.');
			if (dot != std::string::npos)
			{
				NormalFileName = rawFileName.substr(0, dot) + "_normal" + rawFileName.substr(dot);
			}
			std::string normalPath;
			if (!directory.empty()) normalPath = directory + "/" + NormalFileName;
			else normalPath = NormalFileName;
			normalID = loadTexture(normalPath.c_str());
			if (normalID != 0) hasnormal = true;
		}
		g_materialTextures.push_back(texID);
		g_materialNormalTextures.push_back(normalID);
		g_materialHasNormalMap.push_back(hasnormal);
	}
}
void ProcessMesh(aiMesh* mesh,const aiScene* scene)
{

	vector<float> vertexData;
	Mathmethod::Vec3 min = Mathmethod::Vec3(99999.0f, 99999.0f, 99999.0f);
	Mathmethod::Vec3 max = Mathmethod::Vec3(0.0f, 0.0f, 0.0f);
	vector<Mathmethod::Vec3> accumulatedTangents(mesh->mNumVertices, Mathmethod::Vec3(0.0f, 0.0f, 0.0f));

	for (unsigned int f = 0; f < mesh->mNumFaces; f++)
	{
		aiFace face = mesh->mFaces[f];
		unsigned int idx0 = face.mIndices[0];
		unsigned int idx1 = face.mIndices[1];
		unsigned int idx2 = face.mIndices[2];

		Mathmethod::Vec3 tangent(0.0f,0.0f,0.0f);
		if (mesh->mTextureCoords[0]) 
		{
			Mathmethod::Vec3 v0(mesh->mVertices[idx0].x, mesh->mVertices[idx0].y, mesh->mVertices[idx0].z);
			Mathmethod::Vec3 v1(mesh->mVertices[idx1].x, mesh->mVertices[idx1].y, mesh->mVertices[idx1].z);
			Mathmethod::Vec3 v2(mesh->mVertices[idx2].x, mesh->mVertices[idx2].y, mesh->mVertices[idx2].z);
			Mathmethod::Vec3 edge1 = v1 - v0;
			Mathmethod::Vec3 edge2 = v2 - v0;
			Mathmethod::Vec2 deltaUV1(mesh->mTextureCoords[0][idx1].x - mesh->mTextureCoords[0][idx0].x, mesh->mTextureCoords[0][idx1].y - mesh->mTextureCoords[0][idx0].y);
			Mathmethod::Vec2 deltaUV2(mesh->mTextureCoords[0][idx2].x - mesh->mTextureCoords[0][idx0].x, mesh->mTextureCoords[0][idx2].y - mesh->mTextureCoords[0][idx0].y);
			float denom = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
			if (abs(denom) < 1e-8f) continue;
			float ff = 1.0f / denom;
			tangent.x = ff * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = ff * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = ff * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		}
		accumulatedTangents[idx0] = accumulatedTangents[idx0] + tangent;
		accumulatedTangents[idx1] = accumulatedTangents[idx1] + tangent;
		accumulatedTangents[idx2] = accumulatedTangents[idx2] + tangent;
	}
	for (unsigned int f = 0; f < mesh->mNumFaces; f++)
	{
		aiFace face = mesh->mFaces[f];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			unsigned int i = face.mIndices[j];
			vertexData.push_back(mesh->mVertices[i].x);
			vertexData.push_back(mesh->mVertices[i].y);
			vertexData.push_back(mesh->mVertices[i].z);
			min.x = std::min(min.x, mesh->mVertices[i].x);
			min.y = std::min(min.y, mesh->mVertices[i].y);
			min.z = std::min(min.z, mesh->mVertices[i].z);
			max.x = std::max(max.x, mesh->mVertices[i].x);
			max.y = std::max(max.y, mesh->mVertices[i].y);
			max.z = std::max(max.z, mesh->mVertices[i].z);
			if (mesh->HasNormals())
			{
				vertexData.push_back(mesh->mNormals[i].x);
				vertexData.push_back(mesh->mNormals[i].y);
				vertexData.push_back(mesh->mNormals[i].z);
			}
			else
			{
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
			}
			if (mesh->mTextureCoords[0])
			{
				vertexData.push_back(mesh->mTextureCoords[0][i].x);
				vertexData.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
			}
			Mathmethod::Vec3 finalTangent = accumulatedTangents[i].Normalize();
			if (finalTangent.Length() > 1e-8f)finalTangent = finalTangent.Normalize();
			else
			{
				Mathmethod::Vec3 n(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

				Mathmethod::Vec3 helper = abs(n.y) < 0.999f ? Mathmethod::Vec3(0.0f, 1.0f, 0.0f) : Mathmethod::Vec3(1.0f, 0.0f, 0.0f);
				finalTangent = helper.Cross(n).Normalize();
			}
			vertexData.push_back(finalTangent.x);
			vertexData.push_back(finalTangent.y);
			vertexData.push_back(finalTangent.z);
		}
	}
	MinVertex.x = std::min(min.x, MinVertex.x);
	MinVertex.y = std::min(min.y, MinVertex.y);
	MinVertex.z = std::min(min.z, MinVertex.z);
	MaxVertex.x = std::max(max.x, MaxVertex.x);
	MaxVertex.y = std::max(max.y, MaxVertex.y);
	MaxVertex.z = std::max(max.z, MaxVertex.z);
	SceneCenter = (MinVertex + MaxVertex) * 0.5f;
	SceneSize = MaxVertex - MinVertex;
	SceneRadius = SceneSize.Length() * 0.5f;
	CameraEye = SceneCenter + Mathmethod::Vec3(0.0f, SceneRadius * 0.15f, SceneRadius * 1.5f);
	CameraTarget = (SceneCenter - CameraEye).Normalize();
	SubMesh subMesh;
	subMesh.vertexCount = vertexData.size() / 11;
	glGenVertexArrays(1, &subMesh.VAO);
	glGenBuffers(1, &subMesh.VBO);
	glBindVertexArray(subMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, subMesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
	GLsizei stride = 11 * sizeof(float);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
	unsigned int matIndex = mesh->mMaterialIndex;
	if (matIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		cout << mesh->mMaterialIndex << endl;
		cout << scene->mNumMaterials << endl;
		aiColor3D ambient(0.8f, 0.8f, 0.8f);
		aiColor3D diffuse(1.0f, 1.0f, 1.0f);
		aiColor3D specular(0.2f, 0.2f, 0.2f);
		float shininess = 32.0f;
		float shininessStrength = 1.0f;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient)== AI_SUCCESS)
		{
			subMesh.material.ambientColor = Mathmethod::Vec3(ambient.r, ambient.g, ambient.b);
		}
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
		{
			if (diffuse.r <= 0.001f && diffuse.g <= 0.001f && diffuse.b <= 0.001f) subMesh.material.diffuseColor = Mathmethod::Vec3(1.0f , 1.0f, 1.0f);
			else subMesh.material.diffuseColor = Mathmethod::Vec3(diffuse.r, diffuse.g, diffuse.b);
		}
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular)== AI_SUCCESS)
		{
			subMesh.material.specularColor = Mathmethod::Vec3(specular.r, specular.g, specular.b);
		}
		if(material->Get(AI_MATKEY_SHININESS, shininess)== AI_SUCCESS)
		{
			subMesh.material.Shininess = shininess;
		}
		if (material->Get(AI_MATKEY_SHININESS_STRENGTH,shininessStrength) == AI_SUCCESS)
		{
			subMesh.material.SpecularStrength = shininessStrength;
		}
	}
	if (matIndex < g_materialTextures.size()) subMesh.textureID = g_materialTextures[matIndex];
	if (matIndex < g_materialNormalTextures.size()) subMesh.normalmapID = g_materialNormalTextures[matIndex];
	if (matIndex < g_materialHasNormalMap.size()) subMesh.hasNormalmap = g_materialHasNormalMap[matIndex];
	glBindVertexArray(0);
	g_meshes.push_back(subMesh);
}
void ProcessNodes(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		ProcessMesh(mesh, scene);
	}
}
void loadModelAssimp(const string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |          
		aiProcess_FlipUVs |               
		aiProcess_GenNormals        
	);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << " Assimp Error: " << importer.GetErrorString() << std::endl;
		return;
	}
	ProcessMaterials(scene, path);
	ProcessNodes(scene->mRootNode, scene);
}
void KeyboardInput(GLFWwindow* window)
{
	Mathmethod::Vec3 Verticalmove = Mathmethod::Vec3(0.0f, 0.3f, 0.0f);
	Mathmethod::Vec3 zHorizontalmove;
	zHorizontalmove.x = CameraTarget.x;
	zHorizontalmove.y = 0.0f;
	zHorizontalmove.z = CameraTarget.z;
	zHorizontalmove.Normalize();
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && currentMode != 1) CameraEye += (Verticalmove * movespeed);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && currentMode != 1) CameraEye -= (Verticalmove * movespeed);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && currentMode != 1) CameraEye += (CameraTarget * movespeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && currentMode != 1) CameraEye -= (CameraTarget * movespeed);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && currentMode == 1) CameraEye += (zHorizontalmove * movespeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && currentMode == 1) CameraEye -= (zHorizontalmove * movespeed);
	Mathmethod::Vec3 CameraRight = CameraTarget.Cross(CameraUp).Normalize();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) CameraEye -= (CameraRight * movespeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) CameraEye += (CameraRight * movespeed);
	bool Tappressed = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
	if (Tappressed && !Tapkeypressedlast)
	{
		currentMode = (currentMode == 0) ? 1 : 0;
		CameraModeChange(window,currentMode);
	}
	Tapkeypressedlast = Tappressed;
	if (isWalkMode)
	{
		humanEyeHeight = MinVertex.y;
		CameraEye.y = humanEyeHeight;
	}
}
void MouseRotate(GLFWwindow* window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		if (!isDragging)
		{
			lastX = mouseX;
			lastY = mouseY;
			isDragging = true;
		}
		double deltaX = mouseX - lastX;
		double deltaY = mouseY - lastY;
		lastX = mouseX;
		lastY = mouseY;
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			rotationX += (float)deltaX * rotationSpeed;
			rotationY += (float)deltaY * rotationSpeed;
		}
	}
	else
	{
		isDragging = false;
	}
}
void Scroll_callback(GLFWwindow* window,double xoffset,double yoffset)
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse) return;
	fov -= (float)yoffset * 2.0f;
	if (fov < 1.0f) fov = 1.0f;
	if (fov > 90.0f) fov = 90.0f;
}

const float PI = 3.14159265358979323846f;
inline float toRadians(float degrees) 
{
	return degrees * (PI / 180.0f);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (currentMode == 1)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;
		float sensitivity = 0.1f; 
		xoffset *= sensitivity; yoffset *= sensitivity;
		yaw += xoffset; pitch += yoffset; 
		if (pitch > 89.0f)  pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f; 
		Mathmethod::Vec3 front;
		front.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
		front.y = sin(toRadians(pitch));
		front.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
		CameraTarget = front.Normalize();
	}
}
void Shadercheck(int &vertexS,int &fragmentS,int &shaderP)
{
	GLint success;
	char infoLog[1024];

	glGetShaderiv(vertexS, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexS, 1024, nullptr, infoLog);
		std::cout << "VERTEX ERROR:\n"
			<< infoLog << '\n';
	}

	glGetShaderiv(fragmentS, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentS, 1024, nullptr, infoLog);
		std::cout << "FRAGMENT ERROR:\n"
			<< infoLog << '\n';
	}

	glGetProgramiv(shaderP, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderP, 1024, nullptr, infoLog);
		std::cout << "PROGRAM LINK ERROR:\n"
			<< infoLog << '\n';
	}
}
int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Yuchan", NULL, NULL);
	glfwMakeContextCurrent(window); 
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetScrollCallback(window, Scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	int shaderP = glCreateProgram();
	int vertexS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexS, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexS);
	glAttachShader(shaderP, vertexS);
	int fragmentS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentS, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentS);
	glAttachShader(shaderP, fragmentS);
	glLinkProgram(shaderP);
	glDeleteShader(vertexS); 	glDeleteShader(fragmentS);
	glEnable(GL_DEPTH_TEST);
	IMGUI_CHECKVERSION();
	unsigned int depthMapFBO = 0;
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int Shadow_Width = 1024, Shadow_Height = 1024;
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, Shadow_Width, Shadow_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int LightShaderVertex = glCreateShader(GL_VERTEX_SHADER);
	int LightShaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
	int LightShaderP = glCreateProgram();
	glShaderSource(LightShaderVertex, 1, &LightvertexShaderSource, NULL);
	glCompileShader(LightShaderVertex);
	glAttachShader(LightShaderP, LightShaderVertex);
	glShaderSource(LightShaderFragment, 1, &LightfragmentShaderSource, NULL);
	glCompileShader(LightShaderFragment);
	glAttachShader(LightShaderP, LightShaderFragment);
	glLinkProgram(LightShaderP);
	glDeleteShader(LightShaderVertex);  glDeleteShader(LightShaderFragment);
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 1.5f;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	ImGui_ImplGlfw_InitForOpenGL(window, true); 
	ImGui_ImplOpenGL3_Init("#version 330");  
	ImFont* font = io.Fonts->AddFontFromFileTTF(
		"Fonts/NotoSansKR-VariableFont_wght.ttf",
		18.0f);
	io.FontDefault = font;
	loadModelAssimp("bmw2.obj");
	Mathmethod::Vec3 corners[8] =
	{
		Mathmethod::Vec3(MinVertex.x,MinVertex.y,MinVertex.z),
		Mathmethod::Vec3(MaxVertex.x,MinVertex.y,MinVertex.z),
		Mathmethod::Vec3(MinVertex.x,MaxVertex.y,MinVertex.z),
		Mathmethod::Vec3(MaxVertex.x,MaxVertex.y,MinVertex.z),
		Mathmethod::Vec3(MinVertex.x,MinVertex.y,MaxVertex.z),
		Mathmethod::Vec3(MaxVertex.x,MinVertex.y,MaxVertex.z),
		Mathmethod::Vec3(MinVertex.x,MaxVertex.y,MaxVertex.z),
		Mathmethod::Vec3(MaxVertex.x,MaxVertex.y,MaxVertex.z),
	};
	Shadercheck(vertexS, fragmentS, shaderP);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Mathmethod::Mat4 RotateX = RotateX.Rotate(toRadians(rotationX), Mathmethod::Vec3(0.0f, -1.0f, 0.0f));
		Mathmethod::Mat4 RotateY = RotateY.Rotate(toRadians(rotationY), Mathmethod::Vec3(1.0f, 0.0f, 0.0f));
		Mathmethod::Mat4 model = RotateX * RotateY;
		Mathmethod::Vec3 lightDir;
		float elepi = toRadians(Elevation);
		float azipi = toRadians(Azimuth);
		lightDir.x = cos(elepi) * sin(azipi);
		lightDir.y = -sin(elepi);
		lightDir.z = cos(elepi) * cos(azipi);
		lightDir = lightDir.Normalize();
		Mathmethod::Vec3 lightPos = SceneCenter - lightDir * (SceneRadius * 2.0f);
		Mathmethod::Mat4 lightView = lightView.Lookat(lightPos, SceneCenter, Mathmethod::Vec3(0.0f, 1.0f, 0.0f));
		float minX = FLT_MAX;
		float maxX = -FLT_MAX;
		float minY = FLT_MAX;
		float maxY = -FLT_MAX;
		float minZ = FLT_MAX;
		float maxZ = -FLT_MAX;
		for (int i = 0; i < 8; i++)
		{
			Mathmethod::Vec4 p = lightView * Mathmethod::Vec4(corners[i].x, corners[i].y, corners[i].z, 1.0f);
			minX = std::min(minX, p.x);
			maxX = std::max(maxX, p.x);
			minY = std::min(minY, p.y);
			maxY = std::max(maxY, p.y);
			minZ = std::min(minZ, p.z);
			maxZ = std::max(maxZ, p.z);
		}
		float width = maxX - minX;
		float height = maxY - minY;
		float padding = 0.1f;
		minX -= width * padding;
		maxX += width * padding;
		minY -= height * padding;
		maxY += height * padding;
		float near = -maxZ;
		float far = -minZ;
		float depthRange = far - near;
		float depthPadding = depthRange * 0.1f;
		near = std::max(0.01f, near - depthPadding);
		far += depthPadding;
		Mathmethod::Mat4 lightProjection = lightProjection.OrthoGraphic(minX, maxX, minY, maxY, near, far);
		Mathmethod::Mat4 lightSpace = lightProjection * lightView;
		glUseProgram(LightShaderP);
		glViewport(0, 0, Shadow_Width, Shadow_Height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniformMatrix4fv(glGetUniformLocation(LightShaderP, "lightSpace"), 1, GL_FALSE, &lightSpace.col[0].x);
		glUniformMatrix4fv(glGetUniformLocation(LightShaderP, "model"), 1, GL_FALSE, &model.col[0].x);
		for (const auto& mesh : g_meshes)
		{
			glBindVertexArray(mesh.VAO);
			glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
			glBindVertexArray(0);
		}
		glUseProgram(shaderP);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); 
		glClearColor(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2], BackgroundColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Mathmethod::Mat4 view = view.Lookat(CameraEye, CameraEye + CameraTarget, CameraUp);
		Mathmethod::Mat4 projection = projection.Perspective(toRadians(fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f,1000.0f);
		Mathmethod::Mat3 normalMatrix = model.toMat3().Inverse().Transpose(); 
		glUniformMatrix4fv(glGetUniformLocation(shaderP, "model"), 1, GL_FALSE, &model.col[0].x);
		glUniformMatrix4fv(glGetUniformLocation(shaderP, "view"), 1, GL_FALSE, &view.col[0].x);
		glUniformMatrix4fv(glGetUniformLocation(shaderP, "projection"), 1, GL_FALSE, &projection.col[0].x);
		glUniformMatrix3fv(glGetUniformLocation(shaderP, "normalMatrix"), 1, GL_FALSE, &normalMatrix.col[0].x);
		glUniformMatrix4fv(glGetUniformLocation(shaderP, "lightSpace"), 1, GL_FALSE, &lightSpace.col[0].x);
		glUniform1i(glGetUniformLocation(shaderP, "texture_diffuse"), 0);
		glUniform1i(glGetUniformLocation(shaderP, "texture_normal"), 2);
		glUniform1f(glGetUniformLocation(shaderP, "AmbientStrength"), Ambient);
		glUniform1f(glGetUniformLocation(shaderP, "NormalStrength"), Normal);
		glUniform1f(glGetUniformLocation(shaderP, "ShadowStrength"), Shadow);
		glUniform3f(glGetUniformLocation(shaderP, "viewPos"), CameraEye.x, CameraEye.y, CameraEye.z);
		glUniform3fv(glGetUniformLocation(shaderP, "lightColor"), 1, lightColor);
		glUniform3f(glGetUniformLocation(shaderP, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
		UIset(window, lightColor, Elevation, Azimuth, BackgroundColor, view);
		for (const auto& mesh : g_meshes)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh.textureID);
			glUniform1i(glGetUniformLocation(shaderP, "hasNormal"), mesh.hasNormalmap);
			glUniform3f(glGetUniformLocation(shaderP, "material.diffuseColor"), mesh.material.diffuseColor.x, mesh.material.diffuseColor.y, mesh.material.diffuseColor.z);
			glUniform3f(glGetUniformLocation(shaderP, "material.specularColor"), mesh.material.specularColor.x, mesh.material.specularColor.y, mesh.material.specularColor.z);
			glUniform1f(glGetUniformLocation(shaderP, "material.shininess"), mesh.material.Shininess);
			glUniform1f(glGetUniformLocation(shaderP, "material.specularStrength"), mesh.material.SpecularStrength);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap); 
			glUniform1i(glGetUniformLocation(shaderP, "shadowMap"), 1);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mesh.normalmapID);
			glBindVertexArray(mesh.VAO);
			glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
			glBindVertexArray(0);
		}
		KeyboardInput(window);
		MouseRotate(window);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

