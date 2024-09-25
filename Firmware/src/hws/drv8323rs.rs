#![allow(dead_code)]

use crate::tasks::messages::{Commands, USART_WRITE_SIGNAL};
use defmt::*;
use embassy_time::Timer;
use embedded_hal_async::spi::{self, Operation};

pub const FSR1: u16 = 0x0;
pub const FSR2: u16 = 0x1;
pub const DCR: u16 = 0x2;
pub const HSR: u16 = 0x3;
pub const LSR: u16 = 0x4;
pub const OCPCR: u16 = 0x5;
pub const CSACR: u16 = 0x6;

pub const DIS_CPUV_EN: u16 = 0x0;
pub const DIS_CPUV_DIS: u16 = 0x1;
pub const DIS_GDF_EN: u16 = 0x0;
pub const DIS_GDF_DIS: u16 = 0x1;
pub const OTW_REP_EN: u16 = 0x1;
pub const OTW_REP_DIS: u16 = 0x0;
pub const PWM_MODE_6X: u16 = 0x0;
pub const PWM_MODE_3X: u16 = 0x1;
pub const PWM_MODE_1X: u16 = 0x2;
pub const PWM_MODE_IND: u16 = 0x3;
pub const PWM_1X_COM_SYNC: u16 = 0x0;
pub const PWM_1X_COM_ASYNC: u16 = 0x1;
pub const PWM_1X_DIR_0: u16 = 0x0;
pub const PWM_1X_DIR_1: u16 = 0x1;

pub const LOCK_ON: u16 = 0x6;
pub const LOCK_OFF: u16 = 0x3;
pub const IDRIVEP_HS_10MA: u16 = 0x0;
pub const IDRIVEP_HS_30MA: u16 = 0x1;
pub const IDRIVEP_HS_60MA: u16 = 0x2;
pub const IDRIVEP_HS_80MA: u16 = 0x3;
pub const IDRIVEP_HS_120MA: u16 = 0x4;
pub const IDRIVEP_HS_140MA: u16 = 0x5;
pub const IDRIVEP_HS_170MA: u16 = 0x6;
pub const IDRIVEP_HS_190MA: u16 = 0x7;
pub const IDRIVEP_HS_260MA: u16 = 0x8;
pub const IDRIVEP_HS_330MA: u16 = 0x9;
pub const IDRIVEP_HS_370MA: u16 = 0xA;
pub const IDRIVEP_HS_440MA: u16 = 0xB;
pub const IDRIVEP_HS_570MA: u16 = 0xC;
pub const IDRIVEP_HS_680MA: u16 = 0xD;
pub const IDRIVEP_HS_820MA: u16 = 0xE;
pub const IDRIVEP_HS_1000MA: u16 = 0xF;
pub const IDRIVEN_HS_20MA: u16 = 0x0;
pub const IDRIVEN_HS_60MA: u16 = 0x1;
pub const IDRIVEN_HS_120MA: u16 = 0x2;
pub const IDRIVEN_HS_160MA: u16 = 0x3;
pub const IDRIVEN_HS_240MA: u16 = 0x4;
pub const IDRIVEN_HS_280MA: u16 = 0x5;
pub const IDRIVEN_HS_340MA: u16 = 0x6;
pub const IDRIVEN_HS_380MA: u16 = 0x7;
pub const IDRIVEN_HS_520MA: u16 = 0x8;
pub const IDRIVEN_HS_660MA: u16 = 0x9;
pub const IDRIVEN_HS_740MA: u16 = 0xA;
pub const IDRIVEN_HS_880MA: u16 = 0xB;
pub const IDRIVEN_HS_1140MA: u16 = 0xC;
pub const IDRIVEN_HS_1360MA: u16 = 0xD;
pub const IDRIVEN_HS_1640MA: u16 = 0xE;
pub const IDRIVEN_HS_2000MA: u16 = 0xF;

pub const TDRIVE_500NS: u16 = 0x0;
pub const TDRIVE_1000NS: u16 = 0x1;
pub const TDRIVE_2000NS: u16 = 0x2;
pub const TDRIVE_4000NS: u16 = 0x3;
pub const IDRIVEP_LS_10MA: u16 = 0x0;
pub const IDRIVEP_LS_30MA: u16 = 0x1;
pub const IDRIVEP_LS_60MA: u16 = 0x2;
pub const IDRIVEP_LS_80MA: u16 = 0x3;
pub const IDRIVEP_LS_120MA: u16 = 0x4;
pub const IDRIVEP_LS_140MA: u16 = 0x5;
pub const IDRIVEP_LS_170MA: u16 = 0x6;
pub const IDRIVEP_LS_190MA: u16 = 0x7;
pub const IDRIVEP_LS_260MA: u16 = 0x8;
pub const IDRIVEP_LS_330MA: u16 = 0x9;
pub const IDRIVEP_LS_370MA: u16 = 0xA;
pub const IDRIVEP_LS_440MA: u16 = 0xB;
pub const IDRIVEP_LS_570MA: u16 = 0xC;
pub const IDRIVEP_LS_680MA: u16 = 0xD;
pub const IDRIVEP_LS_820MA: u16 = 0xE;
pub const IDRIVEP_LS_1000MA: u16 = 0xF;
pub const IDRIVEN_LS_20MA: u16 = 0x0;
pub const IDRIVEN_LS_60MA: u16 = 0x1;
pub const IDRIVEN_LS_120MA: u16 = 0x2;
pub const IDRIVEN_LS_160MA: u16 = 0x3;
pub const IDRIVEN_LS_240MA: u16 = 0x4;
pub const IDRIVEN_LS_280MA: u16 = 0x5;
pub const IDRIVEN_LS_340MA: u16 = 0x6;
pub const IDRIVEN_LS_380MA: u16 = 0x7;
pub const IDRIVEN_LS_520MA: u16 = 0x8;
pub const IDRIVEN_LS_660MA: u16 = 0x9;
pub const IDRIVEN_LS_740MA: u16 = 0xA;
pub const IDRIVEN_LS_880MA: u16 = 0xB;
pub const IDRIVEN_LS_1140MA: u16 = 0xC;
pub const IDRIVEN_LS_1360MA: u16 = 0xD;
pub const IDRIVEN_LS_1640MA: u16 = 0xE;
pub const IDRIVEN_LS_2000MA: u16 = 0xF;

pub const TRETRY_4MS: u16 = 0x0;
pub const TRETRY_50US: u16 = 0x1;
pub const DEADTIME_50NS: u16 = 0x0;
pub const DEADTIME_100NS: u16 = 0x1;
pub const DEADTIME_200NS: u16 = 0x2;
pub const DEADTIME_400NS: u16 = 0x3;
pub const OCP_LATCH: u16 = 0x0;
pub const OCP_RETRY: u16 = 0x1;
pub const OCP_REPORT: u16 = 0x2;
pub const OCP_NONE: u16 = 0x3;
pub const OCP_DEG_2US: u16 = 0x0;
pub const OCP_DEG_4US: u16 = 0x1;
pub const OCP_DEG_6US: u16 = 0x2;
pub const OCP_DEG_8US: u16 = 0x3;
pub const VDS_LVL_0_06: u16 = 0x0;
pub const VDS_LVL_0_13: u16 = 0x1;
pub const VDS_LVL_0_2: u16 = 0x2;
pub const VDS_LVL_0_26: u16 = 0x3;
pub const VDS_LVL_0_31: u16 = 0x4;
pub const VDS_LVL_0_45: u16 = 0x5;
pub const VDS_LVL_0_53: u16 = 0x6;
pub const VDS_LVL_0_6: u16 = 0x7;
pub const VDS_LVL_0_68: u16 = 0x8;
pub const VDS_LVL_0_75: u16 = 0x9;
pub const VDS_LVL_0_94: u16 = 0xA;
pub const VDS_LVL_1_13: u16 = 0xB;
pub const VDS_LVL_1_3: u16 = 0xC;
pub const VDS_LVL_1_5: u16 = 0xD;
pub const VDS_LVL_1_7: u16 = 0xE;
pub const VDS_LVL_1_88: u16 = 0xF;

pub const CSA_FET_SP: u16 = 0x0;
pub const VREF_DIV_1: u16 = 0x0;
pub const VREF_DIV_2: u16 = 0x1;
pub const CSA_GAIN_5: u16 = 0x0;
pub const CSA_GAIN_10: u16 = 0x1;
pub const CSA_GAIN_20: u16 = 0x2;
pub const CSA_GAIN_40: u16 = 0x3;
pub const DIS_SEN_EN: u16 = 0x0;
pub const DIS_SEN_DIS: u16 = 0x1;
pub const SEN_LVL_0_25: u16 = 0x0;
pub const SEN_LVL_0_5: u16 = 0x1;
pub const SEN_LVL_0_75: u16 = 0x2;
pub const SEN_LVL_1_0: u16 = 0x3;

pub struct DRV8232RS<SPI> {
    spi: SPI,
}

impl<SPI> DRV8232RS<SPI>
where
    SPI: spi::SpiDevice<u16>,
{
    pub async fn new(spi: SPI) -> DRV8232RS<SPI> {
        DRV8232RS { spi }
    }

    async fn write(&mut self, val: u16) -> u16 {
        let mut rx_data = [0u16; 1];
        if let Err(_) = self
            .spi
            .transaction(&mut [Operation::Transfer(&mut rx_data, &[val])])
            .await
        {
            error!("drv8323 spi transfer failed");
        }
        rx_data[0]
    }

    pub async fn read_fsr1(&mut self) -> u16 {
        let val = (1u16 << 15) | (FSR1 << 11);
        self.write(val).await
    }

    pub async fn read_fsr2(&mut self) -> u16 {
        let val = (1u16 << 15) | (FSR2 << 11);
        self.write(val).await
    }

    pub async fn read_register(&mut self, reg: u16) -> u16 {
        self.write((1u16 << 15) | (reg << 11)).await
    }

    pub async fn write_register(&mut self, reg: u16, val: u16) {
        self.write((reg << 11) | val).await;
    }

    pub async fn write_dcr(
        &mut self,
        dis_cpuv: u16,
        dis_gdf: u16,
        otw_rep: u16,
        pwm_mode: u16,
        pwm_com: u16,
        pwm_dir: u16,
        coast: u16,
        brake: u16,
        clr_flt: u16,
    ) {
        let val = (DCR << 11)
            | (dis_cpuv << 9)
            | (dis_gdf << 8)
            | (otw_rep << 7)
            | (pwm_mode << 5)
            | (pwm_com << 4)
            | (pwm_dir << 3)
            | (coast << 2)
            | (brake << 1)
            | clr_flt;
        debug!("write_dcr: {:016b}", val);
        self.write(val).await;
    }

    pub async fn write_hsr(&mut self, lock: u16, idrivep_hs: u16, idriven_hs: u16) {
        let val = (HSR << 11) | (lock << 8) | (idrivep_hs << 4) | idriven_hs;
        debug!("write_hsr: {:016b}", val);
        self.write(val).await;
    }

    pub async fn write_lsr(&mut self, cbc: u16, tdrive: u16, idrivep_ls: u16, idriven_ls: u16) {
        let val = (LSR << 11) | (cbc << 10) | (tdrive << 8) | (idrivep_ls << 4) | idriven_ls;
        debug!("write_lsr: {:016b}", val);
        self.write(val).await;
    }

    pub async fn write_ocpcr(
        &mut self,
        tretry: u16,
        dead_time: u16,
        ocp_mode: u16,
        ocp_deg: u16,
        vds_lvl: u16,
    ) {
        let val = (OCPCR << 11)
            | (tretry << 10)
            | (dead_time << 8)
            | (ocp_mode << 6)
            | (ocp_deg << 4)
            | vds_lvl;
        debug!("write_ocpcr: {:016b}", val);
        self.write(val).await;
    }

    pub async fn write_csacr(
        &mut self,
        csa_fet: u16,
        vref_div: u16,
        ls_ref: u16,
        csa_gain: u16,
        dis_sen: u16,
        csa_cal_a: u16,
        csa_cal_b: u16,
        csa_cal_c: u16,
        sen_lvl: u16,
    ) {
        let val = (CSACR << 11)
            | (csa_fet << 10)
            | (vref_div << 9)
            | (ls_ref << 8)
            | (csa_gain << 6)
            | (dis_sen << 5)
            | (csa_cal_a << 4)
            | (csa_cal_b << 3)
            | (csa_cal_c << 2)
            | sen_lvl;
        debug!("write_csacr: {:016b}", val);
        self.write(val).await;
    }

    pub async fn dbg_reg_val(&mut self) {
        let fsr1 = self.read_register(FSR1).await;
        Timer::after_micros(10).await;
        let fsr2 = self.read_register(FSR2).await;
        Timer::after_micros(10).await;
        let dcr = self.read_register(DCR).await;
        Timer::after_micros(10).await;
        let hsr = self.read_register(HSR).await;
        Timer::after_micros(10).await;
        let lsr = self.read_register(LSR).await;
        Timer::after_micros(10).await;
        let ocpcr = self.read_register(OCPCR).await;
        Timer::after_micros(10).await;
        let csacr = self.read_register(CSACR).await;
        Timer::after_micros(10).await;
        debug!(
            "FSR1:{:016b} FSR2:{:016b} DCR:{:016b} HSR:{:016b} LSR:{:016b} OCPCR:{:016b} CSACR:{:016b}",
            fsr1,fsr2,dcr,hsr,lsr,ocpcr,csacr
        );
    }

    pub async fn print_faults(&mut self) {
        let val1 = self.read_fsr1().await;
        Timer::after_micros(10).await;
        let val2 = self.read_fsr2().await;
        Timer::after_micros(10).await;

        debug!("FSR1:{} FSR2:{}", val1, val2);

        if val1 & (1 << 10) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("FAULT"));
        }
        if val1 & (1 << 9) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_OCP"));
        }
        if val1 & (1 << 8) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("GDF"));
        }
        if val1 & (1 << 7) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("UVLO"));
        }
        if val1 & (1 << 6) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("OTSD"));
        }
        if val1 & (1 << 5) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_HA"));
        }
        if val1 & (1 << 4) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_LA"));
        }
        if val1 & (1 << 3) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_HB"));
        }
        if val1 & (1 << 2) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_LB"));
        }
        if val1 & (1 << 1) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_HC"));
        }
        if val1 & 1 != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VDS_LC"));
        }

        if val2 & (1 << 10) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("SA_OC"));
        }
        if val2 & (1 << 9) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("SB_OC"));
        }
        if val2 & (1 << 8) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("SC_OC"));
        }
        if val2 & (1 << 7) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("OTW"));
        }
        if val2 & (1 << 6) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("CPUV"));
        }
        if val2 & (1 << 5) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VGS_HA"));
        }
        if val2 & (1 << 4) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VGS_LA"));
        }
        if val2 & (1 << 3) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VGS_HB"));
        }
        if val2 & (1 << 2) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VGS_LB"));
        }
        if val2 & (1 << 1) != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VGS_HC"));
        }
        if val2 & 1 != 0 {
            USART_WRITE_SIGNAL.signal(Commands::UsartTxStr("VGS_LC"));
        }
    }

    /// Write a 1 to this bit to put all MOSFETs in the Hi-Z state
    pub async fn enable_gd(&mut self) {
        let val = (self.read_register(DCR).await) & (!(0x1u16 << 2));
        self.write_register(DCR, val).await;
    }

    pub async fn disable_gd(&mut self) {
        let val = (self.read_register(DCR).await) | (0x1u16 << 2);
        self.write_register(DCR, val).await;
    }

    pub async fn calibrate(&mut self) {
        let val = 0x1u16 << 4 + 0x1u16 << 3 + 0x1u16 << 2;
        self.write_register(CSACR, val).await;
    }
}
