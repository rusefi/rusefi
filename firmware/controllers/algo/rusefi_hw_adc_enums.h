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
	EFI_ADC_16 = 17, // PA0, muxed using exrernal analog mux
	EFI_ADC_17 = 18, // PA1, muxed using exrernal analog mux
	EFI_ADC_18 = 19, // PA2, muxed using exrernal analog mux
	EFI_ADC_19 = 20, // PA3, muxed using exrernal analog mux
	EFI_ADC_20 = 21, // PA4, muxed using exrernal analog mux
	EFI_ADC_21 = 22, // PA5, muxed using exrernal analog mux
	EFI_ADC_22 = 23, // PA6, muxed using exrernal analog mux
	EFI_ADC_23 = 24, // PA7, muxed using exrernal analog mux
	EFI_ADC_24 = 25, // PB0, muxed using exrernal analog mux
	EFI_ADC_25 = 26, // PB1, muxed using exrernal analog mux
	EFI_ADC_26 = 27, // PC0, muxed using exrernal analog mux
	EFI_ADC_27 = 28, // PC1, muxed using exrernal analog mux
	EFI_ADC_28 = 29, // PC2, muxed using exrernal analog mux
	EFI_ADC_29 = 30, // PC3, muxed using exrernal analog mux
	EFI_ADC_30 = 31, // PC4, muxed using exrernal analog mux
	EFI_ADC_31 = 32, // PC5, muxed using exrernal analog mux

	// External ADC channels
	EFI_ADC_32 = 33,
	EFI_ADC_33 = 34,
	EFI_ADC_34 = 35,
	EFI_ADC_35 = 36,
	EFI_ADC_36 = 37,
	EFI_ADC_37 = 38,
	EFI_ADC_38 = 39,
	EFI_ADC_39 = 40,

	EFI_ADC_TOTAL_CHANNELS = 41,	// Please keep this in sync with the last valid channel index!

	EFI_ADC_ERROR = 50,
} adc_channel_e;

/* Please keep updating these define(s) */
#define EFI_ADC_ONCHIP_LAST	EFI_ADC_31
