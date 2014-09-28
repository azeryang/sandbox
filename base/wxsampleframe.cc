#include "sandbox/base/wxsampleframe.h"

enum {
 ID_Hello = 1
};

wxSampleFrame::wxSampleFrame(wxSampleApp::Delegate* delegate, const wxString& title,
                         const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , delegate_(delegate)
    , render_loop_on_(false)
    , render_system_(NULL)
    , prev_time_(0)
    , prev_delta_(0)
    , host_(azer::WindowHost::Options()) {
  CreateUI();
}

bool wxSampleFrame::Init() {
  if (!CreateRender()) {
    return false;
  }

  return true;
}

bool wxSampleFrame::CreateRender() {
  panel_ = new RenderPanel(this);
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

void wxSampleFrame::CreateUI() {
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                   "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);
  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuHelp, "&Help" );
  SetMenuBar( menuBar );
  CreateStatusBar();
  SetStatusText( "Welcome to wxWidgets!" );
}

void wxSampleFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void wxSampleFrame::OnHello(wxCommandEvent& event) {
  wxLogMessage("Hello world from wxWidgets!");
}
wxBEGIN_EVENT_TABLE(wxSampleFrame, wxFrame)
EVT_MENU(ID_Hello,   wxSampleFrame::OnHello)
EVT_MENU(wxID_EXIT,  wxSampleFrame::OnExit)
EVT_IDLE(wxSampleFrame::onIdle)
wxEND_EVENT_TABLE()

void wxSampleFrame::onIdle(wxIdleEvent& evt) {
  DCHECK(delegate_ != NULL);
  evt.RequestMore();
  if (render_loop_on_) {
    delegate_->OnRenderScene(prev_time_, prev_delta_);
  
    ::base::TimeTicks now = ::base::TimeTicks::HighResNow();
    ::base::TimeDelta delta = now - prev_frame_;
    total_ticking_ += delta;
    prev_frame_ = now;
    prev_time_ = total_ticking_.InSecondsF();
    prev_delta_ = (float)delta.InSecondsF();
    delegate_->OnUpdateScene(prev_time_, prev_delta_);
  
    render_system_->Present();
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
