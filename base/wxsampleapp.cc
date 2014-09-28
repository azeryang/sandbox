#include "sandbox/base/sampleapp.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class RenderPanel : public wxPanel {
 public:
  RenderPanel(wxFrame* frame);
  void render();
};

RenderPanel::RenderPanel(wxFrame* frame)
    : wxPanel(frame) {
}

void RenderPanel::render() {
}

class RenderFrame: public wxFrame {
 public:
  RenderFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  void OnRender() {panel_->render();}
 private:
  void OnHello(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);

  RenderPanel* panel_;
  wxDECLARE_EVENT_TABLE();
};
enum {
    ID_Hello = 1
};
wxBEGIN_EVENT_TABLE(RenderFrame, wxFrame)
EVT_MENU(ID_Hello,   RenderFrame::OnHello)
EVT_MENU(wxID_EXIT,  RenderFrame::OnExit)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

RenderFrame::RenderFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
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

  // render panel;
  panel_ = new RenderPanel(this);
  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  sizer->Add(panel_, 1, wxEXPAND);
  SetSizer(sizer);
  Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(MyApp::onIdle));
}
void RenderFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void RenderFrame::OnHello(wxCommandEvent& event) {
  wxLogMessage("Hello world from wxWidgets!");
}

class MyApp: public wxApp {
 public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent& evt);
 private:
  MyFrame* frame_;
  bool render_loop_on_;
};
