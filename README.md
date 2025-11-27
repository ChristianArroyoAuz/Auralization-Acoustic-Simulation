# 🎓 AuralizationMotor - Sistema de Auralización Acústica 3D  
**Tesis de Maestría en Computación - Escuela Politécnica Nacional**

## 🔬 **Contexto Académico e Investigación**

### 📚 **Proyecto de Tesis de Grado**
*"Sistema integral de simulación acústica que modela la propagación del sonido en espacios 3D, aplicando efectos de reverberación, reflexión y absorción en tiempo real mediante técnicas de trazado de rayos y procesamiento de audio digital. Desarrollado en Borland C++ Builder 6 como trabajo de investigación para la Maestría en Computación de la Escuela Politécnica Nacional."*

---

## 🏗️ **Arquitectura del Sistema - Perspectiva Académica**

### **🎛️ Módulos Principales de Investigación**
```
AURALIZATION MOTOR SYSTEM - ARQUITECTURA ACADÉMICA
├── 🎮 Interfaz Gráfica (VCL/OpenGL)
├── 🔊 Procesamiento de Audio (Wave API)
├── 🏗️ Motor de Renderizado 3D
├── 📊 Simulación Acústica (Trazado de Rayos)
├── 🎚️ Control de Efectos de Sonido
```

## 🎯 **Módulo 1: Interfaz Gráfica y Control Avanzado**

### **🏠 Entorno Visual 3D para Investigación**
- **Renderizado OpenGL**: Visualización científica en tiempo real del espacio acústico
- **Habitación 3D Académica**: Modelo arquitectónico con múltiples cuadrantes para experimentación
- **Sistema Dual de Cámaras**: Vista en primera persona (inmersiva) y tercera persona (analítica)
- **Coordenadas Científicas**: Posicionamiento preciso con precisión de 3 decimales

### **🎯 Elementos Visuales de Análisis**
```cpp
// Emisor (Fuente sonora orientable)
glPushMatrix();
glTranslatef(posEmisorX, posEmisorY, posEmisorZ);
glRotatef(emisorAngle_N, 1, 0, 0);  // 8 ejes de rotación
drawSpeaker();  // Esfera acústica codificada por color

// Receptor (Capturador binaural)  
glPushMatrix();
glTranslatef(posReceptorX, posReceptorY, posReceptorZ - 2.0f);
drawMicrophone();  // Esfera con canales L/R diferenciados
```

## 🔊 **Módulo 2: Procesamiento de Audio Científico**

### **🎵 Gestión Avanzada de Señales**
- **Formato WAV Profesional**: Soporte para archivos PCM estéreo 44.1kHz/16-bit
- **Buffer Circular de Investigación**: Reproducción continua con efectos en tiempo real
- **API Windows Wave de Bajo Nivel**: Control preciso del dispositivo de audio

### **⚡ Efectos Acústicos Basados en Modelos Físicos**
```cpp
// Aplicación de efectos con validación científica
ApplyReflections()    // Reflexiones tempranas (50ms delay)
ApplyReverb()        // Reverberación ambiental (3 retardos)  
ApplyDelay()         // Retardos configurables (0-100ms)
ApplyAbsorption()    // Absorción de materiales (filtro paso bajo)

// Modelo matemático de reverberación
const int delays[numDelays] = {4410, 8820, 13230};  // 100ms, 200ms, 300ms
const float decay[numDelays] = {0.5f, 0.25f, 0.125f}; // Decaimiento exponencial
```

## 📊 **Módulo 3: Simulación Acústica con Trazado de Rayos**

### **🎯 Algoritmo de Investigación Principal**
```cpp
void drawRaysFromEmitter(float posEX, float posEY, float posEZ, 
                        float posRX, float posRY, float posRZ, int numRays)
{
    // GENERACIÓN ESTOCÁSTICA DE RAYOS
    float angleLat = minAngleLat + random() * (maxAngleLat - minAngleLat);
    float angleLng = minAngleLng + random() * (maxAngleLng - minAngleLng);
    
    // MODELADO DE PROPAGACIÓN
    for (int reflection = 0; reflection <= maxReflections; ++reflection) {
        // Cálculo de intersecciones con superficies
        // Determinación de canales L/R basada en posición esférica
        // Aplicación de reflexiones múltiples
    }
}
```

### **🔢 Parámetros Científicos Ajustables**
- **Número de rayos**: 0-1000 (control precisión vs rendimiento)
- **Reflexiones máximas**: 5 niveles (balance entre realismo y costo computacional)
- **Umbral de distancia**: 0.0001f (prevención de errores numéricos)
- **Radio receptor**: 2.0 unidades (área de captura acústica)

## 🎧 **Módulo 4: Audio Espacial y Psicoacústica**

### **👂 Modelo Binaural Científico**
```cpp
void UpdateAudio(int numRaysLeft, int numRaysRight)
{
    // PARÁMETROS PSICOACÚSTICOS
    const float headWidth = 0.2f;      // Separación interaural estándar
    const float maxITD = 0.0007f;      // Máxima diferencia temporal (700μs)
    const float maxILD = 6.0f;         // Máxima diferencia de nivel (6dB)
    const float soundSpeed = 343.0f;   // Velocidad del sonido
    
    // CÁLCULO DE DIFERENCIAS INTERAURALES
    float itd = (headWidth * (numRaysRight - numRaysLeft)) / soundSpeed;
    float ild = log10((numRaysRight + 0.01f) / (numRaysLeft + 0.01f)) * maxILD;
    
    // APLICACIÓN EN TIEMPO REAL
    DWORD leftVolume = 0xFFFF * raysFactorLeft * ildFactorLeft;
    DWORD rightVolume = 0xFFFF * raysFactorRight * ildFactorRight;
    waveOutSetVolume(hWaveOut, MAKELONG(leftVolume, rightVolume));
}
```

## 🎓 **Contribución Académica y Metodología**

### **🔬 Preguntas de Investigación Abordadas**
1. **¿Cómo modelar eficientemente la propagación del sonido en espacios 3D?**
   - Implementación de trazado de rayos estocástico
   - Optimización para tiempo real

2. **¿Qué técnicas permiten simular efectos acústicos authenticos?**
   - Modelado físico de reflexiones y reverberación
   - Aplicación de principios psicoacústicos

3. **¿Cómo visualizar fenómenos acústicos complejos?**
   - Codificación por color de niveles de reflexión
   - Representación intuitiva de trayectorias sonoras

### **📚 Aplicaciones en la Enseñanza**
- **Acústica Arquitectónica**: Visualización de propiedades de salas
- **Procesamiento de Señales**: Implementación práctica de efectos de audio
- **Computación Gráfica**: Aplicación de OpenGL en simulación científica
- **Ingeniería de Software**: Desarrollo de sistemas multimedia complejos

## 🛠️ **Tecnologías Utilizadas en la Investigación**

| Tecnología | Aplicación Específica | Contribución Académica |
|------------|----------------------|------------------------|
| **Borland C++ Builder 6** | Entorno principal | Desarrollo de sistemas nativos Windows |
| **OpenGL + GLU** | Renderizado 3D | Visualización científica avanzada |
| **Windows Wave API** | Procesamiento audio | Control de bajo nivel de dispositivos |
| **VCL Framework** | Interfaz de usuario | Desarrollo rápido de aplicaciones nativas |
| **Algoritmos Numéricos** | Cálculos acústicos | Aplicación de métodos computacionales |

## 📈 **Características de Investigación Avanzadas**

### **⚡ Optimizaciones Científicas**
- **Geometría Vectorial Optimizada**: Operaciones con Vec3 para cálculos acústicos
- **Manejo de Excepciones**: Prevención de errores de punto flotante en tiempo real
- **Gestión de Memoria**: Buffers dinámicos con cleanup automático
- **Validación Numérica**: Verificación de valores finitos en todos los cálculos

### **🎨 Visualización Científica**
```cpp
// Código de colores por nivel de reflexión
switch (reflection) {
    case 0: glColor3f(1.0f, 1.0f, 0.0f); break;  // Amarillo - Directo
    case 1: glColor3f(0.0f, 0.0f, 1.0f); break;  // Azul - 1ra reflexión
    case 2: glColor3f(1.0f, 0.0f, 0.0f); break;  // Rojo - 2da reflexión
    case 3: glColor3f(0.0f, 1.0f, 0.0f); break;  // Verde - 3ra reflexión
    // ... Patrón continuo para análisis visual
}
```

## 🌟 **Logros Académicos Destacados**

### **🔬 Innovaciones en Simulación Acústica**
1. **Integración Multidisciplinaria**: Combinación de gráficos 3D, procesamiento de audio y acústica
2. **Control en Tiempo Real**: Modificación dinámica de parámetros durante la simulación
3. **Visualización Educativa**: Representación intuitiva de conceptos acústicos complejos
4. **Framework Extensible**: Base para investigación futura en acústica computacional

### **📊 Métricas de Evaluación Científica**
- **Precisión**: Modelado físico basado en principios acústicos establecidos
- **Rendimiento**: Operación en tiempo real (60 FPS objetivo)
- **Usabilidad**: Interfaz intuitiva para experimentación académica
- **Extensibilidad**: Arquitectura modular para futuras mejoras

## 🚀 **Cómo Utilizar para Investigación**

### **🎮 Controles para Experimentación**
```bash
# MOVIMIENTO Y NAVEGACIÓN
Flechas: Control receptor (vista 1ra persona)
WASD: Control emisor (fuente sonora)
YHGU: Control cámara aérea (vista 3ra persona)
1/3: Cambio entre modos de cámara
R: Restablecimiento completo del sistema

# CONTROL ACÚSTICO
ScrollBarNumeroRayos: Precisión de simulación (0-1000 rayos)
ScrollBarReflection: Intensidad de reflexiones (0-100%)
ScrollBarReverb: Nivel de reverberación (0-100%)
ScrollBarAbsorption: Absorción de materiales (0-100%)
```

### **🔬 Modos de Visualización para Análisis**
- **Rayos Originales**: Trayectorias directas (amarillo)
- **Primera Reflexión**: Primer rebote (azul)
- **Reflexiones Múltiples**: Análisis de propagación compleja
- **Vista Completa**: Todos los rayos simultáneamente

---

## 🎓 **Conclusión Académica**

### **📚 Valor como Trabajo de Maestría**
Este proyecto demuestra competencia avanzada en:
- **Desarrollo de Software Complejo**: Integración de múltiples tecnologías
- **Aplicación de Teoría Acústica**: Implementación de modelos físicos
- **Innovación en Interfaz Humano-Computador**: Visualización de datos complejos
- **Optimización Computacional**: Rendimiento en tiempo real

### **🔮 Trabajo Futuro y Extensiones**
- Implementación de métodos de onda completa (FDTD)
- Integración con HRTF para audio 3D inmersivo
- Soporte para materiales acústicos complejos
- Optimización para hardware moderno (GPU computing)

**¡Una contribución significativa al campo de la acústica computacional y el procesamiento de señales!** 🎧✨

*Desarrollado en la Escuela Politécnica Nacional - Excelencia en educación tecnológica e investigación científica en Ecuador* 🇪🇨

## 📞 **Información para Investigadores**
*Para acceso al código fuente, documentación técnica o colaboración en investigación, contactar a través de los canales académicos de la Escuela Politécnica Nacional.*
