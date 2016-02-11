*-----------------------------------------------------------------------
* Filename:		dmtriode.inc  V2.0  18/12/97
* Simulator:		PSpice
* Device type:		Triodes
* Device model:		Generic with parameters for:
*
*				2A3
*				3CX300
*				6SN7GTB
*				12AT7
*				12AU7
*				12AX7
*
* Author:		Duncan Munro
* Date:			23/9/97
* Copyright:		(C)1997-2000 Duncan Amplification
*
* V1.1 19/11/97	Model altered for 1G ohm resistors between A/G/K
*		and ground to prevent float problems.
*
* V2.0 18/12/97 Two additional parameters added to the model, ERP
*		to alter the power to 1.5 rule, and ERI to increase
*		this with reducing Vg.  Also discrete "no heater"
*		model added, TRIODENH
*
* Please note that this model is provided "as is" and
* no warranty is provided in respect of its suitability
* for any application.
*
* This model is provided for educational and non-profit use.
*
* Email queries to postmaster@duncanamps.com
*
* Pins   A  Anode
*        G  Grid
*        K  Cathode
*	 H1 Heater connection 1 (not used if no heater model)
*	 H2 Heater connection 2 (not used if no heater model)
*
*-----------------------------------------------------------------------

.SUBCKT TRIODE A G K H1 H2
+PARAMS: RCO=1.6 RHO=10.5 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=18E-3 CDO=0 RAP=4E-3
+ ERP=1.5 ERI=0
+ MU0=17.3 MUR=19E-3 EMC=9.6E-6 GCO=0 GCF=213E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p 

************************************************************************
*
* Heater model
*
* Can be operated from AC or DC power sources.
* NB: When operating from DC power sources, "Skip initial transient
*     solution" must be checked, to make use of this model.
*
* PARAMETERS
*
*	RCO Heater resistance cold (ohms)
*	RHO Heater resistance hot (ohms)
*	HTV Normal heater voltage (V)
* 	HWU Heater time to warm up to 90% of emission (seconds)
*
************************************************************************

Rcool H1 HA {RCO}
Rload HA HB 1M
Esens HD 0  VALUE {V(HA,HB)*1000}
Epwr  HE 0  VALUE {V(H1,H2)*V(HD)/(PWR({HTV},2)/{RHO})}
RH1   HE HF 91k
CH1   HF 0  {HWU/1E6}
EH2   HG 0  VALUE {V(HF)}
RH2   HG HH 270k
CH2   HH 0  {HWU/1E6}
EH3   HJ 0  VALUE {LIMIT{V(HH)-0.75,0,1E6}*4}
RH3   HJ HK 91k
CH3   HK 0  {HWU/1E6}
Ghot  HB H2 VALUE {(1/(V(HG)+0.001))/({RHO}-{RCO})*V(HB,H2)}

************************************************************************
*
* Anode/grid model
*
* Models reduction in mu at large negative grid voltages
* Models change in Ra with negative grid voltages
* Models limit in Ia with high +Vg and low Va
*
* PARAMETERS
*
*	LIP Conduction limit exponent
*	LIF Conduction limit factor
*	CDO Conduction offset
*	RAF Anode resistance factor for neg grid voltages
*	RAP Anode resistance factor for positive grid voltages
*	ERP Emission power
*	ERI Emission power increase
*	MU0 Mu between grid and anode at Vg=0
*	MUR Mu reduction factor for large negative grid voltages
*	EMC Emission coefficient
*	GCO Grid current offset in volts
*	GCF Grid current scale factor
*
************************************************************************

Elim  LI 0  VALUE {PWR(LIMIT{V(A,K),0,1E6},{LIP})*{LIF}}
Egg   GG 0  VALUE {V(G,K)-{CDO}}
Erpf  RP 0  VALUE {1+LIMIT{V(GG),0,-1E6}*{RAF}+LIMIT{V(GG),0,1E6}*{RAP}}
Egr   GR 0  VALUE {LIMIT{V(GG),0,1E6}+LIMIT{(V(GG))*(1+V(GG)*{MUR}),0,-1E6}}
Eem   EM 0  VALUE {LIMIT{V(A,K)*V(RP)+V(GR)*{MU0},0,1E6}}
Eep   EP 0  VALUE {PWR(V(EM),ERP-V(G,K)*ERI)*{EMC}}
Eel   EL 0  VALUE {LIMIT{V(EP),0,V(LI)}}
Eld   LD 0  VALUE {LIMIT{V(EP)-V(LI),0,1E6}}
Ga    A  K  VALUE {V(HK)*V(EL)}

************************************************************************
*
* Grid current model
*
* Models grid current, along with rise in grid current at low Va
*
************************************************************************
Egf   GF 0  VALUE {PWR(LIMIT{V(GG),0,1E6},1.5)*{GCF}}
Gg    G  K  VALUE {(V(GF)+V(LD))*V(HK)}

*
* Capacitances and anti-float resistors
*
CM1	G	K	{CGK}
CM2	A	G	{CGA}
CM3	A	K	{CAK}
RF1	A	0	1000MEG
RF2	G	0	1000MEG
RF3	K	0	1000MEG

.ENDS

.SUBCKT TRIODENH A G K
+PARAMS: LIP=1 LIF=3.7E-3 RAF=18E-3 CDO=0 RAP=4E-3
+ ERP=1.5 ERI=0
+ MU0=17.3 MUR=19E-3 EMC=9.6E-6 GCO=0 GCF=213E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p 

************************************************************************
*
* Anode/grid model
*
* Models reduction in mu at large negative grid voltages
* Models change in Ra with negative grid voltages
* Models limit in Ia with high +Vg and low Va
*
* PARAMETERS
*
*	LIP Conduction limit exponent
*	LIF Conduction limit factor
*	CDO Conduction offset
*	RAF Anode resistance factor for neg grid voltages
*	RAP Anode resistance factor for positive grid voltages
*	ERP Emission power
*	ERI Emission power increase
*	MU0 Mu between grid and anode at Vg=0
*	MUR Mu reduction factor for large negative grid voltages
*	EMC Emission coefficient
*	GCO Grid current offset in volts
*	GCF Grid current scale factor
*
************************************************************************

Elim  LI 0  VALUE {PWR(LIMIT{V(A,K),0,1E6},{LIP})*{LIF}}
Egg   GG 0  VALUE {V(G,K)-{CDO}}
Erpf  RP 0  VALUE {1+LIMIT{V(GG),0,-1E6}*{RAF}+LIMIT{V(GG),0,1E6}*{RAP}}
Egr   GR 0  VALUE {LIMIT{V(GG),0,1E6}+LIMIT{(V(GG))*(1+V(GG)*{MUR}),0,-1E6}}
Eem   EM 0  VALUE {LIMIT{V(A,K)*V(RP)+V(GR)*{MU0},0,1E6}}
Eep   EP 0  VALUE {PWR(V(EM),ERP-V(G,K)*ERI)*{EMC}}
Eel   EL 0  VALUE {LIMIT{V(EP),0,V(LI)}}
Eld   LD 0  VALUE {LIMIT{V(EP)-V(LI),0,1E6}}
Ga    A  K  VALUE {V(EL)}

************************************************************************
*
* Grid current model
*
* Models grid current, along with rise in grid current at low Va
*
************************************************************************
Egf   GF 0  VALUE {PWR(LIMIT{V(GG),0,1E6},1.5)*{GCF}}
Gg    G  K  VALUE {(V(GF)+V(LD))}

*
* Capacitances and anti-float resistors
*
CM1	G	K	{CGK}
CM2	A	G	{CGA}
CM3	A	K	{CAK}
RF1	A	0	1000MEG
RF2	G	0	1000MEG
RF3	K	0	1000MEG

.ENDS

*-----------------------------------------------------------------------
*
* Parameters for individual valves.  Subcircuits prefixed with NH have
* no heater model.  You may wish to use this if you don't require a
* heater model (and in most cases, you probably won't).
*
*	6SN7GTB
*	12AX7
*
*-----------------------------------------------------------------------

*
* 2A3 [2.0 18/12/97]
* =====================
*
* 	NH2A3 - Single triode with no heater model (virtual cathode)
*
* V2.0 18/12/97 Initial model
*

.SUBCKT NH2A3 A G K
XV1 	A G K TRIODENH
+PARAMS: LIP=1.5 LIF=3E-3 RAF=4.5E-3 CDO=0 RAP=0.005
+ ERP=1.6247 ERI=-1.9E-5
+ MU0=3.7731 MUR=4.685E-3 EMC=6.15E-5 GCO=0 GCF=1E-4
+ CGA=16.5p CGK=7.5p CAK=5.5p 
.ENDS

*
* 3CX300 [2.0 28/12/97]
* =====================
*
* 	NH3CX300 - Single triode with no heater model
*
* V2.0 28/12/97 Initial model
*

.SUBCKT NH3CX300 A G K
XV1 	A G K TRIODENH
+PARAMS: LIP=1.5 LIF=10 RAF=3E-3 CDO=0 RAP=0.005
+ ERP=1.25 ERI=-5E-4
+ MU0=8.321 MUR=3.46E-3 EMC=5.33E-4 GCO=0 GCF=1E-4
+ CGA=10p CGK=25p CAK=1p 
.ENDS

*
* 6SN7GTB [2.0 23/9/97]
* =====================
*
* 	6SN7GTB - Two triodes together with a heater model
*	NH6SN7GTB - Single triode with no heater model
*
* V1.0 23/09/97 Initial model
*
* V2.0 18/12/97 Model parameters ERP and ERI added
*

.SUBCKT 6SN7GTB A1 G1 K1 A2 G2 K2 H1 H2
XV1 	A1 G1 K1 H1 H2 TRIODE
XV2	A2 G2 K2 H1 H2 TRIODE
+PARAMS: RCO=3.2 RHO=21 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=18E-3 CDO=0 RAP=4E-3
+ ERP=1.5 ERI=0
+ MU0=17.3 MUR=19E-3 EMC=9.6E-6 GCO=0 GCF=213E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p 
.ENDS

.SUBCKT NH6SN7GTB A G K
XV1 	A G K H 0 TRIODE
+PARAMS: RCO=3.2 RHO=21 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=18E-3 CDO=0 RAP=4E-3
+ ERP=1.5 ERI=0
+ MU0=17.3 MUR=19E-3 EMC=9.6E-6 GCO=0 GCF=213E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p
E1	H 0 VALUE {6.3}
.ENDS

*
* 12AT7 [2.0  3/10/97]
* ====================
*
*	12AT7 - half 12AT7 with a single heater element.  Note the
*               heaters can be wired series (12.6V) or parallel (6.3V)
*		for pairs of valves.
*	NH12AT7 - half 12AT7 with no heater model.
*
* V2.0 28/12/97 Initial model
*

.SUBCKT 12AT7 A G K H1 H2
XV1 	A G K H1 H2 TRIODE
+PARAMS: RCO=6.2 RHO=42 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=83.77E-3 CDO=-0.5 RAP=100E-3
+ ERP=1.40 ERI=-1.17E-6
+ MU0=45.093 MUR=92.06E-3 EMC=8.63E-6 GCO=-0.5 GCF=120E-6
+ CGA=1.6p CGK=2.3p CAK=0.40p 
.ENDS

.SUBCKT NH12AT7 A G K
XV1 	A G K H 0 TRIODE
+PARAMS: RCO=6.2 RHO=42 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=83.77E-3 CDO=-0.5 RAP=100E-3
+ ERP=1.40 ERI=-1.17E-6
+ MU0=45.093 MUR=92.06E-3 EMC=8.63E-6 GCO=-0.5 GCF=120E-6
+ CGA=1.6p CGK=2.3p CAK=0.40p 
E1	H 0 VALUE {6.3}
.ENDS

*
* 12AU7 [2.1  3/10/97]
* ====================
*
*	12AU7 - half 12AU7 with a single heater element.  Note the
*               heaters can be wired series (12.6V) or parallel (6.3V)
*		for pairs of valves.
*	NH12AU7 - half 12AU7 with no heater model.
*
* V1.0 03/10/97 Initial model
*
* V2.0 18/12/97 Model parameters ERP and ERI added
* V2.1 27/12/97 Model recalibration for better accuracy
*

.SUBCKT 12AU7 A G K H1 H2
XV1 	A G K H1 H2 TRIODE
+PARAMS: RCO=6.2 RHO=42 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=29.8E-3 CDO=0 RAP=32E-3
+ ERP=1.35 ERI=0
+ MU0=14.036 MUR=32.03E-3 EMC=23.6E-6 GCO=0 GCF=120E-6
+ CGA=1.6p CGK=1.8p CAK=0.45p 
.ENDS

.SUBCKT NH12AU7 A G K
XV1 	A G K H 0 TRIODE
+PARAMS: RCO=6.2 RHO=42 HTV=6.3 HWU=10.5
+ LIP=1 LIF=3.7E-3 RAF=29.8E-3 CDO=0 RAP=32E-3
+ ERP=1.35 ERI=0
+ MU0=14.036 MUR=32.03E-3 EMC=23.6E-6 GCO=0 GCF=120E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p
E1	H 0 VALUE {6.3}
.ENDS


*
* 12AX7 [V2.0  3/10/97]
* =====================
*
*	12AX7 - half 12AX7 with a single heater element.  Note the
*               heaters can be wired series (12.6V) or parallel (6.3V)
*		for pairs of valves.
*	NH12AX7 - half 12AX7 with no heater model.
*
* V1.0  03/10/97 Initial model
*
* V1.1  20/11/97 More accurate fit to data sheet
*
* V2.0  18/12/97 Model parameters ERP and ERI added
*

.SUBCKT 12AX7 A G K H1 H2
XV1 	A G K H1 H2 TRIODE
+PARAMS: RCO=6.2 RHO=42 HTV=6.3 HWU=11
+ LIP=1.5 LIF=16E-6 RAF=0.073578 CDO=-0.53056 RAP=0.18
+ ERP=1.5 ERI=0
+ MU0=87.302 MUR=0.065687 EMC=1.11E-6 GCO=-0.2 GCF=10E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p 
.ENDS

.SUBCKT NH12AX7 A G K
XV1 	A G K H 0 TRIODE
+PARAMS: RCO=6.2 RHO=42 HTV=6.3 HWU=11
+ LIP=1.5 LIF=16E-6 RAF=0.073578 CDO=-0.53056 RAP=0.18
+ ERP=1.5 ERI=0
+ MU0=87.302 MUR=0.065687 EMC=1.11E-6 GCO=-0.2 GCF=10E-6
+ CGA=3.9p CGK=2.4p CAK=0.7p 
E1	H 0 VALUE {6.3}
.ENDS
