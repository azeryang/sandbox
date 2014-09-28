#include "sandbox/base/wxsampleapp.h"
#include "sandbox/base/wxsampleframe.h"
#include "base/base.h"

enum {
  ID_Hello = 1
};

class MyFrame: public wxSampleFrame {
 public:
  MyFrame(wxSampleApp::Delegate* delegate, const wxString& title,
          const wxPoint& pos, const wxSize& size)
      : wxSampleFrame(delegate, title, pos, size) {
  }
  virtual void CreateUI();
  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
 private:
  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxSampleFrame)
EVT_MENU(ID_Hello,   MyFrame::OnHello)
EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
wxEND_EVENT_TABLE()

class MainDelegate : public wxSampleApp::Delegate {
 public:
  virtual bool OnInit() {return true;}
  virtual void OnUpdateScene(double time, float delta_time) {}
  virtual void OnRenderScene(double time, float delta_time) {
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    azer::Renderer* renderer = rs->GetDefaultRenderer();
    renderer->Use();
    renderer->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    renderer->ClearDepthAndStencil();
  }
  virtual void OnQuit() {}
  virtual wxSampleFrame* CreateFrame(const wxString& title,
                                     const wxPoint& pos, const wxSize& size) {
    return new MyFrame(this, title, pos, size);
  }
};

int main(int argc, char* argv[]) {
  ::base::InitApp(&argc, &argv, "");
  MainDelegate delegate;
  wxSampleApp app(&delegate);
  wxSampleApp::MainLoop(argc, argv, &app);
  return 0;
}

void MyFrame::CreateUI() {
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

void MyFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void MyFrame::OnHello(wxCommandEvent& event) {
  wxLogMessage("Hello world from wxWidgets!");
}
