#include "GLViewMarbleRacer.h"

#include <chrono>

#include "AftrGLRendererBase.h"
#include "AftrImGuiIncludes.h"
#include "Axes.h"
#include "Camera.h"
#include "GuiMenuLight.h"
#include "GuiMenuSkyBox.h"
#include "GuiMenuTimer.h"
#include "GuiMenuTransform.h"
#include "MGLSkyBox.h"
#include "ManagerPhysics.h"
#include "PxPhysicsAPI.h"
#include "WO.h"
#include "WOImGuiMain.h"
#include "WOLight.h"
#include "WOPhysics.h"
#include "WOPhysicsGoalBox.h"
#include "WOPhysicsMarble.h"
#include "WOPhysicsTriangleMesh.h"
#include "WOSkyBox.h"
#include "WorldList.h"

using namespace Aftr;

GLViewMarbleRacer *GLViewMarbleRacer::New(
    const std::vector<std::string> &args) {
    GLViewMarbleRacer *glv = new GLViewMarbleRacer(args);
    glv->init(Aftr::GRAVITY, Vector(0, 0, -1.0f), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE);
    glv->onCreate();
    return glv;
}

GLViewMarbleRacer::GLViewMarbleRacer(const std::vector<std::string> &args) : GLView(args) {}

void GLViewMarbleRacer::onCreate() {
    this->setActorChaseType(STANDARDEZNAV);  // Default is STANDARDEZNAV mode
}

GLViewMarbleRacer::~GLViewMarbleRacer() {}

void GLViewMarbleRacer::updateWorld() {
    GLView::updateWorld();

    switch (*state) {
        case RUNNING: {
            ManagerPhysics::simulate(2.f / ImGui::GetIO().Framerate);  // NOTE: Probably is a better way to get current fps
            ManagerPhysics::syncWOsFromPhysics();
        } break;
        case STOPPED:
        case PAUSED: {
            // do nothing
        } break;
        default:
            break;
    }

    maingui->onUpdateWO();  // doesn't get called in updateWorld because it's an ImGui instance
}

void Aftr::GLViewMarbleRacer::loadMap() {
    this->worldLst = new WorldList();  // WorldList is a 'smart' vector that is used to store WO*'s
    this->actorLst = new WorldList();
    this->netLst = new WorldList();
    this->cam->setPosition(40, 50, 35);
    this->cam->rotateAboutRelZ(-120 * DEGtoRAD);
    this->cam->rotateAboutRelY(30 * DEGtoRAD);
    this->state = new GameState(STOPPED);

    // Initialize ManagerPhysics
    {
        ManagerPhysics::init(25.f, Vector(0, 0, -1));
        std::cout << "Initialized ManagerPhysics...\n";
    }

    // Sets OpenGL state variables
    {
        ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
        ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
        ManagerOpenGLState::enableFrustumCulling = false;
        Axes::isVisible = true;
        this->glRenderer->isUsingShadowMapping(false);  // set to TRUE to enable shadow mapping, must be using GL 3.2+
    }

    // Create an ImGui instance
    {
        maingui = WOImGuiMain::New(nullptr);
        maingui->setLabel("Main GUI");
        maingui->setPosition(Vector(0, 0, 0));
        maingui->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;

        GuiMenuTransform *menu = GuiMenuTransform::New(worldLst);
        maingui->addMenu(menu);
        worldLst->push_back(maingui);
    }

    // Create a light
    {
        float ga = 0.1f;  // Global Ambient Light level for this module
        ManagerLight::setGlobalAmbientLight(aftrColor4f(ga, ga, ga, 1.0f));
        WOLight *light = WOLight::New();
        light->isDirectionalLight(true);
        light->setPosition(Vector(0, 0, 100));
        // Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
        // for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
        light->getModel()->setDisplayMatrix(Mat4::rotateIdentityMat({0, 1, 0}, 90.0f * Aftr::DEGtoRAD));
        light->setLabel("Light");
        worldLst->push_back(light);

        GuiMenuLight *menu = GuiMenuLight::New(light);
        maingui->addMenu(menu);
    }

    // Create the SkyBox
    {
        // SkyBox Textures readily available
        std::vector<std::string> skyBoxImageNames;  // vector to store texture paths
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg");
        skyBoxImageNames.push_back(ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg");

        WO *wo = WOSkyBox::New(skyBoxImageNames, this->getCameraPtrPtr());
        wo->setPosition(Vector(0, 0, 0));
        wo->setLabel("Sky Box");
        wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        worldLst->push_back(wo);

        MGLSkyBox *skybox = (MGLSkyBox *)wo->getModel();
        GuiMenuSkyBox *menu = GuiMenuSkyBox::New(skybox);
        maingui->addMenu(menu);
    }

    // Create a few marbles
    {
        WOPhysicsMarble *wo;

        for (size_t i = 0; i < 15; i++) {
            // TODO: replace std:rand with a better rng
            int randx = (std::rand() % 6 - 3);  // number between -3 and 3
            int randy = (std::rand() % 6 - 3);  // number between -3 and 3
            randx = randx > 0 ? randx + 5 : randx - 5;
            randy = randy > 0 ? randy + 5 : randy - 5;
            std::string label = std::format("marble{:02d}", i);

            wo = WOPhysicsMarble::New();
            wo->setLabel(label);
            wo->setPosition(randx, randy, 60);
            worldLst->push_back(wo);
        }
    }

    // Creating test tracks
    {
        std::string straighttrack(ManagerEnvironmentConfiguration::getLMM() + "/models/straighttrack.dae");
        std::string curvedtrack(ManagerEnvironmentConfiguration::getLMM() + "/models/curvedtrack.dae");
        std::string funneltrack(ManagerEnvironmentConfiguration::getLMM() + "/models/funneltrack.dae");
        std::string boxtrack(ManagerEnvironmentConfiguration::getLMM() + "/models/boxtrack.dae");
        WOPhysicsTriangleMesh *wo;
        Mat4 pose;

        wo = WOPhysicsTriangleMesh::New(funneltrack);
        wo->setLabel("track00");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(0, 0, 55));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straighttrack);
        wo->setLabel("track01");
        pose.setX(Vector(0.97, 0, 0.26));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(-0.26, 0, 0.97));
        pose.setPosition(Vector(0, 0, 42.5));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curvedtrack);
        wo->setLabel("track02");
        pose.setX(Vector(0, -1, 0));
        pose.setY(Vector(1, 0, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(-25, 6.5, 40));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straighttrack, Vector(4, 1, 1));
        wo->setLabel("track03");
        pose.setX(Vector(0, 0.98, -0.17));
        pose.setY(Vector(-1, 0, 0));
        pose.setZ(Vector(0, 0.17, 0.98));
        pose.setPosition(Vector(-33.5, 59, 33));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curvedtrack);
        wo->setLabel("track04");
        pose.setX(Vector(-1, 0, 0));
        pose.setY(Vector(0, -1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(-27, 112.5, 26.25));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curvedtrack);
        wo->setLabel("track05");
        pose.setX(Vector(0, 1, 0));
        pose.setY(Vector(-1, 0, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(2, 114.5, 26.25));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straighttrack, Vector(2, 1, 1));
        wo->setLabel("track06");
        pose.setX(Vector(0, 0.87, 0.5));
        pose.setY(Vector(-1, 0, 0));
        pose.setZ(Vector(0, -0.5, 0.87));
        pose.setPosition(Vector(10.5, 83, 16));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straighttrack, Vector(3, 1, 1));
        wo->setLabel("track07");
        pose.setX(Vector(0, 0.97, 0.26));
        pose.setY(Vector(-1, 0, 0));
        pose.setZ(Vector(0, -0.26, 0.97));
        pose.setPosition(Vector(10.5, 37.5, -1.5));
        wo->setPose(pose);
        worldLst->push_back(wo);

        // wo = WOPhysicsTriangleMesh::New(boxtrack, Vector(1, 1, 1));
        wo = WOPhysicsGoalBox::New(Vector(1, 1, 1));
        wo->setLabel("goalbox");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(10.5, -8, -24));
        wo->setPose(pose);
        worldLst->push_back(wo);
    }

    // Create timer gui
    {
        GuiMenuTimer *menu = GuiMenuTimer::New(state);
        maingui->addMenu(menu);
    }

    // Create a test objects
    // {
    //     PxPhysics *px = ManagerPhysics::getPhysics();
    //     PxScene *scene = ManagerPhysics::getScene();
    //     PxMaterial *mat = ManagerPhysics::getDefaultMaterial();

    //     std::string box(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");
    //     PxBoxGeometry geom(2.f, 2.f, 2.f);
    //     PxShape *shape = px->createShape(geom, *mat, true);
    //     PxTransform t({0, 0, 0});

    //     PxRigidDynamic *body = px->createRigidDynamic(t);
    //     body->attachShape(*shape);
    //     WOPhysics *wo = WOPhysics::New(body, box, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
    //     wo->setLabel("box");
    //     wo->setPosition(10, 10, 20);
    //     worldLst->push_back(wo);
    // }
}
