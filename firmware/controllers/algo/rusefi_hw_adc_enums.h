typedef enum __attribute__ ((__packed__)) {
	EFI_ADC_NONE = 0,

	EFI_ADC_0 = 1, // PA0
	EFI_ADC_1 = 2, // PA1
	EFI_ADC_2 = 3, // PA2
	EFI_ADC_3 = 4, // PA3
	EFI_ADC_4 = 5, // PA4
	EFI_ADC_5 = 6, // PA5
	EFI_ADC_6 = 7, // PA6
	EFI_ADC_7 = 8, // PA7
	EFI_ADC_8 = 9, // PB0
	EFI_ADC_9 = 10, // PB1
	EFI_ADC_10 = 11, // PC0
	EFI_ADC_11 = 12, // PC1
	EFI_ADC_12 = 13, // PC2
	EFI_ADC_13 = 14, // PC3
	EFI_ADC_14 = 15, // PC4
	EFI_ADC_15 = 16, // PC5
	EFI_ADC_16 = 17,
	EFI_ADC_17 = 18,
	EFI_ADC_18 = 19,
	EFI_ADC_19 = 20,
	EFI_ADC_20 = 21,
	EFI_ADC_21 = 22,
	EFI_ADC_22 = 23,
	EFI_ADC_23 = 24,
	EFI_ADC_24 = 25,
	EFI_ADC_25 = 26,
	EFI_ADC_26 = 27,
	EFI_ADC_27 = 28,
	EFI_ADC_28 = 29,
	EFI_ADC_29 = 30,
	EFI_ADC_30 = 31,
	EFI_ADC_31 = 32,

	EFI_ADC_TOTAL_CHANNELS = 33,	// Please keep this in sync with the last valid channel index!

	EFI_ADC_ERROR = 50,
} adc_channel_e;
