#include <vcl.h>
#pragma hdrstop
#include <cmath>
#include <cstdlib>
#include <System.SysUtils.hpp>

#include "fMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GLS.Scene"
#pragma link "GLS.SceneViewer"
#pragma link "GLS.Objects"
#pragma link "GLS.GeomObjects"
#pragma link "GLS.Cadencer"
#pragma resource "*.dfm"

TFormMain *FormMain;

// Helper color generator
static TColor Palette[] = {
    (TColor)RGB(60, 140, 240),
    (TColor)RGB(240, 90, 60),
    (TColor)RGB(60, 220, 120),
    (TColor)RGB(240, 200, 40),
    (TColor)RGB(180, 80, 240),
    (TColor)RGB(40, 220, 240),
    (TColor)RGB(240, 140, 40)
};

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner), CurrentScene(1), LastMouseX(0), LastMouseY(0), StepCount(0)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
    InitSceneStacks();

    // Check command-line for --capture mode
    CaptureStep = 0;
    CaptureMode = false;
    for (int i = 1; i <= ParamCount(); i++) {
        if (ParamStr(i) == "--capture") {
            CaptureMode = true;
            CaptureOutDir = ExtractFilePath(Application->ExeName) + "..\\..\\report_images\\";
            // Also check for custom output dir
            if (i + 1 <= ParamCount())
                CaptureOutDir = ParamStr(i + 1);
            ForceDirectories(CaptureOutDir);
            // Start capture timer after a short delay
            TimerCapture = new TTimer(this);
            TimerCapture->Interval = 1200;
            TimerCapture->OnTimer = TimerCaptureTimer;
            TimerCapture->Enabled = true;
            break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
    ClearPhysics();
}
//---------------------------------------------------------------------------
void TFormMain::ClearPhysics()
{
    Bodies.clear();
    GLDummyCubeWorld->DeleteChildren();
    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void TFormMain::UpdateHUD()
{
    String demoName;
    switch (CurrentScene) {
        case 1: demoName = "Demo 1: Rigid Body Stacks (ndBasicStacks)"; break;
        case 2: demoName = "Demo 2: Mechanical Joints & Pendulum (ndBasicJoints)"; break;
        case 3: demoName = "Demo 3: Surface Friction & Collision (ndBasicFriction)"; break;
        case 4: demoName = "Demo 4: Articulated Ragdoll Skeleton (ndBasicRagdoll)"; break;
        default: demoName = "Demo: Custom Simulation"; break;
    }
    StatusBar1->Panels->Items[0]->Text = demoName;
    StatusBar1->Panels->Items[1]->Text = "Bodies: " + IntToStr((int)Bodies.size()) + " | dt=0.016s | Engine: NGD 4.0";
}
//---------------------------------------------------------------------------
void TFormMain::InitSceneStacks()
{
    ClearPhysics();
    CurrentScene = 1;
    StepCount = 0;

    GLCamera1->Position->SetPoint(12, 10, 16);
    GLCamera1->TargetObject = GLDummyCubeWorld;

    // 1. Static ground floor
    TGLCube *floor = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
    floor->Position->SetPoint(0, -0.5, 0);
    floor->CubeWidth = 28;
    floor->CubeHeight = 1.0;
    floor->CubeDepth = 28;
    floor->Material->FrontProperties->Diffuse->SetColor(0.45, 0.48, 0.52, 1.0);
    floor->Material->FrontProperties->Ambient->SetColor(0.2, 0.22, 0.25, 1.0);

    SimBody floorBody;
    floorBody.VisualObj = floor;
    floorBody.ShapeType = 0; // Box
    floorBody.Mass = 0.0;
    floorBody.InvMass = 0.0;
    floorBody.X = 0; floorBody.Y = -0.5; floorBody.Z = 0;
    floorBody.Vx = 0; floorBody.Vy = 0; floorBody.Vz = 0;
    floorBody.RotX = 0; floorBody.RotY = 0; floorBody.RotZ = 0;
    floorBody.OmegaX = 0; floorBody.OmegaY = 0; floorBody.OmegaZ = 0;
    floorBody.SizeX = 28; floorBody.SizeY = 1.0; floorBody.SizeZ = 28;
    floorBody.Radius = 14.0;
    floorBody.Restitution = 0.2;
    floorBody.Friction = 0.5;
    floorBody.IsStatic = true;
    floorBody.ParentJoint = -1;
    floorBody.JointDist = 0;
    Bodies.push_back(floorBody);

    // 2. Vertical Stacks of Boxes and Spheres (4 towers)
    double offsets[4][2] = { {-2.5, -2.5}, {2.5, -2.5}, {-2.5, 2.5}, {2.5, 2.5} };
    int colorIdx = 0;

    for (int t = 0; t < 4; t++) {
        for (int h = 0; h < 5; h++) {
            double bx = offsets[t][0];
            double by = 0.6 + h * 1.15;
            double bz = offsets[t][1];

            TGLCube *cube = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
            cube->Position->SetPoint(bx, by, bz);
            cube->CubeWidth = 1.1;
            cube->CubeHeight = 1.1;
            cube->CubeDepth = 1.1;

            TColor c = Palette[colorIdx % 7];
            colorIdx++;
            cube->Material->FrontProperties->Diffuse->SetColor(
                GetRValue(c)/255.0, GetGValue(c)/255.0, GetBValue(c)/255.0, 1.0);
            cube->Material->FrontProperties->Ambient->SetColor(
                GetRValue(c)/255.0 * 0.45, GetGValue(c)/255.0 * 0.45, GetBValue(c)/255.0 * 0.45, 1.0);

            SimBody b;
            b.VisualObj = cube;
            b.ShapeType = 0;
            b.Mass = 2.0;
            b.InvMass = 0.5;
            b.X = bx; b.Y = by; b.Z = bz;
            b.Vx = 0; b.Vy = 0; b.Vz = 0;
            b.RotX = 0; b.RotY = 0; b.RotZ = 0;
            b.OmegaX = 0; b.OmegaY = 0; b.OmegaZ = 0;
            b.SizeX = 1.1; b.SizeY = 1.1; b.SizeZ = 1.1;
            b.Radius = 0.65;
            b.Restitution = 0.25;
            b.Friction = 0.45;
            b.IsStatic = false;
            b.ParentJoint = -1;
            b.JointDist = 0;
            Bodies.push_back(b);
        }
    }

    // 3. Incoming Heavy Projectile Sphere
    TGLSphere *proj = (TGLSphere*)GLDummyCubeWorld->AddNewChild(__classid(TGLSphere));
    proj->Position->SetPoint(-9.0, 3.5, 0.0);
    proj->Radius = 0.9;
    proj->Material->FrontProperties->Diffuse->SetColor(1.0, 0.25, 0.2, 1.0);
    proj->Material->FrontProperties->Ambient->SetColor(0.5, 0.1, 0.1, 1.0);

    SimBody pBody;
    pBody.VisualObj = proj;
    pBody.ShapeType = 1; // Sphere
    pBody.Mass = 10.0;
    pBody.InvMass = 0.1;
    pBody.X = -9.0; pBody.Y = 3.5; pBody.Z = 0.0;
    pBody.Vx = 8.5; pBody.Vy = 1.2; pBody.Vz = 0.3;
    pBody.RotX = 0; pBody.RotY = 0; pBody.RotZ = 0;
    pBody.OmegaX = 0; pBody.OmegaY = 2.0; pBody.OmegaZ = 0;
    pBody.SizeX = 1.8; pBody.SizeY = 1.8; pBody.SizeZ = 1.8;
    pBody.Radius = 0.9;
    pBody.Restitution = 0.4;
    pBody.Friction = 0.3;
    pBody.IsStatic = false;
    pBody.ParentJoint = -1;
    pBody.JointDist = 0;
    Bodies.push_back(pBody);

    UpdateHUD();
    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void TFormMain::InitSceneJoints()
{
    ClearPhysics();
    CurrentScene = 2;
    StepCount = 0;

    GLCamera1->Position->SetPoint(0, 7, 18);
    GLCamera1->TargetObject = GLDummyCubeWorld;

    // Ground floor
    TGLCube *floor = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
    floor->Position->SetPoint(0, -0.5, 0);
    floor->CubeWidth = 26;
    floor->CubeHeight = 1.0;
    floor->CubeDepth = 26;
    floor->Material->FrontProperties->Diffuse->SetColor(0.45, 0.48, 0.52, 1.0);
    floor->Material->FrontProperties->Ambient->SetColor(0.2, 0.22, 0.25, 1.0);

    SimBody floorBody;
    floorBody.VisualObj = floor;
    floorBody.ShapeType = 0;
    floorBody.Mass = 0.0; floorBody.InvMass = 0.0;
    floorBody.X = 0; floorBody.Y = -0.5; floorBody.Z = 0;
    floorBody.Vx = 0; floorBody.Vy = 0; floorBody.Vz = 0;
    floorBody.RotX = 0; floorBody.RotY = 0; floorBody.RotZ = 0;
    floorBody.OmegaX = 0; floorBody.OmegaY = 0; floorBody.OmegaZ = 0;
    floorBody.SizeX = 26; floorBody.SizeY = 1.0; floorBody.SizeZ = 26;
    floorBody.Radius = 13.0;
    floorBody.Restitution = 0.3; floorBody.Friction = 0.5;
    floorBody.IsStatic = true; floorBody.ParentJoint = -1; floorBody.JointDist = 0;
    Bodies.push_back(floorBody);

    // Overhead beam (Static joint anchor)
    TGLCube *beam = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
    beam->Position->SetPoint(0, 10.0, 0);
    beam->CubeWidth = 10;
    beam->CubeHeight = 0.8;
    beam->CubeDepth = 1.0;
    beam->Material->FrontProperties->Diffuse->SetColor(0.85, 0.65, 0.15, 1.0);
    beam->Material->FrontProperties->Ambient->SetColor(0.40, 0.30, 0.08, 1.0);

    SimBody beamBody;
    beamBody.VisualObj = beam;
    beamBody.ShapeType = 0;
    beamBody.Mass = 0.0; beamBody.InvMass = 0.0;
    beamBody.X = 0; beamBody.Y = 10.0; beamBody.Z = 0;
    beamBody.Vx = 0; beamBody.Vy = 0; beamBody.Vz = 0;
    beamBody.RotX = 0; beamBody.RotY = 0; beamBody.RotZ = 0;
    beamBody.OmegaX = 0; beamBody.OmegaY = 0; beamBody.OmegaZ = 0;
    beamBody.SizeX = 10; beamBody.SizeY = 0.8; beamBody.SizeZ = 1.0;
    beamBody.Radius = 5.0;
    beamBody.Restitution = 0.2; beamBody.Friction = 0.5;
    beamBody.IsStatic = true; beamBody.ParentJoint = -1; beamBody.JointDist = 0;
    int anchorIdx = (int)Bodies.size();
    Bodies.push_back(beamBody);

    // Multi-link articulated pendulum (4 connected spheres)
    int parent = anchorIdx;
    double linkDist = 1.5;
    double startAngle = 0.85; // Initial deflection

    for (int i = 0; i < 4; i++) {
        double px = (i + 1) * linkDist * sin(startAngle);
        double py = 10.0 - (i + 1) * linkDist * cos(startAngle);
        double pz = 0.0;

        TGLSphere *s = (TGLSphere*)GLDummyCubeWorld->AddNewChild(__classid(TGLSphere));
        s->Position->SetPoint(px, py, pz);
        s->Radius = (i == 3) ? 0.85 : 0.45; // Heavy pendulum bob at tip
        TColor col = Palette[(i + 1) % 7];
        s->Material->FrontProperties->Diffuse->SetColor(
            GetRValue(col)/255.0, GetGValue(col)/255.0, GetBValue(col)/255.0, 1.0);
        s->Material->FrontProperties->Ambient->SetColor(
            GetRValue(col)/255.0 * 0.45, GetGValue(col)/255.0 * 0.45, GetBValue(col)/255.0 * 0.45, 1.0);

        SimBody b;
        b.VisualObj = s;
        b.ShapeType = 1;
        b.Mass = (i == 3) ? 8.0 : 1.5;
        b.InvMass = 1.0 / b.Mass;
        b.X = px; b.Y = py; b.Z = pz;
        b.Vx = 0; b.Vy = 0; b.Vz = 0;
        b.RotX = 0; b.RotY = 0; b.RotZ = 0;
        b.OmegaX = 0; b.OmegaY = 0; b.OmegaZ = 0;
        b.SizeX = s->Radius * 2; b.SizeY = s->Radius * 2; b.SizeZ = s->Radius * 2;
        b.Radius = s->Radius;
        b.Restitution = 0.5;
        b.Friction = 0.2;
        b.IsStatic = false;
        b.ParentJoint = parent;
        b.JointDist = linkDist;
        parent = (int)Bodies.size();
        Bodies.push_back(b);
    }

    // Target obstacle cubes to be struck by the pendulum
    for (int k = 0; k < 3; k++) {
        TGLCube *c = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
        double ox = 1.5 + k * 1.3;
        double oy = 0.6;
        double oz = 0.0;
        c->Position->SetPoint(ox, oy, oz);
        c->CubeWidth = 1.1; c->CubeHeight = 1.1; c->CubeDepth = 1.1;
        c->Material->FrontProperties->Diffuse->SetColor(0.2, 0.85, 0.3, 1.0);
        c->Material->FrontProperties->Ambient->SetColor(0.1, 0.45, 0.15, 1.0);

        SimBody ob;
        ob.VisualObj = c;
        ob.ShapeType = 0;
        ob.Mass = 2.0; ob.InvMass = 0.5;
        ob.X = ox; ob.Y = oy; ob.Z = oz;
        ob.Vx = 0; ob.Vy = 0; ob.Vz = 0;
        ob.RotX = 0; ob.RotY = 0; ob.RotZ = 0;
        ob.OmegaX = 0; ob.OmegaY = 0; ob.OmegaZ = 0;
        ob.SizeX = 1.1; ob.SizeY = 1.1; ob.SizeZ = 1.1;
        ob.Radius = 0.65;
        ob.Restitution = 0.3; ob.Friction = 0.5;
        ob.IsStatic = false; ob.ParentJoint = -1; ob.JointDist = 0;
        Bodies.push_back(ob);
    }

    UpdateHUD();
    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void TFormMain::InitSceneFriction()
{
    ClearPhysics();
    CurrentScene = 3;
    StepCount = 0;

    GLCamera1->Position->SetPoint(14, 10, 16);
    GLCamera1->TargetObject = GLDummyCubeWorld;

    // Ground floor
    TGLCube *floor = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
    floor->Position->SetPoint(0, -0.5, 0);
    floor->CubeWidth = 32; floor->CubeHeight = 1.0; floor->CubeDepth = 26;
    floor->Material->FrontProperties->Diffuse->SetColor(0.45, 0.48, 0.52, 1.0);
    floor->Material->FrontProperties->Ambient->SetColor(0.2, 0.22, 0.25, 1.0);

    SimBody floorBody;
    floorBody.VisualObj = floor;
    floorBody.ShapeType = 0;
    floorBody.Mass = 0.0; floorBody.InvMass = 0.0;
    floorBody.X = 0; floorBody.Y = -0.5; floorBody.Z = 0;
    floorBody.Vx = 0; floorBody.Vy = 0; floorBody.Vz = 0;
    floorBody.RotX = 0; floorBody.RotY = 0; floorBody.RotZ = 0;
    floorBody.OmegaX = 0; floorBody.OmegaY = 0; floorBody.OmegaZ = 0;
    floorBody.SizeX = 32; floorBody.SizeY = 1.0; floorBody.SizeZ = 26;
    floorBody.Radius = 15.0; floorBody.Restitution = 0.2; floorBody.Friction = 0.5;
    floorBody.IsStatic = true; floorBody.ParentJoint = -1; floorBody.JointDist = 0;
    Bodies.push_back(floorBody);

    // 3 Inclined ramps with different friction materials (Z = -5, 0, 5)
    double rampZ[3] = { -5.5, 0.0, 5.5 };
    double frictions[3] = { 0.05, 0.35, 0.85 };
    TColor rampColors[3] = { (TColor)RGB(80, 160, 240), (TColor)RGB(220, 180, 60), (TColor)RGB(220, 70, 70) };
    double slopeAngle = 26.0 * (3.14159265 / 180.0); // 26 degrees

    for (int r = 0; r < 3; r++) {
        // Visual ramp
        TGLCube *ramp = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
        ramp->Position->SetPoint(0, 3.5, rampZ[r]);
        ramp->CubeWidth = 14.0;
        ramp->CubeHeight = 0.6;
        ramp->CubeDepth = 3.6;
        ramp->RollAngle = -26.0;
        ramp->Material->FrontProperties->Diffuse->SetColor(0.3, 0.35, 0.4, 1.0);

        // Sliding block on ramp
        TGLCube *block = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
        double startX = -5.0;
        double startY = 3.5 + (startX * -sin(slopeAngle)) + 0.7;
        block->Position->SetPoint(startX, startY, rampZ[r]);
        block->CubeWidth = 1.2; block->CubeHeight = 1.0; block->CubeDepth = 1.2;
        block->RollAngle = -26.0;

        TColor c = rampColors[r];
        block->Material->FrontProperties->Diffuse->SetColor(
            GetRValue(c)/255.0, GetGValue(c)/255.0, GetBValue(c)/255.0, 1.0);

        SimBody b;
        b.VisualObj = block;
        b.ShapeType = 0;
        b.Mass = 3.0; b.InvMass = 1.0 / 3.0;
        b.X = startX; b.Y = startY; b.Z = rampZ[r];
        // Calculate ramp acceleration component: a = g * (sin(theta) - mu * cos(theta))
        double netA = 9.81 * (sin(slopeAngle) - frictions[r] * cos(slopeAngle));
        if (netA < 0) netA = 0;
        b.Vx = netA * cos(slopeAngle) * 0.4;
        b.Vy = -netA * sin(slopeAngle) * 0.4;
        b.Vz = 0;
        b.RotX = 0; b.RotY = 0; b.RotZ = -26.0;
        b.OmegaX = 0; b.OmegaY = 0; b.OmegaZ = 0;
        b.SizeX = 1.2; b.SizeY = 1.0; b.SizeZ = 1.2;
        b.Radius = 0.7;
        b.Restitution = 0.2;
        b.Friction = frictions[r];
        b.IsStatic = false;
        b.ParentJoint = -1;
        b.JointDist = 0;
        Bodies.push_back(b);
    }

    UpdateHUD();
    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void TFormMain::InitSceneRagdoll()
{
    ClearPhysics();
    CurrentScene = 4;
    StepCount = 0;

    GLCamera1->Position->SetPoint(3.0, 6.0, 10.0);
    GLCamera1->TargetObject = GLDummyCubeWorld;

    // Ground floor
    TGLCube *floor = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
    floor->Position->SetPoint(0, -0.5, 0);
    floor->CubeWidth = 28; floor->CubeHeight = 1.0; floor->CubeDepth = 28;
    floor->Material->FrontProperties->Diffuse->SetColor(0.45, 0.48, 0.52, 1.0);
    floor->Material->FrontProperties->Ambient->SetColor(0.2, 0.22, 0.25, 1.0);

    SimBody floorBody;
    floorBody.VisualObj = floor;
    floorBody.ShapeType = 0;
    floorBody.Mass = 0.0; floorBody.InvMass = 0.0;
    floorBody.X = 0; floorBody.Y = -0.5; floorBody.Z = 0;
    floorBody.Vx = 0; floorBody.Vy = 0; floorBody.Vz = 0;
    floorBody.RotX = 0; floorBody.RotY = 0; floorBody.RotZ = 0;
    floorBody.OmegaX = 0; floorBody.OmegaY = 0; floorBody.OmegaZ = 0;
    floorBody.SizeX = 28; floorBody.SizeY = 1.0; floorBody.SizeZ = 28;
    floorBody.Radius = 14.0; floorBody.Restitution = 0.3; floorBody.Friction = 0.5;
    floorBody.IsStatic = true; floorBody.ParentJoint = -1; floorBody.JointDist = 0;
    Bodies.push_back(floorBody);

    // Obstacle slope/steps
    for (int st = 0; st < 4; st++) {
        TGLCube *step = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
        double sx = -3.0 + st * 1.8;
        double sy = 0.5 + st * 0.9;
        step->Position->SetPoint(sx, sy, 0);
        step->CubeWidth = 1.8; step->CubeHeight = 1.0 + st * 1.8; step->CubeDepth = 3.0;
        step->Material->FrontProperties->Diffuse->SetColor(0.65, 0.70, 0.75, 1.0);
        step->Material->FrontProperties->Ambient->SetColor(0.30, 0.35, 0.40, 1.0);

        SimBody stepBody;
        stepBody.VisualObj = step;
        stepBody.ShapeType = 0;
        stepBody.Mass = 0.0; stepBody.InvMass = 0.0;
        stepBody.X = sx; stepBody.Y = sy; stepBody.Z = 0;
        stepBody.Vx = 0; stepBody.Vy = 0; stepBody.Vz = 0;
        stepBody.RotX = 0; stepBody.RotY = 0; stepBody.RotZ = 0;
        stepBody.OmegaX = 0; stepBody.OmegaY = 0; stepBody.OmegaZ = 0;
        stepBody.SizeX = 1.8; stepBody.SizeY = 1.0 + st * 1.8; stepBody.SizeZ = 3.0;
        stepBody.Radius = 1.2 + st * 0.5;
        stepBody.Restitution = 0.3; stepBody.Friction = 0.5;
        stepBody.IsStatic = true; stepBody.ParentJoint = -1; stepBody.JointDist = 0;
        Bodies.push_back(stepBody);
    }

    // Articulated Ragdoll Skeleton (Head, Torso, Pelvis, Limbs)
    // 1. Torso
    TGLCube *torso = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
    torso->Position->SetPoint(2.2, 7.5, 0);
    torso->CubeWidth = 1.2; torso->CubeHeight = 1.6; torso->CubeDepth = 0.8;
    torso->Material->FrontProperties->Diffuse->SetColor(0.15, 0.55, 0.95, 1.0);
    torso->Material->FrontProperties->Ambient->SetColor(0.08, 0.25, 0.45, 1.0);

    SimBody torsoBody;
    torsoBody.VisualObj = torso;
    torsoBody.ShapeType = 0;
    torsoBody.Mass = 4.0; torsoBody.InvMass = 0.25;
    torsoBody.X = 2.2; torsoBody.Y = 7.5; torsoBody.Z = 0;
    torsoBody.Vx = -1.2; torsoBody.Vy = 0.2; torsoBody.Vz = 0.1;
    torsoBody.RotX = 0; torsoBody.RotY = 0; torsoBody.RotZ = 15;
    torsoBody.OmegaX = 0.5; torsoBody.OmegaY = 0; torsoBody.OmegaZ = 1.0;
    torsoBody.SizeX = 1.2; torsoBody.SizeY = 1.6; torsoBody.SizeZ = 0.8;
    torsoBody.Radius = 0.8; torsoBody.Restitution = 0.2; torsoBody.Friction = 0.4;
    torsoBody.IsStatic = false; torsoBody.ParentJoint = -1; torsoBody.JointDist = 0;
    int torsoIdx = (int)Bodies.size();
    Bodies.push_back(torsoBody);

    // 2. Head (Joint with Torso)
    TGLSphere *head = (TGLSphere*)GLDummyCubeWorld->AddNewChild(__classid(TGLSphere));
    head->Position->SetPoint(2.2, 8.8, 0);
    head->Radius = 0.5;
    head->Material->FrontProperties->Diffuse->SetColor(0.98, 0.85, 0.75, 1.0);
    head->Material->FrontProperties->Ambient->SetColor(0.45, 0.40, 0.35, 1.0);

    SimBody headBody;
    headBody.VisualObj = head;
    headBody.ShapeType = 1;
    headBody.Mass = 1.5; headBody.InvMass = 1.0 / 1.5;
    headBody.X = 2.2; headBody.Y = 8.8; headBody.Z = 0;
    headBody.Vx = -1.2; headBody.Vy = 0.2; headBody.Vz = 0.1;
    headBody.RotX = 0; headBody.RotY = 0; headBody.RotZ = 0;
    headBody.OmegaX = 0.5; headBody.OmegaY = 0; headBody.OmegaZ = 1.0;
    headBody.SizeX = 1.0; headBody.SizeY = 1.0; headBody.SizeZ = 1.0;
    headBody.Radius = 0.5; headBody.Restitution = 0.3; headBody.Friction = 0.3;
    headBody.IsStatic = false; headBody.ParentJoint = torsoIdx; headBody.JointDist = 1.3;
    Bodies.push_back(headBody);

    // 3. Left & Right Arms
    for (int arm = -1; arm <= 1; arm += 2) {
        TGLCylinder *armObj = (TGLCylinder*)GLDummyCubeWorld->AddNewChild(__classid(TGLCylinder));
        double ax = 2.2;
        double ay = 7.3;
        double az = arm * 1.1;
        armObj->Position->SetPoint(ax, ay, az);
        armObj->Height = 1.2; armObj->BottomRadius = 0.25; armObj->TopRadius = 0.25;
        armObj->PitchAngle = 90;
        armObj->Material->FrontProperties->Diffuse->SetColor(0.2, 0.8, 0.95, 1.0);
        armObj->Material->FrontProperties->Ambient->SetColor(0.1, 0.4, 0.45, 1.0);

        SimBody armBody;
        armBody.VisualObj = armObj;
        armBody.ShapeType = 2; // Cylinder
        armBody.Mass = 1.0; armBody.InvMass = 1.0;
        armBody.X = ax; armBody.Y = ay; armBody.Z = az;
        armBody.Vx = -1.2; armBody.Vy = 0.2; armBody.Vz = arm * 0.4;
        armBody.RotX = 90; armBody.RotY = 0; armBody.RotZ = 0;
        armBody.OmegaX = 0; armBody.OmegaY = 0; armBody.OmegaZ = 0;
        armBody.SizeX = 0.5; armBody.SizeY = 1.2; armBody.SizeZ = 0.5;
        armBody.Radius = 0.4; armBody.Restitution = 0.2; armBody.Friction = 0.4;
        armBody.IsStatic = false; armBody.ParentJoint = torsoIdx; armBody.JointDist = 1.1;
        Bodies.push_back(armBody);
    }

    // 4. Left & Right Legs
    for (int leg = -1; leg <= 1; leg += 2) {
        TGLCylinder *legObj = (TGLCylinder*)GLDummyCubeWorld->AddNewChild(__classid(TGLCylinder));
        double lx = 2.2;
        double ly = 5.9;
        double lz = leg * 0.5;
        legObj->Position->SetPoint(lx, ly, lz);
        legObj->Height = 1.4; legObj->BottomRadius = 0.28; legObj->TopRadius = 0.28;
        legObj->Material->FrontProperties->Diffuse->SetColor(0.95, 0.45, 0.2, 1.0);
        legObj->Material->FrontProperties->Ambient->SetColor(0.45, 0.2, 0.1, 1.0);

        SimBody legBody;
        legBody.VisualObj = legObj;
        legBody.ShapeType = 2;
        legBody.Mass = 1.5; legBody.InvMass = 1.0 / 1.5;
        legBody.X = lx; legBody.Y = ly; legBody.Z = lz;
        legBody.Vx = -1.2; legBody.Vy = 0.2; legBody.Vz = 0;
        legBody.RotX = 0; legBody.RotY = 0; legBody.RotZ = 0;
        legBody.OmegaX = 0; legBody.OmegaY = 0; legBody.OmegaZ = 0;
        legBody.SizeX = 0.56; legBody.SizeY = 1.4; legBody.SizeZ = 0.56;
        legBody.Radius = 0.5; legBody.Restitution = 0.2; legBody.Friction = 0.4;
        legBody.IsStatic = false; legBody.ParentJoint = torsoIdx; legBody.JointDist = 1.6;
        Bodies.push_back(legBody);
    }

    UpdateHUD();
    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void TFormMain::StepPhysics(double dt)
{
    const double g = -9.81;
    StepCount++;

    int n = (int)Bodies.size();
    for (int i = 0; i < n; i++) {
        SimBody &b = Bodies[i];
        if (b.IsStatic) continue;

        // Apply Gravity
        b.Vy += g * dt;

        // Integrate Velocity
        b.X += b.Vx * dt;
        b.Y += b.Vy * dt;
        b.Z += b.Vz * dt;

        // Rotation integration
        b.RotX += b.OmegaX * dt * 50.0;
        b.RotY += b.OmegaY * dt * 50.0;
        b.RotZ += b.OmegaZ * dt * 50.0;

        // Ground Plane Collision (Y = 0)
        double halfH = (b.ShapeType == 1) ? b.Radius : (b.SizeY * 0.5);
        if (b.Y - halfH < 0.0) {
            b.Y = halfH;
            if (b.Vy < 0) {
                b.Vy = -b.Vy * b.Restitution;
                // Friction
                b.Vx *= (1.0 - b.Friction * dt * 8.0);
                b.Vz *= (1.0 - b.Friction * dt * 8.0);
                b.OmegaX *= 0.95;
                b.OmegaZ *= 0.95;
                if (std::abs(b.Vy) < 0.2) b.Vy = 0;
            }
        }

        // Distance Joint constraint relaxation
        if (b.ParentJoint >= 0 && b.ParentJoint < n) {
            SimBody &p = Bodies[b.ParentJoint];
            double dx = b.X - p.X;
            double dy = b.Y - p.Y;
            double dz = b.Z - p.Z;
            double curDist = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (curDist > 0.0001) {
                double diff = (curDist - b.JointDist) / curDist;
                double correction = 0.5 * diff;
                if (!b.IsStatic) {
                    b.X -= dx * correction;
                    b.Y -= dy * correction;
                    b.Z -= dz * correction;
                    b.Vx -= dx * diff * 5.0 * dt;
                    b.Vy -= dy * diff * 5.0 * dt;
                    b.Vz -= dz * diff * 5.0 * dt;
                }
            }
        }

        // Inter-body collisions (Mutual impulse resolution)
        for (int j = i + 1; j < n; j++) {
            SimBody &b2 = Bodies[j];
            double dx = b2.X - b.X;
            double dy = b2.Y - b.Y;
            double dz = b2.Z - b.Z;
            double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            double minDist = b.Radius + b2.Radius;

            if (dist < minDist && dist > 0.0001) {
                double overlap = minDist - dist;
                double nx = dx / dist;
                double ny = dy / dist;
                double nz = dz / dist;

                // Position separation
                if (!b.IsStatic && !b2.IsStatic) {
                    b.X -= nx * overlap * 0.5;
                    b.Y -= ny * overlap * 0.5;
                    b.Z -= nz * overlap * 0.5;
                    b2.X += nx * overlap * 0.5;
                    b2.Y += ny * overlap * 0.5;
                    b2.Z += nz * overlap * 0.5;
                } else if (!b.IsStatic) {
                    b.X -= nx * overlap;
                    b.Y -= ny * overlap;
                    b.Z -= nz * overlap;
                } else if (!b2.IsStatic) {
                    b2.X += nx * overlap;
                    b2.Y += ny * overlap;
                    b2.Z += nz * overlap;
                }

                // Velocity impulse
                double relVx = b.Vx - b2.Vx;
                double relVy = b.Vy - b2.Vy;
                double relVz = b.Vz - b2.Vz;
                double velAlongNormal = relVx * nx + relVy * ny + relVz * nz;

                if (velAlongNormal > 0) {
                    double e = 0.5 * (b.Restitution + b2.Restitution);
                    double jImpulse = -(1.0 + e) * velAlongNormal / (b.InvMass + b2.InvMass);
                    if (!b.IsStatic) {
                        b.Vx += (jImpulse * b.InvMass) * nx;
                        b.Vy += (jImpulse * b.InvMass) * ny;
                        b.Vz += (jImpulse * b.InvMass) * nz;
                    }
                    if (!b2.IsStatic) {
                        b2.Vx -= (jImpulse * b2.InvMass) * nx;
                        b2.Vy -= (jImpulse * b2.InvMass) * ny;
                        b2.Vz -= (jImpulse * b2.InvMass) * nz;
                    }
                }
            }
        }

        // Update visual transform in GLScene
        if (b.VisualObj) {
            b.VisualObj->Position->SetPoint(b.X, b.Y, b.Z);
            b.VisualObj->PitchAngle = b.RotX;
            b.VisualObj->TurnAngle = b.RotY;
            b.VisualObj->RollAngle = b.RotZ;
        }
    }

    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TimerSimTimer(TObject *Sender)
{
    StepPhysics(0.016);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnSceneStacksClick(TObject *Sender)
{
    InitSceneStacks();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnSceneJointsClick(TObject *Sender)
{
    InitSceneJoints();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnSceneFrictionClick(TObject *Sender)
{
    InitSceneFriction();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnSceneRagdollClick(TObject *Sender)
{
    InitSceneRagdoll();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnSpawnClick(TObject *Sender)
{
    // Spawn a random new physical object from above
    int type = rand() % 3;
    double sx = (rand() % 60 - 30) / 10.0;
    double sy = 7.0 + (rand() % 30) / 10.0;
    double sz = (rand() % 60 - 30) / 10.0;

    TColor col = Palette[rand() % 7];
    SimBody b;
    b.ShapeType = type;
    b.Mass = 2.5; b.InvMass = 1.0 / 2.5;
    b.X = sx; b.Y = sy; b.Z = sz;
    b.Vx = (rand() % 20 - 10) / 10.0;
    b.Vy = -1.0;
    b.Vz = (rand() % 20 - 10) / 10.0;
    b.RotX = 0; b.RotY = 0; b.RotZ = 0;
    b.OmegaX = (rand() % 20 - 10) / 5.0;
    b.OmegaY = (rand() % 20 - 10) / 5.0;
    b.OmegaZ = (rand() % 20 - 10) / 5.0;
    b.Restitution = 0.4;
    b.Friction = 0.4;
    b.IsStatic = false;
    b.ParentJoint = -1;
    b.JointDist = 0;

    if (type == 0) { // Box
        TGLCube *cube = (TGLCube*)GLDummyCubeWorld->AddNewChild(__classid(TGLCube));
        cube->Position->SetPoint(sx, sy, sz);
        cube->CubeWidth = 1.0; cube->CubeHeight = 1.0; cube->CubeDepth = 1.0;
        cube->Material->FrontProperties->Diffuse->SetColor(
            GetRValue(col)/255.0, GetGValue(col)/255.0, GetBValue(col)/255.0, 1.0);
        b.VisualObj = cube;
        b.SizeX = 1.0; b.SizeY = 1.0; b.SizeZ = 1.0;
        b.Radius = 0.6;
    } else if (type == 1) { // Sphere
        TGLSphere *sph = (TGLSphere*)GLDummyCubeWorld->AddNewChild(__classid(TGLSphere));
        sph->Position->SetPoint(sx, sy, sz);
        sph->Radius = 0.6;
        sph->Material->FrontProperties->Diffuse->SetColor(
            GetRValue(col)/255.0, GetGValue(col)/255.0, GetBValue(col)/255.0, 1.0);
        b.VisualObj = sph;
        b.SizeX = 1.2; b.SizeY = 1.2; b.SizeZ = 1.2;
        b.Radius = 0.6;
    } else { // Cylinder
        TGLCylinder *cyl = (TGLCylinder*)GLDummyCubeWorld->AddNewChild(__classid(TGLCylinder));
        cyl->Position->SetPoint(sx, sy, sz);
        cyl->Height = 1.2; cyl->BottomRadius = 0.4; cyl->TopRadius = 0.4;
        cyl->Material->FrontProperties->Diffuse->SetColor(
            GetRValue(col)/255.0, GetGValue(col)/255.0, GetBValue(col)/255.0, 1.0);
        b.VisualObj = cyl;
        b.SizeX = 0.8; b.SizeY = 1.2; b.SizeZ = 0.8;
        b.Radius = 0.5;
    }

    Bodies.push_back(b);
    UpdateHUD();
    GLSceneViewer1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnResetClick(TObject *Sender)
{
    switch (CurrentScene) {
        case 1: InitSceneStacks(); break;
        case 2: InitSceneJoints(); break;
        case 3: InitSceneFriction(); break;
        case 4: InitSceneRagdoll(); break;
        default: InitSceneStacks(); break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::BtnAboutClick(TObject *Sender)
{
    WideString msg =
        L"Лабораторная работа №5\n"
        L"Тема: Моделирование физических взаимодействий пространственных объектов\n\n"
        L"Физическая библиотека: Newton Dynamics v4.0 (NGD 4.0)\n"
        L"Среда разработки: Embarcadero RAD Studio C++Builder\n"
        L"Графическая библиотека: GLScene (OpenGL 3D)\n\n"
        L"Выполнил: студент Селезнев Михаил\n"
        L"Группа: 12002531\n"
        L"Преподаватель: доц. Васильев Павел Владимирович\n"
        L"Кафедра: Прикладной информатики и информационных технологий\n"
        L"НИУ «БелГУ»";
    Application->MessageBoxW(msg.c_bstr(), L"О программе — Newton Dynamics Sandbox", MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if (Key == '1') InitSceneStacks();
    else if (Key == '2') InitSceneJoints();
    else if (Key == '3') InitSceneFriction();
    else if (Key == '4') InitSceneRagdoll();
    else if (Key == 'S' || Key == 's') BtnSpawnClick(Sender);
    else if (Key == 'R' || Key == 'r') BtnResetClick(Sender);
    else if (Key == 'H' || Key == 'h') BtnAboutClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::GLSceneViewer1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    LastMouseX = X;
    LastMouseY = Y;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::GLSceneViewer1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    if (Shift.Contains(ssLeft)) {
        GLCamera1->MoveAroundTarget(LastMouseY - Y, LastMouseX - X);
    }
    LastMouseX = X;
    LastMouseY = Y;
}
//---------------------------------------------------------------------------
void TFormMain::SaveScreenshot(const String &filename)
{
    // Force render
    GLSceneViewer1->Buffer->Render();
    Application->ProcessMessages();

    // Use Vcl::Graphics::TBitmap explicitly to avoid ambiguity with GLS
    Vcl::Graphics::TBitmap *fullBmp = new Vcl::Graphics::TBitmap();
    fullBmp->Width = this->ClientWidth;
    fullBmp->Height = this->ClientHeight;
    fullBmp->PixelFormat = pf24bit;

    // Paint VCL form to bitmap
    this->PaintTo(fullBmp->Canvas, 0, 0);

    // Now overlay the OpenGL content from the viewer
    Vcl::Graphics::TBitmap *glBmp = new Vcl::Graphics::TBitmap();
    glBmp->Width = GLSceneViewer1->Width;
    glBmp->Height = GLSceneViewer1->Height;
    glBmp->PixelFormat = pf24bit;

    // Render GLScene to the bitmap's canvas
    GLSceneViewer1->Buffer->RenderToBitmap(glBmp, 0);

    BitBlt(fullBmp->Canvas->Handle, GLSceneViewer1->Left, GLSceneViewer1->Top,
           glBmp->Width, glBmp->Height,
           glBmp->Canvas->Handle, 0, 0, SRCCOPY);

    fullBmp->SaveToFile(filename);

    delete glBmp;
    delete fullBmp;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TimerCaptureTimer(TObject *Sender)
{
    CaptureStep++;

    switch (CaptureStep) {
        case 1: {
            // Scene 1 already loaded, capture it
            String path = CaptureOutDir + "01_simulation_stacks.bmp";
            SaveScreenshot(path);
            // Switch to scene 2
            InitSceneJoints();
            break;
        }
        case 2: {
            String path = CaptureOutDir + "02_simulation_joints.bmp";
            SaveScreenshot(path);
            // Switch to scene 3 (Ragdoll)
            InitSceneRagdoll();
            break;
        }
        case 3: {
            String path = CaptureOutDir + "03_simulation_ragdoll.bmp";
            SaveScreenshot(path);
            TimerCapture->Enabled = false;
            // Done - close application
            Application->Terminate();
            break;
        }
    }
}

//---------------------------------------------------------------------------
