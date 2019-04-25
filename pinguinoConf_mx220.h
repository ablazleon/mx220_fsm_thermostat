/* ************************************************************************** */
/** Descriptive File Name
 @Date
    16-April-2019
  
  @Programmer
    Josué Pagán  
 
  @Company
    Universidad Politécnica de Madrid
 */
/* ************************************************************************** */

#ifndef _PINGUINO_CONF_MX220_H    /* Guard against multiple inclusion */
#define _PINGUINO_CONF_MX220_H 

/* DEFINES-MACROS*/
#define LED_GREEN_TOGGLE() LED1_TOGGLE() 
#define LED_GREEN_OFF() LED1_OFF()
#define LED_GREEN_ON() LED1_ON()
#define LED_GREEN_INIT() LED1_INIT()

#define LED_RED_TOGGLE() LED2_TOGGLE() 
#define LED_RED_OFF() LED2_OFF()
#define LED_RED_ON() LED2_ON()
#define LED_RED_INIT() LED2_INIT()

#define LED1_TOGGLE() PORTBINV = _PORTB_RB15_MASK
#define LED1_OFF() PORTBbits.RB15 = 0
#define LED1_ON() PORTBbits.RB15 = 1
#define LED1_INIT() TRISBbits.TRISB15 = 0

#define LED2_TOGGLE() PORTAINV = _PORTA_RA10_MASK
#define LED2_OFF() PORTAbits.RA10 = 0
#define LED2_ON() PORTAbits.RA10 = 1
#define LED2_INIT() TRISAbits.TRISA10 = 0

#define BUTTON_INIT() TRISBbits.TRISB7 = 1
#define BUTTON_PUSHED  IFS0bits.INT0IF
#define BUTTON_IF_CLEAR() IFS0CLR = _IFS0_INT0IF_MASK // IFS0bits.INT0IF = 0
#define BUTTON_INTERR_EN() IEC0SET = _IEC0_INT0IE_MASK // IEC0bits.INT0IE = 1
#define BUTTON_INTERR_DIS() IEC0CLR = _IEC0_INT0IE_MASK // IEC0bits.INT0IE = 0
#define BUTTON_EDGE_RISING()  INTCONbits.INT0EP = 1
#define BUTTON_EDGE_FALLING() INTCONbits.INT0EP = 0
#define BUTTON_INTERR_PRIOR(x) IPC0bits.INT0IP = x
#define BUTTON_INTERR_SUBPRIOR(x) IPC0bits.INT0IS = x


// Completar aquí con condición del apartado 3.1.1
#define LDR_SENSOR_REG ???
#define LDR_SENSOR_INIT() ANSEL???SET = TRIS???bits.TRIS???
#define LDR_SENSOR_AS_INPUT() TRIS???SET = TRIS???bits.TRIS???


#endif /* _PINGUINO_CONF_MX220_H */


