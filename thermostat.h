/* ************************************************************************** */
/** Descriptive File Name

  @Date
    24-April-2018
  
  @Programmer
    Josu� Pag�n  
   
  @Company
    Universidad Polit�cnica de Madrid

  @File Name
    thermostat.h

  @Description
    Define prototypes of functions and other elements.
 */
/* ************************************************************************** */

#ifndef _THERMOSTAT_H    /* Guard against multiple inclusion */
#define _THERMOSTAT_H

/* DEFINES */
//#define _DEBUG_MODE
#define FLAG_START_HEAT 0x01
#define FLAG_STOP_HEAT 0x02

// Completar aqu� con condici�n del apartado 3.2.7
#define THRESHOLD_TEMP ??? //�C

/* PROTOTYPES FUNCTIONS */
/* Configuration functions */
void led_setup(void);
void timer1_setup(void);
void sensor_setup(void);

/* Check functions*/
// Completar aqu� con condici�n del apartado 3.2.6
int ??? (fsm_t *this);
int ??? (fsm_t *this);

/* Actuation functions */
// Completar aqu� con condici�n del apartado 3.2.6
void ??? (fsm_t *this);
void ??? (fsm_t *this);

/* Others */
void adcManualConfig(int prescale, int tadMultiplier);
int analogRead(char analogPIN);
double volts2celsius(int adcValue);

#endif /* _THERMOSTAT_H */