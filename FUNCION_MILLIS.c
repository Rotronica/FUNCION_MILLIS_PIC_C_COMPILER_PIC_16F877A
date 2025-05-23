/*
 * Autor:Rodrigo Calle C.
 * El siguiente programa realiza el parpadeo de leds con distintos tiempos, todos parpadeando paralelamente esto se hace con el uso
 * de la funicion llamada millis el cual es utilizado en arduino y otros programas para embebidos, es una forma muy eficiente de programacion
 * sin utilizar la sentencia delay_ms() ya que el delay detiene o trunca la secuencia o flujo de nuestro codigo.
 * esta funcion hace uso del TMR0
 */
#include <16f877a.h>
#fuses xt,nowdt,noput,noprotect,nolvp,nobrownout
#use delay(clock = 4M)
#include <stdint.h>

#define  TIME_1   1000  //Tiempo para 1 segundo
#define  TIME_2   500   //Tiempo para 500ms
#define  TIME_3   3000  //Tiempo para 3 segundos
uint32_t milisegundos = 0;
//Subrutina o funcion que atiende la interrupcion por TMR0
#INT_TIMER0
void INT_TMR0(void){
   set_timer0(131);  //se carga el valor 131 para una temporizacion de 1ms
   milisegundos++;   //incrementa cada un milisegundo
}
//Prototipo de funciones 
void configurar_hardware(void);  //Configura registros port y tris
void configurar_tmr0(void);      //Configura el modulo tmr 0 y la interrupcion por TMR0
uint32_t millis(void);           //Funcion que se encarga de generar un tick de 1ms

//Aqui inicia la logica de programa
void configurar_hardware(void){
   set_tris_d(0x00); //Todo el puerto D como salida
   output_d(0x00);   //Se limpia todo el puerto D 
}
void configurar_tmr0(void){
   //Configuramos el TMR0
   setup_timer_0(T0_INTERNAL|RTCC_DIV_8); //Setea el modulo TMR0 como temporizador y utiliza un prescaler de 1:8
   //Configurar la interrupcion TMR0
   enable_interrupts(int_timer0); //habilita la interrupcion TMR0
   enable_interrupts(global); //habilita la interrupcion GLOBAL
   set_timer0(131);           //Carga el numero 131 para la obtencion de 1ms
}

//Funcion millis
uint32_t millis(void){
   static uint32_t m;   //Variable local para guardar cada milisegundo
   disable_interrupts(global);   //Se desactivan las interrupciones globales
   m=milisegundos;               //Se guardan los milisegundos en la variable m
   enable_interrupts(global);    //Se activa las interrupciones globales
   return m;                     //Retorna el contenido de la varible m
}

//Funcion principal
void main(void){
   //Variables locales donde se guarda cada milisegundos de la funcion millis
   uint32_t save_time1 = 0;
   uint32_t save_time2 = 0;
   uint32_t save_time3 = 0;
   
   uint32_t time_milis = 0;   //Variable en donde se guarda cada milisegundo
   configurar_hardware();     //llamada a funcion
   configurar_tmr0();         //Configurar el modulo TMR0
   while(true){
      time_milis = millis();  
      //Primer led parpadea cada segundo
      if((time_milis - save_time1) >= TIME_1){
         save_time1 = time_milis;   
         output_toggle(PIN_D0);
      }
      //Segundo led parapadea cada 500ms
      if((time_milis - save_time2) >= TIME_2){
         save_time2 = time_milis;
         output_toggle(PIN_D1);
      }
      //Tercer led parpadea cada 3 segundos
      if((time_milis - save_time3) >= TIME_3){
         save_time3 = time_milis;
         output_toggle(PIN_D2);
      }
   }
}
