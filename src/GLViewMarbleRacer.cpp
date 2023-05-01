#include "GLViewMarbleRacer.h"

#include <chrono>

#include "AftrGLRendererBase.h"
#include "AftrImGuiIncludes.h"
#include "Axes.h"
#include "Camera.h"
#include "GuiMenuContainer.h"
#include "GuiMenuLight.h"
#include "GuiMenuMarble.h"
#include "GuiMenuSimulation.h"
#include "GuiMenuSkyBox.h"
#include "GuiMenuTransform.h"
#include "MGLSkyBox.h"
#include "ManagerMarble.h"
#include "ManagerPhysics.h"
#include "PxPhysicsAPI.h"
#include "WO.h"
#include "WOImGuiMain.h"
#include "WOLight.h"
#include "WOPhysics.h"
#include "WOPhysicsGoalBox.h"
#include "WOPhysicsMarble.h"
#include "WOPhysicsRotatingChamber.h"
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
    this->cam->setPosition(35, -80, 110);
    this->cam->rotateAboutRelZ(90 * DEGtoRAD);
    this->cam->rotateAboutRelY(30 * DEGtoRAD);
    this->state = new GameState(STOPPED);

    // Initialize Managers
    {
        ManagerMarble::init();
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
        maingui->addMenu(menu, "Transform Menu");
        worldLst->push_back(maingui);
    }

    // Create the light and skybox
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
        GuiMenuSkyBox *skyboxMenu = GuiMenuSkyBox::New(skybox);
        GuiMenuLight *lightMenu = GuiMenuLight::New(light);
        GuiMenuContainer *menu = GuiMenuContainer::New();
        menu->addMenu(skyboxMenu);
        menu->addMenu(lightMenu);
        maingui->addMenu(menu, "Visual Menu");
    }

    // Create a few marbles
    {
        WO *wo;
        for (size_t i = 0; i < 10; i++) {
            std::string label = std::format("marble{:02d}", ManagerMarble::getMarbleCount());
            float x = ManagerRandomNumber::getRandomFloat(-8, 8);
            float y = ManagerRandomNumber::getRandomFloat(-8, 8);
            float z = 105;

            wo = WOPhysicsMarble::New();
            wo->setLabel(label);
            wo->setPosition(x, y, z);
            worldLst->push_back(wo);
        }

        GuiMenuMarble *menu = GuiMenuMarble::New(worldLst);
        maingui->addMenu(menu, "Marble Menu");
    }

    // Creating test tracks
    {
        std::string straight_track(ManagerEnvironmentConfiguration::getLMM() + "/models/straight_track.dae");
        std::string curved_left_track(ManagerEnvironmentConfiguration::getLMM() + "/models/curved_track_left.dae");
        std::string curved_right_track(ManagerEnvironmentConfiguration::getLMM() + "/models/curved_track_right.dae");
        std::string funnel_track(ManagerEnvironmentConfiguration::getLMM() + "/models/funnel_track.dae");
        std::string rotating_chamber(ManagerEnvironmentConfiguration::getLMM() + "/models/rotating_chamber.dae");
        std::string rotating_pole(ManagerEnvironmentConfiguration::getLMM() + "/models/rotating_pole.dae");
        WOPhysicsTriangleMesh *wo;
        Mat4 pose;

        wo = WOPhysicsTriangleMesh::New(funnel_track);
        wo->setLabel("track00");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(0, 0, 100));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straight_track, Vector(3, 1, 1));
        wo->setLabel("track01");
        pose.setX(Vector(0.97, 0, -0.26));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0.26, 0, 0.97));
        pose.setPosition(Vector(20.43f, 0, 83.443f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curved_right_track);
        wo->setLabel("track02");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(57.002f, -1.5f, 75.878f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straight_track, Vector(2, 1, 1));
        wo->setLabel("track03");
        pose.setX(Vector(0, -0.94, -0.34));
        pose.setY(Vector(1, 0, 0));
        pose.setZ(Vector(0, -0.34, 0.94));
        pose.setPosition(Vector(58.502f, -29.991f, 68.909f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curved_left_track);
        wo->setLabel("track04");
        pose.setX(Vector(0, -1, 0));
        pose.setY(Vector(1, 0, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(60.002f, -56.088f, 62.360f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straight_track, Vector(2, 1, 1));
        wo->setLabel("track05");
        pose.setX(Vector(0.87, 0, -0.5));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0.5, 0, 0.87));
        pose.setPosition(Vector(87.573f, -57.588f, 51.973f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straight_track, Vector(3, 1, 1));
        wo->setLabel("track06");
        pose.setX(Vector(0.97, 0, -0.26));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0.26, 0, 0.97));
        pose.setPosition(Vector(133.02f, -57.588f, 34.558f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curved_left_track, Vector(1, 1, 1));
        wo->setLabel("track07");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(169.6f, -56.088f, 26.994f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straight_track, Vector(0.5f, 1, 1));
        wo->setLabel("track08");
        pose.setX(Vector(0, 0.98, -0.17));
        pose.setY(Vector(-1, 0, 0));
        pose.setZ(Vector(0, 0.17, 0.98));
        pose.setPosition(Vector(171.1f, -42.056f, 26.155f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsRotatingChamber::New();
        wo->setLabel("track09");
        pose.setX(Vector(0, 1, 0));
        pose.setY(Vector(-1, 0, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(171.1f, -17.74f, 25.356f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(curved_right_track, Vector(1, 1, 1));
        wo->setLabel("track10");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(169.6f, 10.76f, 25.25f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        wo = WOPhysicsTriangleMesh::New(straight_track, Vector(2, 1, 1));
        wo->setLabel("track11");
        pose.setX(Vector(0.94, 0, 0.34));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(-0.34, 0, 0.94));
        pose.setPosition(Vector(141.11f, 12.26f, 18.150f));
        wo->setPose(pose);
        worldLst->push_back(wo);

        // wo = WOPhysicsTriangleMesh::New(boxtrack, Vector(1, 1, 1));
        wo = WOPhysicsGoalBox::New(Vector(1, 1, 1));
        wo->setLabel("goalbox");
        pose.setX(Vector(1, 0, 0));
        pose.setY(Vector(0, 1, 0));
        pose.setZ(Vector(0, 0, 1));
        pose.setPosition(Vector(105, 12.26f, -10));
        wo->setPose(pose);
        worldLst->push_back(wo);
    }

    // Create timer gui
    {
        GuiMenuSimulation *menu = GuiMenuSimulation::New(state);
        maingui->addMenu(menu, "Simulation Menu");
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
