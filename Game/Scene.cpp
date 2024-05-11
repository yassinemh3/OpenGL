#include "Scene.h"
#include"../SceneElements/Transform.h"
#include <AssetManager.h>
//camera position
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Scene::Scene(OpenGLWindow * window) :
        m_window(window)
{
    assert(window != nullptr);
}

Scene::~Scene()
{}

bool Scene::init()
{
    try
    {
        //Load shader
        m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
        m_shader = m_assets.getShaderProgram("shader");
        m_shader->use();

        //create VAO
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        //Create vbo
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), &cubeVert, GL_STATIC_DRAW);

        //Describe data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        //Create ibo
        GLuint iboID;
        glGenBuffers(1, &iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), &cubeInd, GL_STATIC_DRAW);

        //Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        std::cout << "Scene initialization done\n";
        return true;
    }
    catch (std::exception& ex)
    {
        throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
    }
}

void Scene::render(float dt)
{
    //render buffer
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //delete background
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //Bind shader
    m_shader->use();

    //Bind VAO
    glBindVertexArray(vaoID);

    //camera view
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    float fov = 45.0f;
    float aspectRatio = 1280.0 / 720.0;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    m_shader->use();
    m_shader->setUniform("view", view, false);
    m_shader->setUniform("projection", projection, false);

    //Draw
    Scene::DrawRobot(dt);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(1.0);

    //Unbind VAO
    glBindVertexArray(0);

}

void Scene::update(float dt)
{

}

OpenGLWindow * Scene::getWindow()
{
    return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{
    if(key == Key::W)
    {
        cameraPos.x -= 0.1;
    }
    if(key == Key::S)
    {
        cameraPos.x += 0.1;
    }
    if (key == Key::A)
    {
        cameraPos.z -= 0.1;
    }
    if (key == Key::D)
    {
        cameraPos.z += 0.1;
    }
    if(key == Key::Q)
    {
        cameraPos.y -= 0.1;
    }
    if(key == Key::E)
    {
        cameraPos.y += 0.1;
    }
}

void Scene::onMouseMove(MousePosition mouseposition)
{

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{
    if(button == MouseButton::Left)
    {
        cameraTarget.x += 0.1;
    }
    if(button == MouseButton::Right)
    {
        cameraTarget.x -= 0.1;
    }
}

void Scene::onMouseScroll(double xscroll, double yscroll)
{
    cameraPos.x += xscroll;
    cameraPos.y += yscroll;
}

void Scene::onFrameBufferResize(int width, int height)
{

}
void Scene::shutdown()
{

}

void Scene::DrawRobot(float dt)
{

    float jointLeftY = 0.5f;
    float jointRightY = 0.5f;
    float yRotation = 0.261799f;
    // Update rotation
    static float angle = 0.0f;
    static bool isIncreasing = true;
    if(isIncreasing == true){
        angle += dt*0.7f;
        if(angle > 0.872665f){
            isIncreasing = false;
        }
    }
    else{
        angle -= dt*0.7f;
        if(angle < -0.872665f){
            isIncreasing = true;
        }
    }

    //Draw Body
    glPushMatrix();
    Transform* bodyTrans = new Transform();
    bodyTrans->scale(glm::vec3(0.3, 1, 0.25));
    bodyTrans->translate(glm::vec3(0, 0.2, 0));
    bodyTrans->rotate(glm::vec3(0, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", bodyTrans->getMatrix(), false);

    //Draw Head
    glPushMatrix();
    Transform* headTrans = new Transform();
    headTrans->translate(glm::vec3(0, 1, 0));
    headTrans->rotate(glm::vec3(0, 0, 0));
    headTrans->scale(glm::vec3(0.25, 0.5, 0.25));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", headTrans->getMatrix(), false);
    glPopMatrix();


    //Draw Left upper Arm
    glPushMatrix();
    Transform* leftUpperTrans = new Transform();
    leftUpperTrans->scale(glm::vec3(0.1, 0.375, 0.085));
    leftUpperTrans->translate(glm::vec3(-0.25, 0.35, 0));
    //leftUpperTrans->rotate(glm::vec3(0, 0, 0));
    leftUpperTrans->rotateAroundPoint(glm::vec3(-0.35, jointLeftY, 0), glm::vec3(-angle, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", leftUpperTrans->getMatrix(), false);
    glPopMatrix();

    //Draw left lower Arm
    Transform* leftLowerTrans = new Transform();
    leftLowerTrans->scale(glm::vec3(0.1, 0.375, 0.085));
    leftLowerTrans->translate(glm::vec3(-0.25, -0.1, 0));
    //leftLowerTrans->rotate(glm::vec3(0, 0, 0));
    leftLowerTrans->rotateAroundPoint(glm::vec3(0.35, jointLeftY, 0), glm::vec3(-angle, 0, 0));
    //leftLowerTrans->rotateAroundPoint(glm::vec3(-0.35, 0.075, 0.2), glm::vec3(angle, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", leftLowerTrans->getMatrix(), false);

    //Draw Right upper Arm
    Transform* rightUpperTrans = new Transform();
    rightUpperTrans->scale(glm::vec3(0.1, 0.375, 0.085));
    rightUpperTrans->translate(glm::vec3(0.25, 0.35, 0));
    rightUpperTrans->rotateAroundPoint(glm::vec3(0.35, jointRightY, 0), glm::vec3(angle, 0, 0));
    //rightUpperTrans->rotate(glm::vec3(0, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", rightUpperTrans->getMatrix(), false);

    //Draw Right lower Arm
    Transform* rightLowerTrans = new Transform();
    rightLowerTrans->scale(glm::vec3(0.1, 0.375, 0.085));
    rightLowerTrans->translate(glm::vec3(0.25, -0.1, 0));
    rightLowerTrans->rotateAroundPoint(glm::vec3(0.35, jointRightY, 0), glm::vec3(angle, 0, 0));
    //rightLowerTrans->rotate(glm::vec3(0, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", rightLowerTrans->getMatrix(), false);


    //Draw left foot
    Transform* leftFootTrans = new Transform();
    leftFootTrans->scale(glm::vec3(0.15, 0.6, 0.125));
    leftFootTrans->translate(glm::vec3(-0.1, -0.65, 0));
    leftFootTrans->rotateAroundPoint(glm::vec3(-0.1, -0.34, 0), glm::vec3(angle, 0, 0));
    //leftFootTrans->rotate(glm::vec3(0, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", leftFootTrans->getMatrix(), false);

    //Draw Right foot
    Transform* rightFootTrans = new Transform();
    rightFootTrans->setScale(glm::vec3(0.15, 0.6, 0.125));
    rightFootTrans->translate(glm::vec3(0.1, -0.65, 0));
    rightFootTrans->rotateAroundPoint(glm::vec3(-0.1, -0.34, 0), glm::vec3(-angle, 0, 0));
    //rightFootTrans->rotate(glm::vec3(0, 0, 0));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_shader->setUniform("model", rightFootTrans->getMatrix(), false);

    glPopMatrix();


}