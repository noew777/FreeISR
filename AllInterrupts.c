/*****************************************************
 * 
 *            Noeww bare-metal Arduino :)
 *            
 *          Interrupt example in ATMega328
 * 
 ******************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t ISTstate[3];   

void setUpInterrupts(void);
void setUpIO(void);

/**************************
    MAIN FUNCTION JJJJJJ 
***************************/
int main() {
  setUpInterrupts();  // Set up all interrupts
  setUpIO();  // Set up all IO ports

  SREG |= (0x80); // Enable global interrupts 
  
  // Main loop, nothing here :)
  while(1){
  }
  
  return 0;
}

//
// Configure IO ports
//
void setUpIO(void){
  //    Config Port D 
  //- PIN2 (input, pullup)
  DDRB |= (0x20); // 1output
  PORTD |= (0x04);  // 1pull-up
}

//
// Configure interrupts
//
void setUpInterrupts(uint8_t  *ISTstate){
  //
  // PCIE2 -> Enables interrupts for PCINT[23-16] pins and PCMSK2 each sigle one
  // PCIE1 -> Enables interrupts for PCINT[14-8] pins and PCMSK1 each sigle one
  // PCIE0 -> Enables interrupts for PCINT[7-0] pins and PCMSK0 each sigle one
  //
  PCICR |= (1 << PCIE2 | 1 << PCIE1 | 1 << PCIE0);


  PCMSK2 |= (1 << PCINT23); 
  PCMSK2 |= (1 << PCINT23); 
  PCMSK2 |= (1 << PCINT23); 
  
  PCIFR &= ~(1 << PCIF2 | 1 << PCIF1 | 1 << PCIF0);
}


// here the code must check that the port is configured as input1!!!!!
void setUpAnyInterrupt(uint8_t  *ISTstate){
  volatile unsigned int *maskReg = (volatile unsigned int *)0x6d; // PCMSK2 address
  //volatile unsigned int *maskReg = (volatile unsigned int *) (*PCMSK2); NOEE TRY THIS ONE PLS

  for(int i = 0; i < 3; i++){
    if(*(ISTstate + i)){    // If any pin from each PCINT is enable...
        PCICR |= (1 << i);  // Enable its register 
        *maskReg = (uint8_t) *(ISTstate + i); // Enable their mask 
    }
    maskReg -= 1;   // Update mask register
  }
}

void updateGlobalInterrupts(uint8_t setGinterrupts){
    if(setUpAnyInterrupt == 1)
        SREG |= (0x80); // Enable global interrupts
    else if(setUpAnyInterrupt == 0)
        SREG &= ~(0x80); // Disable global interrupts
}



/****************************************************
    INTERRUPTIONS SERVICES ROUTINES FOR ALL PINES
*****************************************************/

// Aqui seguramente lo mas guay es que te fabriques un array con *funciones
// y que segun la interrupcion que sea llames a una u otra. El usuario deberia
// poder decirle al sistema que se ha creado una funcion para tal pin, y mandarle un
// puntero que apunte a esta última.

//
// ISR handler for PC[23-16]
//
ISR(PCINT2_vect){
    for(int i = 0; i < 8; i++){
        //si esta activada su mascara ISTstate...
            //comprobamos estado y si ha cambiado...
                //ejecutamos su callback del array de funciones "ISR"
    }
}

//
// ISR handler for PC[14-8]
//
ISR(PCINT1_vect){
  if(PIND & 0x04)
    PINB |= (0x20); // Toggles PB5(Pin 13)
  else
    PINB &= ~(0x20);
}

//
// ISR handler for PC[7-0]
//
ISR(PCINT0_vect){
  if(PIND & 0x04)
    PINB |= (0x20); // Toggles PB5(Pin 13)
  else
    PINB &= ~(0x20);
}
