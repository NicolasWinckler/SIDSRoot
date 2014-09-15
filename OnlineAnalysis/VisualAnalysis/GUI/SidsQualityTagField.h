/* 
 * File:   SidsQualityTagField.h
 * Author: winckler
 *
 * Created on September 15, 2014, 12:18 PM
 */

#ifndef SIDSQUALITYTAGFIELD_H
#define	SIDSQUALITYTAGFIELD_H

#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TG3DLine.h>
#include <TApplication.h>
#include <stdlib.h>

#include <string>

using namespace std;

class SidsQualityTagField : public TGGroupFrame 
{

public:
   SidsQualityTagField(const TGWindow *p, const char* name, Int_t DecayID=666);
   virtual ~SidsQualityTagField();
   
   
   TGTextEntry *GetTagEntry() const { return fQualityEntry->GetNumberEntry(); }
   TGTextEntry *GetCommentEntry() const { return fCommentEntry; }
   
   void SetTagEntry(Int_t number){fQualityEntry->SetIntNumber(number);}
   void SetCommentEntry(string comment){fCommentEntry->SetText(comment.c_str());}
   
   
   void ClearWindow();
   
   
   void GetTagField(char* val);
   void GetCommentField(char* val);
   
   int GetTag() const {return fQualityTag;}
   string GetComment() const {return fComment;}
   

protected:
   
   TGHorizontalFrame *fhorframe1;
   
   TGNumberEntry *fQualityEntry;
   TGTextEntry* fCommentEntry;
   
   TGLabel *flabel1;
   TGLabel *flabel2;
      
   int fQualityTag;
   string fComment;
   
   ClassDef(SidsQualityTagField, 1);
};


#endif	/* SIDSQUALITYTAGFIELD_H */

