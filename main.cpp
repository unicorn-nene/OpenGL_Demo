#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

#include "Skybox.h"

#pragma region gaboal_value

#pragma region ��ѧ����
const float toRadians = 3.14159265f / 180.0f;	///< 1 ���� = �� / 180
#pragma endregion

#pragma region shader_uniform���
GLuint uniformProjection{};						    ///< ͶӰ���� uniform λ��
GLuint uniformModel{};							    ///< ģ�;��� uniform λ��
GLuint uniformView{};							    ///< ��ͼ���� uniform λ��
GLuint uniformEyePosition{};					    ///< ���λ�� uniform λ��
GLuint uniformSpecularIntensity{};				    ///< ���淴��ǿ�� uniform λ��
GLuint uniformShininess{};						    ///< �߹�ϵ�� uniform λ��
GLuint uniformDirectionalLightTransform = 0;	    ///< ƽ�й�任���� uniform λ��
GLuint uniformOmniLightPos = 0;					    ///< ���Դλ�� uniform λ��
GLuint uniformFarPlane = 0;						    ///< Զ�ü�ƽ�� uniform λ��
#pragma endregion		

#pragma region Window and Scene
Window mainWindow{};							    ///< �����ڶ���
std::vector<Mesh*> meshList{};					    ///< Mesh����ָ�������б�(vector)

std::vector<Shader> shaderList{};				    ///< Shader �����б�
Shader directionalShadowShader{};				    ///< ƽ�й���Ӱ Shader
Shader omniShadowShader{};						    ///< ���Դ��Ӱ Shader

Camera camera{};								    ///< �������
#pragma endregion							  

#pragma region Texture and materia
Texture brickTexture{};							    ///< ש������
Texture dirtTexture{};							    ///< ��������
Texture plainTexture{};							    ///< ��ͨ����

Material shinyMaterial{};						    ///< ��������
Material dullMaterial{};						    ///< ��������
#pragma endregion	

#pragma region �ⲿ Model
Model xwing{};									    ///< x-wing ս�� Model
Model blackhawk{};								    ///< ��ӥֱ���� Model
#pragma endregion									 

#pragma region ��Դ
DirectionalLight mainLight{};					    ///< ��ƽ�й�(Դ)
PointLight pointLights[MAX_POINT_LIGHTS];		    ///< ���Դ����
SpotLight spotLights[MAX_SPOT_LIGHTS];			    ///< �۹������
#pragma endregion

#pragma region ��պ�
Skybox skybox;									    ///< ��պж���
#pragma endregion

#pragma region ���Դ����
unsigned int pointLightCount = 0;				    ///< ��ǰ���Դ����
unsigned int spotLightCount = 0;				    ///< ��ǰ�۹������
#pragma endregion

#pragma region ʱ��
GLfloat deltaTime = 0.0f;						    ///< ��ʱ��, ÿ֮֡��ʱ���
GLfloat lastTime = 0.0f;						    ///< ��һ֡ʱ��
#pragma endregion

#pragma region Model Status
GLfloat blackhawkAngle = 0.0f;					    ///< ��ӥֱ������ת�Ƕ�
#pragma endregion

#pragma region Shader Path
static const char* vShader = "Shaders/shader.vert"; ///< ������ɫ���ļ�·��
static const char* fShader = "Shaders/shader.frag"; ///< Ƭ����ɫ���ļ�·��
#pragma endregion

#pragma endregion

#pragma region pre-function

/// <summary>
/// Ϊ������������������������ƽ�����ߣ�ƽ������)
/// </summary>
/// <param name="indices">��������</param>
/// <param name="indiceCount">��������</param>
/// <param name="vertices">��������</param>
/// <param name="verticeCount">��������</param>
/// <param name="vLength">ÿ������� float����</param>
/// <param name="normalOffset">����(nx,ny,nz)�ڶ����ڲ���ƫ��</param>
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{	// ����ṹ: x, y, z,		u, v,		nx, ny, nz

	// ��ÿ�������μ����淨�߲��ۼӵ���������ķ��߷���
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		// v1 = p1 - p0, v2 = p2 - p0
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 1]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 1]);

		// �淨�� = cross(v1, v2)
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		// ������õ����淨���ۼӵ�ÿ������ķ���λ��
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	// ��һ��ÿһ������ķ���, ʹ���Ϊ��λ����
	for (size_t i = 0; i < verticeCount / vLength; ++i)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

/// <summary>
/// ���� Mesh ʵ��, ���ﴴ���������������һ������������ Mesh
/// </summary>
void CreateObjects() 
{
	// ���������������
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// ������Ķ�������
	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	// �ذ� Mesh ����������
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};
	// �ذ� mesh �Ķ�������
	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	// ����������ƽ������
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	// ���� Mesh ������ MeshList
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

/// <summary>
/// ���������ظ�����ɫ����������ɫ�� + ��Ӱ��ɫ����
/// </summary>
void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	// ƽ�й���Ӱ��ͼ shader (ֻ������)
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	// ���Դ ȫ����Ӱ��ͼ (��Ҫ���� Shader ��չ�� cubemap ����Ⱦ)
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

/// <summary>
/// �ѳ���������������Ƴ���,��������Ⱦ����Ӱ��Ⱦ���ֳ�������ͬ Shader �ֱ���ã�
/// </summary>
void RenderScene()
{
	glm::mat4 model(1.0f);

	// ��һ������(λ��, ����, ����, ��Ⱦ)
#pragma region objA
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();
#pragma endregion

	// �ڶ�������
#pragma region objB
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();
#pragma endregion

	// ����
#pragma region floor
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();
#pragma endregion

	// ���ز���Ⱦ x-wing ģ��(model)
#pragma region X-Wing
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	xwing.RenderModel();
#pragma endregion

	// ���ز���Ⱦ ��ӥֱ����
#pragma region Blackhwak
	// ����ֱ���� ���� Y����ת
	blackhawkAngle += 0.1f;
	if (blackhawkAngle > 360.0f)
	{
		blackhawkAngle = 0.1f;
	}

	// ���ز���Ⱦ
	model = glm::mat4(1.0f);
	model = glm::rotate(model, -blackhawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 0.0f));
	model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	blackhawk.RenderModel();
#pragma endregion
}

/// <summary>
/// ����/ƽ�й���Ӱ��ͼ����Ⱦͨ��(ֻд�����)
/// 1. �󶨷����ר�� shader
/// 2. �����ӿڵ� shadow map ��С
/// 3. �� shadow FBO ��Ϊ����Ŀ�겢������
/// 4. ���� light-space ����uniform��
/// 5. ��Ⱦ������ֻ��д����ȣ�
/// </summary>
/// <param name="light">��Ⱦ��ƽ�й����ָ��</param>
void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();			// ����ȾĿ���л�Ϊ ��Ӱ��ͼ�� FBO
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	auto lTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lTransform);

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// Ϊ����۹�����ȫ����Ӱ��ͼ��д�������������ͼ����
/// ��Ҫ�ѹ�Դλ���� farPlane �ȴ��� shader�����ṩ 6 �� light matrices��
/// </summary>
void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	// ��ȡ��Ҫ�� omni shadow shader ��д��� uniform λ����Ϣ
	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	//  �� 6 ��ͶӰ��ͼ������ shader��ÿ����Ӧ��Ƭ face
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// ʵ�����ճ�����Ⱦ:
/// ������Ⱦ���̣�
/// 1. �����ӿڻص����ڴ�С�������Ļ
/// 2. ������պУ����д����ʱ�رգ�
/// 3. ʹ���� shader ���Ƴ��������ø��� uniform ��� shadow map��
/// </summary>
/// <param name="viewMatrix">������� view ����</param>
/// <param name="projectionMatrix">ͶӰ����</param>
void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1920, 1080);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	// ���� �� shader
	shaderList[0].UseShader();

	// ��ȡ������ shader �е� uniformλ���Ա�����
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	// �ϴ�ͶӰ/��ͼ���� �� �۲���λ��
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	// �ѹ�Դ����д�� shader, ����ʹ�� textureUnit 3
	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

	// д��ƽ�й�Ĺ��ձ任���� shader
	auto lTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&lTransform);

	// �󶨷���� shadow map �� texture 2
	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	// ���������΢���������ֵ�Ͳ��spot light��λ�ã���ͬ������
	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();

	// ��Ⱦ�������������壩��lighting + shadow ���� shader �ڼ���
	RenderScene();
}
#pragma endregion


int main() 
{
	// ������������ʼ�� OpenGL/GLFW/GLEW
	mainWindow = Window(1920, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	// ���� mesh ���� �� shader
	CreateObjects();
	CreateShaders();
	
	// �������ʼ��
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.3f);

	// ��������
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	// ��ʼ�� Material
	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	// ���� model
	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	blackhawk = Model();
	blackhawk.LoadModel("Models/uh60.obj");

	// ƽ�й�
	mainLight = DirectionalLight(2048, 2048,
								1.0f, 0.53f, 0.3f, 
								0.1f, 0.9f,
								-10.0f, -12.0f, 18.5f);
	// ���Դ
	pointLights[0] = PointLight(1024, 1024,
								0.01f, 100.0f,
								0.0f, 0.0f, 1.0f,
								0.0f, 1.0f,
								1.0f, 2.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
								0.01f, 100.0f, 
								0.0f, 1.0f, 0.0f,
								0.0f, 1.0f,
								-4.0f, 3.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// �۹��
	spotLights[0] = SpotLight(1024, 1024,
						0.01f, 100.0f, 
						1.0f, 1.0f, 1.0f,
						0.0f, 2.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
						0.01f, 100.0f, 
						1.0f, 1.0f, 1.0f,
						0.0f, 1.0f,
						0.0f, -1.5f, 0.0f,
						-100.0f, -1.0f, 0.0f,
						1.0f, 0.0f, 0.0f,
						20.0f);
	spotLightCount++;

	// ��պ�
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	// �ֲ� uniform
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	// ����ͶӰ����
	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
		static_cast<float>(mainWindow.getBufferWidth() )/static_cast<float>( mainWindow.getBufferHeight()),
		0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = static_cast<float>(glfwGetTime()); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// �л��ֵ�Ͳ���� F ����
		if (mainWindow.getsKeys()[GLFW_KEY_F])
		{
			spotLights[0].Toggle();
			mainWindow.getsKeys()[GLFW_KEY_F] = false;
		}

		// ===================== ������Ӱ��ͼ ============================
		DirectionalShadowMapPass(&mainLight);

		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}

		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}

		// ========================== ������Ⱦ�׶� ===========================
		RenderPass(camera.calculateViewMatrix(), projection);

		// �������壬��ʾ
		mainWindow.swapBuffers();
	}

	return 0;
}