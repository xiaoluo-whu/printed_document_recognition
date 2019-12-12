object Form1: TForm1
  Left = 370
  Top = -18
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #32441#29702#21512#25104#31243#24207
  ClientHeight = 746
  ClientWidth = 891
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 400
    Top = 152
    Width = 89
    Height = 13
    Caption = #35831#36755#20837'N'#30340#20540#65306'   '
  end
  object BitBtn2: TBitBtn
    Left = 208
    Top = 136
    Width = 177
    Height = 41
    Caption = #21512#25104'N'#24133#32441#29702#22270#20687
    TabOrder = 0
    OnClick = BitBtn2Click
  end
  object BitBtn3: TBitBtn
    Left = 288
    Top = 96
    Width = 129
    Height = 41
    Caption = #20851#38381#31243#24207
    TabOrder = 1
    OnClick = BitBtn3Click
  end
  object BitBtn4: TBitBtn
    Left = 16
    Top = 184
    Width = 177
    Height = 41
    Caption = #25552#21462#26816#26448'FT'#29305#24449#24182#35745#31639#24179#22343#20540
    TabOrder = 2
    OnClick = BitBtn4Click
  end
  object ListBox1: TListBox
    Left = 16
    Top = 240
    Width = 177
    Height = 465
    ItemHeight = 13
    TabOrder = 3
  end
  object BitBtn5: TBitBtn
    Left = 208
    Top = 184
    Width = 177
    Height = 41
    Caption = #25552#21462#26816#26448'GLCM'#29305#24449#24182#35745#31639#24179#22343#20540
    TabOrder = 4
    OnClick = BitBtn5Click
  end
  object ListBox2: TListBox
    Left = 208
    Top = 240
    Width = 177
    Height = 465
    ItemHeight = 13
    TabOrder = 5
  end
  object ListBox3: TListBox
    Left = 504
    Top = 238
    Width = 177
    Height = 467
    ItemHeight = 13
    TabOrder = 6
  end
  object ListBox4: TListBox
    Left = 696
    Top = 238
    Width = 177
    Height = 467
    ItemHeight = 13
    TabOrder = 7
  end
  object BitBtn10: TBitBtn
    Left = 144
    Top = 16
    Width = 273
    Height = 41
    Caption = #35745#31639#26816#26448#19982#26679#26412'FT'#29305#24449#21521#37327#27431#27663#36317#31163
    TabOrder = 8
    OnClick = BitBtn10Click
  end
  object BitBtn11: TBitBtn
    Left = 584
    Top = 16
    Width = 273
    Height = 41
    Caption = #35745#31639#26816#26448#19982#26679#26412'GLCM'#29305#24449#21521#37327#27431#27663#36317#31163
    TabOrder = 9
    OnClick = BitBtn11Click
  end
  object Edit1: TEdit
    Left = 144
    Top = 72
    Width = 273
    Height = 21
    TabOrder = 10
  end
  object Edit2: TEdit
    Left = 584
    Top = 72
    Width = 273
    Height = 21
    TabOrder = 11
  end
  object BitBtn7: TBitBtn
    Left = 696
    Top = 136
    Width = 177
    Height = 41
    Caption = #21512#25104'N'#24133#32441#29702#22270#20687
    TabOrder = 12
    OnClick = BitBtn7Click
  end
  object BitBtn8: TBitBtn
    Left = 504
    Top = 184
    Width = 177
    Height = 41
    Caption = #25552#21462#26679#26412'FT'#29305#24449#24182#35745#31639#24179#22343#20540
    TabOrder = 13
    OnClick = BitBtn8Click
  end
  object BitBtn9: TBitBtn
    Left = 696
    Top = 184
    Width = 177
    Height = 41
    Caption = #25552#21462#26679#26412'GLCM'#29305#24449#24182#35745#31639#24179#22343#20540
    TabOrder = 14
    OnClick = BitBtn9Click
  end
  object Edit3: TEdit
    Left = 392
    Top = 176
    Width = 105
    Height = 21
    TabOrder = 15
  end
  object BitBtn12: TBitBtn
    Left = 16
    Top = 712
    Width = 177
    Height = 33
    Caption = #23558#26816#26448'FT'#29305#24449#24179#22343#20540#23548#20986#21040'Excel'#20013
    TabOrder = 16
    OnClick = BitBtn12Click
  end
  object BitBtn13: TBitBtn
    Left = 208
    Top = 712
    Width = 177
    Height = 33
    Caption = #23558#26816#26448'GLCM'#29305#24449#24179#22343#20540#23548#20986#21040'Excel'#20013
    TabOrder = 17
    OnClick = BitBtn13Click
  end
  object BitBtn14: TBitBtn
    Left = 504
    Top = 712
    Width = 177
    Height = 33
    Caption = #23558#26679#26412'FT'#29305#24449#24179#22343#20540#23548#20986#21040'Excel'#20013
    TabOrder = 18
    OnClick = BitBtn14Click
  end
  object BitBtn15: TBitBtn
    Left = 696
    Top = 712
    Width = 177
    Height = 33
    Caption = #23558#26679#26412'GLCM'#29305#24449#24179#22343#20540#23548#20986#21040'Excel'#20013
    TabOrder = 19
    OnClick = BitBtn15Click
  end
  object OpenDialog1: TOpenDialog
    Left = 344
  end
  object SaveDialog1: TSaveDialog
    Left = 296
  end
end
