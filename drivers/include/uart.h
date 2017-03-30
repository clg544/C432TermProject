#ifndef __UART__
#define __UART__

/* Base addresses section 2.1 */
#define UART_PORT0 0x44E09000

// Uart Setup
void UART_Init(unsigned int port);
void UARTModuleReset(unsigned int baseAdd);

// Uart Printers
unsigned int UARTRegConfigModeEnable(unsigned int baseAdd, unsigned int modeFlag);
void UARTCharPut(unsigned int baseAdd, unsigned char byteTx);
signed char UARTCharGet(unsigned int baseAdd);
void UART_puts(unsigned int baseAdd, char *s, unsigned int length);

// Uart Config
void UARTStdioInitExpClk(unsigned int baudRate,
                                unsigned int rxTrigLevel,
                                unsigned int txTrigLevel);
void UARTModuleReset(unsigned int baseAdd);
void UartFIFOConfigure(unsigned int txTrigLevel,
                              unsigned int rxTrigLevel);
void UartBaudRateSet(unsigned int baudRate);
void UARTLineCharacConfig(unsigned int baseAdd, 
                          unsigned int wLenStbFlag,
                          unsigned int parityFlag);
void UARTDivisorLatchDisable(unsigned int baseAdd);
void UARTBreakCtl(unsigned int baseAdd, unsigned int breakState);
unsigned int UARTOperatingModeSelect(unsigned int baseAdd,
                                     unsigned int modeFlag);
unsigned int UARTFIFOConfig(unsigned int baseAdd,
                            unsigned int fifoConfig);
unsigned int UARTDivisorValCompute(unsigned int moduleClk,
                                   unsigned int baudRate,
                                   unsigned int modeFlag,
                                   unsigned int mirOverSampRate);
unsigned int UARTDivisorLatchWrite(unsigned int baseAdd, 
                                   unsigned int divisorValue);
unsigned int UARTEnhanFuncEnable(unsigned int baseAdd);
unsigned int UARTSubConfigTCRTLRModeEn(unsigned int baseAdd);
void UARTFIFORegisterWrite(unsigned int baseAdd, unsigned int fcrValue);
void UARTTCRTLRBitValRestore(unsigned int baseAdd,
                             unsigned int tcrTlrBitVal);
void UARTEnhanFuncBitValRestore(unsigned int baseAdd, 
                                unsigned int enhanFnBitVal);

#endif
