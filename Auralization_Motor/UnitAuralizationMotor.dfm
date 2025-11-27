object FormAuralizationMotor: TFormAuralizationMotor
  Left = -8
  Top = -8
  Width = 1552
  Height = 840
  Caption = 'Motor de Auralizaci'#243'n'
  Color = clActiveCaption
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxEmisor: TGroupBox
    Left = 10
    Top = 56
    Width = 271
    Height = 185
    Caption = 'Emisor (Parlante)'
    DockSite = True
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 102
      Height = 24
      Caption = 'Infomaci'#243'n: '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 32
      Top = 40
      Width = 207
      Height = 16
      Caption = 'El emisor (parlante) es el elemento'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 8
      Top = 82
      Width = 52
      Height = 24
      Caption = 'Datos:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 32
      Top = 112
      Width = 84
      Height = 16
      Caption = 'Posici'#243'n en X:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Pos_X_Emi: TLabel
      Left = 128
      Top = 112
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 31
      Top = 56
      Width = 136
      Height = 16
      Caption = 'circular de color verde.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 32
      Top = 136
      Width = 85
      Height = 16
      Caption = 'Posici'#243'n en Y:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Pos_Y_Emi: TLabel
      Left = 128
      Top = 136
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label30: TLabel
      Left = 32
      Top = 160
      Width = 84
      Height = 16
      Caption = 'Posici'#243'n en Z:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Pos_Z_Emi: TLabel
      Left = 128
      Top = 160
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object StaticText1: TStaticText
    Left = 512
    Top = 2
    Width = 492
    Height = 52
    Caption = 'MOTOR DE AURALIZACI'#211'N'
    Color = clActiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindow
    Font.Height = -37
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 1
  end
  object GroupBoxReceptor: TGroupBox
    Left = 8
    Top = 272
    Width = 273
    Height = 185
    Caption = 'Receptor (Micr'#243'fono)'
    TabOrder = 2
    object Label7: TLabel
      Left = 8
      Top = 16
      Width = 102
      Height = 24
      Caption = 'Infomaci'#243'n: '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 32
      Top = 40
      Width = 226
      Height = 16
      Caption = 'El receptor (micr'#243'fono) es el elemento'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 31
      Top = 56
      Width = 124
      Height = 16
      Caption = 'circular de color rojo.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 8
      Top = 82
      Width = 52
      Height = 24
      Caption = 'Datos:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label11: TLabel
      Left = 32
      Top = 112
      Width = 84
      Height = 16
      Caption = 'Posici'#243'n en X:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label12: TLabel
      Left = 32
      Top = 136
      Width = 85
      Height = 16
      Caption = 'Posici'#243'n en Y:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Pos_X_Rec: TLabel
      Left = 128
      Top = 112
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Pos_Y_Rec: TLabel
      Left = 128
      Top = 136
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label31: TLabel
      Left = 32
      Top = 160
      Width = 84
      Height = 16
      Caption = 'Posici'#243'n en Z:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Pos_Z_Rec: TLabel
      Left = 128
      Top = 160
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBoxCanales: TGroupBox
    Left = 8
    Top = 576
    Width = 273
    Height = 97
    Caption = 'Prueba Sonido Canales'
    TabOrder = 3
    object ButtonIniciarPrueba: TButton
      Left = 8
      Top = 64
      Width = 121
      Height = 25
      Caption = 'Iniciar'
      TabOrder = 0
      TabStop = False
      OnClick = ButtonIniciarPruebaClick
    end
    object ButtonDetenerPrueba: TButton
      Left = 144
      Top = 64
      Width = 121
      Height = 25
      Caption = 'Detener'
      Enabled = False
      TabOrder = 1
      TabStop = False
      OnClick = ButtonDetenerPruebaClick
    end
    object ScrollBarPrueba: TScrollBar
      Left = 8
      Top = 24
      Width = 257
      Height = 25
      Enabled = False
      PageSize = 0
      TabOrder = 2
      TabStop = False
      OnChange = ScrollBarPruebaChange
    end
  end
  object GroupBoxServicios: TGroupBox
    Left = 8
    Top = 480
    Width = 273
    Height = 73
    Caption = 'Servicios'
    TabOrder = 4
    object RadioButtonCanales: TRadioButton
      Left = 8
      Top = 16
      Width = 249
      Height = 17
      Caption = 'Activar Prueba de Sonido de Canales'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      TabStop = True
      OnClick = RadioButtonCanalesClick
    end
    object RadioButtonMotor: TRadioButton
      Left = 8
      Top = 48
      Width = 249
      Height = 17
      Caption = 'Activar Motor de Aurelizaci'#243'n'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = RadioButtonMotorClick
    end
  end
  object GroupBoxRelativo: TGroupBox
    Left = 1224
    Top = 56
    Width = 297
    Height = 425
    Caption = 'Posiciones Relativas'
    Enabled = False
    TabOrder = 5
    object Label13: TLabel
      Left = 8
      Top = 16
      Width = 102
      Height = 24
      Caption = 'Infomaci'#243'n: '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label14: TLabel
      Left = 34
      Top = 40
      Width = 239
      Height = 16
      Caption = 'Posici'#243'n relativa del emisor con respeto'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label15: TLabel
      Left = 34
      Top = 56
      Width = 67
      Height = 16
      Caption = 'al receptor.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label16: TLabel
      Left = 8
      Top = 82
      Width = 52
      Height = 24
      Caption = 'Datos:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label17: TLabel
      Left = 32
      Top = 112
      Width = 84
      Height = 16
      Caption = 'Posici'#243'n en X:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label18: TLabel
      Left = 32
      Top = 136
      Width = 85
      Height = 16
      Caption = 'Posici'#243'n en Y:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Relativo_X: TLabel
      Left = 128
      Top = 112
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Relativo_Y: TLabel
      Left = 128
      Top = 136
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label24: TLabel
      Left = 8
      Top = 186
      Width = 67
      Height = 24
      Caption = 'Niveles:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label25: TLabel
      Left = 32
      Top = 216
      Width = 47
      Height = 16
      Caption = 'Nivel R:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label26: TLabel
      Left = 32
      Top = 240
      Width = 44
      Height = 16
      Caption = 'Nivel L:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Nivel_R: TLabel
      Left = 128
      Top = 216
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Nivel_L: TLabel
      Left = 128
      Top = 240
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label32: TLabel
      Left = 32
      Top = 160
      Width = 84
      Height = 16
      Caption = 'Posici'#243'n en Z:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Relativo_Z: TLabel
      Left = 128
      Top = 160
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object ButtonIniciarAuralizacion: TButton
      Left = 8
      Top = 264
      Width = 281
      Height = 25
      Caption = 'Iniciar'
      Enabled = False
      TabOrder = 0
      TabStop = False
      OnClick = ButtonIniciarAuralizacionClick
    end
    object ButtonDetenerAuralizacion: TButton
      Left = 8
      Top = 392
      Width = 281
      Height = 25
      Caption = 'Detener'
      Enabled = False
      TabOrder = 1
      TabStop = False
      OnClick = ButtonDetenerAuralizacionClick
    end
    object ButtonReestablecerAuralizacion: TButton
      Left = 8
      Top = 360
      Width = 281
      Height = 25
      Caption = 'Restablecer'
      Enabled = False
      TabOrder = 2
      TabStop = False
      OnClick = ButtonReestablecerAuralizacionClick
    end
    object ButtonControlEmisor: TButton
      Left = 8
      Top = 296
      Width = 137
      Height = 25
      Caption = 'Control Emisor'
      Enabled = False
      TabOrder = 3
      TabStop = False
      OnClick = ButtonControlEmisorClick
    end
    object ButtonControlReceptor: TButton
      Left = 152
      Top = 296
      Width = 137
      Height = 25
      Caption = 'Control Receptor'
      Enabled = False
      TabOrder = 4
      TabStop = False
      OnClick = ButtonControlReceptorClick
    end
    object ButtonCero: TButton
      Left = 48
      Top = 328
      Width = 38
      Height = 25
      Caption = '0'
      Enabled = False
      TabOrder = 5
      OnClick = ButtonCeroClick
    end
    object ButtonUno: TButton
      Left = 88
      Top = 328
      Width = 38
      Height = 25
      Caption = '1'
      Enabled = False
      TabOrder = 6
      OnClick = ButtonUnoClick
    end
    object ButtonDos: TButton
      Left = 128
      Top = 328
      Width = 38
      Height = 25
      Caption = '2'
      Enabled = False
      TabOrder = 7
      OnClick = ButtonDosClick
    end
    object ButtonTres: TButton
      Left = 168
      Top = 328
      Width = 38
      Height = 25
      Caption = '3'
      Enabled = False
      TabOrder = 8
      OnClick = ButtonTresClick
    end
    object ButtonCuatro: TButton
      Left = 208
      Top = 328
      Width = 38
      Height = 25
      Caption = '4'
      Enabled = False
      TabOrder = 9
      OnClick = ButtonCuatroClick
    end
    object ButtonCinco: TButton
      Left = 248
      Top = 328
      Width = 38
      Height = 25
      Caption = '5'
      Enabled = False
      TabOrder = 10
      OnClick = ButtonCincoClick
    end
    object ButtonTodosRayos: TButton
      Left = 8
      Top = 328
      Width = 38
      Height = 25
      Caption = 'T'
      Enabled = False
      TabOrder = 11
      OnClick = ButtonTodosRayosClick
    end
  end
  object GroupBoxControles: TGroupBox
    Left = 1224
    Top = 488
    Width = 297
    Height = 297
    Caption = 'Controles'
    Enabled = False
    TabOrder = 6
    object Label19: TLabel
      Left = 8
      Top = 72
      Width = 169
      Height = 16
      Caption = 'Atenuaci'#243'n para reflexiones:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label20: TLabel
      Left = 8
      Top = 128
      Width = 188
      Height = 16
      Caption = 'Atenuaci'#243'n para reverberaci'#243'n:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label21: TLabel
      Left = 8
      Top = 184
      Width = 127
      Height = 16
      Caption = 'Retraso en muestras:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label22: TLabel
      Left = 8
      Top = 240
      Width = 225
      Height = 16
      Caption = 'Coeficiente de absorci'#243'n del material:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Dato_Reflexion: TLabel
      Left = 248
      Top = 72
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Dato_Reverberacion: TLabel
      Left = 248
      Top = 128
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Dato_Retraso: TLabel
      Left = 248
      Top = 184
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Dato_Absorcion: TLabel
      Left = 248
      Top = 232
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label33: TLabel
      Left = 8
      Top = 16
      Width = 110
      Height = 16
      Caption = 'N'#250'mero de Rayos'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Dato_Rayos: TLabel
      Left = 248
      Top = 16
      Width = 24
      Height = 16
      Caption = '0,00'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object ScrollBarReflection: TScrollBar
      Left = 8
      Top = 96
      Width = 281
      Height = 17
      Enabled = False
      LargeChange = 10
      PageSize = 0
      SmallChange = 10
      TabOrder = 0
      OnChange = ScrollBarReflectionChange
    end
    object ScrollBarReverb: TScrollBar
      Left = 8
      Top = 152
      Width = 281
      Height = 17
      Enabled = False
      LargeChange = 10
      PageSize = 0
      SmallChange = 10
      TabOrder = 1
      OnChange = ScrollBarReverbChange
    end
    object ScrollBarDelay: TScrollBar
      Left = 8
      Top = 208
      Width = 281
      Height = 17
      Enabled = False
      LargeChange = 10
      PageSize = 0
      SmallChange = 10
      TabOrder = 2
      OnChange = ScrollBarDelayChange
    end
    object ScrollBarAbsorption: TScrollBar
      Left = 8
      Top = 264
      Width = 281
      Height = 17
      Enabled = False
      LargeChange = 10
      PageSize = 0
      SmallChange = 10
      TabOrder = 3
      OnChange = ScrollBarAbsorptionChange
    end
    object ScrollBarNumeroRayos: TScrollBar
      Left = 8
      Top = 40
      Width = 281
      Height = 17
      Enabled = False
      LargeChange = 100
      PageSize = 0
      SmallChange = 100
      TabOrder = 4
      OnChange = ScrollBarNumeroRayosChange
    end
  end
  object GroupBoxAcerca: TGroupBox
    Left = 8
    Top = 696
    Width = 273
    Height = 89
    Caption = 'Acerca'
    TabOrder = 7
    object Label23: TLabel
      Left = 8
      Top = 16
      Width = 213
      Height = 16
      Caption = 'Facultad de Ingenieria en Sistemas.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label27: TLabel
      Left = 8
      Top = 32
      Width = 194
      Height = 16
      Caption = 'Msc. Arroyo Auz Christian Xavier.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label28: TLabel
      Left = 8
      Top = 48
      Width = 152
      Height = 16
      Caption = 'Maestr'#237'a en Computaci'#243'n'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label29: TLabel
      Left = 8
      Top = 64
      Width = 119
      Height = 16
      Caption = 'Qito - Ecuador. 2025'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object PanelRenderScene: TPanel
    Left = 296
    Top = 64
    Width = 913
    Height = 721
    Caption = 'Activar e Iniciar e Motor de Aurilizaci'#243'n'
    TabOrder = 8
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    OnResize = FormResize
  end
  object OpenDialog1: TOpenDialog
  end
end
