#if defined(_MSC_VER) && !defined(_XBOX) && (_MSC_VER >= 1500 && _MSC_VER < 1900)
#if (_MSC_VER >= 1700)
/* https://support.microsoft.com/en-us/kb/980263 */
#pragma execution_character_set("utf-8")
#endif
#pragma warning(disable:4566)
#endif

MSG_HASH(
   MSG_HASH_MGBA_SOLAR_SENSOR_LEVEL_DESC,
   "Nivel del sensor solar"
   )
MSG_HASH(
   MSG_HASH_MGBA_SOLAR_SENSOR_LEVEL_INFO,
   "Ajusta la intensidad de la luz solar ambiental. Para juegos que contenían un sensor solar en sus cartuchos, p. ej.: la saga Boktai."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_USE_DEVICE_SENSOR_IF_AVAILABLE,
   "Utilizar dispositivo sensor si está disponible"
   )
MSG_HASH(
   MSG_HASH_MGBA_ALLOW_OPPOSING_DIRECTIONS_DESC,
   "Permitir entradas direccionales opuestas"
   )
MSG_HASH(
   MSG_HASH_MGBA_ALLOW_OPPOSING_DIRECTIONS_INFO,
   "Permite pulsar, alternar rápidamente o mantener las direcciones hacia la izquierda y hacia la derecha (o hacia arriba y abajo) al mismo tiempo. Puede provocar defectos en el movimiento."
   )
MSG_HASH(
   MSG_HASH_MGBA_GB_MODEL_DESC,
   "Modelo de Game Boy (es necesario reiniciar)"
   )
MSG_HASH(
   MSG_HASH_MGBA_GB_MODEL_INFO,
   "Carga el contenido cargado utilizando un modelo de Game Boy específico. La opción «Autodetectar» seleccionará el modelo más adecuado para el juego actual."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_AUTODETECT,
   "Autodetectar"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GAME_BOY,
   "Game Boy"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SUPER_GAME_BOY,
   "Super Game Boy"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GAME_BOY_COLOR,
   "Game Boy Color"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_GAME_BOY_ADVANCE,
   "Game Boy Advance"
   )
MSG_HASH(
   MSG_HASH_MGBA_USE_BIOS_DESC,
   "Utilizar BIOS en caso de encontrarla (es necesario reiniciar)"
   )
MSG_HASH(
   MSG_HASH_MGBA_USE_BIOS_INFO,
   "Si se encuentran en el directorio de sistema de RetroArch, se utilizarán los archivos de la BIOS y el bootloader oficiales para emular el hardware."
   )
MSG_HASH(
   MSG_HASH_MGBA_SKIP_BIOS_DESC,
   "Omitir introducción de la BIOS (es necesario reiniciar)"
   )
MSG_HASH(
   MSG_HASH_MGBA_SKIP_BIOS_INFO,
   "Al utilizar una BIOS y bootloader oficiales, omitirá la animación del logotipo al arrancar. Esta opción será ignorada si «Utilizar BIOS en caso de encontrarla» está desactivada."
   )
MSG_HASH(
   MSG_HASH_MGBA_SGB_BORDERS_DESC,
   "Utilizar bordes de Super Game Boy (es necesario reiniciar)"
   )
MSG_HASH(
   MSG_HASH_MGBA_SGB_BORDERS_INFO,
   "Muestra los bordes de Super Game Boy al ejecutar juegos compatibles con este sistema."
   )
MSG_HASH(
   MSG_HASH_MGBA_IDLE_OPTIMIZATION_DESC,
   "Eliminar bucle de inactividad"
   )
MSG_HASH(
   MSG_HASH_MGBA_IDLE_OPTIMIZATION_INFO,
   "Minimiza la carga del sistema optimizando los llamados bucles de inactividad: secciones de código en las que no ocurre nada, pero la CPU se ejecuta a máxima velocidad (como cuando un vehículo es revolucionado sin tener la marcha puesta). Mejora el rendimiento y debería activarse en hardware de bajas prestaciones."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_REMOVE_KNOWN,
   "Eliminar bucles conocidos"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_DETECT_AND_REMOVE,
   "Detectar y eliminar"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_DONT_REMOVE,
   "No eliminar"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_DESC,
   "Omisión de fotogramas"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_INFO,
   "Omite fotogramas para no saturar el búfer de audio (chasquidos en el sonido). Mejora el rendimiento a costa de perder fluidez visual. El valor Automática omite fotogramas según lo aconseje el front-end. El valor Automática (umbral) utiliza el ajuste Umbral de omisión de fotogramas (%). El valor «Intervalos fijos» utiliza el ajuste «Intervalo de omisión de fotogramas»."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_AUTO,
   "Automática"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_AUTO_THRESHOLD,
   "Automática (umbral)"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_FIXED_INTERVAL,
   "Intervalos fijos"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_THRESHOLD_DESC,
   "Umbral de omisión de fotogramas (%)"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_THRESHOLD_INFO,
   "Cuando la omisión de fotogramas esté configurada como Automática (umbral), este ajuste especifica el umbral de ocupación del búfer de audio (en porcentaje) por el que se omitirán fotogramas si el valor es inferior. Un valor más elevado reduce el riesgo de chasquidos omitiendo fotogramas con una mayor frecuencia."
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_INTERVAL_DESC,
   "Intervalo de omisión de fotogramas"
   )
MSG_HASH(
   MSG_HASH_MGBA_FRAMESKIP_INTERVAL_INFO,
   "Cuando la omisión de fotogramas esté configurada como Intervalos fijos, el valor que se asigne aquí será el número de fotogramas omitidos una vez se haya renderizado un fotograma. Por ejemplo: «0» = 60 FPS, «1» = 30 FPS, «2» = 15 FPS, etcétera."
   )
MSG_HASH(
   MSG_HASH_MGBA_FORCE_GBP_DESC,
   "Vibración de Game Boy Player (es necesario reiniciar)"
   )
MSG_HASH(
   MSG_HASH_MGBA_FORCE_GBP_INFO,
   "Permite que aquellos juegos compatibles con el logotipo de arranque de Game Boy Player hagan vibrar el mando. Debido a el método que utilizó Nintendo, puede provocar fallos gráficos, como parpadeos o retrasos de señal en algunos de estos juegos."
   )

#if defined(COLOR_16_BIT) && defined(COLOR_5_6_5)
MSG_HASH(
   MSG_HASH_MGBA_COLOR_CORRECTION_DESC,
   "Corrección de color"
   )
MSG_HASH(
   MSG_HASH_MGBA_COLOR_CORRECTION_INFO,
   "Ajusta los colores de la salida de imagen para que esta coincida con la que mostraría un hardware real de GBA/GBC."
   )
MSG_HASH(
   MSG_HASH_MGBA_INTERFRAME_BLENDING_DESC,
   "Fusión interfotograma"
   )
MSG_HASH(
   MSG_HASH_MGBA_INTERFRAME_BLENDING_INFO,
   "Simula el efecto fantasma de una pantalla LCD. «Sencilla» mezcla los fotogramas previos y posteriores en un 50%. «Inteligente» intenta detectar los parpadeos de pantalla y solo lleva a cabo la fusión del 50% en los fotogramas afectados. «Efecto fantasma de LCD» imita los tiempos de respuesta naturales de una pantalla LCD combinando varios fotogramas guardados en el búfer. La fusión sencilla o inteligente es necesaria en aquellos juegos que aprovechan de forma agresiva el efecto fantasma de la pantalla LCD para los efectos de transparencia (Wave Race, Chikyuu Kaihou Gun ZAS, F-Zero, la saga Boktai...)."
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SIMPLE,
   "Sencilla"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_SMART,
   "Inteligente"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_LCD_GHOSTING_ACCURATE,
   "Efecto fantasma de LCD (preciso)"
   )
MSG_HASH(
   MSG_HASH_OPTION_VAL_LCD_GHOSTING_FAST,
   "Efecto fantasma de LCD (rápido)"
   )
#endif
