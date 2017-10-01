#ifndef LIRUDY_H
#define LIRUDY_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// GEOMETRY
const double pi = 3.14;			
const double radius = 0.00175;	
const double length = 0.0164;	
const double rcg = 1.54;

// PHYSICAL CONSTANTS
const double frdy = 96485;		      
const double R = 8314;			  
const double temp = 310;		    

const double nao = 140;			
const double cao = 1.8;			
const double ko  = 5.4;			
const double clo = 100;		

const double zna = 1;			
const double zk  = 1;			
const double zcl = -1;		
const double zca = 2;			
const double ganai = 0.75;		
const double ganao = 0.75;		
const double gaki  = 0.75;	
const double gako  = 0.75;	
const double gacai = 1.0;		
const double gacao = 0.341;

// VOLTAGE			
const double dvdtthresh = 1;		

// STIMULUS CURRENT
const double istim = -80;
//const double stimdur = 0.5; // default
const double stimdur = 3.0;

// MEMBRANE IONIC CURRENTS
const double gna = 18;	  
const double gnal2 = 0.052;	
const double gnal3 = 0.018;
const double pca = 1.9926e-4;	
const double powtau = 10;	
const double gcat = 0.07875;    	
const double gtos = 0.1414;	   
const double gtof = 0.042;  
const double prnak = 0.014;
const double gnab = 0.0025;     

const double pcab = 3.99e-8;	  
const double pnab = 0.64e-8;

const double inacamax = 2.52;
const double kmcaact = 0.000125;
const double kmnai1 = 12.3;		
const double kmnao = 87.5;		
const double kmcai = 0.0036;	
const double kmcao = 1.3;		
const double nu = 0.35;			
const double ksat = 0.27;	
const double ibarnak = 1.1004;
const double ipcabar = 0.0115;		   
const double kmpca = 0.0005;

// CALCIUM FLUXES RATE CONSTANTS
const double tautr1 = 120;
const double tautr2 = 120;	
const double gaptau = 12;
const double sstau = 0.2;

const double k1 = 150000;
const double k1a = 16.5;
const double k0 = 96000;
const double k0a = 9.6;
const double k2 = 1800;
const double k2a = 0.21;
const double tauip3r = 3.7;

const double  dqupcamkbar = 0.75;
const double  dkmplbbar = 0.00017;
const double nsrbar = 15.0;
const double bsrbar = 0.019975;	
const double kmbsr = 0.00087;		
const double bslbar = 0.4777;	
const double kmbsl = 0.0087;
const double csqnbar = 2.88;		    
const double kmcsqn = 0.8;

const double cmdnbar = 0.1125;	
const double kmcmdn = 2.38e-3;
const double trpnbar = 3.15e-2;
const double kmtrpn = 0.5e-3;

const double camk0 = 0.05;		
const double alphacamk = 0.05;		
const double betacamk = 0.00068;	
const double kmcam = 0.0015;		
const double kmcamk = 0.15;	
const double fca_dtaucamkbar = 10.0;

const double trpnbar1 = 3.5e-3;
const double cmdnbar1 = 1.25e-2;
const double csqnbar1 = 1.2;


// TODO: transformar em uma classe
class Cell
{
public:
    // State variables
    double v, m, h, j, d, f, f2, fca, fca2, xs1, xs2, xr;
    double a, i, i2, ml, ml3, hl, hl3, jl, jl3; 
    double casss, cajsr, cacsr, cansr, cassl;
    double nai, nassl, nasss, ki, cai, b, g, u, y, camktrap;

    // Stimulus variables
    int beats;
    int stimcount;
    double BCL;
    double S2;
    double tstim, stimtime, dvdtclock;

    // Currents
    double ina, inal, inal2, inal3, inab, inacass, inaca, inak, inatot;
    double icat, ical, icab, icatot, ibarca;
    double itos, itof, ito1;
    double ikr, iks, ik1, iktot;
    double ipca;
    double ifna, ifk, iftotal;
    double itot;

    // Calcium dinamycs 
    double camkactive;
    double qip3, qrel1, qrel2, qup1, qup2, qtr1, qtr2;
    double caavg;

    // VOLTAGE
    double dvdt;

    // REVERSAL POTENTIALS
    double ena,ek,eca;

    // MEMBRANE IONIC CURRENTS
    double ma,mb,mtau,mss,ha,hb,htau,hss,ja,jb,jtau,jss;
    double alphaml,betaml,mltau,mlss,hltau,hlss;
    double jltau, jlss, jl3tau, jl3ss;

    double i3tau, i3ss, i3,Rtau, Rss, Ri,ml3tau, ml3ss, hl3tau, hl3ss,ireltau, REL;

    double dss,dtau,dpower,powss;
    double fss,ftau,f2ss,f2tau,fcass,fcatau,fca2tau,fca2ss;

    double taub,taug,bss,gss;

    double rto1,alphaa,betaa,atau,ass,alphai,betai,itau,iss,alphai2,betai2,i2tau,i2ss;

    double gkr,xrss,xrtau,rkr;

    double gks,eks;
    double xs1tau,xs2tau,xsss;

    double gk1,k1ss;
    double yss, ytau;

    double allo,num,denommult,denomterm1,denomterm2,deltaE;

    // CALCIUM FLUXES AND CONCENTRATIONS
    double irelss;
    double IP3 = 0.0001;
    double du,POip3;

    double  dqupcamk;
    double  dkmplb;
    double kmup   = 0.00028;

    double bsss,csqn1,bjsr,cjsr,csqn,bcsr,ccsr;
    double dcasss,cassstot,bsr,bsl,b1,c1;
    double dcassl;
    double qdiff;

    double dcajsr,cajsrtot;
    double dcacsr,cacsrtot;
    double dcansr;

    double dcai,catotal,cmdn;
    double trpn;
    double bmyo,cmyo,dmyo;

    // SODIUM/POTASSIUM FLUXES AND CONCENTRATIONS
    double dnai,dnasss,dki,ksss,dksss;
    double qgap, qdiffna, qgapna, dnassl;

    // CAMKII DYNAMICS
    double camkbound;
    double fca_dtaucamk;

public:
    void setInitialConditions ();
    void setStimulus (bool stim);
    void timestep (int n);

    void comp_revs ();
    void comp_ina ();
    void comp_inal ();
    void comp_inab ();
    void comp_ical ();
    void comp_icat ();
    void comp_icab ();
    void comp_ito1 ();
    void comp_ikr ();
    void comp_iks ();
    void comp_ik1 ();
    void comp_inaca ();
    void comp_inak ();
    void comp_ipca ();
    void comp_if ();
    void comp_istim (double t);
    void comp_itot ();

    void comp_ip3 ();
    void comp_qrel1 ();
    void comp_qrel2 ();
    void comp_qup1 ();
    void comp_qup2 ();
    void comp_qtr1 ();
    void comp_qtr2 ();
    void comp_conc ();

};

void setTimeSettings (double dt, double tmax);

void compGeometrics ();

#endif
