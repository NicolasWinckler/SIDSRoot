/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                    OscConstField header file                  -----
// -----                Created 25/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------

#include "OscFieldCreator.h"

#include "OscFieldPar.h"
#include "OscConstField.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairField.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

static OscFieldCreator gOscFieldCreator;

OscFieldCreator::OscFieldCreator()
  :FairFieldFactory(),
   fFieldPar(NULL)
{
	fCreator=this;
}

OscFieldCreator::~OscFieldCreator()
{
}

void OscFieldCreator::SetParm()
{
  FairRunAna *Run = FairRunAna::Instance();
  FairRuntimeDb *RunDB = Run->GetRuntimeDb();
  fFieldPar = (OscFieldPar*) RunDB->getContainer("OscFieldPar");

}

FairField* OscFieldCreator::createFairField()
{ 
  FairField *fMagneticField=0;

  if ( ! fFieldPar ) {
    cerr << "-E-  No field parameters available!"
	 << endl;
  }else{
	// Instantiate correct field type
	Int_t fType = fFieldPar->GetType();
	if      ( fType == 0 ) fMagneticField = new OscConstField(fFieldPar);
    else cerr << "-W- FairRunAna::GetField: Unknown field type " << fType
		<< endl;
	cout << "New field at " << fMagneticField << ", type " << fType << endl;
	// Initialise field
	if ( fMagneticField ) {
		fMagneticField->Init();
		fMagneticField->Print();
	}
  }
  return fMagneticField;
}


ClassImp(OscFieldCreator)
