/* ************************************************************************** */
/** Descriptive File Name
 @Date
    17-April-2019
  
  @Programmer
    Josué Pagán  
   
  @Company
    Universidad Politécnica de Madrid

  @File Name
    main.c

  @Summary
 Basic FSM reading analog data from a linear thermistor LM35 (0mv + 10mV/ºC).

  @Note: 
    josue: if we get this message: A heap is required, but has not been specified... 
    This is due to memory allocation in fsm_new. 
    See: http://www.microchip.com/forums/m737701.aspx
    and see "The heap" in https://os.mbed.com/handbook/Memory-Model
/* ************************************************************************** */

/* INLCUDES */
// Completar aquí con condición del apartado 3.2.8
#include <xc.h>
#include <sys/attribs.h>
#include "myMacros_pic32mx220f032d.h"
#include "thermostat.h"

/* VARIABLES */
volatile int flags = 0x00;

enum fsm_state {
 // Completar aquí con condición del apartado 3.2.6
};

/*
 * Maquina de estados: lista de transiciones
 * { EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
 */
static fsm_trans_t thermostat_tt[] = {
  // Completar aquí con condición del apartado 3.2.6
  {-1, NULL, -1, NULL },
};

/* FUNCTIONS */
/* @Description: Interrupt Routine Service
 */
void __ISR(_TIMER_1_VECTOR, IPL5AUTO) timer1_ISR (void) {
    double temp = volts2celsius(analogRead(TEMP_SENSOR_REG));
    
    if (temp < THRESHOLD_TEMP){
        flags |= FLAG_START_HEAT;            
    } else {
        flags |= FLAG_STOP_HEAT;       
    }
    
    TIMER1_IF_CLEAR();
}

void led_setup() {    
    LED_RED_INIT(); // Configure PORT registers' for LED_RED
}

// Check functions
// Completar aquí con condición del apartado 3.2.6
int checkHeat (fsm_t *this) {
  return (flags & FLAG_START_HEAT);
}

int checkCold (fsm_t *this) {
  return (flags & FLAG_STOP_HEAT);
}

// Output functions
// Completar aquí con condición del apartado 3.2.6
void thermostat_off (fsm_t *this) {
  LED_RED_OFF();
  flags &= ~FLAG_STOP_HEAT;
}

void thermostat_on (fsm_t *this) {
  LED_RED_ON();
  flags &= ~FLAG_START_HEAT;
}    


int analogRead(char analogPIN){
    ADC1_SELECT_CHANNEL(analogPIN);  // Select the input pins to be connected to the SHA.
    
    /* Ensure Persistent bits are cleared */
    ADC1_CLR_DONE();
    ADC1_IF_CLEAR();  
    
    ADC1_CONVERT();
   
    while(!ADC1_CKECK_DONE);     // Wait until conversion done

    return ADC1BUF0;      // Result stored in ADC1BUF0
 }

void adcManualConfig(int prescale, int tadMultiplier){
    ADC1_OFF();    // disable ADC before configuration
    
    ADC1_AUTOSTART();     // Sampling begins immediately after last conversion completes; SAMP bit is automatically set.
    ADC1_FORM_INT16();    // (Default) Data output format int16 (only 10 bits available) 
    ADC1_VREF_TO_VDD();   // (Default) Set voltage reference to pins AVSS/AVDD
    ADC1_USE_PERIPHERAL_CLOCK(); // (Default) 
    ADC1_CLOCK_PRESCALE(prescale); 
    ADC1_TIME_SAMPLING(tadMultiplier);
    
    ADC1_ON(); // Enable ADC
}

void sensor_setup(){
    TEMP_SENSOR_INIT(); // (Default) set RC0 (AN6) to analog. 1 as analog, 0 as digital
    TEMP_SENSOR_AS_INPUT(); // (Default) set RC0 as an input 
    adcManualConfig(0, 0);  // ADC clock = Peripheral clock / 64 --> TAD = 64*TPB
                              // Acquisition time = 15*TAD; Auto-sample time bits                               
}

void timer1_setup () {    
    // INT step 2
#ifdef _DEBUG_MODE
     TIMER1_PRESCALE_1_1(); // Prescaler 1
    PR1 = 7999; // 1 ms and 1 prescaler
#else
    TIMER1_PRESCALE_1_256(); // Prescaler 256
    PR1 = 31249; // 500 ms and 256 prescaler
#endif
    TIMER1_SOURCE_INTERNAL(); // Internal peripheral clock
    TIMER1_ENABLE();  
    TIMER1_INTERR_PRIOR(5); // INT Step 3
    TIMER1_INTERR_SUBPRIOR(0); 
    TIMER1_RESET(); // INT Step 4
    TIMER1_INTERR_EN(); // INT Step 5   
}

double volts2celsius(int adcValue){
    double mvolts = (VREF_MV*adcValue)/((1<<ADC1_NUM_BITS)-1);
    double celsius = mvolts/10;
    return celsius;
}

int main() {
    fsm_t* fsm = fsm_new(thermostat_tt);
 
    led_setup();
    timer1_setup();
    sensor_setup(); // Configure ADC
    // Completar aquí con condición del apartado 3.2.8
    thermostat_off(fsm);

    INTERR_MVEC_EN(); //Interrupt Controller configured for multi vectored mode
    GLOBAL_INTERR_EN(); // Enable global interrupt

    while(1) {
      fsm_fire(fsm);    
    }
    return 0;
}
