// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
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

class MyFrame;
class MyApp: public wxApp {
 public:
  virtual bool OnInit();
  void onIdle(wxIdleEvent& evt);
 private:
  MyFrame* frame_;
  bool render_loop_on_;
};

class MyFrame: public wxFrame {
 public:
  MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
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
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello,   MyFrame::OnHello)
EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  render_loop_on_ = false;
  frame_ = new MyFrame("Hello World", wxPoint(50, 50), wxSize(800, 600));
  render_loop_on_ = true;
  frame_->Show(true);
  return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
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
void MyFrame::OnExit(wxCommandEvent& event) {
  Close(true);
}

void MyFrame::OnHello(wxCommandEvent& event) {
  wxLogMessage("Hello world from wxWidgets!");
}

void MyApp::onIdle(wxIdleEvent& evt) {
  if (render_loop_on_) {
    frame_->OnRender();
    evt.RequestMore();
  }
}
