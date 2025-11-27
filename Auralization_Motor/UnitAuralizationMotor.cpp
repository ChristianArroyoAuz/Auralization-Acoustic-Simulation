//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Dialogs.hpp>
#include <mmsystem.h>
#include <Windows.h>
#include <algorithm>
#include <GL/glu.h>
#include <GL/gl.h>
#include <fstream>
#include <cstdlib>
#include <float.h>
#include <cstdio>
#include <vector>
#include <vcl.h>
#include <cmath>
#include <ctime>
#include <cmath>
#pragma hdrstop


//---------------------------------------------------------------------------
#define CLAMP(value, low, high) (value < low ? low : (value > high ? high : value))
#include "UnitAuralizationMotor.h"
#include "Math.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//---------------------------------------------------------------------------
TFormAuralizationMotor *FormAuralizationMotor;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Variables para el control de audio
HWAVEOUT hWaveOut = NULL;                                                       // Identificador del dispositivo de salida de audio
PBYTE audioBuffer = NULL;                                                       // Buffer de audio
bool etiquetasP = false;                                                        // Activar etiquetas de información de posición
bool isPlaying = false;                                                         // Indicador para controlar la reproducción
WAVEHDR waveHeader;                                                             // Estructura de la cabecera para el buffer de audio
WAVEFORMATEX wf;                                                                // Estructura que define el formato del audio


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Declaración de las variables para el procesamiento del sonido
PBYTE originalAudioBuffer = NULL;                                               // Copia del buffer de audio
float reflectionValue = 0.0f;                                                   // Nivel de reflexiones
float absorptionValue = 0.0f;                                                   // Nivel de absorción
float reverbValue = 0.0f;                                                       // Nivel de reverberación
float delayValue = 0.0f;                                                        // Nivel de delay


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Declaración de variables globales
float posReceptorZ = 13.5f;                                                     // Posición inicial en Z del receptor
float posReceptorX = 0.0f;                                                      // Posición inicial en X del receptor
float posReceptorY = 2.0f;                                                      // Posición inicial en Y del receptor
float posEmisorZ = -3.5f;                                                       // Posición inicial en Z del emisor
float posEmisorX = 0.0f;                                                        // Posición inicial en X del emisor
float posEmisorY = 2.0f;                                                        // Posición inicial en Y del emisor

// Declaracipon de las variables de los elementos generales
float posAereoX = -4.0f, posAereoY = 4.0f, posAereoZ = 4.0f;
float cameraAngleAereo = 0.0f, cameraPitchAereo = 1.0f;
float cameraAngle = 0.0f, cameraPitch = 0.0f;
bool controlAnguloReceptor = true;
bool controlAnguloEmisor = true;
bool leftButtonDown = false;
int lastMouseX, lastMouseY;
bool mouseCaptured = false;
bool isTransparent = true;
HGLRC hRC;
HDC hDC;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Variables para cambio de vista 
enum CameraMode { FIRST_PERSON, THIRD_PERSON };
CameraMode cameraMode = FIRST_PERSON;
float thirdPersonDistance = 20.0f;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Creación de paneles flotantes
TPanel *gamepadPanelReceptor;
TPanel *gamepadPanelEmisor;
TPanel *temporal;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Creación de ángulos iniciales del receptor y emisor
float inicialReceptorAngle = 90.0f;
float inicialEmisorAngle = 180.0f;

// Creación de ángulos del receptor
float receptorAngle_N = 0.0f;
float receptorAngle_S = 0.0f;
float receptorAngle_O = 0.0f;
float receptorAngle_E = 0.0f;
float receptorAngle_NE = 0.0f;
float receptorAngle_NO = 0.0f;
float receptorAngle_SE = 0.0f;
float receptorAngle_SO = 0.0f;

// Creación de ángulos del emisor
float emisorAngle_N = 0.0f;
float emisorAngle_S = 0.0f;
float emisorAngle_O = 0.0f;
float emisorAngle_E = 0.0f;
float emisorAngle_NO = 0.0f;
float emisorAngle_NE = 0.0f;
float emisorAngle_SO = 0.0f;
float emisorAngle_SE = 0.0f;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Estructura para representar un vector 3D
struct Vec3 {float x, y, z;};


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Función para calcular la intersección de un rayo con un plano
bool intersectPlane(const Vec3& planeNormal, const Vec3& planePoint, const Vec3& rayOrigin, const Vec3& rayDirection, Vec3& intersectionPoint);

// Funciones auxiliar dot
float dot(const Vec3& a, const Vec3& b) {return a.x * b.x + a.y * b.y + a.z * b.z;}

// Funciones auxiliar longitud
float length(const Vec3& a) {return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);}

// Función para restar dos vectores
Vec3 subtract(const Vec3& a, const Vec3& b);

// Función para multiplicar un vector por un escalar
Vec3 multiply(const Vec3& a, float scalar);

// Función para sumar dos vectores
Vec3 add(const Vec3& a, const Vec3& b);

// Estableciendo el número de rayos iniciales
int numeroRayos = 100;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Presentación visual de los rayos y sus reflexiones
bool showSecondReflection = false;
bool showFourthReflection = false;
bool showThirdReflection = false;
bool showFifthReflection = false;
bool showFirstReflection = false;
bool showOriginalRays = false;
bool showTotalRays = false;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Función para calcular la intersección de un rayo con los canales del receptor
bool intersectSphere(const Vec3& center, float radius, const Vec3& origin, const Vec3& direction, Vec3& intersectionPoint);

// Vector de interseccción con el receptor
Vec3 transformPoint(const float matrix[16], const Vec3& point);


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Asegúrarse de que los valores del buffer no excedan los límites de un entero de 16 bits
short ClampSample(int sample){return CLAMP(sample, -32768, 32767);}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Variables de los canales de L y R
int rayosCanalL = 0;
int rayosCanalR = 0;


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TFormAuralizationMotor::TFormAuralizationMotor(TComponent* Owner): TForm(Owner)
{
    // Estilo de la pantalla
    WindowState = wsMaximized;                                                  // Iniciar maximizado
    BorderStyle = bsSingle;                                                     // Evita que la ventana sea redimensionable
    BorderIcons = BorderIcons >> biMinimize >> biMaximize;                      // Deshabilitar los botones de minimizar y restaurar

    // Inicializar la estructura de WAVEFORMATEX para prueba de audio
    wf.wFormatTag = WAVE_FORMAT_PCM;                                            // Formato PCM (audio sin comprimir)
    wf.nChannels = 2;                                                           // Dos canales (estéreo)
    wf.nSamplesPerSec = 44100;                                                  // Frecuencia de muestreo de 44.1 kHz
    wf.nAvgBytesPerSec = 176400;                                                // Promedio de bytes por segundo
    wf.nBlockAlign = 4;                                                         // Bloque de 4 bytes por muestra
    wf.wBitsPerSample = 16;                                                     // 16 bits por muestra
    wf.cbSize = 0;                                                              // Tamaño inicial en 0

    // Abrir el dispositivo de salida de audio
    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
    {
        ShowMessage("No se pudo abrir el dispositivo de audio.");
    }

    // Inicializar el ScrollBar Sonido en el centro
    ScrollBarPrueba->Min = 0;                                                   // Valor minimo en 0
    ScrollBarPrueba->Max = 100;                                                 // Valor maximo en 100
    ScrollBarPrueba->Position = 50;                                             // Valor del centro
    ScrollBarPrueba->Enabled = false;
    ScrollBarPruebaChange(NULL);                                                // Ajustar volumen inicial

    // Inicializar el ScrollBar de Número de rayos
    ScrollBarNumeroRayos->Min = 0;                                              // Valor minimo en 0
    ScrollBarNumeroRayos->Max = 1000;                                           // Valor maximo en 100
    ScrollBarNumeroRayos->Position = 0;
    ScrollBarNumeroRayos->Enabled = false;

    // Inicializando el ScrollBar de Reflexión
    ScrollBarReflection->Min = 0;                                               // Rango para Reflexión
    ScrollBarReflection->Max = 100;
    ScrollBarReflection->Position = 0;
    ScrollBarReflection->Enabled = false;

    // Inicializando el ScrollBar de Reberveración
    ScrollBarReverb->Min = 0;                                                   // Rango para Reverberación
    ScrollBarReverb->Max = 100;
    ScrollBarReverb->Position = 0;
    ScrollBarReverb->Enabled = false;

    // Inicializando el ScrollBar de Retraso
    ScrollBarDelay->Min = 0;                                                    // Rango para Retraso
    ScrollBarDelay->Max = 100;
    ScrollBarDelay->Position = 0;
    ScrollBarDelay->Enabled = false;

    // Inicializando el ScrollBar de Absorción
    ScrollBarAbsorption->Min = 0;                                               // Rango para Absorción
    ScrollBarAbsorption->Max = 100;
    ScrollBarAbsorption->Position = 0;
    ScrollBarAbsorption->Enabled = false;

    // Deshabilitando control del panel
    Application->OnMessage = DisableKeyboard;
    PanelRenderScene->Enabled = false;

    // Evitar excepciones de punto flotante
    disableFPExceptions();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormClose(TObject *Sender, TCloseAction &Action)
{
    Application->OnMessage = NULL;
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormDestroy(TObject *Sender)
{
    if (hRC)                                                                    // Verificar si el contexto de renderizado existe
    {
        wglMakeCurrent(NULL, NULL);                                             // Desasignar el contexto actual
        wglDeleteContext(hRC);                                                  // Eliminar el contexto de renderizado
        hRC = NULL;
    }
    if (hDC)                                                                    // Verificar si el contexto de dispositivo existe
    {
        ReleaseDC(Handle, hDC);                                                 // Liberar el contexto de dispositivo
        hDC = NULL;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormResize(TObject *Sender)
{
    if (PanelRenderScene->Width == 0 || PanelRenderScene->Height == 0)
    {
        return;
    }
    glViewport(0, 0, PanelRenderScene->Width, PanelRenderScene->Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)PanelRenderScene->Width / (GLfloat)PanelRenderScene->Height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormCreate(TObject *Sender)
{
    hDC = GetDC(PanelRenderScene->Handle);
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),                                          // Tamaño de este descriptor de formato de píxel
        1,                                                                      // Número de versión
        PFD_DRAW_TO_WINDOW |                                                    // El formato debe admitir ventana
        PFD_SUPPORT_OPENGL |                                                    // El formato debe ser compatible con OpenGL
        PFD_DOUBLEBUFFER,                                                       // Debe admitir doble almacenamiento en búfer
        PFD_TYPE_RGBA,                                                          // Solicitar un formato RGBA
        32,                                                                     // Seleccione nuestra profundidad de color
        0, 0, 0, 0, 0, 0,                                                       // Bits de color ignorados
        0,                                                                      // Sin búfer alfa
        0,                                                                      // Se ignoró el bit de cambio
        0,                                                                      // Sin buffer de acumulación
        0, 0, 0, 0,                                                             // Bits de acumulación ignorados
        24,                                                                     // Buffer Z de 32 bits (buffer de profundidad)
        8,                                                                      // Con búfer de plantilla
        0,                                                                      // Sin buffer auxiliar
        PFD_MAIN_PLANE,                                                         // Capa de dibujo principal
        0,                                                                      // Reservado
        0, 0, 0                                                                 // Máscaras de capa ignoradas
    };
    int format = ChoosePixelFormat(hDC, &pfd);                                  // Escoger el formato de píxel adecuado
    SetPixelFormat(hDC, format, &pfd);                                          // Establecer el formato de píxel
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)PanelRenderScene->Width / (GLfloat)PanelRenderScene->Height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    // Llamada a RenderScene para renderizar la escena al inicio
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormPaint(TObject *Sender)
{
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::disableFPExceptions()
{
    // Desactivar excepciones de punto flotante
    _control87(MCW_EM, MCW_EM);
}


//---------------------------------------------------------------------------
__fastcall TFormAuralizationMotor::~TFormAuralizationMotor()
{
    // Verificar si hWaveOut fue abierto antes de cerrarlo
    if (hWaveOut)
    {
        waveOutReset(hWaveOut);                                                 // Resetear el dispositivo de salida de audio
        waveOutClose(hWaveOut);                                                 // Cerrar el dispositivo de audio
    }

    // Verificar si el buffer de audio fue asignado antes de liberarlo
    if (audioBuffer)
    {
        free(audioBuffer);                                                      // Liberar el buffer de audio
        audioBuffer = NULL;                                                     // Evitar el uso de un puntero colgante
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::drawMicrophone()
{
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_LINE);                                        // Dibuja con líneas
    glLineWidth(1.0f);

    int slices = 16;
    int stacks = 16;
    float radius = 0.5f;

    // Graficar planos X, Y, Z
    glBegin(GL_LINES);
    // Plano X (rojo)
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    // Plano Y (verde)
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    // Plano Z (azul)
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

    for (int i = 0; i < stacks; ++i)
    {
        float lat0 = 3.14159265359f * (-0.5f + (float)(i) / stacks);
        float z0  = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = 3.14159265359f * (-0.5f + (float)(i + 1) / stacks);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_LINE_LOOP);                                                  // Usar GL_LINE_LOOP para dibujar líneas
        for (int j = 0; j <= slices; ++j)
        {
            float lng = 2 * 3.14159265359f * (float)(j) / slices;
            float x = cos(lng);
            float y = sin(lng);

            // Cambiar color dependiendo de la latitud
            if (i < stacks / 4)                                                 // Polo amarillo
            {
                glColor4f(1.0f, 1.0f, 0.0f, 1.0f);                              // Canal Izquierdo
            }
            else if (i > 3 * stacks / 4) // Polo morado
            {
                glColor4f(0.0f, 0.75f, 1.0f, 1.0f);                             // Canal Derecho
            }
            // Ecuador
            else
            {
                glColor4f(1.0f, 0.5f, 0.5f, 1.0f);                              // Rojo claro
            }

            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }

    gluDeleteQuadric(quad);
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::drawSpeaker()
{
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_LINE);                                        // Dibuja con líneas
    glLineWidth(1.0f);

    int slices = 16;
    int stacks = 16;
    float radius = 0.5f;

    for (int i = 0; i < stacks; ++i)
    {
        float lat0 = 3.14159265359f * (-0.5f + (float)(i) / stacks);
        float z0  = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = 3.14159265359f * (-0.5f + (float)(i + 1) / stacks);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_LINE_LOOP);                                                  // Usar GL_LINE_LOOP para dibujar líneas
        for (int j = 0; j <= slices; ++j)
        {
            float lng = 2 * 3.14159265359f * (float)(j) / slices;
            float x = cos(lng);
            float y = sin(lng);

            // Cambiar color dependiendo de la latitud
            if (i < stacks / 4) // Polo amarillo
            {
                glColor4f(1.0f, 1.0f, 0.0f, 1.0f);                              // Amarillo
            }
            else
            {
                glColor4f(102.0/255.0, 173.0/255.0, 67.0/255.0, 1.0f);          // Verde
            }

            glVertex3f(radius * x * zr0, radius * y * zr0, radius * z0);
            glVertex3f(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();
    }

    gluDeleteQuadric(quad);
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Verificación del tipo de vista
    if (cameraMode == FIRST_PERSON)
    {
        gluLookAt(posReceptorX, posReceptorY, posReceptorZ,
                  posReceptorX + sin(cameraAngle),
                  posReceptorY + sin(cameraPitch),
                  posReceptorZ - cos(cameraAngle), 0, 1, 0);
    }
    else if (cameraMode == THIRD_PERSON)
    {
        gluLookAt(posAereoX - thirdPersonDistance * sin(cameraAngleAereo),
                  posAereoY + thirdPersonDistance * sin(cameraPitchAereo),
                  posAereoZ + thirdPersonDistance * cos(cameraAngleAereo),
                  posAereoX, posAereoY, posAereoZ, 0, 1, 0);
    }

    // Estado de transparencia
    if (!isTransparent)
    {
        // Efecto de transparencia a los planos
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        // Dibujar habitación cerrada
        // Piso de la habitación
        glBegin(GL_QUADS);
        glColor3f(0.3f, 0.3f, 0.3f);
        // Piso Primer Cuadrante
        glVertex3f(-4, 0, -4);      glVertex3f(4, 0, -4);
        glVertex3f(4, 0, 4);        glVertex3f(-4, 0, 4);
        // Piso Segundo Cuadrante
        glVertex3f(-4, 0, 4);       glVertex3f(4, 0, 4);
        glVertex3f(4, 0, 12);       glVertex3f(-4, 0, 12);
        // Piso Tercer Cuadrante
        glVertex3f(-4, 0, 4);       glVertex3f(-4, 0, 12);
        glVertex3f(-12, 0, 12);     glVertex3f(-12, 0, 4);
        glEnd();

        // Paredes de la habitación
        glBegin(GL_QUADS);
        glColor3f(0.6f, 0.6f, 0.6f);
        // Primera Pared
        glVertex3f(-4, 0, -4);      glVertex3f(-4, 8, -4);
        glVertex3f(4, 8, -4);       glVertex3f(4, 0, -4);
        // Segunda Pared
        glVertex3f(4, 0, -4);       glVertex3f(4, 8, -4);
        glVertex3f(4, 8, 4);        glVertex3f(4, 0, 4);
        // Tercera Pared
        glVertex3f(4, 0, 4);        glVertex3f(4, 8, 4);
        glVertex3f(4, 8, 12);       glVertex3f(4, 0, 12);
        // Cuarta Pared
        glVertex3f(-4, 0, -4);      glVertex3f(-4, 8, -4);
        glVertex3f(-4, 8, 4);       glVertex3f(-4, 0, 4);
        // Quinta Pared
        glVertex3f(-4, 0, 4);       glVertex3f(-4, 8, 4);
        glVertex3f(-12, 8, 4);      glVertex3f(-12, 0, 4);
        // Sexta Pared
        glVertex3f(-12, 0, 4);      glVertex3f(-12, 8, 4);
        glVertex3f(-12, 8, 12);     glVertex3f(-12, 0, 12);
        // Septima Pared
        glVertex3f(-12, 0, 12);     glVertex3f(-12, 8, 12);
        glVertex3f(-4, 8, 12);      glVertex3f(-4, 0, 12);
        // Octava Pared
        glVertex3f(-4, 0, 12);      glVertex3f(-4, 8, 12);
        glVertex3f(4, 8, 12);       glVertex3f(4, 0, 12);
        glEnd();
        
        // Techo de la habitación
        glBegin(GL_QUADS);
        glColor3f(0.9f, 0.9f, 0.9f);
        // Techo Primer Cuadrante
        glVertex3f(-4, 8, -4);      glVertex3f(4, 8, -4);
        glVertex3f(4, 8, 4);        glVertex3f(-4, 8, 4);
        // Techo Segundo Cuadrante
        glVertex3f(-4, 8, 4);       glVertex3f(4, 8, 4);
        glVertex3f(4, 8, 12);       glVertex3f(-4, 8, 12);
        // Techo Tercer Cuadrante
        glVertex3f(-4, 8, 4);       glVertex3f(-4, 8, 12);
        glVertex3f(-12, 8, 12);     glVertex3f(-12, 8, 4);
        glEnd();
    }

    // Contornos
    // Establece el grosor de las líneas
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // Color azul
    glColor3f(0.0f, 0.0f, 1.0f);
    // Contorno Primer Cuadrante (Piso)
    glVertex3f(-4, 0, -4); glVertex3f(4, 0, -4);
    glVertex3f(4, 0, -4); glVertex3f(4, 0, 4);
    glVertex3f(4, 0, 4); glVertex3f(-4, 0, 4);
    glVertex3f(-4, 0, 4); glVertex3f(-4, 0, -4);
    // Contorno Segundo Cuadrante (Piso)
    glVertex3f(-4, 0, 4); glVertex3f(4, 0, 4);
    glVertex3f(4, 0, 4); glVertex3f(4, 0, 12);
    glVertex3f(4, 0, 12); glVertex3f(-4, 0, 12);
    glVertex3f(-4, 0, 12); glVertex3f(-4, 0, 4);
    // Contorno Tercer Cuadrante (Piso)
    glVertex3f(-4, 0, 4); glVertex3f(-4, 0, 12);
    glVertex3f(-4, 0, 12); glVertex3f(-12, 0, 12);
    glVertex3f(-12, 0, 12); glVertex3f(-12, 0, 4);
    glVertex3f(-12, 0, 4); glVertex3f(-4, 0, 4);
    // Contorno Primer Cuadrante (Techo)
    glVertex3f(-4, 8, -4); glVertex3f(4, 8, -4);
    glVertex3f(4, 8, -4); glVertex3f(4, 8, 4);
    glVertex3f(4, 8, 4); glVertex3f(-4, 8, 4);
    glVertex3f(-4, 8, 4); glVertex3f(-4, 8, -4);
    // Contorno Segundo Cuadrante (Techo)
    glVertex3f(-4, 8, 4); glVertex3f(4, 8, 4);
    glVertex3f(4, 8, 4); glVertex3f(4, 8, 12);
    glVertex3f(4, 8, 12); glVertex3f(-4, 8, 12);
    glVertex3f(-4, 8, 12); glVertex3f(-4, 8, 4);
    // Contorno Tercer Cuadrante (Techo)
    glVertex3f(-4, 8, 4); glVertex3f(-4, 8, 12);
    glVertex3f(-4, 8, 12); glVertex3f(-12, 8, 12);
    glVertex3f(-12, 8, 12); glVertex3f(-12, 8, 4);
    glVertex3f(-12, 8, 4); glVertex3f(-4, 8, 4);
    // Contorno Primer Cuadrante (Paredes)
    glVertex3f(-4, 0, -4); glVertex3f(-4, 8, -4);
    glVertex3f(4, 0, -4); glVertex3f(4, 8, -4);
    glVertex3f(4, 0, 4); glVertex3f(4, 8, 4);
    glVertex3f(-4, 0, 4); glVertex3f(-4, 8, 4);
    // Contorno Segundo Cuadrante (Paredes)
    glVertex3f(4, 0, -4); glVertex3f(4, 8, -4);
    glVertex3f(4, 0, 4); glVertex3f(4, 8, 4);
    glVertex3f(4, 0, 12); glVertex3f(4, 8, 12);
    glVertex3f(-4, 0, 12); glVertex3f(-4, 8, 12);
    // Contorno Tercer Cuadrante (Paredes)
    glVertex3f(-4, 0, 4); glVertex3f(-4, 8, 4);
    glVertex3f(-4, 0, 12); glVertex3f(-4, 8, 12);
    glVertex3f(-12, 0, 12); glVertex3f(-12, 8, 12);
    glVertex3f(-12, 0, 4); glVertex3f(-12, 8, 4);
    glEnd();

    // Dibujar emisor
    glPushMatrix();                                                             // Guardar la matriz actual
    glTranslatef(posEmisorX, posEmisorY, posEmisorZ);
    glScalef(1.0f, 1.0f, 1.0f);                                                 // Escalar la bocina al tamaño deseado
    glRotatef(inicialEmisorAngle, 1, 0, 0);
    glRotatef(emisorAngle_N, 1, 0, 0);
    glRotatef(emisorAngle_S, 1, 0, 0);
    glRotatef(emisorAngle_O, 0, 1, 0);
    glRotatef(emisorAngle_E, 0, 1, 0);
    glRotatef(emisorAngle_NE, 1, 1, 0);
    glRotatef(emisorAngle_SO, 1, 1, 0);
    glRotatef(emisorAngle_NO, -1, 1, 0);
    glRotatef(emisorAngle_SE, -1, 1, 0);
    drawSpeaker();
    glPopMatrix();
    glEnd();

    // Dibujar receptor
    float redSphereX = posReceptorX;
    float redSphereY = posReceptorY;
    float redSphereZ = posReceptorZ - 2.0f;
    glPushMatrix();                                                             // Guardar la matriz actual
    glTranslatef(redSphereX, redSphereY, redSphereZ);
    glScalef(1.0f, 1.0f, 1.0f);                                                 // Escalar la bocina al tamaño deseado
    glRotatef(inicialReceptorAngle, 0, 1, 0);
    glRotatef(receptorAngle_N, 1, 0, 0);
    glRotatef(receptorAngle_S, 1, 0, 0);
    glRotatef(receptorAngle_O, 0, 1, 0);
    glRotatef(receptorAngle_E, 0, 1, 0);
    glRotatef(receptorAngle_NE, 1, 1, 0);
    glRotatef(receptorAngle_SO, 1, 1, 0);
    glRotatef(receptorAngle_NO, -1, 1, 0);
    glRotatef(receptorAngle_SE, -1, 1, 0);
    drawMicrophone();
    glPopMatrix();
    glEnd();
    
    if (etiquetasP == true)
    {
        // Actualizar los labels con las coordenadas del emisor
        Pos_X_Emi->Caption = FormatFloat("0.000", posEmisorX);
        Pos_Y_Emi->Caption = FormatFloat("0.000", posEmisorY);
        Pos_Z_Emi->Caption = FormatFloat("0.000", posEmisorZ);

        // Actualizar los labels con las coordenadas del receptor
        Pos_X_Rec->Caption = FormatFloat("0.000", posReceptorX);
        Pos_Y_Rec->Caption = FormatFloat("0.000", posReceptorY);
        Pos_Z_Rec->Caption = FormatFloat("0.000", posReceptorZ - 2.0f);

        // Actualizar los labels con las coordenadas de la posición relativa emisor-receptor
        Relativo_X->Caption = FormatFloat("0.000", posEmisorX - posReceptorX);
        Relativo_Y->Caption = FormatFloat("0.000", posEmisorY - posReceptorY);
        Relativo_Z->Caption = FormatFloat("0.000", posEmisorZ - posReceptorZ + 2.0f);
    }

    // Dibujar rayos desde el emisor
    drawRaysFromEmitter(posEmisorX, posEmisorY, posEmisorZ, posReceptorX, posReceptorY, posReceptorZ, numeroRayos);
    
    SwapBuffers(hDC);

    // Método de actualización de audio
    UpdateAudio(rayosCanalL, rayosCanalR);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button == mbLeft)
    {
        leftButtonDown = true;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button == mbLeft)
    {
        leftButtonDown = false;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    // Función de movimiento del mouse, restringida al área del panel
    // Obtener la posición del mouse dentro del panel
    TPoint pt = PanelRenderScene->ScreenToClient(Mouse->CursorPos);
    int panelX = pt.x;
    int panelY = pt.y;
    // Asegurase de que la posición esté dentro de las dimensiones del panel
    if (panelX >= 0 && panelX < PanelRenderScene->Width && panelY >= 0 && panelY < PanelRenderScene->Height)
    {
        // Verificar si el botón izquierdo esta presionado en primera persona
        if (leftButtonDown)
        {
            float sensitivity = 0.005f;

            cameraAngle += (panelX - lastMouseX) * sensitivity;
            cameraPitch += (panelY - lastMouseY) * sensitivity;
            if (cameraPitch > 1.5f)
            {
                cameraPitch = 1.5f;
            }
            if (cameraPitch < -1.5f)
            {
                cameraPitch = -1.5f;
            }

            cameraAngleAereo += (panelX - lastMouseX) * sensitivity;
            cameraPitchAereo += (panelY - lastMouseY) * sensitivity;
            if (cameraPitchAereo > 1.5f)
            {
                cameraPitchAereo = 1.5f;
            }
            if (cameraPitchAereo < -1.5f)
            {
                cameraPitchAereo = -1.5f;
            }

            RenderScene();
        }

        // Guardar la posición actual del ratón
        lastMouseX = panelX;
        lastMouseY = panelY;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    // Control de velocidad
    float speed = 0.1f;

    // Control de la receptor
    float newPosRecX = posReceptorX;
    float newPosRecY = posReceptorY;
    float newPosRecZ = posReceptorZ;

    // Control de la emisor
    float newPosEmiX = posEmisorX;
    float newPosEmiY = posEmisorY;
    float newPosEmiZ = posEmisorZ;

    // Control de la camara aerea
    float newPosAereoX = posAereoX;
    float newPosAereoY = posAereoY;
    float newPosAereoZ = posAereoZ;

    // Control de movimiento del receptor
    if (Key == VK_UP)
    {
        newPosRecX += speed * sin(cameraAngle);
        newPosRecZ -= speed * cos(cameraAngle);
    }
    else if (Key == VK_DOWN)
    {
        newPosRecX -= speed * sin(cameraAngle);
        newPosRecZ += speed * cos(cameraAngle);
    }
    else if (Key == VK_RIGHT)
    {
        newPosRecX += speed * cos(cameraAngle);
        newPosRecZ += speed * sin(cameraAngle);
    }
    else if (Key == VK_LEFT)
    {
        newPosRecX -= speed * cos(cameraAngle);
        newPosRecZ -= speed * sin(cameraAngle);
    }
    else if (Key == 'Z')
    {
        newPosRecY += speed;
    }
    else if (Key == 'X')
    {
        newPosRecY -= speed;
    }

    // Control de movimiento del emisor
    if (Key == 'W')
    {
        newPosEmiX += speed * sin(cameraAngle);
        newPosEmiZ -= speed * cos(cameraAngle);
    }
    else if (Key == 'S')
    {
        newPosEmiX -= speed * sin(cameraAngle);
        newPosEmiZ += speed * cos(cameraAngle);
    }
    else if (Key == 'A')
    {
        newPosEmiX -= speed * cos(cameraAngle);
        newPosEmiZ -= speed * sin(cameraAngle);
    }
    else if (Key == 'D')
    {
        newPosEmiX += speed * cos(cameraAngle);
        newPosEmiZ += speed * sin(cameraAngle);
    }
    else if (Key == 'Q')
    {
        newPosEmiY += speed;
    }
    else if (Key == 'E')
    {
        newPosEmiY -= speed;
    }

    // Control de movimiento camara aerea
    if (Key == 'Y')
    {
        newPosAereoX += speed * cos(cameraAngleAereo);
        newPosAereoZ -= speed * sin(cameraAngleAereo);
        posAereoX = newPosAereoX;
        posAereoZ = newPosAereoZ;
    }
    else if (Key == 'H')
    {
        newPosAereoX -= speed * cos(cameraAngleAereo);
        newPosAereoZ += speed * sin(cameraAngleAereo);
        posAereoX = newPosAereoX;
        posAereoZ = newPosAereoZ;
    }
    else if (Key == 'G')
    {
        newPosAereoX += speed * cos(cameraAngleAereo);
        newPosAereoZ += speed * sin(cameraAngleAereo);
        posAereoX = newPosAereoX;
        posAereoZ = newPosAereoZ;
    }
    else if (Key == 'J')
    {
        newPosAereoX -= speed * cos(cameraAngleAereo);
        newPosAereoZ -= speed * sin(cameraAngleAereo);
        posAereoX = newPosAereoX;
        posAereoZ = newPosAereoZ;
    }
    else if (Key == 'T')
    {
        newPosAereoY += speed;
        posAereoY = newPosAereoY;
    }
    else if (Key == 'U')
    {
        newPosAereoY -= speed;
        posAereoY = newPosAereoY;
    }
    
    // Control de Zoom en Tercera Persona
    if (Key == 'I' && cameraMode == THIRD_PERSON)
    {
        thirdPersonDistance -= speed;
    }
    else if (Key == 'O' && cameraMode == THIRD_PERSON)
    {
        thirdPersonDistance += speed;
    }

    // Control de Restablecimiento
    if (Key == 'R')
    {
        // Restablecer todo
        thirdPersonDistance = 20.0f;
        cameraAngleAereo = 0.0f;
        cameraPitchAereo = 1.0f;
        newPosRecZ = 13.5f;
        cameraAngle = 0.0f;
        cameraPitch = 0.0f;
        newPosEmiZ = -3.5f;
        newPosRecX = 0.0f;
        newPosRecY = 2.0f;
        newPosEmiX = 0.0f;
        newPosEmiY = 2.0f;
    }
    else if (Key == 'F')
    {
        // Restablecer la vista de la cámara
        thirdPersonDistance = 20.0f;
        cameraAngle = 0.0f;
        cameraPitch = 0.0f;
        posReceptorX = 0.0f;
        posReceptorY = 0.0f;
        posReceptorZ = 5.0f;
    }

    // Control Tipo de Persona
    if (Key == '1')
    {
        // Cambiando modo de camara
        cameraMode = FIRST_PERSON;
        // Reestableciendo controles
        thirdPersonDistance = 20.0f;
        cameraAngle = 0.0f;
        cameraPitch = 0.0f;
        posReceptorX = 0.0f;
        posReceptorY = 0.0f;
        posReceptorZ = 5.0f;
    }
    else if (Key == '3')
    {
        // Cambiando modo de camara
        cameraMode = THIRD_PERSON;
        // Reestableciendo controles
        cameraPitchAereo = 1.0f;
        cameraAngleAereo = 0.0f;
        posAereoX = -4.0f;
        posAereoY = 4.0f;
        posAereoZ = 4.0f;
    }

    // Control cambiar color elementos
    if (Key == 'C')
    {
        isTransparent = !isTransparent;
    }
    
    // Limitar la posición del receptor dentro de los límites de la habitación
    if (newPosRecX >= -3.5f && newPosRecX <= 3.5f && newPosRecZ >= -1.5f && newPosRecZ <= 13.5f && newPosRecY >= 0.5f && newPosRecY <= 7.5f)
    {
        posReceptorX = newPosRecX;
        posReceptorY = newPosRecY;
        posReceptorZ = newPosRecZ;

    }
    else if (newPosRecX >= -11.5f && newPosRecX < -3.5f && newPosRecZ >= 6.5f && newPosRecZ <= 13.5f && newPosRecY >= 0.5f && newPosRecY <= 7.5f)
    {
        posReceptorX = newPosRecX;
        posReceptorY = newPosRecY;
        posReceptorZ = newPosRecZ;
    }

    // Limitar la posición del emisor dentro de los límites de la habitación
    if (newPosEmiX >= -3.5f && newPosEmiX <= 3.5f && newPosEmiZ >= -3.5f && newPosEmiZ <= 11.5f && newPosEmiY >= 0.5f && newPosEmiY <= 7.5f)
    {
        posEmisorX = newPosEmiX;
        posEmisorY = newPosEmiY;
        posEmisorZ = newPosEmiZ;
    }
    else if (newPosEmiX >= -11.5f && newPosEmiX < -3.5f && newPosEmiZ >= 4.5f && newPosEmiZ <= 11.5f && newPosEmiY >= 0.5f && newPosEmiY <= 7.5f)
    {
        posEmisorX = newPosEmiX;
        posEmisorY = newPosEmiY;
        posEmisorZ = newPosEmiZ;
    }

    RenderScene();
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::DisableKeyboard(tagMSG &Msg, bool &Handled)
{
    if (Msg.message >= WM_KEYFIRST && Msg.message <= WM_KEYLAST)
    {
        Handled = true;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::DisableClose(TObject *Sender, bool &CanClose)
{
    CanClose = false;                                                           // Evitar que el formulario se cierre
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonReestablecerAuralizacionClick(TObject *Sender)
{
    // Restablecer posiciones
    thirdPersonDistance = 20.0f;
    cameraAngleAereo = 0.0f;
    cameraPitchAereo = 1.0f;
    posReceptorZ = 13.5f;
    posReceptorX = 0.0f;
    posReceptorY = 2.0f;
    posEmisorZ = -3.5f;
    cameraAngle = 0.0f;
    cameraPitch = 0.0f;
    posEmisorX = 0.0f;
    posEmisorY = 2.0f;

    // Restablecer ángulos del emisor
    emisorAngle_N = 0.0f;
    emisorAngle_S = 0.0f;
    emisorAngle_O = 0.0f;
    emisorAngle_E = 0.0f;
    emisorAngle_NE = 0.0f;
    emisorAngle_NO = 0.0f;
    emisorAngle_SE = 0.0f;
    emisorAngle_SO = 0.0f;

    // Reestablecimiento de ángulo
    receptorAngle_N = 0.0f;
    receptorAngle_S = 0.0f;
    receptorAngle_O = 0.0f;
    receptorAngle_E = 0.0f;
    receptorAngle_NE = 0.0f;
    receptorAngle_NO = 0.0f;
    receptorAngle_SE = 0.0f;
    receptorAngle_SO = 0.0f;

    // Devolviendo el foco al formulario principal
    ScrollBarNumeroRayos->Position = 100;
    ScrollBarReflection->Position = 0;
    ScrollBarAbsorption->Position = 0;
    ScrollBarReverb->Position = 0;
    ScrollBarDelay->Position = 0;
    ButtonReestablecerAuralizacion->Enabled = false;
    ButtonReestablecerAuralizacion->Enabled = true;

    //Graficando Escena
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonIniciarAuralizacionClick(TObject *Sender)
{
    // Abrir cuadro de dialogo
    OpenDialog1->Filter = "Archivos de audio WAV (*.wav)|*.wav|Todos los archivos (*.*)|*.*";
    OpenDialog1->DefaultExt = "wav";                                            // Extensión predeterminada

    // Mostrar el cuadro de diálogo
    if (OpenDialog1->Execute())
    {
        // Abrir cuadro de dialogo
        String fileName = OpenDialog1->FileName;

        // Usar std::ifstream para abrir el archivo
        std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::ate); // Abre en modo binario

        // Abrir el archivo de audio
        if (file.is_open())
        {
            // Obtener el tamaño del archivo
            std::streampos fileSize = file.tellg();                             // Toma la posición al final del archivo
            file.seekg(0, std::ios::beg);                                       // Regresar al principio del archivo

            // Leer el archivo en el buffer
            audioBuffer = (PBYTE)malloc(fileSize);
            file.read(reinterpret_cast<char*>(audioBuffer), fileSize);
            file.close();

            // Crear una copia del buffer original
            originalAudioBuffer = (PBYTE)malloc(fileSize);
            memcpy(originalAudioBuffer, audioBuffer, fileSize);                 // Cerrar el archivo

            // Configurar la cabecera para la reproducción del archivo WAV
            waveHeader.lpData = (LPSTR)audioBuffer;
            waveHeader.dwBufferLength = (DWORD)fileSize;
            waveHeader.dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
            waveHeader.dwLoops = INFINITE;                                      // Reproducción en bucle

            MMRESULT result = waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
            if (result != MMSYSERR_NOERROR)
            {
                ShowMessage("Error al preparar el encabezado del audio.");
                return;
            }
            
            // Habilitar la reproducción en bucle
            result = waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));
            if (result == MMSYSERR_NOERROR)
            {
                isPlaying = true;
                etiquetasP = true;                                              // Habilitar etiquetas de posición
                ButtonUno->Enabled = true;
                ButtonDos->Enabled = true;
                ButtonTres->Enabled = true;
                ButtonCero->Enabled = true;
                ButtonCinco->Enabled = true;
                ButtonCuatro->Enabled = true;
                Application->OnMessage = NULL;                                  // Habilitar Teclado
                ScrollBarDelay->Enabled = true;
                ScrollBarReverb->Enabled = true;
                ButtonTodosRayos->Enabled = true;
                PanelRenderScene->Enabled = true;                               // Habilitar Panel de Renderizado
                this->OnCloseQuery = DisableClose;                              // Deshabilitar Cierre Formulario
                GroupBoxControles->Enabled = true;
                GroupBoxServicios->Enabled = false;
                ButtonControlEmisor->Enabled = true;
                ScrollBarReflection->Enabled = true;
                ScrollBarAbsorption->Enabled = true;
                ScrollBarNumeroRayos->Enabled = true;
                ButtonControlReceptor->Enabled = true;
                ButtonDetenerAuralizacion->Enabled = true;
                ButtonIniciarAuralizacion->Enabled = false;
                ScrollBarNumeroRayos->Position = numeroRayos;
                ButtonReestablecerAuralizacion->Enabled = true;

                // Creación de un panel flotante para manejo del ángulo del emisor
                temporal = new TPanel(this);
                temporal->Parent = this;
                temporal->Width = 0;
                temporal->Height = 0;
                temporal->Left = 0;                                             // Posición inicial
                temporal->Top = 0;                                              // Posición inicial
                temporal->BevelOuter = bvNone;
                temporal->Color = clDefault;
            }
            else
            {
                ShowMessage("Error al reproducir el archivo de audio.");
            }
        }
        else
        {
            ShowMessage("No se pudo abrir el archivo WAV.");
        }
    }

}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonDetenerAuralizacionClick(TObject *Sender)
{
    // Restablecer los controles y las posiciones iniciales
    PanelRenderScene->Caption = "No Borrar esto es necesario para el correto funcionamiento.";
    ButtonReestablecerAuralizacion->Enabled = false;
    ButtonDetenerAuralizacion->Enabled = false;
    ButtonIniciarAuralizacion->Enabled = true;
    Application->OnMessage = DisableKeyboard;                                   // Deshabilitar Teclado
    ButtonControlReceptor->Enabled = false;
    ScrollBarNumeroRayos->Enabled = false;
    ScrollBarAbsorption->Enabled = false;
    ButtonControlEmisor->Enabled = false;
    ScrollBarReflection->Enabled = false;
    GroupBoxControles->Enabled = false;
    ButtonTodosRayos->Enabled = false;
    GroupBoxServicios->Enabled = true;
    ScrollBarReflection->Position = 0;
    ScrollBarAbsorption->Position = 0;
    PanelRenderScene->Enabled = false;                                          // Deshabilitar Panel de Renderizado
    ScrollBarReverb->Enabled = false;
    ScrollBarDelay->Enabled = false;
    ScrollBarReverb->Position = 0;
    ButtonCuatro->Enabled = false;
    ButtonCinco->Enabled = false;
    controlAnguloReceptor = true;
    ScrollBarDelay->Position = 0;
    Dato_Rayos->Caption = "0,00";
    Relativo_X->Caption = "0,00";
    Relativo_Y->Caption = "0,00";
    Relativo_Z->Caption = "0,00";
    Pos_X_Emi->Caption = "0,00";
    Pos_Y_Emi->Caption = "0,00";
    Pos_Z_Emi->Caption = "0,00";
    Pos_X_Rec->Caption = "0,00";
    Pos_Y_Rec->Caption = "0,00";
    Pos_Z_Rec->Caption = "0,00";
    ButtonCero->Enabled = false;
    ButtonTres->Enabled = false;
    controlAnguloEmisor = true;
    ButtonUno->Enabled = false;
    ButtonDos->Enabled = false;
    Nivel_R->Caption = "0,00";
    Nivel_L->Caption = "0,00";
    this->OnCloseQuery = NULL;                                                  // Habilitar Cierre Formulario
    etiquetasP = false;                                                         // Deshabilitar etiquetas de posición
    
    // Restablecer posiciones
    thirdPersonDistance = 20.0f;
    cameraAngleAereo = 0.0f;
    cameraPitchAereo = 1.0f;
    posReceptorZ = 13.5f;
    posReceptorX = 0.0f;
    posReceptorY = 2.0f;
    posEmisorZ = -3.5f;
    cameraAngle = 0.0f;
    cameraPitch = 0.0f;
    posEmisorX = 0.0f;
    posEmisorY = 2.0f;
    numeroRayos = 100;

    // Restablecer ángulos del emisor
    emisorAngle_N = inicialEmisorAngle;
    emisorAngle_S = inicialEmisorAngle;
    emisorAngle_O = inicialEmisorAngle;
    emisorAngle_E = inicialEmisorAngle;
    emisorAngle_NE = inicialEmisorAngle;
    emisorAngle_NO = inicialEmisorAngle;
    emisorAngle_SE = inicialEmisorAngle;
    emisorAngle_SO = inicialEmisorAngle;

    // Reestablecimiento de ángulo
    receptorAngle_N = inicialReceptorAngle;
    receptorAngle_S = inicialReceptorAngle;
    receptorAngle_O = inicialReceptorAngle;
    receptorAngle_E = inicialReceptorAngle;
    receptorAngle_NE = inicialReceptorAngle;
    receptorAngle_NO = inicialReceptorAngle;
    receptorAngle_SE = inicialReceptorAngle;
    receptorAngle_SO = inicialReceptorAngle;

    // Asegúrase de eliminar cualquier instancia existente
    if (gamepadPanelReceptor != NULL)
    {
        gamepadPanelReceptor->Visible = false;
        delete gamepadPanelReceptor;
        gamepadPanelReceptor = NULL;
    }
    // Asegúrase de eliminar cualquier instancia existente
    if (gamepadPanelEmisor != NULL)
    {
        gamepadPanelEmisor->Visible = false;
        delete gamepadPanelEmisor;
        gamepadPanelEmisor = NULL;
    }

    // Si se esta reproduciendo un sonido se lo detiene
    if (isPlaying)
    {
        waveOutReset(hWaveOut);                                                 // Detener la reproducción
        isPlaying = false;
    }

    PanelRenderScene->Caption = "Activar e Iniciar e Motor de Aurilización";
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonControlReceptorClick(TObject *Sender)
{
    try
    {
        if(controlAnguloReceptor)
        {
            // Creación de un panel flotante para manejo del ángulo del emisor
            gamepadPanelReceptor = new TPanel(this);
            gamepadPanelReceptor->Parent = this;
            gamepadPanelReceptor->Width = 130;
            gamepadPanelReceptor->Height = 130;
            gamepadPanelReceptor->Left = 1080;                                  // Posición inicial
            gamepadPanelReceptor->Top = 655;                                    // Posición inicial
            gamepadPanelReceptor->BevelOuter = bvNone;
            gamepadPanelReceptor->Color = clDefault;

            // Creación de los botones del panel
            TButton* ReceptorNOButton = new TButton(gamepadPanelReceptor);
            ReceptorNOButton->Parent = gamepadPanelReceptor;
            ReceptorNOButton->Width = 30;
            ReceptorNOButton->Height = 30;
            ReceptorNOButton->Left = 10;
            ReceptorNOButton->Top = 10;
            ReceptorNOButton->Caption = "NO";
            ReceptorNOButton->OnClick = ReceptorNOButtonClick;

            TButton* ReceptorNButton = new TButton(gamepadPanelReceptor);
            ReceptorNButton->Parent = gamepadPanelReceptor;
            ReceptorNButton->Width = 30;
            ReceptorNButton->Height = 30;
            ReceptorNButton->Left = 50;
            ReceptorNButton->Top = 10;
            ReceptorNButton->Caption = "N";
            ReceptorNButton->OnClick = ReceptorNButtonClick;

            TButton* ReceptorNEButton = new TButton(gamepadPanelReceptor);
            ReceptorNEButton->Parent = gamepadPanelReceptor;
            ReceptorNEButton->Width = 30;
            ReceptorNEButton->Height = 30;
            ReceptorNEButton->Left = 90;
            ReceptorNEButton->Top = 10;
            ReceptorNEButton->Caption = "NE";
            ReceptorNEButton->OnClick = ReceptorNEButtonClick;

            TButton* ReceptorOButton = new TButton(gamepadPanelReceptor);
            ReceptorOButton->Parent = gamepadPanelReceptor;
            ReceptorOButton->Width = 30;
            ReceptorOButton->Height = 30;
            ReceptorOButton->Left = 10;
            ReceptorOButton->Top = 50;
            ReceptorOButton->Caption = "O";
            ReceptorOButton->OnClick = ReceptorOButtonClick;

            TButton* ReceptorCButton = new TButton(gamepadPanelReceptor);
            ReceptorCButton->Parent = gamepadPanelReceptor;
            ReceptorCButton->Width = 30;
            ReceptorCButton->Height = 30;
            ReceptorCButton->Left = 50;
            ReceptorCButton->Top = 50;
            ReceptorCButton->Caption = "R";
            ReceptorCButton->OnClick = ReceptorCButtonClick;

            TButton* ReceptorEButton = new TButton(gamepadPanelReceptor);
            ReceptorEButton->Parent = gamepadPanelReceptor;
            ReceptorEButton->Width = 30;
            ReceptorEButton->Height = 30;
            ReceptorEButton->Left = 90;
            ReceptorEButton->Top = 50;
            ReceptorEButton->Caption = "E";
            ReceptorEButton->OnClick = ReceptorEButtonClick;

            TButton* ReceptorSOButton = new TButton(gamepadPanelReceptor);
            ReceptorSOButton->Parent = gamepadPanelReceptor;
            ReceptorSOButton->Width = 30;
            ReceptorSOButton->Height = 30;
            ReceptorSOButton->Left = 10;
            ReceptorSOButton->Top = 90;
            ReceptorSOButton->Caption = "SO";
            ReceptorSOButton->OnClick = ReceptorSOButtonClick;

            TButton* ReceptorSButton = new TButton(gamepadPanelReceptor);
            ReceptorSButton->Parent = gamepadPanelReceptor;
            ReceptorSButton->Width = 30;
            ReceptorSButton->Height = 30;
            ReceptorSButton->Left = 50;
            ReceptorSButton->Top = 90;
            ReceptorSButton->Caption = "S";
            ReceptorSButton->OnClick = ReceptorSButtonClick;

            TButton* ReceptorSEButton = new TButton(gamepadPanelReceptor);
            ReceptorSEButton->Parent = gamepadPanelReceptor;
            ReceptorSEButton->Width = 30;
            ReceptorSEButton->Height = 30;
            ReceptorSEButton->Left = 90;
            ReceptorSEButton->Top = 90;
            ReceptorSEButton->Caption = "SE";
            ReceptorSEButton->OnClick = ReceptorSEButtonClick;

            // Habilitar la visualización
            gamepadPanelReceptor->Visible = true;
            controlAnguloReceptor = false;
        }
        else
        {
            gamepadPanelReceptor->Visible = false;
            controlAnguloReceptor = true;
            PanelRenderScene->Repaint();
        }

        // Habilar Foco
        ButtonControlReceptor->Enabled =false;
        ButtonControlReceptor->Enabled =true;
    }
    catch (Exception & exception)
    {
        ShowMessage("Ha ocurrido un error de ejecución: " + exception.Message);
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonControlEmisorClick(TObject *Sender)
{
    try
    {
        if(controlAnguloEmisor)
        {
            // Creación de un panel flotante para manejo del ángulo del emisor
            gamepadPanelEmisor = new TPanel(this);
            gamepadPanelEmisor->Parent = this;
            gamepadPanelEmisor->Width = 130;
            gamepadPanelEmisor->Height = 130;
            gamepadPanelEmisor->Left = 1080;                                    // Posición inicial
            gamepadPanelEmisor->Top = 64;                                       // Posición inicial
            gamepadPanelEmisor->BevelOuter = bvNone;
            gamepadPanelEmisor->Color = clDefault;

            // Creación de los botones del panel
            TButton* EmisorNOButton = new TButton(gamepadPanelEmisor);
            EmisorNOButton->Parent = gamepadPanelEmisor;
            EmisorNOButton->Width = 30;
            EmisorNOButton->Height = 30;
            EmisorNOButton->Left = 10;
            EmisorNOButton->Top = 10;
            EmisorNOButton->Caption = "NO";
            EmisorNOButton->OnClick = EmisorNOButtonClick;

            TButton* EmisorNButton = new TButton(gamepadPanelEmisor);
            EmisorNButton->Parent = gamepadPanelEmisor;
            EmisorNButton->Width = 30;
            EmisorNButton->Height = 30;
            EmisorNButton->Left = 50;
            EmisorNButton->Top = 10;
            EmisorNButton->Caption = "N";
            EmisorNButton->OnClick = EmisorNButtonClick;

            TButton* EmisorNEButton = new TButton(gamepadPanelEmisor);
            EmisorNEButton->Parent = gamepadPanelEmisor;
            EmisorNEButton->Width = 30;
            EmisorNEButton->Height = 30;
            EmisorNEButton->Left = 90;
            EmisorNEButton->Top = 10;
            EmisorNEButton->Caption = "NE";
            EmisorNEButton->OnClick = EmisorNEButtonClick;

            TButton* EmisorOButton = new TButton(gamepadPanelEmisor);
            EmisorOButton->Parent = gamepadPanelEmisor;
            EmisorOButton->Width = 30;
            EmisorOButton->Height = 30;
            EmisorOButton->Left = 10;
            EmisorOButton->Top = 50;
            EmisorOButton->Caption = "O";
            EmisorOButton->OnClick = EmisorOButtonClick;

            TButton* EmisorCButton = new TButton(gamepadPanelEmisor);
            EmisorCButton->Parent = gamepadPanelEmisor;
            EmisorCButton->Width = 30;
            EmisorCButton->Height = 30;
            EmisorCButton->Left = 50;
            EmisorCButton->Top = 50;
            EmisorCButton->Caption = "R";
            EmisorCButton->OnClick = EmisorCButtonClick;

            TButton* EmisorEButton = new TButton(gamepadPanelEmisor);
            EmisorEButton->Parent = gamepadPanelEmisor;
            EmisorEButton->Width = 30;
            EmisorEButton->Height = 30;
            EmisorEButton->Left = 90;
            EmisorEButton->Top = 50;
            EmisorEButton->Caption = "E";
            EmisorEButton->OnClick = EmisorEButtonClick;

            TButton* EmisorSOButton = new TButton(gamepadPanelEmisor);
            EmisorSOButton->Parent = gamepadPanelEmisor;
            EmisorSOButton->Width = 30;
            EmisorSOButton->Height = 30;
            EmisorSOButton->Left = 10;
            EmisorSOButton->Top = 90;
            EmisorSOButton->Caption = "SO";
            EmisorSOButton->OnClick = EmisorSOButtonClick;

            TButton* EmisorSButton = new TButton(gamepadPanelEmisor);
            EmisorSButton->Parent = gamepadPanelEmisor;
            EmisorSButton->Width = 30;
            EmisorSButton->Height = 30;
            EmisorSButton->Left = 50;
            EmisorSButton->Top = 90;
            EmisorSButton->Caption = "S";
            EmisorSButton->OnClick = EmisorSButtonClick;

            TButton* EmisorSEButton = new TButton(gamepadPanelEmisor);
            EmisorSEButton->Parent = gamepadPanelEmisor;
            EmisorSEButton->Width = 30;
            EmisorSEButton->Height = 30;
            EmisorSEButton->Left = 90;
            EmisorSEButton->Top = 90;
            EmisorSEButton->Caption = "SE";
            EmisorSEButton->OnClick = EmisorSEButtonClick;

            // Habilitar la visualización
            gamepadPanelEmisor->Visible = true;
            controlAnguloEmisor = false;
        }
        else
        {
            gamepadPanelEmisor->Visible = false;
            PanelRenderScene->Repaint();
            controlAnguloEmisor = true;
        }

        // Habilar Foco
        ButtonControlEmisor->Enabled =false;
        ButtonControlEmisor->Enabled =true;
    }
    catch (Exception & exception)
    {
        ShowMessage("Ha ocurrido un error de ejecución: " + exception.Message);
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonDetenerPruebaClick(TObject *Sender)
{
    // Restablecer los controles y las posiciones iniciales
    GroupBoxServicios->Enabled = true;
    ScrollBarPrueba->Enabled = false;
    ScrollBarPrueba->Position = 50;                                             // Valor del centro
    this->OnCloseQuery = NULL;                                                  // Permitir que el formulario se cierre
    ButtonDetenerPrueba->Enabled = false;
    ButtonIniciarPrueba->Enabled = true;
    if (isPlaying)
    {
        waveOutReset(hWaveOut);                                                 // Detener la reproducción
        isPlaying = false;
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonIniciarPruebaClick(TObject *Sender)
{
    // Abrir cuadro de dialogo
    OpenDialog1->Filter = "Archivos de audio WAV (*.wav)|*.wav|Todos los archivos (*.*)|*.*";
    OpenDialog1->DefaultExt = "wav";                                            // Extensión predeterminada

    // Mostrar el cuadro de diálogo
    if (OpenDialog1->Execute())
    {
	    // Abrir cuadro de dialogo
	    String fileName = OpenDialog1->FileName;

	    // Usar std::ifstream para abrir el archivo
	    std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::ate);     // Abre en modo binario

	    // Abrir el archivo de audio
	    if (file.is_open())
	    {
		    // Obtener el tamaño del archivo
		    std::streampos fileSize = file.tellg();                     // Toma la posición al final del archivo
		    file.seekg(0, std::ios::beg);                               // Regresar al principio del archivo

		    // Leer el archivo en el buffer
		    audioBuffer = (PBYTE)malloc(fileSize);
		    file.read(reinterpret_cast<char*>(audioBuffer), fileSize);
		    file.close();                                                       // Cerrar el archivo

		    // Configurar la cabecera para la reproducción del archivo WAV
		    waveHeader.lpData = (LPSTR)audioBuffer;
		    waveHeader.dwBufferLength = (DWORD)fileSize;
		    waveHeader.dwFlags = 0;
		    waveHeader.dwLoops = 1;                                             // Reproducir una vez inicialmente


		    // Preparar el buffer para la reproducción
		    waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));

		    // Reproducir el audio
                    ButtonIniciarPrueba->Enabled = false;
		    ButtonDetenerPrueba->Enabled = true;
		    GroupBoxServicios->Enabled = false;
                    this->OnCloseQuery = DisableClose;
                    ScrollBarPrueba->Enabled = true;
		    MMRESULT result = waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));
		    if (result == MMSYSERR_NOERROR)
		    {
			    isPlaying = true;

			    // Activar bucle manualmente en caso de que termine
			    while (isPlaying)
			    {
				    if (waveHeader.dwFlags & WHDR_DONE)         // Verifica si terminó la reproducción
				    {
					    waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));       // Reinicia la reproducción
				    }
				    Application->ProcessMessages();             // Permite que la aplicación responda
			    }
		    }
		    else
		    {
			    ShowMessage("Error al reproducir el archivo de audio.");
		    }
	    }
	    else
	    {
		    ShowMessage("No se pudo abrir el archivo WAV.");
	    }
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonTodosRayosClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showTotalRays)
    {
        showTotalRays = true;

        ButtonCuatro->Enabled = false;
        ButtonCinco->Enabled = false;
        ButtonCero->Enabled = false;
        ButtonTres->Enabled = false;
        ButtonUno->Enabled = false;
        ButtonDos->Enabled = false;
    }
    else
    {
        showTotalRays = false;

        ButtonCuatro->Enabled = true;
        ButtonCinco->Enabled = true;
        ButtonCero->Enabled = true;
        ButtonTres->Enabled = true;
        ButtonUno->Enabled = true;
        ButtonDos->Enabled = true;
    }

    // Habilar Foco
    ButtonTodosRayos->Enabled =false;
    ButtonTodosRayos->Enabled =true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonCuatroClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showFourthReflection)
    {
        showFourthReflection = true;
    }
    else
    {
        showFourthReflection = false;
    }

    // Habilar Foco
    ButtonCuatro->Enabled =false;
    ButtonCuatro->Enabled =true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonCincoClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showFifthReflection)
    {
        showFifthReflection = true;
    }
    else
    {
        showFifthReflection = false;
    }

    // Habilar Foco
    ButtonCinco->Enabled =false;
    ButtonCinco->Enabled =true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonCeroClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showOriginalRays)
    {
        showOriginalRays = true;
    }
    else
    {
        showOriginalRays = false;
    }

    // Habilar Foco
    ButtonCero->Enabled =false;
    ButtonCero->Enabled =true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonTresClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showThirdReflection)
    {
        showThirdReflection = true;
    }
    else
    {
        showThirdReflection = false;
    }

    // Habilar Foco
    ButtonTres->Enabled =false;
    ButtonTres->Enabled =true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonUnoClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showFirstReflection)
    {
        showFirstReflection = true;
    }
    else
    {
        showFirstReflection = false;
    }
    // Habilar Foco
    ButtonUno->Enabled =false;
    ButtonUno->Enabled =true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ButtonDosClick(TObject *Sender)
{
    // Habilitar la vista de los rayos
    if(!showSecondReflection)
    {
        showSecondReflection = true;
    }
    else
    {
        showSecondReflection = false;
    }

    // Habilar Foco
    ButtonDos->Enabled =false;
    ButtonDos->Enabled =true;
    RenderScene();
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::RadioButtonCanalesClick(TObject *Sender)
{
    // Detenar audio
    ButtonIniciarAuralizacion->Enabled = false;
    ButtonIniciarPrueba->Enabled = true;
    if (isPlaying)
    {
        waveOutReset(hWaveOut);                                                 // Detener la reproducción
        isPlaying = false;
    }

    // Habilitar el GroupBox si el RadioButton está seleccionado
    if (RadioButtonCanales->Checked)
    {
        GroupBoxCanales->Enabled = true;                                        // Habilitar
        GroupBoxRelativo->Enabled = false;                                      // Deshabilitar
    }
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::RadioButtonMotorClick(TObject *Sender)
{
    // Detenar audio
    ButtonIniciarAuralizacion->Enabled = true;
    ButtonIniciarPrueba->Enabled = false;
    ScrollBarPrueba->Position = 50;                                             // Valor del centro
    if (isPlaying)
    {
        waveOutReset(hWaveOut);                                                 // Detener la reproducción
        isPlaying = false;
    }

    // Habilitar el GroupBox si el RadioButton está seleccionado
    if (RadioButtonMotor->Checked)
    {
        GroupBoxCanales->Enabled = false;                                       // Habilitar
        GroupBoxRelativo->Enabled = true;                                       // Deshabilitar
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TFormAuralizationMotor::EmisorNOButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_NO += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorNEButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_NE -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorSOButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_SO += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorSEButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_SE -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorNButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_N -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorOButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_O += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorCButtonClick(TObject *Sender)
{
    // Restablecer ángulos del emisor
    emisorAngle_N = 0.0f;
    emisorAngle_S = 0.0f;
    emisorAngle_O = 0.0f;
    emisorAngle_E = 0.0f;
    emisorAngle_NE = 0.0f;
    emisorAngle_NO = 0.0f;
    emisorAngle_SE = 0.0f;
    emisorAngle_SO = 0.0f;
    
    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorEButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_E -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::EmisorSButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    emisorAngle_S += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelEmisor->Visible = false;
    gamepadPanelEmisor->Visible = true;
    RenderScene();
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorNOButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_NO += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorNEButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_NE -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorSOButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_SO += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorSEButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_SE -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorNButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_N -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorOButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_O -= 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorCButtonClick(TObject *Sender)
{
    // Reestablecimiento de ángulo
    receptorAngle_N = 0.0f;
    receptorAngle_S = 0.0f;
    receptorAngle_O = 0.0f;
    receptorAngle_E = 0.0f;
    receptorAngle_NE = 0.0f;
    receptorAngle_NO = 0.0f;
    receptorAngle_SE = 0.0f;
    receptorAngle_SO = 0.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorEButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_E += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ReceptorSButtonClick(TObject *Sender)
{
    // Cambio de ángulo
    receptorAngle_S += 30.0f;

    // Devolviendo el foco al formulario principal
    gamepadPanelReceptor->Visible = false;
    gamepadPanelReceptor->Visible = true;
    RenderScene();
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ScrollBarNumeroRayosChange(TObject *Sender)
{
    // Obteniendo el número de rayos de la posición del scrollbar
    numeroRayos = ScrollBarNumeroRayos->Position;

    // Llama a la función drawRaysFromEmitter con el nuevo número de rayos
    drawRaysFromEmitter(posEmisorX, posEmisorY, posEmisorZ, posReceptorX, posReceptorY, posReceptorZ, numeroRayos);
    RenderScene();

    // Llamar a la función de actualización
    UpdateScrollBarLabels();

    // Devolviendo el foco al formulario principal
    ScrollBarNumeroRayos->Enabled = false;
    ScrollBarNumeroRayos->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ScrollBarReflectionChange(TObject *Sender)
{
    // Agregando efectos de sonido
    reflectionValue = ScrollBarReflection->Position / (float)ScrollBarReflection -> Max;        // Normalizar entre [0, 1]
    ApplyAudioEffects();                                                                        // Llamar al método que aplica los efectos

    // Llamar a la función de actualización
    UpdateScrollBarLabels();

    // Devolviendo el foco al formulario principal
    ScrollBarReflection->Enabled = false;
    ScrollBarReflection->Enabled = true;
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ScrollBarAbsorptionChange(TObject *Sender)
{
    // Agregando efectos de sonido
    absorptionValue = ScrollBarAbsorption->Position / (float)ScrollBarAbsorption -> Max;
    ApplyAudioEffects();                                                        // Llamar al método que aplica los efectos

    // Llamar a la función de actualización
    UpdateScrollBarLabels();

    // Devolviendo el foco al formulario principal
    ScrollBarAbsorption->Enabled = false;
    ScrollBarAbsorption->Enabled = true;
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ScrollBarPruebaChange(TObject *Sender)
{
    // El valor del ScrollBar se mapea a un rango de -1 (totalmente izquierdo) a 1 (totalmente derecho)
    float panning = (ScrollBarPrueba->Position - ScrollBarPrueba->Min) / float(ScrollBarPrueba->Max - ScrollBarPrueba->Min) * 2 - 1;

    // Definir la mezcla de los canales izquierdo y derecho
    DWORD leftVolume = 0xFFFF * (1 - (panning > 0 ? panning : 0));
    DWORD rightVolume = 0xFFFF * (1 + (panning < 0 ? panning : 0));

    // Ajustar la mezcla de los canales
    waveOutSetVolume(hWaveOut, MAKELONG(leftVolume, rightVolume));
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ScrollBarReverbChange(TObject *Sender)
{
    // Agregando efectos de sonido
    reverbValue = ScrollBarReverb->Position / (float)ScrollBarReverb -> Max;    // Normalizar entre [0, 1]
    ApplyAudioEffects();                                                        // Llamar al método que aplica los efectos

    // Llamar a la función de actualización
    UpdateScrollBarLabels();

    // Devolviendo el foco al formulario principal
    ScrollBarReverb->Enabled = false;
    ScrollBarReverb->Enabled = true;
}


//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::ScrollBarDelayChange(TObject *Sender)
{
    // Agregando efectos de sonido
    delayValue = ScrollBarDelay->Position / (float)ScrollBarDelay -> Max;
    ApplyAudioEffects();

    // Llamar a la función de actualización
    UpdateScrollBarLabels();

    // Devolviendo el foco al formulario principal
    ScrollBarDelay->Enabled = false;
    ScrollBarDelay->Enabled = true;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::UpdateScrollBarLabels()
{
    // Mostrar etiquetas con valores de los ScrollBars
    Dato_Rayos->Caption = IntToStr(ScrollBarNumeroRayos->Position) + ",00";
    Dato_Reflexion->Caption = IntToStr(ScrollBarReflection->Position) + "%";
    Dato_Reverberacion->Caption = IntToStr(ScrollBarReverb->Position) + "%";
    Dato_Retraso->Caption = IntToStr(ScrollBarDelay->Position) + " ms";
    Dato_Absorcion->Caption = IntToStr(ScrollBarAbsorption->Position) + "%";
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
Vec3 transformPoint(const float matrix[16], const Vec3& point)
{
    Vec3 result;
    result.x = matrix[0] * point.x + matrix[4] * point.y + matrix[8] * point.z;
    result.y = matrix[1] * point.x + matrix[5] * point.y + matrix[9] * point.z;
    result.z = matrix[2] * point.x + matrix[6] * point.y + matrix[10] * point.z;
    return result;
}


//---------------------------------------------------------------------------
Vec3 subtract(const Vec3& a, const Vec3& b)
{
    // Función para restar dos vectores
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    // Verificar si el resultado es válido
    if (!_finite(result.x) || !_finite(result.y) || !_finite(result.z))
    {
        // Si hay error, se asignan valores seguros
        result.x = result.y = result.z = 0.0f;
    }

    return result;
}


//---------------------------------------------------------------------------
Vec3 multiply(const Vec3& a, float scalar)
{
    // Función para multiplicar un vector por un escalar
    Vec3 result;
    
    // Evitar valores extremos en la multiplicación
    if (!_finite(scalar) || fabs(scalar) > 1e6f)
    {
        // Si el escalar es infinito o muy grande, se limita a 1.0f
        scalar = 1.0f;
    }

    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;

    // Verificar si el resultado es válido
    if (!_finite(result.x) || !_finite(result.y) || !_finite(result.z))
    {
        // Se asignan valores seguros si hay error
        result.x = result.y = result.z = 0.0f;
    }

    return result;
}


//---------------------------------------------------------------------------
Vec3 add(const Vec3& a, const Vec3& b)
{
    // Función para sumar dos vectores
    Vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

     // Verificar si el resultado contiene valores finitos (no INF o NaN)
    if (!_finite(result.x) || !_finite(result.y) || !_finite(result.z))
    {
        // Evitar problemas asignando valores seguros
        result.x = result.y = result.z = 0.0f;
    }

    return result;
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool intersectPlane(const Vec3& planeNormal, const Vec3& planePoint, const Vec3& rayOrigin, const Vec3& rayDirection, Vec3& intersectionPoint)
{
    // Función para calcular la intersección de un rayo con un plano
    float d = -dot(planeNormal, planePoint);

    // Calcular el denominador de la ecuación paramétrica
    float denominator = dot(planeNormal, rayDirection);

    // Evitar la división por cero o valores muy pequeños
    if (std::fabs(denominator) > FLT_EPSILON)
    {
        float t = -(dot(planeNormal, rayOrigin) + d) / denominator;

        // Si t >= 0 la intersección es válida
        if (t >= 0.0f)
        {
            intersectionPoint = add(rayOrigin, multiply(rayDirection, t));
            return true;
        }
    }

    // No hay intersección
    return false;
}


//---------------------------------------------------------------------------
bool intersectSphere(const Vec3& center, float radius, const Vec3& origin, const Vec3& direction, Vec3& intersectionPoint)
{
    Vec3 oc = subtract(origin, center);
    float a = dot(direction, direction);
    float b = 2.0f * dot(oc, direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        // No hay intersección
        return false;
    }
    else
    {
        // Calcular los puntos de intersección
        float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

        // Elegir el t más cercano que sea positivo
        float t = t1;
        if (t1 < 0)
        {
            // Usar t2 si t1 es negativo
            t = t2;
        }
        if (t2 < 0)
        {
            // Si ambos t1 y t2 son negativos, no hay intersección válida
            t = -1;
        }

        if (t < 0)
        {
            // La intersección está detrás del origen del rayo
            return false;
        }

        // Calcular el punto de intersección
        intersectionPoint = add(origin, multiply(direction, t));
        return true;
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFormAuralizationMotor::ApplyReflections(short *buffer, int size, float intensity)
{
    // Mejorar la reflexión para que sea acumulativa y más clara
    if (intensity <= 0.0f) return;

    int reflectionDelay = 2205;                                                 // Retardo de 50ms
    for (int i = reflectionDelay; i < size / 2; i++)
    {
        buffer[i] += intensity * buffer[i - reflectionDelay];
    }
}


//---------------------------------------------------------------------------
void TFormAuralizationMotor::ApplyAbsorption(short *buffer, int size, float intensity)
{
    // Simular la absorción aplicando un filtro de paso bajo
    if (intensity <= 0.0f) return;

    for (int i = 1; i < size / 2; i++)
    {
        buffer[i] = buffer[i - 1] + (buffer[i] - buffer[i - 1]) * (1.0f - intensity);
    }
}


//---------------------------------------------------------------------------
void TFormAuralizationMotor::ApplyReverb(short *buffer, int size, float intensity)
{
    // Agregar múltiples retardos de diferentes intensidades para simular una reverberación más natural
    if (intensity <= 0.0f) return;

    const int numDelays = 3;                                                    // Número de retardos para simular la reverberación
    const int delays[numDelays] = {4410, 8820, 13230};                          // Retardos en muestras (100ms, 200ms, 300ms)
    const float decay[numDelays] = {0.5f, 0.25f, 0.125f};                       // Decadencia para cada retardo

    for (int i = 0; i < numDelays; i++)
    {
        for (int j = delays[i]; j < size / 2; j++)
        {
            buffer[j] += intensity * decay[i] * buffer[j - delays[i]];
        }
    }
}


//---------------------------------------------------------------------------
void TFormAuralizationMotor::ApplyDelay(short *buffer, int size, float intensity)
{
    // Permitir retardos configurables mediante el valor del scrollbar
    // Se usa el teorema de muestreo de Nyquist-Shannon, si se desea mejorar 48000 Hz
    if (intensity <= 0.0f) return;

    int delaySamples = (int)(44100 * (delayValue / 100.0f));                    // Calcular retardo según scrollbar
    for (int i = delaySamples; i < size / 2; i++)
    {
        buffer[i] += intensity * buffer[i - delaySamples];
    }
}


//---------------------------------------------------------------------------
void TFormAuralizationMotor::ApplyAudioEffects()
{
    if (!audioBuffer || !originalAudioBuffer) return;                           // Asegúrate de que los buffers están inicializados

    // Restaurar el buffer al estado original
    int bufferSize = waveHeader.dwBufferLength;
    memcpy(audioBuffer, originalAudioBuffer, bufferSize);

    // Crear un buffer temporal para procesar los efectos
    short *tempBuffer = reinterpret_cast<short*>(malloc(bufferSize));
    memcpy(tempBuffer, audioBuffer, bufferSize);                                // Copiar el buffer original

    // Aplicar efectos si sus valores son mayores a 0
    if (reflectionValue > 0.0f)
        ApplyReflections(tempBuffer, bufferSize, reflectionValue);

    if (reverbValue > 0.0f)
        ApplyReverb(tempBuffer, bufferSize, reverbValue);

    if (delayValue > 0.0f)
        ApplyDelay(tempBuffer, bufferSize, delayValue);

    if (absorptionValue > 0.0f)
        ApplyAbsorption(tempBuffer, bufferSize, absorptionValue);

    // Actualizar el buffer procesado al header del audio
    memcpy(audioBuffer, tempBuffer, bufferSize);
    free(tempBuffer);

    // Reproducir el buffer actualizado
    waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));
}


//---------------------------------------------------------------------------
void TFormAuralizationMotor::CleanupAudio()
{
    // Limpiar el buffer de audio original
    if (originalAudioBuffer)
    {
        free(originalAudioBuffer);
        originalAudioBuffer = NULL;
    }

    // Limpiar el buffer de la copia de audio
    if (audioBuffer)
    {
        free(audioBuffer);
        audioBuffer = NULL;
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TFormAuralizationMotor::ClampBuffer(short *buffer, int size)
{
    // Asegúrarse de que los valores del buffer no excedan los límites de un entero de 16 bits
    for (int i = 0; i < size / 2; i++)
    {
        buffer[i] = ClampSample(buffer[i]);
    }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormAuralizationMotor::drawRaysFromEmitter(float posEX, float posEY, float posEZ, float posRX, float posRY, float posRZ, int numRays)
{
    // Definición de umbrales para evitar errores numéricos y establecer límites
    float minDistanceThreshold = 0.0001f;
    float reflectionThreshold = 0.0001f;

    // Definición de los rangos de ángulos de emisión de los rayos
    float maxAngleLng = 120.0f;
    float maxAngleLat = -30.0f;
    float minAngleLat = 30.0f;
    float minAngleLng = 60.0f;

    // Parámetros de longitud de los rayos y número máximo de reflexiones
    float rayLength = 100.0f;
    int maxReflections = 5;

    // Definición de planos que representan el entorno
    Vec3 planeNormals[] = {{0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}, {0, 0, 1}, {0, 0, -1}};
    Vec3 planePoints[] = {{0, 0, 0}, {0, 8, 0}, {4, 0, 0}, {-12, 0, 0}, {0, 0, 12}, {0, 0, -4}};

    // Inicialización de la semilla para la generación de números aleatorios
    // Semilla fija para resultados reproducibles
    //srand(0);
    srand(static_cast<unsigned int>(time(0)));

    // Contadores para los rayos que llegan a cada canal
    int raysLeftChannel = 0;
    int raysRightChannel = 0;

    for (int i = 0; i < numRays; ++i)
    {
        // Cálculo de ángulos aleatorios dentro del rango permitido
        float angleLat = minAngleLat + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxAngleLat - minAngleLat);
        float angleLng = minAngleLng + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxAngleLng - minAngleLng);

        // Conversión de ángulos a radianes
        float radAngleLat = angleLat * 3.14159265359f / 180.0f;
        float radAngleLng = angleLng * 3.14159265359f / 180.0f;

        // Definición del punto de origen del rayo y su dirección inicial
        Vec3 rayOrigin = {posEX, posEY, posEZ};
        Vec3 rayDirection = {cos(radAngleLat) * cos(radAngleLng), sin(radAngleLat), cos(radAngleLat) * sin(radAngleLng)};

        // Definición del punto de destino
        Vec3 receiverPosition = {posRX, posRY, posRZ};

        // Aplicación de transformaciones para orientar el emisor
        glPushMatrix();
        glLoadIdentity();
        float matrix[16];
        glRotatef(emisorAngle_N, 1, 0, 0);
        glRotatef(emisorAngle_S, 1, 0, 0);
        glRotatef(-emisorAngle_O, 0, 1, 0);
        glRotatef(-emisorAngle_E, 0, 1, 0);
        glRotatef(-emisorAngle_NO, 1, 1, 0);
        glRotatef(-emisorAngle_SE, 1, 1, 0);
        glRotatef(-emisorAngle_NE, -1, 1, 0);
        glRotatef(-emisorAngle_SO, -1, 1, 0);
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        glPopMatrix();
        
        // Aplicación de la transformación a la dirección del rayo
        Vec3 transformedRayDirection =
        {
            matrix[0] * rayDirection.x + matrix[4] * rayDirection.y + matrix[8] * rayDirection.z,
            matrix[1] * rayDirection.x + matrix[5] * rayDirection.y + matrix[9] * rayDirection.z,
            matrix[2] * rayDirection.x + matrix[6] * rayDirection.y + matrix[10] * rayDirection.z
        };

        // Dirección rayo global
        Vec3 rayDirectionGlobal = transformedRayDirection;

        for (int reflection = 0; reflection <= maxReflections; ++reflection)
        {
            // Cálculo de la reflexiones en los planos del entorno
            float closestDistance = rayLength;
            Vec3 closestIntersection;
            Vec3 normalAtIntersection;
            bool intersectsFourthQuadrant = false;
            Vec3 intersectionInFourthQuadrant;

            // Detección de intersección con los planos del entorno
            for (int j = 0; j < sizeof(planeNormals) / sizeof(planeNormals[j]); ++j)
            {
                Vec3 intersectionPoint;
                if (intersectPlane(planeNormals[j], planePoints[j], rayOrigin, rayDirectionGlobal, intersectionPoint))
                {
                    float distance = length(subtract(intersectionPoint, rayOrigin));
                    if (distance <= closestDistance && distance >= minDistanceThreshold)
                    {
                        closestDistance = distance;
                        closestIntersection = intersectionPoint;
                        normalAtIntersection = planeNormals[j];
                    }
                }
            }

            // Vector de la intersección más cercana
            Vec3 endPoint = closestIntersection;
            Vec3 currentPoint = rayOrigin;
            float t = 0.0f;
            float step = 0.1f;

            // Vector de la intersección previa para el cuarto cuadrante
            Vec3 previousPoint = rayOrigin;
            
            while (t <= closestDistance)
            {
                Vec3 nextPoint = add(rayOrigin, multiply(rayDirectionGlobal, t));

                if ((nextPoint.x >= -12 && nextPoint.x <= -4) && (nextPoint.y >= 0 && nextPoint.y <= 8) && (nextPoint.z >= -4 && nextPoint.z <= 4))
                {
                    intersectsFourthQuadrant = true;
                    intersectionInFourthQuadrant = previousPoint;
                    break;
                }

                // Dibujar segmento de rayo en OpenGL
                glLineWidth(1.0f);//QUITAR ESTO
                glBegin(GL_LINES);
                switch (reflection)
                {
                    case 0: glColor3f(1.0f, 1.0f, 0.0f); break;  // Amarillo
                    case 1: glColor3f(0.0f, 0.0f, 1.0f); break;  // Azul
                    case 2: glColor3f(1.0f, 0.0f, 0.0f); break;  // Rojo
                    case 3: glColor3f(0.0f, 1.0f, 0.0f); break;  // Verde
                    case 4: glColor3f(1.0f, 0.0f, 1.0f); break;  // Magenta
                    case 5: glColor3f(0.0f, 1.0f, 1.0f); break;  // Cian
                    default: glColor3f(0.5f, 0.5f, 0.5f); break; // Gris
                }

                // Grafica de los rayos habilitados
                if(showTotalRays == true && reflection >= 0)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                    showSecondReflection = false;
                    showFourthReflection = false;
                    showThirdReflection = false;
                    showFifthReflection = false;
                    showFirstReflection = false;
                    showOriginalRays = false;
                }
                if(showOriginalRays == true && reflection == 0)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                }
                if(showFirstReflection == true && reflection == 1)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                }
                if(showSecondReflection == true && reflection == 2)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                }
                if(showThirdReflection == true && reflection == 3)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                }
                if(showFourthReflection == true && reflection == 4)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                }
                if(showFifthReflection == true && reflection == 5)
                {
                    glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                    glVertex3f(nextPoint.x, nextPoint.y, nextPoint.z);
                }
                glEnd();

                // Continuando con la siguiente reflexion
                previousPoint = currentPoint;
                currentPoint = nextPoint;
                t += step;
            }

            // Intersección con el cuaryo cudrante
            if (intersectsFourthQuadrant)
            {
                // Reflexión en el cuarto cuadrante
                if (length(subtract(intersectionInFourthQuadrant, rayOrigin)) >= reflectionThreshold)
                {
                    rayOrigin = intersectionInFourthQuadrant;
                    rayDirectionGlobal = multiply(subtract(rayDirectionGlobal, multiply(normalAtIntersection, 2.0f * dot(rayDirectionGlobal, normalAtIntersection))), -1.0f);
                }
                continue;
            }

            // Dibujar segmento de rayo en OpenGL
            glBegin(GL_LINES);
            switch (reflection)
            {
                case 0: glColor3f(1.0f, 1.0f, 0.0f); break;                     // Amarillo
                case 1: glColor3f(0.0f, 0.0f, 1.0f); break;                     // Azul
                case 2: glColor3f(1.0f, 0.0f, 0.0f); break;                     // Rojo
                case 3: glColor3f(0.0f, 1.0f, 0.0f); break;                     // Verde
                case 4: glColor3f(1.0f, 0.0f, 1.0f); break;                     // Magenta
                case 5: glColor3f(0.0f, 1.0f, 1.0f); break;                     // Cian
                default: glColor3f(0.5f, 0.5f, 0.5f); break;                    // Gris
            }

            // Grafica de los rayos habilitados
            if(showTotalRays == true && reflection >= 0)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
                showSecondReflection = false;
                showFourthReflection = false;
                showThirdReflection = false;
                showFifthReflection = false;
                showFirstReflection = false;
                showOriginalRays = false;
            }
            if(showOriginalRays == true && reflection == 0)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
            }
            if(showFirstReflection == true && reflection == 1)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
            }
            if(showSecondReflection == true && reflection == 2)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
            }
            if(showThirdReflection == true && reflection == 3)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
            }
            if(showFourthReflection == true && reflection == 4)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
            }
            if(showFifthReflection == true && reflection == 5)
            {
                glVertex3f(currentPoint.x, currentPoint.y, currentPoint.z);
                glVertex3f(endPoint.x, endPoint.y, endPoint.z);
            }
            glEnd();

            // Matriz de transformación de la esfera receptora
            float receiverMatrix[16];
            glPushMatrix();
            glLoadIdentity();
            glRotatef(receptorAngle_N, 1, 0, 0);
            glRotatef(receptorAngle_S, 1, 0, 0);
            glRotatef(receptorAngle_O, 0, 1, 0);
            glRotatef(receptorAngle_E, 0, 1, 0);
            glRotatef(receptorAngle_NE, 1, 1, 0);
            glRotatef(receptorAngle_SO, 1, 1, 0);
            glRotatef(receptorAngle_NO, -1, 1, 0);
            glRotatef(receptorAngle_SE, -1, 1, 0);
            glGetFloatv(GL_MODELVIEW_MATRIX, receiverMatrix);
            glPopMatrix();

            // Aplicación de la transformación a la dirección del rayo
            Vec3 transformedRayDirectionReceptor =
            {
                receiverMatrix[0] * rayDirection.x + receiverMatrix[4] * rayDirection.y + receiverMatrix[8] * rayDirection.z,
                receiverMatrix[1] * rayDirection.x + receiverMatrix[5] * rayDirection.y + receiverMatrix[9] * rayDirection.z,
                receiverMatrix[2] * rayDirection.x + receiverMatrix[6] * rayDirection.y + receiverMatrix[10] * rayDirection.z
            };

            // Dirección rayo havia el receptor
            Vec3 rayDirectionGlobalReceptor = transformedRayDirectionReceptor;

            // Radio de la esfera receptora
            float receiverRadius = 2.0f;
            
            // Comprobar intersección con la esfera receptora
            Vec3 sphereIntersection;
            if (intersectSphere(receiverPosition, receiverRadius, rayOrigin, rayDirectionGlobalReceptor, sphereIntersection))
            {
                float distanceReceptor = length(subtract(sphereIntersection, rayOrigin));
                float closestDistanceReceptor = rayLength;
                Vec3 closestIntersectionReceptor;

                if (distanceReceptor <= closestDistanceReceptor && distanceReceptor >= minDistanceThreshold)
                {
                    closestDistanceReceptor = distanceReceptor;
                    closestIntersectionReceptor = sphereIntersection;

                    // Calcular el punto de intersección transformado al espacio local de la esfera receptora
                    Vec3 localIntersectionPoint = transformPoint(receiverMatrix, subtract(closestIntersectionReceptor, receiverPosition));

                    // Determinar el canal basado en la latitud
                    float interseccionX = localIntersectionPoint.x / receiverRadius;
                    float interseccionY = localIntersectionPoint.y / receiverRadius;
                    float interseccionZ = localIntersectionPoint.z / receiverRadius;

                    // Asegurarse de que los valores están dentro del rango [-1, 1]
                    interseccionX = std::max(std::min(interseccionX, 1.0f), -1.0f);
                    interseccionY = std::max(std::min(interseccionY, 1.0f), -1.0f);
                    interseccionZ = std::max(std::min(interseccionZ, 1.0f), -1.0f);

                    float angleX = asin(interseccionX);
                    float angleY = asin(interseccionY);
                    float angleZ = asin(interseccionZ);

                    // Condiciones para el Canal Izquierdo
                    if ((angleX >= -1.0f && angleX < 0) &&
                        (angleY >= -1.0f && angleY <= 1.0f) &&
                        (angleZ >= -1.0f && angleZ <= 1.0f))
                    {
                        ++raysLeftChannel;
                    }
                    // Condiciones para el Canal Derecho
                    if ((angleX > 0 && angleX <= 1.0f) &&
                        (angleY >= -1.0f && angleY <= 1.0f) &&
                        (angleZ >= -1.0f && angleZ <= 1.0f))
                    {
                        ++raysRightChannel;
                    }
                    // Caso especial cuando x = 0, se suma a ambos canales
                    if (angleX == 0)
                    {
                        ++raysLeftChannel;
                        ++raysRightChannel;
                    }

                }
            }
            // Actualización del origen del rayo para la siguiente reflexión
            rayOrigin = closestIntersection;
            rayDirectionGlobal = subtract(rayDirectionGlobal, multiply(normalAtIntersection, 2.0f * dot(rayDirectionGlobal, normalAtIntersection)));
        }
    }
    // Actualización de las variables con los contadores de rayos
    rayosCanalL = raysLeftChannel;
    rayosCanalR = raysRightChannel;

    // Actualización de las etiquets con los contadores de rayos
    //Label34->Caption = "Rayos Canal Izquierdo: " + IntToStr(raysLeftChannel);
    //Label35->Caption = "Rayos Canal Derecho: " + IntToStr(raysRightChannel);
}


//---------------------------------------------------------------------------
void TFormAuralizationMotor::UpdateAudio(int numRaysLeft, int numRaysRight)
{
    // Parámetros ajustables
    const float headWidth = 0.2f;   // Distancia entre los oídos (~20 cm)
    const float maxITD = 0.0007f;   // Máximo retraso (700 µs)
    const float maxILD = 6.0f;      // Máxima diferencia en dB
    const float soundSpeed = 343.0f;// Velocidad del sonido en el aire (m/s)

    // Valores mínimos para evitar errores matemáticos
    const float minRays = 0.01f;
    const float maxRays = 100.0f;
    const float minVolume = 0.01f; // Evita volumen nulo

    if (etiquetasP) 
    {
        // Normalización de rayos (entre 0 y 1)
        float raysFactorLeft = CLAMP(numRaysLeft / maxRays, minVolume, 1.0f);
        float raysFactorRight = CLAMP(numRaysRight / maxRays, minVolume, 1.0f);

        // Cálculo de ITD (Interaural Time Difference)
        float itd = (headWidth * (numRaysRight - numRaysLeft)) / soundSpeed;
        itd = CLAMP(itd, -maxITD, maxITD);

        // Cálculo de ILD (Interaural Level Difference)
        float ild = 0.0f;
        if (numRaysLeft > 0 && numRaysRight > 0) 
        {
            float ild1 = std::log10((numRaysRight + minRays) / (numRaysLeft + minRays)) * maxILD;
            float ild2 = std::log10((numRaysLeft + minRays) / (numRaysRight + minRays)) * maxILD;
            ild = (ild1 - ild2) / 2.0f;
        }
        ild = CLAMP(ild, -maxILD, maxILD);

        // Aplicar ITD con Sleep (convertido a ms correctamente)
        DWORD delayMs = static_cast<DWORD>(std::abs(itd) * 1000);
        Sleep(delayMs);

        // Cálculo de balance (panning) basado en ILD
        float ildFactorLeft = std::max(std::pow(10, ild / 20.0f), minVolume);
        float ildFactorRight = std::max(std::pow(10, -ild / 20.0f), minVolume);

        // Calcular niveles de volumen en DWORD (16 bits por canal)
        DWORD leftVolume = static_cast<DWORD>(CLAMP(0xFFFF * raysFactorLeft * ildFactorLeft, 0.0, 65535.0));
        DWORD rightVolume = static_cast<DWORD>(CLAMP(0xFFFF * raysFactorRight * ildFactorRight, 0.0, 65535.0));

        // Ajustar el volumen del audio
        waveOutSetVolume(hWaveOut, MAKELONG(leftVolume, rightVolume));

        // Normalizar los valores de nivel en porcentaje (máx. 99%)
        double leftChannelLevel = CLAMP((leftVolume / 65535.0) * 100.0, minVolume * 100.0f, 99.0);
        double rightChannelLevel = CLAMP((rightVolume / 65535.0) * 100.0, minVolume * 100.0f, 99.0);

        // Actualizar etiquetas de nivel de audio por canal
        Nivel_L->Caption = FloatToStrF(leftChannelLevel, ffFixed, 6, 2);
        Nivel_R->Caption = FloatToStrF(rightChannelLevel, ffFixed, 6, 2);
    }
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------




