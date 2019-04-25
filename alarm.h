/* ************************************************************************** */
/** Descriptive File Name

  @Date
    17-April-2019
  
  @Programmer
    Josué Pagán  
   
  @Company
    Universidad Politécnica de Madrid

  @File Name
    alarm.h

  @Description
    Define prototypes of functions and other elements.
 */
/* ************************************************************************** */

#ifndef _ALARM_H    /* Guard against multiple inclusion */
#define _ALARM_H

/* DEFINES */
//#define _DEBUG_MODE
#define FLAG_PRESENCE 0x01
#define FLAG_DEACTIVATION 0x02

// Completar aquí con condición del apartado 3.1.3
#define THRESHOLD_LDR 1650      //ºC??

/* PROTOTYPES FUNCTIONS */
/* Configuration functions */
void led_setup(void);
void button_setup(void);
void timer1_setup(void);
void timer2_setup(void);
void comparator_setup(void);

void sensor_setup(void);
// Completar aquí con condición del apartado 3.1.4


/* Check functions*/
// Completar aquí con condición del apartado 3.1.4
int checkPresence (fsm_t *this);
int checkDeactivation (fsm_t *this);

/* Actuation functions */
void alarm_off (fsm_t *this);
void alarm_on (fsm_t *this);

/* Others */
void adcManualConfig(int prescale, int tadMultiplier);
int analogRead(char analogPIN);

#endif /* _ALARM_H */

/* *****************************************************************************
 End of File
 */
