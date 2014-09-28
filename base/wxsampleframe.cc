#include "sandbox/base/wxsampleframe.h"

wxSampleFrame::wxSampleFrame(wxSampleApp::Delegate* delegate, const wxString& title,
                         const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , delegate_(delegate)
    , render_loop_on_(false)
    , render_system_(NULL)
    , prev_time_(0)
    , prev_delta_(0)
    , host_(azer::WindowHost::Options()) {
}

bool wxSampleFrame::Init() {
  if (!CreateRender()) {
    return false;
  }

  CreateUI();
  return true;
}

bool wxSampleFrame::CreateRender() {
  panel_ = new AzerCanvas(this, wxID_ANY, wxDefaultPosition,
                          wxSize(800, 600), wxSUNKEN_BORDER);
  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  sizer->Add(panel_, 1, wxEXPAND);
  SetSizer(sizer);

  host_.Attach((azer::window::NativeWindowHandle)panel_->GetHWND());
  if (!azer::LoadRenderSystem(&host_)) {
    return false;
  }
  render_system_ = azer::RenderSystem::Current();
  if (!delegate_->OnInit()) {
    return false;
  }

  delegate_->OnUpdateScene(0.0, 0.0f);
  render_loop_on_ = true;
  return true;
}

wxBEGIN_EVENT_TABLE(wxSampleFrame, wxFrame)
EVT_IDLE(wxSampleFrame::onIdle)
wxEND_EVENT_TABLE()

void wxSampleFrame::onIdle(wxIdleEvent& evt) {
  DCHECK(delegate_ != NULL);
  evt.RequestMore();
  if (render_loop_on_) {
    delegate_->OnRenderScene(prev_time_, prev_delta_);
    render_system_->Present();
    ::base::TimeTicks now = ::base::TimeTicks::HighResNow();
    ::base::TimeDelta delta = now - prev_frame_;
    total_ticking_ += delta;
    prev_frame_ = now;
    prev_time_ = total_ticking_.InSecondsF();
    prev_delta_ = (float)delta.InSecondsF();
    delegate_->OnUpdateScene(prev_time_, prev_delta_);
    prev_frame_ = now;
  }
}

bool AzerRSApp::OnInit() {
  wxSampleApp::Delegate* delegate = sample_->delegate();
  frame_ = delegate->CreateFrame(sample_->title(), wxPoint(50, 50),
                                 wxSize(sample_->width(), sample_->height()));
  if (!frame_->Init()) {
    return false;
  }

  frame_->Show(true);
  return true;
}
