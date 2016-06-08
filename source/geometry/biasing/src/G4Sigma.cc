//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: G4Sigma.cc,v 1.4 2002/04/09 16:23:50 gcosmo Exp $
// GEANT4 tag $Name: geant4-04-01 $
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// G4Sigma.cc
//
// ----------------------------------------------------------------------

#include "G4Sigma.hh"
#include "globals.hh"
#include <cstdio>
#include <cmath>
 
G4Sigma::G4Sigma()
{
  Init();
}

G4Sigma::~G4Sigma()
{}

void G4Sigma::Init()
{
  fEntries = 0;
  fMean = 0.;
  fSigma = -1.;
  fXsum = 0;
  fXXsum = 0;
  fWsum = 0;
  fWXsum = 0;
  fWXXsum = 0;
  fcalc = 0;
}

G4int G4Sigma::Xin(G4double x, G4double w)
{
  if (w<0.) Error("Xin: w < 0");
  fEntries++;
  fXsum+=x;
  fXXsum+=x*x;
  fWsum += w;
  fWXsum += w*x;
  fWXXsum += w*x*x;
  fcalc = 0;
  return fEntries;
}



G4int G4Sigma::GetCalc() const
{
  return fcalc;
}

G4int G4Sigma::Calculate() const
{
  if (fcalc==0) {
    if(fWsum>0) {
      fMean=fWXsum/fWsum;
      fSigma = sqrt( fWXXsum / fWsum - fMean * fMean);
      fcalc = 1;
    } else {
      fcalc = -1;
    }
  }
  return fcalc;
}

G4int G4Sigma::GetEntries() const
{
  return fEntries;
}

G4double G4Sigma::GetMean() const
{
  if (fcalc==0) Calculate();
  return fMean;
}

G4double G4Sigma::GetSigma() const
{
  if (fcalc==0) Calculate();
  return fSigma;
}

G4double G4Sigma::GetXsum() const {return fXsum;}
G4double G4Sigma::GetXXsum() const {return fXXsum;}
G4double G4Sigma::GetSumOfWeights() const {return fWsum;}
G4double G4Sigma::GetWeightedXsum() const {return fWXsum;}
G4double G4Sigma::GetWeightedXXsum() const {return fWXXsum;}

void G4Sigma::Error(const G4String &m)
{
  G4cout << "ERROR: G4Sigma::" << m << G4endl;
}

G4std::ostream& operator<<(G4std::ostream &out, const G4Sigma &s)
{
  out << "entries                             : " << s.GetEntries() << "\n";
  out << "Sum(w)                              : " << s.GetSumOfWeights()<<"\n";
  out << "Sum(w*x)                            : " << s.GetWeightedXsum() << "\n";
  out << "Sum(w*x*x)                          : " << s.GetWeightedXXsum() << "\n";
  out << "mean=Sum(w*x) / Sum(w)              : " << s.GetMean() << "\n";
  out << "sigma=sqrt(Sum(w*x*x)/Sum(w)-mean^2): " << s.GetSigma() << "\n";
  out << "Sum(x)                              : " << s.GetXsum() << "\n";
  out << "Sum(x^2)                            : " << s.GetXXsum() << "\n";
  
  return out;
}