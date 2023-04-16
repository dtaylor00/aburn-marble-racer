#include "GLViewMarbleRacer.h"

#include "AftrGLRendererBase.h"
#include "Axes.h"
#include "Camera.h"
#include "GuiMenuLight.h"
#include "GuiMenuSkyBox.h"
#include "GuiMenuTransform.h"
#include "MGLSkyBox.h"
#include "ManagerPhysics.h"
#include "PxPhysicsAPI.h"
#include "WO.h"
#include "WOImGuiMain.h"
#include "WOLight.h"
#include "WOPhysics.h"
#include "WOPhysicsMarble.h"
#include "WOPhysicsTriangleMesh.h"
#include "WOSkyBox.h"
#include "WorldList.h"

using namespace Aftr;

bool paused = true;

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

    if (!paused) {
        ManagerPhysics::simulate(1.f / 60.f);
        ManagerPhysics::syncWOsFromPhysics();
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

    // Initialize ManagerPhysics
    {
        ManagerPhysics::init(20.f, Vector(0, 0, -1));
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

    // Create the grass plane
    {
        std::string grass(ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl");
        PxMaterial *mat = ManagerPhysics::getPhysics()->createMaterial(0.5f, 0.5f, 0.4f);
        PxRigidStatic *plane = PxCreatePlane(*ManagerPhysics::getPhysics(), PxPlane(0, 0, 1, 0), *mat);
        WO *wo = WOPhysics::New(plane, grass, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
        wo->setPosition(Vector(0, 0, 0));
        wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
        wo->upon_async_model_loaded([wo]() {
            ModelMeshSkin &grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0);
            grassSkin.getMultiTextureSet().at(0).setTexRepeats(5.0f);
            grassSkin.setAmbient(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f));   // Color of object when it is not in any light
            grassSkin.setDiffuse(aftrColor4f(1.0f, 1.0f, 1.0f, 1.0f));   // Diffuse color components (ie, matte shading color of this object)
            grassSkin.setSpecular(aftrColor4f(0.4f, 0.4f, 0.4f, 1.0f));  // Specular color component (ie, how "shiney" it is)
            grassSkin.setSpecularCoefficient(10);                        // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
        });
        wo->setLabel("Grass");
        worldLst->push_back(wo);
    }

    // Create a few marbles
    {
        WOPhysicsMarble *wo = WOPhysicsMarble::New();
        wo->setLabel("marble");
        wo->setPosition(5, 5, 5);
        worldLst->push_back(wo);
    }

    // Creating test tracks
    {
        std::string track(ManagerEnvironmentConfiguration::getLMM() + "/models/track.dae");
        WOPhysicsTriangleMesh *wo = WOPhysicsTriangleMesh::New(track);
        wo->setLabel("track");
        // wo->setPosition(-5, -5, 20);
        worldLst->push_back(wo);

        std::string curvedtrack(ManagerEnvironmentConfiguration::getLMM() + "/models/curvedtrack.dae");
        WOPhysicsTriangleMesh *wo2 = WOPhysicsTriangleMesh::New(curvedtrack);
        wo2->setLabel("curved track");
        // // wo->setPosition(-5, -5, 20);
        worldLst->push_back(wo2);
    }

    // Create a test objects
    {
        PxPhysics *px = ManagerPhysics::getPhysics();
        PxScene *scene = ManagerPhysics::getScene();
        PxMaterial *mat = ManagerPhysics::getDefaultMaterial();

        std::string box(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");
        PxBoxGeometry geom(2.f, 2.f, 2.f);
        PxShape *shape = px->createShape(geom, *mat, true);
        PxTransform t({0, 0, 0});

        PxRigidDynamic *body = px->createRigidDynamic(t);
        body->attachShape(*shape);
        WOPhysics *wo = WOPhysics::New(body, box, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT);
        wo->setLabel("box");
        wo->setPosition(10, 10, 20);
        worldLst->push_back(wo);
    }
}

void GLViewMarbleRacer::onKeyDown(const SDL_KeyboardEvent &key) {
    GLView::onKeyDown(key);
    if (key.keysym.sym == SDLK_SPACE) paused = !paused;
}
