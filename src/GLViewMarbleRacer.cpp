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
#include "GuiMenuTrack.h"
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
        light->getModel()->setDisplayMatrix(Mat4::rotateIdentityMat({0, 1, 0}, 45.0f * Aftr::DEGtoRAD));
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

        GuiMenuMarble *menu = GuiMenuMarble::New();
        maingui->addMenu(menu, "Marble Menu");
    }

    // Creating demo track
    {
        GuiMenuTrack *menu = GuiMenuTrack::New();
        maingui->addMenu(menu, "Track Menu");
        Mat4 pose;
        Vector scale(1, 1, 1);

        // FIXME: Definitely should have made a manager class for tracks like the marbles.
        // Spawning tracks through the menu is messy and ugly and needs to be factored out.
        // Morever, because spawning the tracks requires waiting for the model to load,
        // the track number assigned is not the same order as created here, which could also
        // probably be fixed with a manager class.
        {  // whole bunch of track posing and scaling code
            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(0, 0, 100));
            scale = Vector(1, 1, 1);
            menu->spawnTrack(0, pose, scale);

            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(12, 0, 80));
            scale = Vector(1, 2, 1);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0.87, 0, -0.5));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0.5, 0, 0.87));
            pose.setPosition(Vector(65, 0, 51));
            scale = Vector(1, 1.2, 1);
            menu->spawnTrack(5, pose, scale);

            pose.setX(Vector(0.85, -0.49, -0.17));
            pose.setY(Vector(0.5, 0.87, 0));
            pose.setZ(Vector(0.15, -0.09, 0.98));
            pose.setPosition(Vector(103, -13.8, 29));
            scale = Vector(1.75, 1.5, 1);
            menu->spawnTrack(1, pose, scale);

            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(135, -25, 18));
            scale = Vector(2, 2, 1);
            menu->spawnTrack(0, pose, scale);

            pose.setX(Vector(0.94, 0, -0.34));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0.34, 0, 0.94));
            pose.setPosition(Vector(140, -25, 8));
            scale = Vector(1.5, 0.8, 0.8);
            menu->spawnTrack(1, pose, scale);

            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(154.132, -17.201, -17.489));
            scale = Vector(1, 1, 1.5);
            menu->spawnTrack(6, pose, scale);

            pose.setX(Vector(0, 1, 0));
            pose.setY(Vector(-1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(169.82, -26.34, -50.20));
            scale = Vector(1, 1, 1);
            menu->spawnTrack(7, pose, scale);

            pose.setX(Vector(0, 0, -1));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(1, 0, 0));
            pose.setPosition(Vector(164, -55, -90));
            scale = Vector(1, 1, 1.5);
            menu->spawnTrack(5, pose, scale);

            pose.setX(Vector(0, 0, -1));
            pose.setY(Vector(0, -1, 0));
            pose.setZ(Vector(-1, 0, 0));
            pose.setPosition(Vector(174, -55, -90));
            scale = Vector(1, 1, 1.5);
            menu->spawnTrack(5, pose, scale);

            pose.setX(Vector(0, -1, 0));
            pose.setY(Vector(1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, 0, -55));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0, 1, 0));
            pose.setY(Vector(-1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, -5, -65));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0, -1, 0));
            pose.setY(Vector(1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, 0, -75));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0, 1, 0));
            pose.setY(Vector(-1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, -5, -85));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0, -1, 0));
            pose.setY(Vector(1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, 0, -95));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0, 1, 0));
            pose.setY(Vector(-1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, -5, -105));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(0, -1, 0));
            pose.setY(Vector(1, 0, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, 0, -115));
            scale = Vector(0.5, 1.5, 0.5);
            menu->spawnTrack(4, pose, scale);

            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, -45, -140));
            scale = Vector(3, 3, 2);
            menu->spawnTrack(0, pose, scale);

            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(170, -45, -150));
            scale = Vector(1, 1, 1);
            menu->spawnTrack(0, pose, scale);

            pose.setX(Vector(0.94, 0, 0.34));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(-0.34, 0, 0.94));
            pose.setPosition(Vector(145, -45, -170));
            scale = Vector(4, 0.5, 2);
            menu->spawnTrack(1, pose, scale);

            pose.setX(Vector(1, 0, 0));
            pose.setY(Vector(0, 1, 0));
            pose.setZ(Vector(0, 0, 1));
            pose.setPosition(Vector(95.5, -45, -205));
            scale = Vector(1, 1, 1);
            menu->spawnTrack(8, pose, scale);
        }
    }

    // Create timer gui
    {
        GuiMenuSimulation *menu = GuiMenuSimulation::New(state);
        maingui->addMenu(menu, "Simulation Menu");
    }
}
