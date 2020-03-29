#include "RayTracerResource.h"
#include "RayTracerGadget.h"
#include <ACGM_RayTracer_lib/SceneImporter.h>
#include <HIRO/HIRO.h>

RayTracerGadget::RayTracerGadget(const hiro::Resource *res)
  : hiro::Gadget(res)
{
}

void RayTracerGadget::Initialize()
{
  hiro::Gadget::Initialize();
  style_ = std::make_shared<hiro::draw::RasterStyle>();
  style_->use_nearest_filtering = true;
  AddRenderer(GetResource<RayTracerResource>()->GetRenderer(), style_);
}

void RayTracerGadget::GenerateGui(hiro::GuiGenerator& gui)
{
    hiro::Gadget::GenerateGui(gui);
    hiro::gui::Droplist* scene_selector_;
    hiro::gui::Button* scene_button_;
    scene_selector_ = gui.AddDroplist("Scene File")
        ->AddItemsIndexed({ "scene0.txt", "scene1.txt", "scene2.txt" })
        ->Set(0);

    scene_button_ = gui.AddButton("Import and Render");
    scene_button_->Subscribe([scene_selector_](const hiro::gui::Button* button) {
        std::string file_name = scene_selector_->GetText();
        ImportAndRender(file_name);
    });
}

void RayTracerGadget::ImportAndRender(std::string fileName)
{
    bool importCheck;
    acgm::SceneImporter SceneImporter;
    acgm::RenderOptions result;
    std::shared_ptr<acgm::Scene> scene;

    //! Import scene from file
    importCheck = SceneImporter.Import(fileName);
    result = SceneImporter.GetRenderOptions();
    scene = SceneImporter.GetScene();

    //! Set resolution
    auto resource = std::make_shared<RayTracerResource>("RayTraced Scene");
    resource->GetRenderer()->SetResolution({ result.resolution.x, result.resolution.y });
    hiro::AddResource(resource);

    //! Raytrace scene
    scene->Raytrace(*resource->GetRenderer());
};