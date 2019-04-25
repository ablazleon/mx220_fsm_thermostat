/* ************************************************************************** */
/** Descriptive File Name
 @Date
    16-April-2019
  
  @Programmer
    Josué Pagán  
   
  @Company
    Universidad Politécnica de Madrid

  @File Name
    main.c

  @Summary
    Basic FSM. Turn on a led during 1 second if a button is pushed. 
    Read a FLAG activated by an interruption in the button.

  @Note: 
    josue: if we get this message: A heap is required, but has not been specified... 
    This is due to memory allocation in fsm_new. 
    See: http://www.microchip.com/forums/m737701.aspx
    and see "The heap" in https://os.mbed.com/handbook/Memory-Model
/* ************************************************************************** */

/* INLCUDES */
// Completar aquí con condición del apartado 3.1.4
#include <xc.h>
#include <sys/attribs.h>
#include "myMacros_pic32mx220f032d.h"
#include "pinguinoConf_mx220.h"
#include "fsm.h"

/* VARIABLES */
volatile int flags = 0x00;

enum fsm_state {
  ALARM_ON,
  ALARM_OFF,
};

/*
 * Maquina de estados: lista de transiciones
 * { EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
 */
static fsm_trans_t alarm_tt[] = {
  {ALARM_OFF, checkPresence, ALARM_ON,  alarm_on},
  {ALARM_ON,  checkDeactivation, ALARM_OFF, alarm_off},
  {-1, NULL, -1, NULL },
};

/* FUNCTIONS */
/* @Description: Interrupt Routine Service
 */

void  __ISR(_EXTERNAL_0_VECTOR, IPL3AUTO) button_ISR (void) {
    flags |= FLAG_DEACTIVATION;
    BUTTON_IF_CLEAR(); // Reset interruption    
}

void __ISR(_TIMER_1_VECTOR, IPL2AUTO) timer1_ISR (void) {
    TIMER1_IF_CLEAR(); // Reset interruption
    LED2_TOGGLE();      
}

void __ISR(_TIMER_2_VECTOR, IPL1AUTO) timer2_ISR (void) {
    TIMER2_IF_CLEAR(); // Reset interruption
    double adcValue = 0.0;
    double mvolts = 0.0;
    adcValue = analogRead(LDR_SENSOR_REG);
    mvolts = (VREF_MV*(double) adcValue)/((1<<ADC1_NUM_BITS)-1);    

    // Completar aquí con condición del apartado 3.1.3    
    if (mvolts < THRESHOLD_LDR){
        flags |= FLAG_PRESENCE;
    }
}

void led_setup() {    
    LED_RED_INIT(); // Configure PORT registers' for LED_RED
    LED_GREEN_INIT();
    
    LED_RED_OFF();
    LED_GREEN_OFF();
}

void button_setup() {
    // Configure PORT registers' for BUT
    BUTTON_INIT();
    
     // Interruption setup
    BUTTON_INTERR_DIS(); // disable this interrupt
    BUTTON_EDGE_FALLING(); // edge polarity for this interrupt     
    BUTTON_INTERR_PRIOR(3); // interrupt priority
    BUTTON_INTERR_SUBPRIOR(1); // interrupt sub priority
    BUTTON_INTERR_EN(); // enable this interrupt
}


int analogRead(char analogPIN){
    ADC1_SELECT_CHANNEL(analogPIN);  // Select the input pins to be connected to the SHA.
  
    /* Ensure Persistent bits are cleared */
    ADC1_CLR_DONE();
    ADC1_IF_CLEAR();  
    
    ADC1_START_SAMPLING();  // Begin sampling

    while(ADC1_SAMPLING);  // Wait until acquisition is done
    while(!ADC1_CKECK_DONE);     // Wait until conversion done

    return ADC1BUF0;      // Result stored in ADC1BUF0
 }

void adcManualConfig(int prescale, int tadMultiplier){
    ADC1_OFF();    // disable ADC before configuration
    ADC1_AUTO_CONVERSION();       // Internal counter ends sampling and starts conversion (auto-convert), manual sample
    ADC1_FORM_INT16();    // (Default) Data output format int16 (only 10 bits available) 
    ADC1_VREF_TO_VDD();   // (Default) Set voltage reference to pins AVSS/AVDD
    ADC1_USE_PERIPHERAL_CLOCK(); // (Default) 
    ADC1_CLOCK_PRESCALE(prescale); 
    ADC1_TIME_SAMPLING(tadMultiplier);
    ADC1_ON(); // Enable ADC
}

void sensor_setup() {    
    // Completar aquí con condición del apartado 3.1.1
    LDR_SENSOR_INIT(); // Set pin RPC1 (AN7) to analog. 1 as analog, 0 as digital
    LDR_SENSOR_AS_INPUT(); // Set RC1 as an input 
    adcManualConfig(0, 0); 
}

void timer1_setup() {    
    // INT step 2
#ifdef _DEBUG_MODE
    TIMER1_PRESCALE_1_1(); // Prescaler 1
    PR1 = 7999; // 1 ms and 1 prescaler
#else
    // Completar aquí con condición del apartado 3.1.2
    TIMER1_PRESCALE_1_256(); // Prescaler 256
    PR1 = 12499; // 400 ms and 256 prescaler    
#endif
    TIMER1_SOURCE_INTERNAL(); // Internal peripheral clock
    TIMER1_ENABLE();  
    TIMER1_INTERR_PRIOR(2); // INT Step 3
    TIMER1_INTERR_SUBPRIOR(0); 
    TIMER1_RESET(); // INT Step 4
    //TIMER1_INTERR_EN(); // INT Step 5   
}

void timer2_setup() {    
    // INT step 2
#ifdef _DEBUG_MODE
    TIMER2_PRESCALE_1_1(); // Prescaler 1
    PR2 = 7999; // 1 ms and 1 prescaler
#else
    TIMER2_PRESCALE_1_64(); // Prescaler 64
    PR2 = 6249; // 100 ms and 64 prescaler
#endif
    TIMER2_SOURCE_INTERNAL(); // Internal peripheral clock
    TIMER2_ENABLE();  
    TIMER2_INTERR_PRIOR(1); // INT Step 3
    TIMER2_INTERR_SUBPRIOR(0); 
    TIMER2_RESET(); // INT Step 4
    TIMER2_INTERR_EN(); // INT Step 5   
}

// Check functions
int checkPresence (fsm_t *this) {
  return (flags & FLAG_PRESENCE);
}

int checkDeactivation (fsm_t *this) {
  return (flags & FLAG_DEACTIVATION);
}

// Output functions
void alarm_off (fsm_t *this) {
    TIMER1_INTERR_DIS(); // Disable interruptions
    BUTTON_INTERR_DIS();
    LED_RED_OFF();
    TIMER2_INTERR_EN();
    flags &= ~FLAG_DEACTIVATION; // Restore flag
}

void alarm_on (fsm_t *this) {
    TIMER2_INTERR_DIS(); // Disable interruptions
    LED_RED_ON();    
    TIMER1_RESET(); // Start from 0
    TIMER1_INTERR_EN(); // Enable interruptions
    BUTTON_INTERR_EN();
    flags &= ~FLAG_PRESENCE; // Restore flag
}

int main() {
    fsm_t* fsm = fsm_new(alarm_tt);
    
    // INT step 1
    GLOBAL_INTERR_DIS(); // Disable global interrupt
    led_setup();
    sensor_setup();
    button_setup();    
    timer1_setup();
    timer2_setup();       
    alarm_off(fsm);  

    INTERR_MVEC_EN(); //Interrupt Controller configured for multi vectored mode
    // INT Step 7
    GLOBAL_INTERR_EN(); // Enable global interrupt
    
    while(1) {      
      fsm_fire(fsm);    
    }
    return 0;
}