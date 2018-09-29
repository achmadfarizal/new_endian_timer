#elif defined(__AVR__) // define yang digunakan adalah _AVR_ sehingga menggunakan part source yang bagian ini

#if defined (__AVR_ATmega8__)
  //in some io definitions for older microcontrollers TIMSK is used instead of TIMSK1
  #define TIMSK1 TIMSK
#endif
	
  public:
    //****************************
    //  Configuration
    //****************************
    void initialize(unsigned long microseconds=1000000) __attribute__((always_inline)) {
	TCCR1B = _BV(WGM13);        
	/* Waveform Generation Mode terdapat pada bit ke 3 dan 4 pada settingan timer TCCR1B.
	TCCR1B di inisiasi dengan mengaktifkan pin WGM13 yakni bit ke-4 dari register TCCR1B, 
	menandakan mode yang digunakan adalah Phase and Frecuency correct PWM
	Ketika bit WGM13 di set aktif, maka nilai treshold counternya adalah ICR1 dan OCR1A.
	Sedangkan ketika WGM13 bernilai low, maka threshold dari counternya adalah bukan lagi dari ICR1 dan 0CR1A;*/
	
	
	TCCR1A = 0;                 /* register bernilai 0 menandakan tidak ada timer clock yang digunakan*/ 
	setPeriod(microseconds);
    }
    void setPeriod(unsigned long microseconds) __attribute__((always_inline)) {
	const unsigned long cycles = ((F_CPU/100000 * microseconds) / 20);  
	
	/*Skala clock (pre-scaller) yang terdapat pada chip keluarga AVR adalah 1,8,64,256, dan 1024
	Dalam mode Phase Correct PWM, untuk menentukan besar duty cylce maka Skala clock akan dikalikan dengan 512.
	Sedangkan untuk mode fast PWM, dikalikan dengan 256.*/
	
	if (cycles < TIMER1_RESOLUTION) {
		clockSelectBits = _BV(CS10); /*untuk nilai pre-scaller=1, maka bit yang digunakan adalah bit ke-0 pada register TCCR1_B*/
		pwmPeriod = cycles;
	} else
	if (cycles < TIMER1_RESOLUTION * 8) {
		clockSelectBits = _BV(CS11); /*untuk nilai pre-scaller=8, maka bit yang digunakan adalah bit ke-1 pada register TCCR1_B*/
		pwmPeriod = cycles / 8;
	} else
	if (cycles < TIMER1_RESOLUTION * 64) {
		clockSelectBits = _BV(CS11) | _BV(CS10); /*untuk nilai pre-scaller=64, maka bit yang digunakan adalah bit ke-0 dan bit ke-1 pada register TCCR1_B*/
		pwmPeriod = cycles / 64;
	} else
	if (cycles < TIMER1_RESOLUTION * 256) {
		clockSelectBits = _BV(CS12); /*untuk nilai pre-scaller=256, maka bit yang digunakan adalah bit ke-2 pada register TCCR1_B*/
		pwmPeriod = cycles / 256;
	} else
	if (cycles < TIMER1_RESOLUTION * 1024) {
		clockSelectBits = _BV(CS12) | _BV(CS10); /*untuk nilai pre-scaller=1024, maka bit yang digunakan adalah bit ke-0 dan bit ke-2 pada register TCCR1_B*/
		pwmPeriod = cycles / 1024;
	} else {
		clockSelectBits = _BV(CS12) | _BV(CS10 /*untuk nilai pre-scaller selain diatas, maka bit yang digunakan adalah bit ke-0 dan bit ke-2 pada register TCCR1_B*/
		pwmPeriod = TIMER1_RESOLUTION - 1;
	}
	ICR1 = pwmPeriod; /*ICR didefinisakan untuk treshlod dari counter */
	TCCR1B = _BV(WGM13) | clockSelectBits; 
    }

    //****************************
    //  Run Control
    //****************************
    void start() __attribute__((always_inline)) {
	TCCR1B = 0;
	TCNT1 = 0;		// TODO: does this cause an undesired interrupt?
	resume();
    }
    void stop() __attribute__((always_inline)) {
	TCCR1B = _BV(WGM13);
    }
    void restart() __attribute__((always_inline)) {
	start();
    }
    void resume() __attribute__((always_inline)) {
	TCCR1B = _BV(WGM13) | clockSelectBits;
    }

    //****************************
    //  PWM outputs
    //****************************
    void setPwmDuty(char pin, unsigned int duty) __attribute__((always_inline)) {
	unsigned long dutyCycle = pwmPeriod;
	dutyCycle *= duty;
	dutyCycle >>= 10;
	
	/*Menset pin OCRn diperlukan untuk membangkitkan gelombang PWM */
	if (pin == TIMER1_A_PIN) OCR1A = dutyCycle; 
	/*digunakan untuk men-set 0CR1A dengan nilai dutyCycle*/
	#ifdef TIMER1_B_PIN
	else if (pin == TIMER1_B_PIN) OCR1B = dutyCycle 
	/*digunakan untuk men-set 0CR1B dengan nilai dutyCycle jika support TIMER1_B_PIN */
	#endif
	#ifdef TIMER1_C_PIN
	else if (pin == TIMER1_C_PIN) OCR1C = dutyCycle; 
	/*digunakan untuk men-set 0CR1C dengan nilai dutyCycle jika support TIMER1_C_PIN */
	#endif
    }
	
	/*
	PWM diatur dengan menggunakan register TCCR1A dan TCCR1B.
	Bit - bit yang dapat membangkitkan PWM adalah WGM1O dan WGM11 jika menggunakan TCCR1A.
	Namun jika yang digunakan adalah TCCR1B makan bit itu adalah WGM12 dan WGM13.
	*/
    void pwm(char pin, unsigned int duty) __attribute__((always_inline)) {
	if (pin == TIMER1_A_PIN) { pinMode(TIMER1_A_PIN, OUTPUT); TCCR1A |= _BV(COM1A1); }
	/*Menset TCCR1A sebagai pin output jika pin tersebut aktif*/
	/*nilai pada register TCCR1A kemudian di "OR" kan dengan nilai bit ke-7 dari register TCCR1A untuk mengupdate nilai COM1A1*/
	
	#ifdef TIMER1_B_PIN
	else if (pin == TIMER1_B_PIN) { pinMode(TIMER1_B_PIN, OUTPUT); TCCR1A |= _BV(COM1B1); }
	/*Jika pin timer TCCR1_B diaktifkan maka akan Menset TCCR1_B sebagai pin output*/
	/*nilai pada register TCCR1A kemudian di "OR" kan dengan nilai bit ke-5 dari register TCCR1B untuk mengupdate nilai COM1B1*/
	
	#endif
	#ifdef TIMER1_C_PIN
	else if (pin == TIMER1_C_PIN) { pinMode(TIMER1_C_PIN, OUTPUT); TCCR1A |= _BV(COM1C1); }
	/*Jika timer TCCR1_C diaktifkan maka akan Menset TCCR1_C sebagai pin output*/
	/*nilai pada register TCCR1A kemudian di "OR" kan dengan nilai bit ke-6 dari register TCCR1A untuk mengupdate nilai COM1C1*/
	
	#endif
	setPwmDuty(pin, duty);
	TCCR1B = _BV(WGM13) | clockSelectBits; /*Nilai dari bit ke-4 timer TCCR1B di "OR" dengan settingan clock yang digunakan */
    }
    void pwm(char pin, unsigned int duty, unsigned long microseconds) __attribute__((always_inline)) {
	if (microseconds > 0) setPeriod(microseconds);
	pwm(pin, duty);
    }
    void disablePwm(char pin) __attribute__((always_inline)) {
	/*Untuk mematikan PWM maka nilai dari register TCCR1A akan di "and" dengan nilai pada COM1 yang digunakan*/
	if (pin == TIMER1_A_PIN) TCCR1A &= ~_BV(COM1A1); /*mode pwm dimatikan dengan cara meng-"and" kan nilai TCCR1A dengan negasi bit COM1A1 */
	#ifdef TIMER1_B_PIN
	else if (pin == TIMER1_B_PIN) TCCR1A &= ~_BV(COM1B1 /*mode pwm dimatikan dengan cara meng-"and" kan nilai TCCR1A dengan negasi bit COM1B1 */
	#endif
	#ifdef TIMER1_C_PIN
	else if (pin == TIMER1_C_PIN) TCCR1A &= ~_BV(COM1C1);/*mode pwm dimatikan dengan cara meng-"and" kan nilai TCCR1A dengan negasi bit COM1C1 */
	#endif
    }

    //****************************
    //  Interrupt Function
    //****************************
	
    void attachInterrupt(void (*isr)()) __attribute__((always_inline)) {
	isrCallback = isr;
	TIMSK1 = _BV(TOIE1);
    }
    void attachInterrupt(void (*isr)(), unsigned long microseconds) __attribute__((always_inline)) {
	if(microseconds > 0) setPeriod(microseconds);
	attachInterrupt(isr);
    }
    void detachInterrupt() __attribute__((always_inline)) {
	TIMSK1 = 0;
    }
    static void (*isrCallback)();
    static void isrDefaultUnused();

  private:
    // properties
    static unsigned short pwmPeriod;
    static unsigned char clockSelectBits;
