object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'Newton Dynamics 4.0 Physics Sandbox '#8212' '#1057#1080#1076#1077#1085#1082#1086' '#1042#1072#1083#1077#1088#1080#1103' '#1043#1077#1085#1085#1072#1076#1100#1077#1074#1085#1072' ('#1075#1088'. 12002531)'
  ClientHeight = 600
  ClientWidth = 1000
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  KeyPreview = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 578
    Width = 1000
    Height = 22
    Panels = <
      item
        Text = 'Demo: [1] Stacks'
        Width = 310
      end
      item
        Text = 'Rigid Bodies: 0 | Step: dt=0.016s'
        Width = 270
      end
      item
        Text = #1057#1090#1091#1076#1077#1085#1090#1082#1072': '#1057#1080#1076#1077#1085#1082#1086' '#1042#1072#1083#1077#1088#1080#1103' '#1043#1077#1085#1085#1072#1076#1100#1077#1074#1085#1072' ('#1075#1088'. 12002531)'
        Width = 420
      end>
  end
  object PanelTop: TPanel
    Left = 0
    Top = 0
    Width = 1000
    Height = 42
    Align = alTop
    BevelOuter = bvNone
    Color = 2105376
    ParentBackground = False
    TabOrder = 1
    object LabelTitle: TLabel
      Left = 14
      Top = 11
      Width = 230
      Height = 19
      Caption = 'NEWTON DYNAMICS 4.0 (NGD)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 65535
      Font.Height = -14
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelStudent: TLabel
      Left = 410
      Top = 12
      Width = 575
      Height = 17
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = #1053#1048#1059' '#171#1041#1077#1083#1043#1059#187' | '#1058#1054#1050#1052' | '#1057#1090#1091#1076#1077#1085#1090#1082#1072': '#1057#1080#1076#1077#1085#1082#1086' '#1042#1072#1083#1077#1088#1080#1103' '#1043#1077#1085#1085#1072#1076#1100#1077#1074#1085#1072' ('#1075#1088'. 12002531)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 15790320
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object PanelRight: TPanel
    Left = 780
    Top = 42
    Width = 220
    Height = 536
    Align = alRight
    BevelOuter = bvNone
    Color = 2697513
    ParentBackground = False
    TabOrder = 2
    object LabelMode: TLabel
      Left = 14
      Top = 12
      Width = 190
      Height = 15
      Caption = #1060#1080#1079#1080#1095#1077#1089#1082#1080#1077' '#1089#1094#1077#1085#1099' (NGD v4.0):'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 15790320
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelStats: TLabel
      Left = 14
      Top = 230
      Width = 190
      Height = 15
      Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077' '#1089#1080#1084#1091#1083#1103#1094#1080#1077#1081':'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 15790320
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabelHelp: TLabel
      Left = 14
      Top = 410
      Width = 190
      Height = 100
      AutoSize = False
      Caption = #1043#1086#1088#1103#1095#1080#1077' '#1082#1083#1072#1074#1080#1096#1080':'#13#10'[1] '#8212' Rigid Body Stacks'#13#10'[2] '#8212' Mechanical Joints'#13#10'[3] '#8212' Friction & Collisions'#13#10'[4] '#8212' Ragdoll Skeleton'#13#10'[S] '#8212' '#1057#1073#1088#1086#1089#1080#1090#1100' '#1086#1073#1098#1077#1082#1090#13#10'[R] '#8212' '#1055#1077#1088#1077#1079#1072#1087#1091#1089#1082
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 12632256
      Font.Height = -11
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object BtnSceneStacks: TButton
      Left = 14
      Top = 38
      Width = 190
      Height = 36
      Caption = '1. Stacks (ndBasicStacks)'
      TabOrder = 0
      OnClick = BtnSceneStacksClick
    end
    object BtnSceneJoints: TButton
      Left = 14
      Top = 82
      Width = 190
      Height = 36
      Caption = '2. Joints (ndBasicJoints)'
      TabOrder = 1
      OnClick = BtnSceneJointsClick
    end
    object BtnSceneFriction: TButton
      Left = 14
      Top = 126
      Width = 190
      Height = 36
      Caption = '3. Friction (ndBasicFriction)'
      TabOrder = 2
      OnClick = BtnSceneFrictionClick
    end
    object BtnSceneRagdoll: TButton
      Left = 14
      Top = 170
      Width = 190
      Height = 36
      Caption = '4. Ragdoll (ndBasicRagdoll)'
      TabOrder = 3
      OnClick = BtnSceneRagdollClick
    end
    object BtnSpawn: TButton
      Left = 14
      Top = 256
      Width = 190
      Height = 34
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1090#1077#1083#1086' (Spawn Body)'
      TabOrder = 4
      OnClick = BtnSpawnClick
    end
    object BtnReset: TButton
      Left = 14
      Top = 298
      Width = 190
      Height = 34
      Caption = #1057#1073#1088#1086#1089' / '#1055#1077#1088#1077#1079#1072#1087#1091#1089#1082' (Reset)'
      TabOrder = 5
      OnClick = BtnResetClick
    end
    object BtnAbout: TButton
      Left = 14
      Top = 350
      Width = 190
      Height = 34
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077'...'
      TabOrder = 6
      OnClick = BtnAboutClick
    end
  end
  object GLSceneViewer1: TGLSceneViewer
    Left = 0
    Top = 42
    Width = 780
    Height = 536
    Camera = GLCamera1
    Buffer.BackgroundColor = 7829367
    FieldOfView = 140.000000000000000000
    PenAsTouch = False
    Align = alClient
    OnMouseDown = GLSceneViewer1MouseDown
    OnMouseMove = GLSceneViewer1MouseMove
    TabOrder = 0
  end
  object GLScene1: TGLScene
    Left = 40
    Top = 64
    object GLCamera1: TGLCamera
      DepthOfView = 500.000000000000000000
      FocalLength = 50.000000000000000000
      TargetObject = GLDummyCubeWorld
      Position.Coordinates = {00002041000000410000A0410000803F}
    end
    object GLDummyCubeWorld: TGLDummyCube
      ShowAxes = False
      CubeSize = 1.000000000000000000
      VisibleAtRunTime = True
    end
    object GLLightSource1: TGLLightSource
      Ambient.Color = {CDCC4C3FCDCC4C3FCDCC4C3F0000803F}
      ConstAttenuation = 1.000000000000000000
      Diffuse.Color = {0000803F0000803F0000803F0000803F}
      Position.Coordinates = {0000204100004041000020410000803F}
      LightStyle = lsParallel
      Specular.Color = {0000803F0000803F0000803F0000803F}
      SpotCutOff = 180.000000000000000000
    end
    object GLLightSource2: TGLLightSource
      Ambient.Color = {9A99193F9A99193F9A99193F0000803F}
      ConstAttenuation = 1.000000000000000000
      Diffuse.Color = {CDCC4C3FCDCC4C3FCDCC4C3F0000803F}
      Position.Coordinates = {000020C1000020410000A0C00000803F}
      LightStyle = lsParallel
      SpotCutOff = 180.000000000000000000
    end
  end
  object GLCadencer1: TGLCadencer
    Scene = GLScene1
    Left = 120
    Top = 64
  end
  object TimerSim: TTimer
    Interval = 16
    OnTimer = TimerSimTimer
    Left = 200
    Top = 64
  end
end
