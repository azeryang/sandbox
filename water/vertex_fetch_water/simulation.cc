#include "sandbox/water/vertex_fetch_water/simulation.h"

#include "sandbox/base/base.h"
#include "base/base.h"
#include "azer/render/render.h"

#include "water_simulation.afx.h"
#include "quad.afx.h"
#define EFFECT_GEN_DIR "out/dbg/gen/sandbox/water/vertex_fetch_water/"
#define SHADER_NAME "water_simulation.afx"
#define SHADER2_NAME "quad.afx"

#define DAMPENING_TERTEX  FILE_PATH_LITERAL("sandbox/media/textures/dampening.tga")

bool WaterSimulation::Init(azer::RenderSystem* rs) {
  if (!InitVertex(rs)) {
    return false;
  }

  azer::ShaderArray shaders;
  CHECK(azer::LoadVertexShader(EFFECT_GEN_DIR SHADER2_NAME ".vs", &shaders));
  CHECK(azer::LoadPixelShader(EFFECT_GEN_DIR SHADER2_NAME ".ps", &shaders));
  QuadEffect* effect = new QuadEffect(shaders.GetShaderVec(), rs);
  quad_effect_.reset(effect);
  dampening_tex_.reset(azer::Texture::LoadShaderTexture(DAMPENING_TERTEX, rs));

  for (uint32 i = 0; i < arraysize(target_); ++i) {
    target_[i].reset(new azer::TexRenderTarget(800, 600));
    target_[i]->Init(rs);
    target_[i]->Begin(azer::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
  }

  azer::Renderer* renderer = rs->GetDefaultRenderer();
  renderer->Use();
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
    WaterSimulationEffect::Vertex(azer::Vector4( 1.0f,  1.0f, 0.0f, 1.0f ),
                                  azer::Vector2( 0.0f,  1.0f)),
    WaterSimulationEffect::Vertex(azer::Vector4(-1.0f, -1.0f, 0.0f, 1.0f ),
                                  azer::Vector2( 1.0f,  0.0f)),
    WaterSimulationEffect::Vertex(azer::Vector4( 1.0f, -1.0f, 0.0f, 1.0f ),
                                  azer::Vector2( 1.0f,  1.0f)),
  };
  azer::VertexData vdata(effect->GetVertexDesc(), ARRAYSIZE(v));
  memcpy(vdata.pointer(), (uint8*)v, sizeof(v));
  vb_.reset(rs->CreateVertexBuffer(azer::VertexBuffer::Options(), &vdata));

  int32 indices[] = {1, 0, 2, 1, 2, 3};
  azer::IndicesData idata(arraysize(indices), azer::IndicesData::kUint32);
  memcpy(idata.pointer(), indices, sizeof(indices));
  ib_.reset(rs->CreateIndicesBuffer(azer::IndicesBuffer::Options(), &idata));

  return true;
}

void WaterSimulation::Render(azer::Renderer* renderer) {
  renderer->Use();
  quad_effect_->Use(renderer);
  renderer->DrawIndex(vb_.get(), ib_.get(), azer::kTriangleList);
}

void WaterSimulation::CalcSimulation() {
  azer::Vector4 color(0.0f, 0.0f, 0.0f, 0.0f);
  azer::TexRenderTargetPtr prev = target_[index_ % 3];
  azer::TexRenderTargetPtr curr = target_[(index_ + 1) % 3];
  azer::Renderer* renderer = target_[(index_ + 2) % 3]->Begin(color);
  WaterSimulationEffect* effect = (WaterSimulationEffect*)effect_.get();
  effect->SetPrevHeight(prev->GetRTTex());
  effect->SetCurrHeight(curr->GetRTTex());
  effect->SetDampenTex(dampening_tex_);
  effect->SetWaterSize(azer::Vector4(100.0f, 0.0f, 100.0f, 0.0f));
  effect->SetPositionWeight(azer::Vector4(1.99f, 0.99f, 0.0f, 0.0f));
  effect->SetWaveSpeed(azer::Vector4(0.0f, 10.0f, 0.0f, 0.0f));
  effect->SetSampleUnit(azer::Vector2(1.0f / 800.0f, 1.0f / 600.0f));
  effect->Use(renderer);
  renderer->DrawIndex(vb_.get(), ib_.get(), azer::kTriangleList);
  index_++;
}
