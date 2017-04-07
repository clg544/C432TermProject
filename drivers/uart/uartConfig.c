/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "uartDefs.h"
#include "uart_hw.h"
#include <uart.h>
#include <common.h>

#define UART_MODULE_INPUT_CLK (48000000)

unsigned int UARTRegConfigModeEnable(unsigned int baseAdd, unsigned int modeFlag)
{
    unsigned int lcrRegValue = 0;

    /* Preserving the current value of LCR. */
    lcrRegValue = DEREF(baseAdd + UART_LCR);

    switch(modeFlag)
    {
        case UART_REG_CONFIG_MODE_A:
        case UART_REG_CONFIG_MODE_B:
            DEREF(baseAdd + UART_LCR) = (modeFlag & 0xFF);
        break;

        case UART_REG_OPERATIONAL_MODE:
            DEREF(baseAdd + UART_LCR) &= 0x7F;
        break;

        default:
        break;
    }

    return lcrRegValue;
}

void UARTStdioInitExpClk(unsigned int baudRate,
                                unsigned int rxTrigLevel,
                                unsigned int txTrigLevel)
{
    /* Performing a module reset. */
    UARTModuleReset(UART_PORT0);

    /* Performing FIFO configurations. */
    UartFIFOConfigure(txTrigLevel, rxTrigLevel);

    /* Performing Baud Rate settings. */
    UartBaudRateSet(baudRate);

    /* Switching to Configuration Mode B. */
    UARTRegConfigModeEnable(UART_PORT0, UART_REG_CONFIG_MODE_B);

    /* Programming the Line Characteristics. */
    UARTLineCharacConfig(UART_PORT0,
                         (UART_FRAME_WORD_LENGTH_8 | UART_FRAME_NUM_STB_1),
                         UART_PARITY_NONE);

    /* Disabling write access to Divisor Latches. */
    UARTDivisorLatchDisable(UART_PORT0);

    /* Disabling Break Control. */
    UARTBreakCtl(UART_PORT0, UART_BREAK_COND_DISABLE);

    /* Switching to UART16x operating mode. */
    UARTOperatingModeSelect(UART_PORT0, UART16x_OPER_MODE);
}

void UartFIFOConfigure(unsigned int txTrigLevel,
                       unsigned int rxTrigLevel)
{
    unsigned int fifoConfig = 0;

    /* Setting the TX and RX FIFO Trigger levels as 1. No DMA enabled. */
    fifoConfig = UART_FIFO_CONFIG(UART_TRIG_LVL_GRANULARITY_1,
                                  UART_TRIG_LVL_GRANULARITY_1,
                                  txTrigLevel,
                                  rxTrigLevel,
                                  1,
                                  1,
                                  UART_DMA_EN_PATH_SCR,
                                  UART_DMA_MODE_0_ENABLE);

    /* Configuring the FIFO settings. */
    UARTFIFOConfig(UART_PORT0, fifoConfig);
}

void UartBaudRateSet(unsigned int baudRate)
{
    unsigned int divisorValue = 0;

    /* Computing the Divisor Value. */
    divisorValue = UARTDivisorValCompute(UART_MODULE_INPUT_CLK,
                                         baudRate,
                                         UART16x_OPER_MODE,
                                         UART_MIR_OVERSAMPLING_RATE_42);

    /* Programming the Divisor Latches. */
    UARTDivisorLatchWrite(UART_PORT0, divisorValue);
}

void UARTLineCharacConfig(unsigned int baseAdd, 
                          unsigned int wLenStbFlag,
                          unsigned int parityFlag)
{
    /* Clearing the CHAR_LENGTH and NB_STOP fields in LCR.*/
    DEREF(baseAdd + UART_LCR) &= ~(UART_LCR_NB_STOP | UART_LCR_CHAR_LENGTH);
    /* Programming the CHAR_LENGTH and NB_STOP fields in LCR. */
    DEREF(baseAdd + UART_LCR) |= (wLenStbFlag &
                                  (UART_LCR_NB_STOP | UART_LCR_CHAR_LENGTH));

    /* Clearing the PARITY_EN, PARITY_TYPE1 and PARITY_TYPE2 fields in LCR. */
    DEREF(baseAdd + UART_LCR) &= ~(UART_LCR_PARITY_TYPE2 |
                                   UART_LCR_PARITY_TYPE1 |
                                   UART_LCR_PARITY_EN);
    /* Programming the PARITY_EN, PARITY_TYPE1 and PARITY_TYPE2 fields in LCR.*/
    DEREF(baseAdd + UART_LCR) |= (parityFlag & (UART_LCR_PARITY_TYPE2 |
                                                UART_LCR_PARITY_TYPE1 |
                                                UART_LCR_PARITY_EN));

}

void UARTDivisorLatchDisable(unsigned int baseAdd)
{
    /* Disabling access to Divisor Latch registers by clearing LCR[7] bit. */
    DEREF(baseAdd + UART_LCR) &= ~(UART_LCR_DIV_EN);
}

void UARTBreakCtl(unsigned int baseAdd, unsigned int breakState)
{
    /* Clearing the BREAK_EN bit in LCR. */
    DEREF(baseAdd + UART_LCR) &= ~(UART_LCR_BREAK_EN);

    /* Programming the BREAK_EN bit in LCR. */
    DEREF(baseAdd + UART_LCR) |= (breakState & UART_LCR_BREAK_EN);
}

unsigned int UARTOperatingModeSelect(unsigned int baseAdd,
                                     unsigned int modeFlag)
{
    unsigned int operMode = 0;

    operMode = (DEREF(baseAdd + UART_MDR1) & UART_MDR1_MODE_SELECT);

    /* Clearing the MODESELECT field in MDR1. */
    DEREF(baseAdd + UART_MDR1) &= ~(UART_MDR1_MODE_SELECT);
    /* Programming the MODESELECT field in MDR1. */
    DEREF(baseAdd + UART_MDR1) |= (modeFlag & UART_MDR1_MODE_SELECT);

    return operMode;
}

unsigned int UARTFIFOConfig(unsigned int baseAdd,
                            unsigned int fifoConfig)
{
    unsigned int txGra = (fifoConfig & UART_FIFO_CONFIG_TXGRA) >> 26;
    unsigned int rxGra = (fifoConfig & UART_FIFO_CONFIG_RXGRA) >> 22;

    unsigned int txTrig = (fifoConfig & UART_FIFO_CONFIG_TXTRIG) >> 14;
    unsigned int rxTrig = (fifoConfig & UART_FIFO_CONFIG_RXTRIG) >> 6;

    unsigned int txClr = (fifoConfig & UART_FIFO_CONFIG_TXCLR) >> 5;
    unsigned int rxClr = (fifoConfig & UART_FIFO_CONFIG_RXCLR) >> 4;

    unsigned int dmaEnPath = (fifoConfig & UART_FIFO_CONFIG_DMAENPATH) >> 3;
    unsigned int dmaMode = (fifoConfig & UART_FIFO_CONFIG_DMAMODE);

    unsigned int enhanFnBitVal = 0;
    unsigned int tcrTlrBitVal = 0;
    unsigned int tlrValue = 0;
    unsigned int fcrValue = 0;

    /* Setting the EFR[4] bit to 1. */
    enhanFnBitVal = UARTEnhanFuncEnable(baseAdd);

    tcrTlrBitVal = UARTSubConfigTCRTLRModeEn(baseAdd);

    /* Enabling FIFO mode of operation. */
    fcrValue |= UART_FCR_FIFO_EN;

    /* Setting the Receiver FIFO trigger level. */
    if(UART_TRIG_LVL_GRANULARITY_1 != rxGra)
    {
        /* Clearing the RXTRIGGRANU1 bit in SCR. */
        DEREF(baseAdd + UART_SCR) &= ~(UART_SCR_RX_TRIG_GRANU1);

        /* Clearing the RX_FIFO_TRIG_DMA field of TLR register. */
        DEREF(baseAdd + UART_TLR) &= ~(UART_TLR_RX_FIFO_TRIG_DMA);

        fcrValue &= ~(UART_FCR_RX_FIFO_TRIG);
        
        /*
        ** Checking if 'rxTrig' matches with the RX Trigger level values
        ** in FCR.
        */
        if((UART_FCR_RX_TRIG_LVL_8 == rxTrig)  ||
           (UART_FCR_RX_TRIG_LVL_16 == rxTrig) ||
           (UART_FCR_RX_TRIG_LVL_56 == rxTrig) ||
           (UART_FCR_RX_TRIG_LVL_60 == rxTrig))
        {
            fcrValue |= (rxTrig & UART_FCR_RX_FIFO_TRIG);
        }
        else
        {
            /* RX Trigger level will be a multiple of 4. */
            /* Programming the RX_FIFO_TRIG_DMA field of TLR register. */
            DEREF(baseAdd + UART_TLR) |= ((rxTrig <<
                                           UART_TLR_RX_FIFO_TRIG_DMA_SHIFT) &
                                           UART_TLR_RX_FIFO_TRIG_DMA);
        }
    }
    else
    {
        /* 'rxTrig' now has the 6-bit RX Trigger level value. */

        rxTrig &= 0x003F;

        /* Collecting the bits rxTrig[5:2]. */
        tlrValue = (rxTrig & 0x003C) >> 2;

        /* Collecting the bits rxTrig[1:0] and writing to 'fcrValue'. */
        fcrValue |= (rxTrig & 0x0003) << UART_FCR_RX_FIFO_TRIG_SHIFT;
        
        /* Setting the RXTRIGGRANU1 bit of SCR register. */
        DEREF(baseAdd + UART_SCR) |= UART_SCR_RX_TRIG_GRANU1;

        /* Programming the RX_FIFO_TRIG_DMA field of TLR register. */
        DEREF(baseAdd + UART_TLR) |= (tlrValue << UART_TLR_RX_FIFO_TRIG_DMA_SHIFT);

    }

    /* Setting the Transmitter FIFO trigger level. */
    if(UART_TRIG_LVL_GRANULARITY_1 != txGra)
    {
        /* Clearing the TXTRIGGRANU1 bit in SCR. */
        DEREF(baseAdd + UART_SCR) &= ~(UART_SCR_TX_TRIG_GRANU1);

        /* Clearing the TX_FIFO_TRIG_DMA field of TLR register. */
        DEREF(baseAdd + UART_TLR) &= ~(UART_TLR_TX_FIFO_TRIG_DMA);

        fcrValue &= ~(UART_FCR_TX_FIFO_TRIG);

        /*
        ** Checking if 'txTrig' matches with the TX Trigger level values
        ** in FCR.
        */
        if((UART_FCR_TX_TRIG_LVL_8 == (txTrig))  ||
           (UART_FCR_TX_TRIG_LVL_16 == (txTrig)) ||
           (UART_FCR_TX_TRIG_LVL_32 == (txTrig)) ||
           (UART_FCR_TX_TRIG_LVL_56 == (txTrig)))
        {
            fcrValue |= (txTrig & UART_FCR_TX_FIFO_TRIG);
        }
        else
        {
            /* TX Trigger level will be a multiple of 4. */
            /* Programming the TX_FIFO_TRIG_DMA field of TLR register. */
            DEREF(baseAdd + UART_TLR) |= ((txTrig <<
                                           UART_TLR_TX_FIFO_TRIG_DMA_SHIFT) &
                                           UART_TLR_TX_FIFO_TRIG_DMA);
        }
    }
    else
    {
        /* 'txTrig' now has the 6-bit TX Trigger level value. */

        txTrig &= 0x003F;

        /* Collecting the bits txTrig[5:2]. */
        tlrValue = (txTrig & 0x003C) >> 2;

        /* Collecting the bits txTrig[1:0] and writing to 'fcrValue'. */
        fcrValue |= (txTrig & 0x0003) << UART_FCR_TX_FIFO_TRIG_SHIFT;
        
        /* Setting the TXTRIGGRANU1 bit of SCR register. */
        DEREF(baseAdd + UART_SCR) |= UART_SCR_TX_TRIG_GRANU1;

        /* Programming the TX_FIFO_TRIG_DMA field of TLR register. */
        DEREF(baseAdd + UART_TLR) |= (tlrValue << UART_TLR_TX_FIFO_TRIG_DMA_SHIFT);
    }

    if(UART_DMA_EN_PATH_FCR == dmaEnPath)
    {
        /* Configuring the UART DMA Mode through FCR register. */
        DEREF(baseAdd + UART_SCR) &= ~(UART_SCR_DMA_MODE_CTL);

        dmaMode &= 0x1;

        /* Clearing the bit corresponding to the DMA_MODE in 'fcrValue'. */
        fcrValue &= ~(UART_FCR_DMA_MODE);

        /* Setting the DMA Mode of operation. */
        fcrValue |= (dmaMode << UART_FCR_DMA_MODE_SHIFT);
    }
    else
    {
        dmaMode &= 0x3;

        /* Configuring the UART DMA Mode through SCR register. */
        DEREF(baseAdd + UART_SCR) |= UART_SCR_DMA_MODE_CTL;

        /* Clearing the DMAMODE2 field in SCR. */
        DEREF(baseAdd + UART_SCR) &= ~(UART_SCR_DMA_MODE_2);

        /* Programming the DMAMODE2 field in SCR. */
        DEREF(baseAdd + UART_SCR) |= (dmaMode << UART_SCR_DMA_MODE_2_SHIFT);
    }

    /* Programming the bits which clear the RX and TX FIFOs. */
    fcrValue |= (rxClr << UART_FCR_RX_FIFO_CLEAR_SHIFT);
    fcrValue |= (txClr << UART_FCR_TX_FIFO_CLEAR_SHIFT);

    /* Writing 'fcrValue' to the FIFO Control Register(FCR). */
    UARTFIFORegisterWrite(baseAdd, fcrValue);

    /* Restoring the value of TCRTLR bit in MCR. */
    UARTTCRTLRBitValRestore(baseAdd, tcrTlrBitVal);

    /* Restoring the value of EFR[4] to the original value. */
    UARTEnhanFuncBitValRestore(baseAdd, enhanFnBitVal);

    return fcrValue;
}

unsigned int UARTDivisorValCompute(unsigned int moduleClk,
                                   unsigned int baudRate,
                                   unsigned int modeFlag,
                                   unsigned int mirOverSampRate)
{
    unsigned int divisorValue = 0;

    modeFlag &= UART_MDR1_MODE_SELECT;

    switch(modeFlag)
    {
        case UART16x_OPER_MODE:
        case UART_SIR_OPER_MODE:
            divisorValue = (moduleClk)/(16 * baudRate);
        break;

        case UART13x_OPER_MODE:
            divisorValue = (moduleClk)/(13 * baudRate);
        break;
        
        case UART_MIR_OPER_MODE:
            divisorValue = (moduleClk)/(mirOverSampRate * baudRate);
        break;

        case UART_FIR_OPER_MODE:
            divisorValue = 0;
        break;

        default:
        break;
    }
    
    return divisorValue;
}

unsigned int UARTDivisorLatchWrite(unsigned int baseAdd, 
                                   unsigned int divisorValue)
{
    volatile unsigned int enhanFnBitVal = 0;
    volatile unsigned int sleepMdBitVal = 0;
    volatile unsigned int lcrRegValue = 0;
    volatile unsigned int operMode = 0;
    unsigned int divRegVal = 0;

    /* Switching to Register Configuration Mode B. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Collecting the current value of EFR[4] and later setting it. */
    enhanFnBitVal = DEREF(baseAdd + UART_EFR) & UART_EFR_ENHANCED_EN;
    DEREF(baseAdd + UART_EFR) |= UART_EFR_ENHANCED_EN;

    /* Switching to Register Operational Mode. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_OPERATIONAL_MODE);

    /*
    ** Collecting the current value of IER[4](SLEEPMODE bit) and later
    ** clearing it.
    */
    sleepMdBitVal = DEREF(baseAdd + UART_IER) & UART_IER_SLEEP_MODE_IT;
    DEREF(baseAdd + UART_IER) &= ~(UART_IER_SLEEP_MODE_IT);

    /* Switching to Register Configuration Mode B. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Collecting the current value of Divisor Latch Registers. */
    divRegVal = DEREF(baseAdd + UART_DLL) & 0xFF;
    divRegVal |= ((DEREF(baseAdd + UART_DLH) & 0x3F) << 8);

    /* Switch the UART instance to Disabled state. */
    operMode = UARTOperatingModeSelect(baseAdd, UART_MDR1_MODE_SELECT_DISABLED);

    /* Writing to Divisor Latch Low(DLL) register. */
    DEREF(baseAdd + UART_DLL) = (divisorValue & 0x00FF);

    /* Writing to Divisor Latch High(DLH) register. */
    DEREF(baseAdd + UART_DLH) = ((divisorValue & 0x3F00) >> 8);

    /* Restoring the Operating Mode of UART. */
    UARTOperatingModeSelect(baseAdd, operMode);

    /* Switching to Register Operational Mode. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_OPERATIONAL_MODE);

    /* Restoring the value of IER[4] to its original value. */
    DEREF(baseAdd + UART_IER) |= sleepMdBitVal;

    /* Switching to Register Configuration Mode B. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Restoring the value of EFR[4] to its original value. */
    DEREF(baseAdd + UART_EFR) &= ~(UART_EFR_ENHANCED_EN);
    DEREF(baseAdd + UART_EFR) |= enhanFnBitVal;

    /* Restoring the value of LCR Register. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;

    return divRegVal;
}

unsigned int UARTEnhanFuncEnable(unsigned int baseAdd)
{
    unsigned int enhanFnBitVal = 0;
    unsigned int lcrRegValue = 0;

    /* Enabling Configuration Mode B of operation. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Collecting the current value of ENHANCEDEN bit of EFR. */
    enhanFnBitVal = (DEREF(baseAdd + UART_EFR) & UART_EFR_ENHANCED_EN);

    /* Setting the ENHANCEDEN bit in EFR register. */
    DEREF(baseAdd + UART_EFR) |= UART_EFR_ENHANCED_EN;

    /* Programming LCR with the collected value. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;

    return enhanFnBitVal;
}

unsigned int UARTSubConfigTCRTLRModeEn(unsigned int baseAdd)
{
    unsigned int enhanFnBitVal = 0;
    unsigned int tcrTlrValue = 0;
    unsigned int lcrRegValue = 0;

    /* Switching to Register Configuration Mode B. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Collecting the current value of EFR[4] and later setting it. */
    enhanFnBitVal = DEREF(baseAdd + UART_EFR) & UART_EFR_ENHANCED_EN;
    DEREF(baseAdd + UART_EFR) |= UART_EFR_ENHANCED_EN;

    /* Switching to Register Configuration Mode A. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_A);

    /* Collecting the bit value of MCR[6]. */
    tcrTlrValue = (DEREF(baseAdd + UART_MCR) & UART_MCR_TCR_TLR);

    /* Setting the TCRTLR bit in Modem Control Register(MCR). */
    DEREF(baseAdd + UART_MCR) |= (UART_MCR_TCR_TLR);

    /* Switching to Register Configuration Mode B. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Restoring the value of EFR[4] to its original value. */
    DEREF(baseAdd + UART_EFR) &= ~(UART_EFR_ENHANCED_EN);
    DEREF(baseAdd + UART_EFR) |= enhanFnBitVal;

    /* Restoring the value of LCR. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;

    return tcrTlrValue;
}

void UARTFIFORegisterWrite(unsigned int baseAdd, unsigned int fcrValue)
{
    unsigned int divLatchRegVal = 0;
    unsigned int enhanFnBitVal = 0;
    unsigned int lcrRegValue = 0;

    /* Switching to Register Configuration Mode A of operation. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_A);

    /* Clearing the contents of Divisor Latch Registers. */
    divLatchRegVal = UARTDivisorLatchWrite(baseAdd, 0x0000);

    /* Set the EFR[4] bit to 1. */
    enhanFnBitVal = UARTEnhanFuncEnable(baseAdd);

    /* Writing the 'fcrValue' to the FCR register. */
    DEREF(baseAdd + UART_FCR) = fcrValue;

    /* Restoring the value of EFR[4] to its original value. */
    UARTEnhanFuncBitValRestore(baseAdd, enhanFnBitVal);

    /* Programming the Divisor Latch Registers with the collected value. */
    UARTDivisorLatchWrite(baseAdd, divLatchRegVal);

    /* Reinstating LCR with its original value. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;
}

void UARTTCRTLRBitValRestore(unsigned int baseAdd,
                             unsigned int tcrTlrBitVal)
{
    unsigned int enhanFnBitVal = 0;
    unsigned int lcrRegValue = 0;

    /* Switching to Register Configuration Mode B. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Collecting the current value of EFR[4] and later setting it. */
    enhanFnBitVal = DEREF(baseAdd + UART_EFR) & UART_EFR_ENHANCED_EN;
    DEREF(baseAdd + UART_EFR) |= UART_EFR_ENHANCED_EN;

    /* Switching to Configuration Mode A of operation. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_A);

    /* Programming MCR[6] with the corresponding bit value in 'tcrTlrBitVal'. */
    DEREF(baseAdd + UART_MCR) &= ~(UART_MCR_TCR_TLR);
    DEREF(baseAdd + UART_MCR) |= (tcrTlrBitVal & UART_MCR_TCR_TLR);

    /* Switching to Register Configuration Mode B. */
    UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Restoring the value of EFR[4] to its original value. */
    DEREF(baseAdd + UART_EFR) &= ~(UART_EFR_ENHANCED_EN);
    DEREF(baseAdd + UART_EFR) |= enhanFnBitVal;

    /* Restoring the value of LCR. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;
}

void UARTEnhanFuncBitValRestore(unsigned int baseAdd, 
                                unsigned int enhanFnBitVal)
{
    unsigned int lcrRegValue = 0;

    /* Enabling Configuration Mode B of operation. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_CONFIG_MODE_B);

    /* Restoring the value of EFR[4]. */
    DEREF(baseAdd + UART_EFR) &= ~(UART_EFR_ENHANCED_EN);
    DEREF(baseAdd + UART_EFR) |= (enhanFnBitVal & UART_EFR_ENHANCED_EN);

    /* Programming LCR with the collected value. */
    DEREF(baseAdd + UART_LCR) = lcrRegValue;
}