#include "sandbox/base/envmap.h"
#include "sandbox/base/mesh.h"

#include "base/files/file_path.h"

#include "skydome.afx.h"
#define MESH_PATH AZER_LITERAL("sandbox/media/model/sphere.3ds")
#define EFFECT_GEN_DIR "out/dbg/gen/sandbox/base/"
#define SHADER_NAME "envmap.afx"

using ::base::FilePath;

bool EnvMap::Skydome::Init(MeshData* data, azer::RenderSystem* rs) {
  azer::ShaderArray shaders;
  CHECK(azer::LoadVertexShader(EFFECT_GEN_DIR SHADER_NAME ".vs", &shaders));
  CHECK(azer::LoadPixelShader(EFFECT_GEN_DIR SHADER_NAME ".ps", &shaders));
  SkydomeEffect* effect = new SkydomeEffect(shaders.GetShaderVec(), rs);
  effect_.reset(effect);
  desc_ = effect->GetVertexDesc();

  if (!InitPhyBuf(data, effect->GetVertexDesc(), rs)) {
    return false;
  }

  cubemap_.reset(azer::Texture::LoadShaderTexture(envmap_->path(), rs));
  return true;
}

void EnvMap::Skydome::UseEffect(const Group& group, azer::Renderer* renderer) {
  effect_->Use(renderer);
}

void EnvMap::Skydome::UpdateVertex(void* dest, const MeshData::Vertex& vertex) {
  SkydomeEffect::Vertex* v = (SkydomeEffect::Vertex*)dest;
  v->position = azer::Vector3(vertex.position);
  v->texcoord0= vertex.tex0;
  v->normal = azer::Vector3(vertex.normal);
}

void EnvMap::Skydome::UpdateAll(azer::VertexData* vdata, azer::IndicesData* idata) {
}

void EnvMap::Skydome::OnUpdateScene(const azer::Camera& camera) {
  azer::Matrix4 world = azer::Scale(100.0f, 100.0f, 100.0f);
  SkydomeEffect* effect = (SkydomeEffect*)effect_.get();
  effect->SetCubemap(cubemap_);
  effect->SetWorld(world);
  effect->SetDirLight(envmap_->light_);
  effect->SetPVW(camera.GetProjViewMatrix() * world);
}


bool EnvMap::Init(azer::RenderSystem* rs) {
  MeshData data;
  if (!LoadMeshData(path_, &data, rs)) {
    return false;
  }

  skydome_.reset(new Skydome(this));
  if (!skydome_->Init(&data, rs)) {
    return false;
  }

  return true;
}

void EnvMap::Render(azer::Renderer* renderer) {
  skydome_->Render(renderer);
}
