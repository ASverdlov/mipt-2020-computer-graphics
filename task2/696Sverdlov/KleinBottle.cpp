#include <Application.hpp>
#include <LightInfo.hpp>
#include <Mesh.hpp>
#include <ShaderProgram.hpp>
#include <Texture.hpp>

#include <iostream>
#include <sstream>
#include <vector>

// http://virtualmathmuseum.org/Surface/klein_bottle/klein_bottle.html
//
//  x = (aa + cos(v / 2) * sin(u) - sin(v / 2) * sin(2 * u)) * cos(v)
//  y = (aa + cos(v / 2) * sin(u) - sin(v / 2) * sin(2 * u)) * sin(v)
//  z = sin(v / 2) * sin(u) + cos(v / 2) * sin(2 * u)
//
//   0.0 < u < 2 * π,  0 < v < 2 * π,  aa = 3
//
glm::vec3 kleinPosition(float u, float v, float aa, float scaler = 0.2f) {
    double x = (aa + glm::cos(v * 0.5f) * glm::sin(u) - glm::sin(v * 0.5f) * glm::sin(2 * u)) * glm::cos(v);
    double y = (aa + glm::cos(v * 0.5f) * glm::sin(u) - glm::sin(v * 0.5f) * glm::sin(2 * u)) * glm::sin(v);
    double z = glm::sin(0.5f * v) * glm::sin(u) + glm::cos(0.5f * v) * glm::sin(2 * u);

    return glm::vec3(x * scaler, y * scaler, z * scaler);
}

// http://virtualmathmuseum.org/Surface/moebius_strip/moebius_strip.html
//
//  x = aa * (cos(v) + u * cos(v / 2) * cos(v))
//  y = aa * (sin(v) + u * cos(v / 2) * sin(v))
//  z = aa * u * sin(v / 2)
//
//
glm::vec3 moebiusPosition(float u, float v, float aa, float scaler = 0.2f) {
    double x = aa * (glm::cos(v) + u * glm::cos(v * 0.5f) * glm::cos(v));
    double y = aa * (glm::sin(v) + u * glm::cos(v * 0.5f) * glm::sin(v));
    double z = aa * u * sin(v * 0.5f);
    return glm::vec3(x * scaler, y * scaler, z * scaler);
}

struct SurfaceFillinParams {
    std::function<glm::vec3(float u, float v, float aa, float scaler)> f;

    float aa;
    float umin;
    float umax;
    float vmin;
    float vmax;
};

void fillInSurfaceAttributes(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texcoords, SurfaceFillinParams params) {
    const float ucnt = 1000;
    const float vcnt = 1000;

    const float aa = params.aa;
    const float umin = params.umin;
    const float umax = params.umax;
    const float vmin = params.vmin;
    const float vmax = params.vmax;

    const float udelta = (umax - umin) / ucnt;
    const float vdelta = (vmax - vmin) / vcnt;

    const float txdelta = 1.0f / ucnt;
    const float tydelta = 1.0f / vcnt;

    for (int ustep = 0; ustep < ucnt; ++ustep) {
        for (int vstep = 0; vstep < vcnt; ++vstep) {
            float u = umin + ustep * udelta;
            float v = vmin + vstep * vdelta;

            // square with (u, v) in upper-left angle
            auto aaPoint = params.f(u, v, aa, 0.5f);
            auto abPoint = params.f(u, v + vdelta, aa, 0.5f);
            auto baPoint = params.f(u + udelta, v, aa, 0.5f);
            auto bbPoint = params.f(u + udelta, v + vdelta, aa, 0.5f);

            float tx = txdelta * ustep;
            float ty = tydelta * vstep;

            // upper-left triangle
            vertices.push_back(aaPoint);
            vertices.push_back(abPoint);
            vertices.push_back(baPoint);

            normals.push_back(glm::normalize(glm::cross(baPoint - aaPoint, abPoint - aaPoint)));
            normals.push_back(glm::normalize(glm::cross(aaPoint - abPoint, bbPoint - abPoint)));
            normals.push_back(glm::normalize(glm::cross(bbPoint - baPoint, aaPoint - baPoint)));

            texcoords.emplace_back(tx, ty);
            texcoords.emplace_back(tx, ty + tydelta);
            texcoords.emplace_back(tx + txdelta, ty);

            // lower-right triangle
            vertices.push_back(bbPoint);
            vertices.push_back(baPoint);
            vertices.push_back(abPoint);

            normals.push_back(glm::normalize(glm::cross(abPoint - bbPoint, baPoint - bbPoint)));
            normals.push_back(glm::normalize(glm::cross(bbPoint - baPoint, aaPoint - baPoint)));
            normals.push_back(glm::normalize(glm::cross(aaPoint - abPoint, bbPoint - abPoint)));

            texcoords.emplace_back(tx + txdelta, ty + tydelta);
            texcoords.emplace_back(tx + txdelta, ty);
            texcoords.emplace_back(tx, ty + tydelta);
        }
    }
}

MeshPtr makeKleinBottle(float size)
{
    std::vector<glm::vec3> vertices1;
    std::vector<glm::vec3> normals1;
    std::vector<glm::vec3> vertices2;
    std::vector<glm::vec3> normals2;
    std::vector<glm::vec2> texcoords;

    auto kleinParams = SurfaceFillinParams{
        kleinPosition,
        3.0f,                       // aa
        0.0f,                     // umin
        2.0f * glm::pi<float>(),  // umax
        0.0f,                     // vmin
        2.0f * glm::pi<float>(),  // vmax
    };

    auto moebiusParams = SurfaceFillinParams{
            moebiusPosition,
            3.0f,                        // aa
            -0.4f,                     // umin
            0.4f,                     // umax
            0.0f,                     // vmin
            2.0f * glm::pi<float>(), // vmax
    };

    fillInSurfaceAttributes(vertices1, normals1, texcoords, kleinParams);
    fillInSurfaceAttributes(vertices2, normals2, texcoords, moebiusParams);

    //----------------------------------------

    DataBufferPtr buf0 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf0->setData(vertices1.size() * sizeof(float) * 3, vertices1.data());

    DataBufferPtr buf1 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf1->setData(normals1.size() * sizeof(float) * 3, normals1.data());

    DataBufferPtr buf2 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf2->setData(vertices2.size() * sizeof(float) * 3, vertices2.data());

    DataBufferPtr buf3 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf3->setData(normals2.size() * sizeof(float) * 3, normals2.data());

    DataBufferPtr buf4 = std::make_shared<DataBuffer>(GL_ARRAY_BUFFER);
    buf4->setData(texcoords.size() * sizeof(float) * 2, texcoords.data());

    MeshPtr mesh = std::make_shared<Mesh>();
    mesh->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0);
    mesh->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 0, 0, buf1);
    mesh->setAttribute(2, 3, GL_FLOAT, GL_FALSE, 0, 0, buf2);
    mesh->setAttribute(3, 3, GL_FLOAT, GL_FALSE, 0, 0, buf3);
    mesh->setAttribute(4, 2, GL_FLOAT, GL_FALSE, 0, 0, buf4);
    mesh->setPrimitiveType(GL_TRIANGLES);
    mesh->setVertexCount(vertices1.size());

    std::cout << "Klein bottle is created with " << vertices1.size() << " vertices\n";

    return mesh;
}


class SampleApplication : public Application
{
public:
    float veinPulse = 0.02f;
    float morphismSpeed = 0.01f;

    uint64_t frames;

    MeshPtr _kleinBottle;
    MeshPtr _backgroundCube;

    MeshPtr _marker; //Меш - маркер для источника света

    //Идентификатор шейдерной программы
    ShaderProgramPtr _commonShader;
    ShaderProgramPtr _markerShader;
    ShaderProgramPtr _skyboxShader;

    //Переменные для управления положением одного источника света
    float _lr = 10.0f;
    float _phi = 2.65f;
    float _theta = 0.48f;

    LightInfo _light;

    TexturePtr _veinsTex;
    TexturePtr _snakeSkinTex;
    TexturePtr _cubeTex;

    GLuint _sampler;
    GLuint _cubeTexSampler;

    CameraInfo _camera2;

    void makeScene() override
    {
        Application::makeScene();

        //=========================================================
        //Создание и загрузка мешей		

        _kleinBottle = makeKleinBottle(0.5f);
        _kleinBottle->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

        _marker = makeSphere(0.1f);

        _backgroundCube = makeCube(10.0f);

        //=========================================================
        //Инициализация шейдеров

        _commonShader = std::make_shared<ShaderProgram>("696SverdlovData2/shaders/klein.vert", "696SverdlovData2/shaders/klein.frag");
        _markerShader = std::make_shared<ShaderProgram>("696SverdlovData2/shaders/marker.vert", "696SverdlovData2/shaders/marker.frag");
        _skyboxShader = std::make_shared<ShaderProgram>("696SverdlovData2/shaders/skybox.vert", "696SverdlovData2/shaders/skybox.frag");

        //=========================================================
        //Инициализация значений переменных освщения
        _light.position = glm::vec3(glm::cos(_phi) * glm::cos(_theta), glm::sin(_phi) * glm::cos(_theta), glm::sin(_theta)) * _lr;
        _light.ambient = glm::vec3(0.2, 0.2, 0.2);
        _light.diffuse = glm::vec3(0.8, 0.8, 0.8);
        _light.specular = glm::vec3(1.0, 1.0, 1.0);

        //=========================================================
        //Загрузка и создание текстур
        _snakeSkinTex = loadTexture("696SverdlovData2/images/snake-skin-2.jpg");
        _veinsTex = loadTexture("696SverdlovData2/images/veins.png");
        _cubeTex = loadCubeTexture("696SverdlovData2/images/cube");

        //=========================================================
        //Инициализация сэмплера, объекта, который хранит параметры чтения из текстуры
        glGenSamplers(1, &_sampler);
        glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenSamplers(1, &_cubeTexSampler);
        glSamplerParameteri(_cubeTexSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(_cubeTexSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(_cubeTexSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(_cubeTexSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(_cubeTexSampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        //=========================================================
        //Инициализация 2й виртуальной камеры

        _camera2.viewMatrix = glm::lookAt(glm::vec3(-5.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f));

        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        _camera2.projMatrix = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.f);
    }

    void updateGUI() override
    {
        Application::updateGUI();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
        if (ImGui::Begin("MIPT OpenGL Sample", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("FPS %.1f", ImGui::GetIO().Framerate);

            if (ImGui::CollapsingHeader("Light"))
            {
                ImGui::ColorEdit3("ambient", glm::value_ptr(_light.ambient));
                ImGui::ColorEdit3("diffuse", glm::value_ptr(_light.diffuse));
                ImGui::ColorEdit3("specular", glm::value_ptr(_light.specular));

                ImGui::SliderFloat("radius", &_lr, 0.1f, 10.0f);
                ImGui::SliderFloat("phi", &_phi, 0.0f, 2.0f * glm::pi<float>());
                ImGui::SliderFloat("theta", &_theta, 0.0f, glm::pi<float>());
            }

            if (ImGui::CollapsingHeader("Klein Bottle"))
            {
                ImGui::SliderFloat("vein pulse", &veinPulse, 0.0f, 0.1f);
                ImGui::SliderFloat("morphism speed", &morphismSpeed, 0.0f, 0.1f);
            }

        }
        ImGui::End();
    }

    void draw() override
    {
        ++frames; // для анимации

        //Получаем текущие размеры экрана и выставлям вьюпорт
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);

        glViewport(0, 0, width, height);

        //Очищаем буферы цвета и глубины от результатов рендеринга предыдущего кадра
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawSceneWithCamera(_camera);
    }

    void drawSceneWithCamera(const CameraInfo& camera)
    {
        //====== РИСУЕМ ФОН С КУБИЧЕСКОЙ ТЕКСТУРОЙ ======
        {
            _skyboxShader->use();

            glm::vec3 cameraPos = glm::vec3(glm::inverse(camera.viewMatrix)[3]); //Извлекаем из матрицы вида положение виртуальный камеры в мировой системе координат

            _skyboxShader->setVec3Uniform("cameraPos", cameraPos);
            _skyboxShader->setMat4Uniform("viewMatrix", camera.viewMatrix);
            _skyboxShader->setMat4Uniform("projectionMatrix", camera.projMatrix);

            //Для преобразования координат в текстурные координаты нужна специальная матрица
            glm::mat3 textureMatrix = glm::mat3(0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            _skyboxShader->setMat3Uniform("textureMatrix", textureMatrix);

            glActiveTexture(GL_TEXTURE0);  //текстурный юнит 0            
            glBindSampler(0, _cubeTexSampler);
            _cubeTex->bind();
            _skyboxShader->setIntUniform("cubeTex", 0);

            glDepthMask(GL_FALSE); //Отключаем запись в буфер глубины

            _backgroundCube->draw();

            glDepthMask(GL_TRUE); //Включаем обратно запись в буфер глубины
        }

        //====== РИСУЕМ ОСНОВНЫЕ ОБЪЕКТЫ СЦЕНЫ ======
        _commonShader->use();

        //Загружаем на видеокарту значения юниформ-переменных
        _commonShader->setMat4Uniform("viewMatrix", camera.viewMatrix);
        _commonShader->setMat4Uniform("projectionMatrix", camera.projMatrix);

        _light.position = glm::vec3(glm::cos(_phi) * glm::cos(_theta), glm::sin(_phi) * glm::cos(_theta), glm::sin(_theta)) * _lr;
        glm::vec3 lightPosCamSpace = glm::vec3(camera.viewMatrix * glm::vec4(_light.position, 1.0));

        _commonShader->setVec3Uniform("light.pos", lightPosCamSpace); //копируем положение уже в системе виртуальной камеры
        _commonShader->setVec3Uniform("light.La", _light.ambient);
        _commonShader->setVec3Uniform("light.Ld", _light.diffuse);
        _commonShader->setVec3Uniform("light.Ls", _light.specular);

        // enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);  //текстурный юнит 0        
        glBindSampler(0, _sampler);
        _veinsTex->bind();
        _commonShader->setIntUniform("diffuseTex", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindSampler(1, _sampler);
        _snakeSkinTex->bind();
        _commonShader->setIntUniform("snakeSkinTex", 1);

        //Загружаем на видеокарту матрицы модели мешей и запускаем отрисовку
        {
            _commonShader->setMat4Uniform("modelMatrix", _kleinBottle->modelMatrix());
            _commonShader->setMat3Uniform("normalToCameraMatrix", glm::transpose(glm::inverse(glm::mat3(camera.viewMatrix * _kleinBottle->modelMatrix()))));
            _commonShader->setFloatUniform("alphaScaler", veinAlphaForNow());
            _commonShader->setFloatUniform("morphismAlpha", morphismAlphaForNow());

            _kleinBottle->draw();
        }

        glDisable(GL_BLEND);

        //Рисуем маркеры для всех источников света		
        {
            _markerShader->use();

            _markerShader->setMat4Uniform("mvpMatrix", camera.projMatrix * camera.viewMatrix * glm::translate(glm::mat4(1.0f), _light.position));
            _markerShader->setVec4Uniform("color", glm::vec4(_light.diffuse, 1.0f));
            _marker->draw();
        }

        //Отсоединяем сэмплер и шейдерную программу
        glBindSampler(0, 0);
        glUseProgram(0);
    }

    float veinAlphaForNow() const {
        return 0.5f * glm::sin(veinPulse * frames) + 0.5f;
    }

    float morphismAlphaForNow() const {
        return 0.5f * glm::sin(morphismSpeed * frames) + 0.5f;
    }
};

int main()
{
    SampleApplication app;
    app.start();

    return 0;
}