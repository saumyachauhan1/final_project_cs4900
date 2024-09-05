#include "finalproject.h"

#include "WorldList.h" 
#include "ManagerOpenGLState.h" 
#include "Axes.h"
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WONVPhysX.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "WOImGui.h"
#include "AftrImGuiIncludes.h"
#include "AftrGLRendererBase.h"
#include "Mat4.h"
#include "WOCar1970sBeater.h"
#include "WONVPhysX.h"
#include <WONVPhysX.h>
#include <irrKlang.h>
#include <iostream>
#include "winsock2.h"
#include "ws2tcpip.h"

#pragma comment(lib, "Ws2_32.lib")  // Link against Winsock library

using namespace Aftr;
using namespace irrklang;

GLViewNewModule* GLViewNewModule::New(const std::vector<std::string>& args)
{
    GLViewNewModule* glv = new GLViewNewModule();
    glv->init(Aftr::GRAVITY, Vector(0, 0, -1.0f), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE);
    glv->onCreate();
    return glv;
}

GLViewNewModule::GLViewNewModule() : GLView(args), isServer(false) // Default constructor, starts as client
{
}

bool GLViewNewModule::hasReachedEndpoint()
{
    // Basic collision detection: check if the distance between the player box and end point is within a threshold
    return (playerBox->getPosition() - endPos).magnitude() < 0.5;
}

void GLViewNewModule::onCreate()
{
    // Determine if this instance is the server or client based on args
    if (!args.empty() && args[0] == "server")
    {
        isServer = true;
        std::cout << "Running as server..." << std::endl;
    }
    else
    {
        isServer = false;
        std::cout << "Running as client..." << std::endl;
    }

    if (this->pe != NULL)
    {
        this->pe->setGravityNormalizedVector(Vector(0, 0, -1.0f));
        this->pe->setGravityScalar(Aftr::GRAVITY);
    }
    this->setActorChaseType(STANDARDEZNAV);

    // Set start and end positions
    //startPos = Vector(1, 10, 1); // Point A
    //endPos = Vector(1, -10, 1);    // Point B
    auto lookDirection = this->getCamera()->getLookDirection().normalizeMe();
    startPos = (lookDirection * 20) + this->getCamera()->getPosition();
    endPos = startPos;
    endPos.y = endPos.y - 20;
    std::cout << endPos << std::endl;
    std::cout << startPos << std::endl;

    // Create the start and end points
    startPoint = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl", Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    startPoint->setPosition(startPos);
    startPoint->setLabel("Start Point");
    worldLst->push_back(startPoint);

    endPoint = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl", Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    endPoint->setPosition(endPos);
    endPoint->setLabel("End Point");
    worldLst->push_back(endPoint);

    // Create the player box
    playerBox = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl", Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    playerBox->setPosition(startPos);  // Start at point A
    playerBox->setLabel("Player Box");
    worldLst->push_back(playerBox);
}

GLViewNewModule::~GLViewNewModule()
{
}

void GLViewNewModule::updateWorld()
{
    GLView::updateWorld();

    if (isServer)
    {
        // Server logic: Check if the player box has reached the endpoint (B)
        if (hasReachedEndpoint())
        {
            std::cout << "Congratulations! You have reached Point B!" << std::endl;
            playerBox->setPosition(startPos); // Reset the box to start position

            // Send the updated position to the client instance
            sendNetMsgSynchronousTCP("resetPosition", playerBox->getPosition(), playerBox->getDisplayMatrix());
        }

        // Receive updates from the client instance if available
        std::string action;
        Vector position;
        Mat4 displayMat;
        receiveNetMsgSynchronousTCP(action, position, displayMat);
        if (action == "updatePosition" || action == "resetPosition")
        {
            playerBox->setPosition(position);
        }
    }
}

void GLViewNewModule::onKeyDown(const SDL_KeyboardEvent& key)
{
    Vector movementVector;

    switch (key.keysym.sym)
    {
    case SDLK_w:    // Move forward
        movementVector = Vector(0.1f, 0, 0);
        break;
    case SDLK_s:    // Move backward
        movementVector = Vector(-0.1f, 0, 0);
        break;
    case SDLK_a:    // Move left
        movementVector = Vector(0, -0.1f, 0);
        break;
    case SDLK_d:    // Move right
        movementVector = Vector(0, 0.1f, 0);
        break;
    case SDLK_q:    // Move up
        movementVector = Vector(0, 0, 0.1f);
        break;
    case SDLK_e:    // Move down
        movementVector = Vector(0, 0, -0.1f);
        break;
    default:
        return;
    }

    // Move the box and send the updated position to the server
    playerBox->moveRelative(movementVector);
    sendNetMsgSynchronousTCP("updatePosition", playerBox->getPosition(), playerBox->getDisplayMatrix());
}
void GLViewNewModule::sendNetMsgSynchronousTCP(const std::string& action, const Vector& position, const Mat4& displayMat)
{
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12346);  // Match the server port

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);  // Use localhost for testing

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    std::string serializedMsg = action + " " +
        std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z) + " ";
    for (int i = 0; i < 16; i++)
    {
        serializedMsg += std::to_string(displayMat[i]) + " ";
    }

    int result = send(sock, serializedMsg.c_str(), serializedMsg.size(), 0);
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
    }

    closesocket(sock);
    WSACleanup();
}

void GLViewNewModule::receiveNetMsgSynchronousTCP(std::string& action, Vector& position, Mat4& displayMat)
{
    WSADATA wsaData;
    SOCKET listenSocket = INVALID_SOCKET, clientSocket = INVALID_SOCKET;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    int opt = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt failed with error: " << WSAGetLastError() << std::endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(12346);  // Match the client port

    if (bind(listenSocket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    if (listen(listenSocket, 3) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::cout << "Waiting for client connection..." << std::endl;

    clientSocket = accept(listenSocket, (struct sockaddr*)&address, &addrlen);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    int valread = recv(clientSocket, buffer, 1024, 0);
    if (valread == SOCKET_ERROR)
    {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::string receivedData(buffer, valread);
    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    std::istringstream iss(receivedData);
    iss >> action >> position.x >> position.y >> position.z;

    for (int i = 0; i < 16; i++)
    {
        iss >> displayMat[i];
    }

    if (action == "updatePose")
    {
        WO* objToUpdate = this->worldLst->at(3);  // Assuming the cube is at index 3
        objToUpdate->setPosition(position);
        objToUpdate->setDisplayMatrix(displayMat);
    }
}
void GLViewNewModule::onResizeWindow(GLsizei width, GLsizei height)
{
    GLView::onResizeWindow(width, height);
}

void GLViewNewModule::onMouseDown(const SDL_MouseButtonEvent& e)
{
    GLView::onMouseDown(e);
}

void GLViewNewModule::onMouseUp(const SDL_MouseButtonEvent& e)
{
    GLView::onMouseUp(e);
}

void GLViewNewModule::onMouseMove(const SDL_MouseMotionEvent& e)
{
    GLView::onMouseMove(e);
}

void GLViewNewModule::onKeyUp(const SDL_KeyboardEvent& key)
{
    GLView::onKeyUp(key);
}

void GLViewNewModule::loadMap()
{
    this->worldLst = new WorldList();
    this->actorLst = new WorldList();
    this->netLst = new WorldList();

    ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
    ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
    ManagerOpenGLState::enableFrustumCulling = false;
    Axes::isVisible = true;
    this->glRenderer->isUsingShadowMapping(false);
    this->cam->setPosition(15, 15, 10);

    std::string shinyRedPlasticCube(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");
    std::string grass(ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl");

    std::vector<std::string> skyBoxImageNames;
    skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg");
    {
        WOLight* light = WOLight::New();
        light->isDirectionalLight(true);
        light->setPosition(Vector(0, 0, 100));
        light->getModel()->setDisplayMatrix(Mat4::rotateIdentityMat({ 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD));
        light->setLabel("Light");
        worldLst->push_back(light);
    }

    {
        WO* wo = WOSkyBox::New(skyBoxImageNames.at(0), this->getCameraPtrPtr());
        wo->setPosition(Vector(0, 0, 0));
        wo->setLabel("Sky Box");
        wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        worldLst->push_back(wo);
    }

    {
        WO* wo = WO::New(grass, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
        wo->setPosition(Vector(0, 0, 0));
        wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        worldLst->push_back(wo);
    }

    //{
    //    WO* a10 = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/spider.obj", Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    //    a10->setPosition(0, 0, 0);
    //    a10->setLabel("Spider");
    //    a10->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    //    worldLst->push_back(a10);
    //}

    //WO* cubewo = WO::New(shinyRedPlasticCube, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    //auto lookDirection = this->getCamera()->getLookDirection().normalizeMe();

    //cubewo->setPosition((lookDirection * 20) + this->getCamera()->getPosition());
    //Vector cubewodefaulatpos = cubewo->getPosition();
    //cubewo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
    //cubewo->setLabel("Cube");
    //worldLst->push_back(cubewo);

    createNewModuleWayPoints();
}

void GLViewNewModule::transformball(Vector globalr, Vector localr, Vector translate, WO* cubewo)
{
    cubewo->rotateAboutGlobalX(globalr[0] * DEGtoRAD);
    cubewo->rotateAboutGlobalY(globalr[1] * DEGtoRAD);
    cubewo->rotateAboutGlobalZ(globalr[2] * DEGtoRAD);
    cubewo->rotateAboutRelX(localr[0] * DEGtoRAD);
    cubewo->rotateAboutRelY(localr[1] * DEGtoRAD);
    cubewo->rotateAboutRelZ(localr[2] * DEGtoRAD);
    cubewo->moveRelative(translate);
}

void GLViewNewModule::createNewModuleWayPoints()
{
    WayPointParametersBase params(this);
    params.frequency = 5000;
    params.useCamera = true;
    params.visible = true;
    WOWayPointSpherical* wayPt = WOWayPointSpherical::New(params, 3);
    wayPt->setPosition(Vector(50, 0, 3));
    worldLst->push_back(wayPt);
}

void GLViewNewModule::setAsServer()
{
    isServer = true;
    std::cout << "This instance is set as a server." << std::endl;
}

void GLViewNewModule::setAsClient()
{
    isServer = false;
    std::cout << "This instance is set as a client." << std::endl;
}
