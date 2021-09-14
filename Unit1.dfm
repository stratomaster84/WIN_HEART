object Form1: TForm1
  Left = 192
  Top = 140
  BorderStyle = bsNone
  Caption = #1057#1077#1088#1076#1094#1077'!!!'
  ClientHeight = 426
  ClientWidth = 688
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu1: TMainMenu
    Left = 16
    Top = 24
    object File1: TMenuItem
      Caption = #1042#1082#1083'/'#1074#1099#1082#1083
      object N1: TMenuItem
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1082#1088#1072#1089#1085#1099#1081' '#1094#1074#1077#1090' - F1'
        OnClick = N1Click
      end
      object N2: TMenuItem
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1092#1086#1085#1086#1074#1099#1081' '#1088#1080#1089#1091#1085#1086#1082' - F2'
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1080#1089#1090#1086#1095#1085#1080#1082' '#1089#1074#1077#1090#1072' - F3'
        OnClick = N3Click
      end
      object N4: TMenuItem
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1079#1077#1088#1082#1072#1083#1100#1085#1086#1077' '#1086#1090#1088#1072#1078#1077#1085#1080#1077' - F4'
        OnClick = N4Click
      end
      object F51: TMenuItem
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1102' '#1090#1077#1082#1089#1090#1091#1088#1099' - F5'
        OnClick = F51Click
      end
      object F61: TMenuItem
        Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1101#1092#1092#1077#1082#1090' "'#1054#1087#1100#1103#1085#1085#1105#1085#1085#1086#1077' '#1089#1077#1088#1076#1094#1077'" - F6'
        OnClick = F61Click
      end
    end
    object N5: TMenuItem
      Caption = #1052#1072#1085#1080#1087#1091#1083#1103#1094#1080#1080
      object N6: TMenuItem
        Caption = #1059#1074#1077#1083#1080#1095#1080#1090#1100' '#1089#1077#1088#1076#1094#1077' - '#39'+'#39
        OnClick = N6Click
      end
      object N7: TMenuItem
        Caption = #1059#1084#1077#1085#1100#1096#1080#1090#1100' '#1089#1077#1088#1076#1094#1077' - '#39'-'#39
        OnClick = N7Click
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object Left1: TMenuItem
        Caption = #1042#1088#1072#1097#1072#1090#1100' '#1080#1089#1090#1086#1095#1085#1080#1082' '#1089#1074#1077#1090#1072' '#1087#1086' '#1095#1072#1089#1086#1074#1086#1081' '#1089#1090#1088#1077#1083#1082#1077' - Left'
        Enabled = False
        OnClick = Left1Click
      end
      object Right1: TMenuItem
        Caption = #1042#1088#1072#1097#1072#1090#1100' '#1080#1089#1090#1086#1095#1085#1080#1082' '#1089#1074#1077#1090#1072' '#1087#1088#1086#1090#1080#1074' '#1095#1072#1089#1086#1074#1086#1081' '#1089#1090#1088#1077#1083#1082#1080' - Right'
        Enabled = False
        OnClick = Right1Click
      end
    end
    object MotionBlur1: TMenuItem
      Caption = #1069#1092#1092#1077#1082#1090' "'#1054#1087#1100#1103#1085#1085#1105#1085#1085#1086#1077' '#1089#1077#1088#1076#1094#1077'"'
      Enabled = False
      object N9: TMenuItem
        Caption = #1059#1074#1077#1083#1080#1095#1080#1090#1100' '#1089#1090#1077#1087#1077#1085#1100' '#1086#1087#1100#1103#1085#1077#1085#1080#1103' - Up'
        OnClick = N9Click
      end
      object SubAccum01051: TMenuItem
        Caption = #1059#1084#1077#1085#1100#1096#1080#1090#1100' '#1089#1090#1077#1087#1077#1085#1100' '#1086#1087#1100#1103#1085#1077#1085#1080#1103' - Down'
        OnClick = SubAccum01051Click
      end
    end
    object Exit1: TMenuItem
      Caption = #1042#1099#1093#1086#1076
      OnClick = Exit1Click
    end
  end
end
