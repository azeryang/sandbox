#include "sandbox/base/base.h"
#include "sandbox/base/envmap.h"
#include "sandbox/effect/dispersion/mesh.h"
#include "base/base.h"

#include "diffuse.afx.h"

#define CUBEMAP_PATH AZER_LITERAL("sandbox/media/textures/nvlobby_new_cube_mipmap.dds")
#define MESH_PATH AZER_LITERAL("sandbox/media/model/HateAlien/HateAlien-POSE.OBJ")
#define EFFECT_GEN_DIR "out/dbg/gen/sandbox/basic/mesh/"
#define SHADER_NAME "diffuse.afx"

class MainDelegate : public wxSampleApp::Delegate {
 public:
  MainDelegate() {}
  virtual bool OnInit();
  virtual void OnUpdateScene(double time, float delta_time);
  virtual void OnRenderScene(double time, float delta_time);
  virtual void OnQuit() {}
 private:
  azer::Camera camera_;
  DirLight light_;
  std::unique_ptr<MyMesh> mesh_;
  std::unique_ptr<EnvMap> envmap_;
  azer::TexturePtr cubemap_;
  DISALLOW_COPY_AND_ASSIGN(MainDelegate);
};

bool MainDelegate::OnInit() {
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  light_.dir = azer::Vector4(0.3f, -0.3f, -0.75f, 0.0f);
  light_.diffuse = azer::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  light_.ambient = azer::Vector4(0.2f, 0.2f, 0.2f, 1.0f);

  envmap_.reset(new EnvMap(::base::FilePath(CUBEMAP_PATH)));
  envmap_->Init(rs);
  // cubemap_.reset(azer::Texture::LoadShaderTexture(CUBEMAP_PATH, rs));
  mesh_.reset(Load(MESH_PATH, rs));
  // mesh_->SetCubemap(cubemap_);
  mesh_->SetCubemap(envmap_->cubemap());

  camera_.SetPosition(azer::Vector3(0.0f, 0.0f, 100.0f));
  camera_.SetLookAt(azer::Vector3(0.0f, 0.0f, 0.0f));
  camera_.frustrum().set_far(10000.0f);
  azer::Renderer* renderer = rs->GetDefaultRenderer();
  renderer->EnableDepthTest(true);
  return true; 
}

void MainDelegate::OnUpdateScene(double time, float delta_time) {
  float rspeed = 3.14f * 2.0f / 4.0f;
  azer::Radians camera_speed(azer::kPI / 2.0f);
  UpdatedownCamera(&camera_, 50.0f, camera_speed, delta_time);
  mesh_->OnUpdateScene(camera_);
  envmap_->OnUpdate(camera_);
}

void MainDelegate::OnRenderScene(double time, float delta_time) {
  azer::RenderSystem* rs = azer::RenderSystem::Current();
  azer::Renderer* renderer = rs->GetDefaultRenderer();
  renderer->Use();
  renderer->Clear(azer::Vector4(0.0f, 0.0f, 1.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  renderer->SetCullingMode(azer::kCullNone);

  mesh_->Render(renderer);
  envmap_->Render(renderer);
}

int main(int argc, char* argv[]) {
  ::base::InitApp(&argc, &argv, "");
  MainDelegate delegate;
  /*
  SampleApp app(&delegate);
  app.Init();
  app.MainLoop();
  */
  wxSampleApp app(&delegate);
  wxSampleApp::MainLoop(argc, argv, &app);
  return 0;
}
