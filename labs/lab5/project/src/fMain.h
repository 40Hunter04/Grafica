//---------------------------------------------------------------------------
#ifndef fMainH
#define fMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <vector>

#include "GLS.Scene.hpp"
#include "GLS.SceneViewer.hpp"
#include "GLS.Objects.hpp"
#include "GLS.GeomObjects.hpp"
#include "GLS.Cadencer.hpp"
#include "Stage.Coordinates.hpp"
#include "Stage.BaseClasses.hpp"
#include "Stage.VectorGeometry.hpp"

//---------------------------------------------------------------------------
struct SimBody {
    TGLBaseSceneObject *VisualObj;
    int ShapeType; // 0=Box, 1=Sphere, 2=Cylinder, 3=Capsule
    double Mass;
    double InvMass;
    double X, Y, Z;
    double Vx, Vy, Vz;
    double RotX, RotY, RotZ;
    double OmegaX, OmegaY, OmegaZ;
    double SizeX, SizeY, SizeZ;
    double Radius;
    double Restitution;
    double Friction;
    bool IsStatic;
    int ParentJoint;
    double JointDist;
};

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelTop;
	TPanel *PanelRight;
	TStatusBar *StatusBar1;
	TGLScene *GLScene1;
	TGLSceneViewer *GLSceneViewer1;
	TGLCadencer *GLCadencer1;
	TGLCamera *GLCamera1;
	TGLLightSource *GLLightSource1;
	TGLLightSource *GLLightSource2;
	TGLDummyCube *GLDummyCubeWorld;
	TGLDummyCube *GLDummyCubeLight;
	TTimer *TimerSim;
	TButton *BtnSceneStacks;
	TButton *BtnSceneJoints;
	TButton *BtnSceneFriction;
	TButton *BtnSceneRagdoll;
	TButton *BtnSpawn;
	TButton *BtnReset;
	TButton *BtnAbout;
	TLabel *LabelTitle;
	TLabel *LabelStudent;
	TLabel *LabelMode;
	TLabel *LabelStats;
	TLabel *LabelHelp;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BtnSceneStacksClick(TObject *Sender);
	void __fastcall BtnSceneJointsClick(TObject *Sender);
	void __fastcall BtnSceneFrictionClick(TObject *Sender);
	void __fastcall BtnSceneRagdollClick(TObject *Sender);
	void __fastcall BtnSpawnClick(TObject *Sender);
	void __fastcall BtnResetClick(TObject *Sender);
	void __fastcall BtnAboutClick(TObject *Sender);
	void __fastcall TimerSimTimer(TObject *Sender);
	void __fastcall GLSceneViewer1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall GLSceneViewer1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

private:	// User declarations
	std::vector<SimBody> Bodies;
	int CurrentScene; // 1=Stacks, 2=Joints, 3=Friction, 4=Ragdoll
	int LastMouseX, LastMouseY;
	int StepCount;

	// Auto-capture mode
	bool CaptureMode;
	int CaptureStep;
	String CaptureOutDir;
	TTimer *TimerCapture;

	void ClearPhysics();
	void InitSceneStacks();
	void InitSceneJoints();
	void InitSceneFriction();
	void InitSceneRagdoll();
	void StepPhysics(double dt);
	void UpdateHUD();
	void SaveScreenshot(const String &filename);
	void __fastcall TimerCaptureTimer(TObject *Sender);

public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
