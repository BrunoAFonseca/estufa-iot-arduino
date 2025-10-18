/* ============================
   Estufa – Arduino UNO (com histerese(O sistema não liga e desliga instantaneamente ao atingir um valor limite, ele mantém uma margem de segurança entre o ponto de ligar e o ponto de desligar))
   Pinos:
     A0  -> Sensor de temperatura (TMP36 ou LM35)
     D7  -> Motor (via transistor NPN + diodo)
     D8  -> LED (ativo-baixo por padrão)
     D9  -> Buzzer (passivo com tone())
   ============================ */

// ======== SELEÇÃO DO SENSOR ========
// Descomente APENAS um:
// #define SENSOR_LM35      // 10 mV/°C, 0°C => 0.0V
#define SENSOR_TMP36        // 10 mV/°C, 0°C => 0.5V

// ======== CONFIGURAÇÃO DE PINOS ========
#define PIN_TEMP   A0
#define PIN_MOTOR  7
#define PIN_LED    8
#define PIN_BUZZER 9

// ======== LÓGICA DO LED ========
// true  -> LED ativo-baixo  (ânodo no +5V; cátodo no pino D8 via resistor)  // acende com LOW
// false -> LED ativo-alto   (pino D8 -> resistor -> ânodo do LED; cátodo -> GND) // acende com HIGH
const bool LED_ACTIVE_LOW = false;  // ajuste para sua fiação

// ======== LIMITES (°C) COM HISTERese ========
const float FAN_ON_C     = 30.0;  // liga ventilador
const float FAN_OFF_C    = 28.0;  // desliga ventilador
const float ALARM_ON_C   = 50.0;  // liga LED + buzzer
const float ALARM_OFF_C  = 48.0;  // desliga LED + buzzer

// ======== ESTADO ========
bool fanOn   = false;
bool alarmOn = false;

// ======== FUNÇÕES AUXILIARES ========
void setLed(bool on) {
  // converte "on" para nível elétrico conforme polaridade
  bool levelHigh = LED_ACTIVE_LOW ? !on : on;  // ativo-baixo inverte
  digitalWrite(PIN_LED, levelHigh ? HIGH : LOW);
}

void setFan(bool on) {
  fanOn = on;
  digitalWrite(PIN_MOTOR, on ? HIGH : LOW);  // HIGH aciona transistor NPN
}

void setAlarm(bool on) {
  alarmOn = on;
  setLed(on);
  if (on) {
    tone(PIN_BUZZER, 2000);  // 2 kHz
  } else {
    noTone(PIN_BUZZER);
    digitalWrite(PIN_BUZZER, LOW); // garante desligado em buzzer ativo
  }
}

float lerTemperaturaC() {
  const float ADC_REF_V = 5.0;            // UNO opera em 5 V
  int raw = analogRead(PIN_TEMP);         // 0..1023
  float vout = (raw * ADC_REF_V) / 1023.0;

  #ifdef SENSOR_LM35
    // LM35: 10 mV/°C, sem offset
    return vout * 100.0;
  #endif

  #ifdef SENSOR_TMP36
    // TMP36: 10 mV/°C com offset de 500 mV (0°C => 0.5 V)
    return (vout - 0.5) * 100.0;
  #endif
}

// ======== ARDUINO ========
void setup() {
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  setFan(false);
  setAlarm(false);

  Serial.begin(9600);
  delay(200);
  Serial.println(F("Estufa iniciada."));

  // --- Teste rápido do LED para confirmar polaridade (opcional) ---
  // Deve piscar 2 vezes no início:
  for (int i = 0; i < 2; i++) {
    setLed(true);  delay(250);
    setLed(false); delay(250);
  }
}

void loop() {
  float t = lerTemperaturaC();

  // Controle do ventilador com histerese
  if (!fanOn && t >= FAN_ON_C)  setFan(true);
  if ( fanOn && t <= FAN_OFF_C) setFan(false);

  // Controle do alarme com histerese
  if (!alarmOn && t >= ALARM_ON_C) setAlarm(true);
  if ( alarmOn && t <= ALARM_OFF_C) setAlarm(false);

  // Log no serial
  Serial.print(F("Temp: "));
  Serial.print(t, 1);
  Serial.print(F(" °C | Fan: "));
  Serial.print(fanOn ? F("ON") : F("OFF"));
  Serial.print(F(" | Alarm: "));
  Serial.println(alarmOn ? F("ON") : F("OFF"));

  delay(500);
}
