#include "sandbox/water/vertex_fetch_water/simulation.h"

#include "sandbox/base/base.h"
#include "base/base.h"
#include "azer/render/render.h"

#include "water_simulation.afx.h"
#include "water_perturb.afx.h"
#include "quad.afx.h"
#define EFFECT_GEN_DIR "out/dbg/gen/sandbox/water/vertex_fetch_water/"
#define SHADER_NAME "water_simulation.afx"
#define SHADER2_NAME "quad.afx"
#define PERTURB_NAME "water_perturb.afx"

#define DAMPENING_TERTEX  FILE_PATH_LITERAL("sandbox/media/textures/dampening.tga")
#define BRUSH_TERTEX  FILE_PATH_LITERAL("sandbox/media/textures/brush.tga")

bool WaterSimulation::Init(azer::RenderSystem* rs) {
  if (!InitVertex(rs)) {
    return false;
  }

  {
    azer::ShaderArray shaders;
    CHECK(azer::LoadVertexShader(EFFECT_GEN_DIR SHADER2_NAME ".vs", &shaders));
    CHECK(azer::LoadPixelShader(EFFECT_GEN_DIR SHADER2_NAME ".ps", &shaders));
    QuadEffect* effect = new QuadEffect(shaders.GetShaderVec(), rs);
    quad_effect_.reset(effect);
  }

  {
    azer::ShaderArray shaders;
    CHECK(azer::LoadVertexShader(EFFECT_GEN_DIR PERTURB_NAME ".vs", &shaders));
    CHECK(azer::LoadPixelShader(EFFECT_GEN_DIR PERTURB_NAME ".ps", &shaders));
    WaterPerturbEffect* effect = new WaterPerturbEffect(shaders.GetShaderVec(), rs);
    perturb_effect_.reset(effect);
  }
  
  dampening_tex_.reset(azer::Texture::LoadShaderTexture(DAMPENING_TERTEX, rs));
  brush_tex_.reset(azer::Texture::LoadShaderTexture(BRUSH_TERTEX, rs));

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

void WaterSimulation::Render(azer::Renderer* renderer,
                             double time, float delta) {
  CalcSimulation(time, delta);
  renderer->Use();
  QuadEffect* effect = (QuadEffect*)quad_effect_.get();
  effect->SetTexture(rttex());
  quad_effect_->Use(renderer);
  
  renderer->DrawIndex(vb_.get(), ib_.get(), azer::kTriangleList);
  index_++;
}

void WaterSimulation::RenderPerturb(azer::Renderer* renderer,
                                    double time, float delta) {
  // draw perturb
  float waketime = time - 0.55f;
  float x = 0.5f * sin(waketime);
  float y = -0.5f * cos(waketime);
  float u[2], v[2];
  v[0] = -0.1f * cos(waketime);
  v[1] = -0.1f * sin(waketime);
  u[0] = -0.1f * -sin(waketime);
  u[1] = -0.1f * cos(waketime);

  WaterPerturbEffect* effect = (WaterPerturbEffect*)perturb_effect_.get();
  effect->SetPerturbTex(brush_tex_);
  // effect->SetDeltaTime(0.3f);
  effect->SetPosition(azer::Vector4(x, y, 0.0f, 1.0f));
  effect->SetDeltaPos(azer::Vector4(u[0], v[0], u[1], v[1]));
  effect->Use(renderer);
  renderer->DrawIndex(vb_.get(), ib_.get(), azer::kTriangleList);
}

void WaterSimulation::CalcSimulation(double time, float delta) {
  azer::Vector4 color(0.0f, 0.0f, 0.0f, 0.0f);
  azer::Renderer* renderer = RenderTarget();
  renderer->EnableDepthTest(false);
  WaterSimulationEffect* effect = (WaterSimulationEffect*)effect_.get();
  effect->SetPrevHeight(prev_height());
  effect->SetCurrHeight(curr_height());
  effect->SetDampenTex(dampening_tex_);
  effect->SetWaterSize(azer::Vector4(100.0f, 0.0f, 100.0f, 0.0f));
  effect->SetPositionWeight(azer::Vector4(1.99f, 0.99f, 0.0f, 0.0f));
  effect->SetWaveSpeed(azer::Vector4(10.0f, 10.0f, 0.0f, 0.0f));
  effect->SetSampleUnit(azer::Vector2(0.001, 0.001));
  effect->SetDeltaTime(azer::Vector4(0.005f, 0.005f, 0.0f, 0.0f));
  effect->Use(renderer);
  renderer->DrawIndex(vb_.get(), ib_.get(), azer::kTriangleList);

  RenderPerturb(renderer, time, delta);
  renderer->EnableDepthTest(true);
}

azer::TexturePtr& WaterSimulation::prev_height() {
  int index = index_ % 3;
  return target_[index]->GetRTTex();
}

azer::TexturePtr& WaterSimulation::curr_height() {
  int index = (index_ + 1) % 3;
  return target_[index]->GetRTTex();
}

azer::TexturePtr& WaterSimulation::rttex() {
  int index = (index_ + 2) % 3;
  return target_[index]->GetRTTex();
}

azer::Renderer* WaterSimulation::RenderTarget() {
  int index = (index_ + 2) % 3;
  return target_[index]->Begin(color);
}
