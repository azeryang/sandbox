#include "sandbox/effect/dispersion/mesh.h"

#define MESH_PATH FILE_PATH_LITERAL("sandbox/media/model/HateAlien/HateAlien-POSE.OBJ")
#define EFFECT_GEN_DIR "out/dbg/gen/sandbox/effect/dispersion/"
#define SHADER_NAME "dispersion.afx"

bool MyMesh::Init(MeshData* data, azer::RenderSystem* rs) {
  azer::ShaderArray shaders;
  CHECK(azer::LoadVertexShader(EFFECT_GEN_DIR SHADER_NAME ".vs", &shaders));
  CHECK(azer::LoadPixelShader(EFFECT_GEN_DIR SHADER_NAME ".ps", &shaders));
  effect_.reset(new DispersionEffect(shaders.GetShaderVec(), rs));
  desc_ = effect_->GetVertexDesc();

  if (!InitPhyBuf(data, effect_->GetVertexDesc(), rs)) {
    return false;
  }

  return true;
}

void MyMesh::UseEffect(const Group& group, azer::Renderer* renderer) {
  effect_->Use(renderer);
}

void MyMesh::UpdateVertex(void* dest, const MeshData::Vertex& vertex) {
  DispersionEffect::Vertex* v = (DispersionEffect::Vertex*)dest;
  v->position = azer::Vector3(vertex.position);
  v->texcoord0= vertex.tex0;
  v->normal = azer::Vector3(vertex.normal);
}

void MyMesh::UpdateAll(azer::VertexData* vdata, azer::IndicesData* idata) {
}

void MyMesh::OnUpdateScene(const azer::Camera& camera) {
  // azer::Matrix4 world = std::move(azer::Scale(0.1f, 0.1f, 0.1f));
  float eta = 1.1f;
  float eta_delta = -0.02f;
  azer::Matrix4 world = azer::Matrix4::kIdentity;
  effect_->SetWorld(world);
  effect_->SetPVW(camera.GetProjViewMatrix() * world);
  effect_->SetCubemap(cubemap_);
  effect_->SetCameraPos(camera.position());
  effect_->SetEta(azer::Vector4(eta, eta + eta_delta, eta + eta_delta * 2.0f, 0.0f));
}

MyMesh* Load(const ::base::FilePath::StringType& path, azer::RenderSystem* rs) {
  MeshData data;
  if (!LoadMeshData(MESH_PATH, &data, rs)) {
    return false;
  }
  
  MyMesh* mesh = new MyMesh;
  if (mesh->Init(&data, rs)) {
    return mesh;
  } else {
    return NULL;
  }
}
