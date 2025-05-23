// Permission is hereby granted, free of charge, to anyone obtaining
// a copy of this document, to do whatever they want with them without
// any restriction, including, but not limited to, copying, modification
// and redistribution.
//
// NO WARRANTY OF ANY KIND IS PROVIDED.

#if 0
// For when https://github.com/stm32duino/Arduino_Core_STM32/pull/1091
// is merged
/* Remap pin name */
PA_9_R    = PA_9 | PREMAP,
PA_10_R   = PA_10 | PREMAP,

/* Alternate pin name */
PA_6_ALT1 = PA_6 | ALT1,
PA_7_ALT1 = PA_7 | ALT1,
PA_7_ALT2 = PA_7 | ALT2,
PA_7_ALT3 = PA_7 | ALT3,
PB_0_ALT1 = PB_0 | ALT1,
PB_1_ALT1 = PB_1 | ALT1,
PB_1_ALT2 = PB_1 | ALT2,
PB_6_ALT1 = PB_6 | ALT1,
#endif

/* SYS_WKUP */
#ifdef PWR_WAKEUP_PIN1
  SYS_WKUP1 = PA_0,
#endif
#ifdef PWR_WAKEUP_PIN2
  SYS_WKUP2 = PC_13,
#endif
#ifdef PWR_WAKEUP_PIN3
  SYS_WKUP3 = NC,
#endif
#ifdef PWR_WAKEUP_PIN4
  SYS_WKUP4 = PA_2,
#endif
#ifdef PWR_WAKEUP_PIN5
  SYS_WKUP5 = NC,
#endif
#ifdef PWR_WAKEUP_PIN6
  SYS_WKUP6 = PB_5,
#endif
#ifdef PWR_WAKEUP_PIN7
  SYS_WKUP7 = NC,
#endif
#ifdef PWR_WAKEUP_PIN8
  SYS_WKUP8 = NC,
#endif

/* No USB */
