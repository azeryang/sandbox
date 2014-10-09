#include "sandbox/base/base.h"
#include "base/base.h"

#include "water_simulation.afx.h"
#define EFFECT_GEN_DIR "out/dbg/gen/sandbox/water/vertex_fetch_water/"
#define SHADER_NAME "water_simulation.afx"

bool WaterSimulation::Init(azer::RenderSystem* rs) {
  if (!InitVertex(rs)) {
    return false;
  }
  return true;
}

bool WaterSimulation::InitVertex(azer::RenderSystem* rs) {
  azer::ShaderArray shaders;
  CHECK(azer::LoadVertexShader(EFFECT_GEN_DIR SHADER_NAME ".vs", &shaders));
  CHECK(azer::LoadPixelShader(EFFECT_GEN_DIR SHADER_NAME ".ps", &shaders));
  WaterSimulationEffect* effect = new WaterSimulationEffect(shaders.GetShaderVec(),
                                                            rs);
  effect_.reset(effect);

  WaterSimulationEffect::Vertex v[] = {
    WaterSimulationEffect::Vertex(azer::Vector4(-1.0f,  1.0f, 0.0f, 1.0f ),
                                  azer::Vector2( 0.0f,  0.0f)),
    WaterSimulationEffect::Vertex(azer::Vector4( 1.0f,  1.0f, 0.5f, 1.0f ),
                                  azer::Vector2( 0.0f,  1.0f)),
    WaterSimulationEffect::Vertex(azer::Vector4(-1.0f, -1.0f, 0.0f, 1.0f ),
                                  azer::Vector2( 1.0f,  0.0f)),
    WaterSimulationEffect::Vertex(azer::Vector4( 1.0f,  1.0f, 0.0f, 1.0f ),
                                  azer::Vector2( 1.0f,  1.0f)),
  };
  azer::VertexData vdata(effect_->GetVertexDesc(), ARRAYSIZE(v));
  memcpy(vdata.pointer(), (uint8*)v, sizeof(v));
  vb_.reset(rs->CreateVertexBuffer(azer::VertexBuffer::Options(), &vdata));

  int32 indices[] = {1, 0, 2, 1, 2, 3};
  azer::IndicesData idata(arraysize(indices), azer::IndicesData::kUint32);
  memcpy(idata.pointer(), indices, sizeof(indices));
  ib_.reset(rs->CreateIndicesBuffer(azer::IndicesBuffer::Options(), &idata));

  return true;
}

void WaterSimulationEffect::Render(azer::Renderer* renderer) {
  WaterSimulationEffect* effect = (WaterSimulationEffect*)effect_.get();
  effect->Use(renderer);
  renderer->DrawIndex(vb_.get(), ib_.get(), azer::TriangleList);
}
