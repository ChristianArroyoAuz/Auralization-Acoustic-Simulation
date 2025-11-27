//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormAuralizationMotor : public TForm
{
__published:	// IDE-managed Components
        TButton *ButtonReestablecerAuralizacion;
        TButton *ButtonDetenerAuralizacion;
        TButton *ButtonIniciarAuralizacion;
        TScrollBar *ScrollBarNumeroRayos;
        TRadioButton *RadioButtonCanales;
        TScrollBar *ScrollBarReflection;
        TScrollBar *ScrollBarAbsorption;
        TRadioButton *RadioButtonMotor;
        TButton *ButtonControlReceptor;
        TGroupBox *GroupBoxControles;
        TGroupBox *GroupBoxServicios;
        TButton *ButtonIniciarPrueba;
        TButton *ButtonDetenerPrueba;
        TButton *ButtonControlEmisor;
        TScrollBar *ScrollBarPrueba;
        TGroupBox *GroupBoxRelativo;
        TGroupBox *GroupBoxReceptor;
        TScrollBar *ScrollBarReverb;
        TGroupBox *GroupBoxCanales;
        TScrollBar *ScrollBarDelay;
        TLabel *Dato_Reverberacion;
        TGroupBox *GroupBoxEmisor;
        TGroupBox *GroupBoxAcerca;
        TButton *ButtonTodosRayos;
        TStaticText *StaticText1;
        TOpenDialog *OpenDialog1;
        TPanel *PanelRenderScene;
        TLabel *Dato_Reflexion;
        TLabel *Dato_Absorcion;
        TButton *ButtonCuatro;
        TButton *ButtonCinco;
        TLabel *Dato_Retraso;
        TButton *ButtonCero;
        TButton *ButtonTres;
        TButton *ButtonUno;
        TButton *ButtonDos;
        TLabel *Relativo_X;
        TLabel *Relativo_Y;
        TLabel *Relativo_Z;
        TLabel *Dato_Rayos;
        TLabel *Pos_X_Emi;
        TLabel *Pos_Y_Emi;
        TLabel *Pos_Z_Emi;
        TLabel *Pos_X_Rec;
        TLabel *Pos_Y_Rec;
        TLabel *Pos_Z_Rec;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Nivel_R;
        TLabel *Nivel_L;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TLabel *Label27;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label33;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        
        void __fastcall drawRaysFromEmitter(float posEX, float posEY, float posEZ, float posRX, float posRY, float posRZ, int numRays);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
        void __fastcall ButtonReestablecerAuralizacionClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ButtonIniciarAuralizacionClick(TObject *Sender);
        void __fastcall ButtonDetenerAuralizacionClick(TObject *Sender);
        void __fastcall DisableClose(TObject *Sender, bool &CanClose);
        void __fastcall ScrollBarNumeroRayosChange(TObject *Sender);
        void __fastcall DisableKeyboard(tagMSG &Msg, bool &Handled);
        void __fastcall ButtonControlReceptorClick(TObject *Sender);
        void __fastcall ScrollBarReflectionChange(TObject *Sender);
        void __fastcall ScrollBarAbsorptionChange(TObject *Sender);
        void __fastcall ButtonControlEmisorClick(TObject *Sender);
        void __fastcall ButtonIniciarPruebaClick(TObject *Sender);
        void __fastcall ButtonDetenerPruebaClick(TObject *Sender);
        void __fastcall RadioButtonCanalesClick(TObject *Sender);
        void __fastcall ButtonTodosRayosClick(TObject *Sender);
        void __fastcall RadioButtonMotorClick(TObject *Sender);
        void __fastcall ScrollBarPruebaChange(TObject *Sender);
        void __fastcall ReceptorNOButtonClick(TObject *Sender);
        void __fastcall ReceptorNEButtonClick(TObject *Sender);
        void __fastcall ReceptorSOButtonClick(TObject *Sender);
        void __fastcall ReceptorSEButtonClick(TObject *Sender);
        void __fastcall ScrollBarReverbChange(TObject *Sender);
        void __fastcall ScrollBarDelayChange(TObject *Sender);
        void __fastcall ReceptorNButtonClick(TObject *Sender);
        void __fastcall ReceptorOButtonClick(TObject *Sender);
        void __fastcall ReceptorCButtonClick(TObject *Sender);
        void __fastcall ReceptorEButtonClick(TObject *Sender);
        void __fastcall ReceptorSButtonClick(TObject *Sender);
        void __fastcall EmisorNOButtonClick(TObject *Sender);
        void __fastcall EmisorNEButtonClick(TObject *Sender);
        void __fastcall EmisorSOButtonClick(TObject *Sender);
        void __fastcall EmisorSEButtonClick(TObject *Sender);
        void __fastcall EmisorNButtonClick(TObject *Sender);
        void __fastcall EmisorOButtonClick(TObject *Sender);
        void __fastcall EmisorCButtonClick(TObject *Sender);
        void __fastcall EmisorEButtonClick(TObject *Sender);
        void __fastcall EmisorSButtonClick(TObject *Sender);
        void __fastcall ButtonCuatroClick(TObject *Sender);
        void __fastcall ButtonCincoClick(TObject *Sender);
        void __fastcall ButtonCeroClick(TObject *Sender);
        void __fastcall ButtonTresClick(TObject *Sender);
        void __fastcall ButtonUnoClick(TObject *Sender);
        void __fastcall ButtonDosClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall UpdateScrollBarLabels();
        void __fastcall disableFPExceptions();
        void __fastcall drawMicrophone();
        void __fastcall drawSpeaker();
        void __fastcall RenderScene();
        
private:	// User declarations
        void ApplyReflections(short *buffer, int size, float intensity);
        void ApplyAbsorption(short *buffer, int size, float intensity);
        void ApplyReverb(short *buffer, int size, float intensity);
        void ApplyDelay(short *buffer, int size, float intensity);
        void ClampBuffer(short *buffer, int size);
        void ApplyAudioEffects();                                               // Efectos de audio
        void CleanupAudio();

public:		// User declarations
        void TFormAuralizationMotor::UpdateAudio(int numRaysLeft, int numRaysRight);
        __fastcall TFormAuralizationMotor(TComponent* Owner);                   // Declaración del constructor
        __fastcall ~TFormAuralizationMotor();                                   // Declaración del destructor
};

//---------------------------------------------------------------------------
extern PACKAGE TFormAuralizationMotor *FormAuralizationMotor;

//---------------------------------------------------------------------------
#endif

