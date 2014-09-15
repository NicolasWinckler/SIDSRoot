/* 
 * File:   SidsQualityTagField.cxx
 * Author: winckler
 * 
 * Created on September 15, 2014, 12:18 PM
 */

#include "SidsQualityTagField.h"

SidsQualityTagField::SidsQualityTagField(const TGWindow *p, const char* name, Int_t ButtonID) : TGGroupFrame(p,name), 
        fhorframe1(NULL), 
        fQualityEntry(NULL), fCommentEntry(NULL), 
        flabel1(NULL), flabel2(NULL),
        fQualityTag(0), fComment()
{
    SetTitlePos(TGGroupFrame::kCenter);
    fhorframe1 = new TGHorizontalFrame(this, 200, 50);
    
    //freq
    flabel2 = new TGLabel(fhorframe1, "Quality tag");
    fhorframe1->AddFrame(flabel2, new TGLayoutHints( kLHintsExpandY,2,2,2,2));
    
    fQualityEntry = new TGNumberEntry(fhorframe1, 0, 6, ButtonID, TGNumberFormat::kNESInteger);
    fhorframe1->AddFrame(fQualityEntry, new TGLayoutHints( kLHintsExpandY,6,2,2,2));

    //time
    flabel1 = new TGLabel(fhorframe1, "Comment");
    fhorframe1->AddFrame(flabel1, new TGLayoutHints( kLHintsExpandY,20,2,2,2));
    
    fCommentEntry = new TGTextEntry(fhorframe1);
    fhorframe1->AddFrame(fCommentEntry, new TGLayoutHints(kLHintsExpandY,6,2,2,2));
    
    //parent frame
    AddFrame(fhorframe1, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));
    
    
}

SidsQualityTagField::~SidsQualityTagField() 
{
    
}

void SidsQualityTagField::ClearWindow()
{
    fhorframe1->RemoveFrame(fQualityEntry);
    fhorframe1->RemoveFrame(fCommentEntry);
    fhorframe1->RemoveFrame(flabel1);
    fhorframe1->RemoveFrame(flabel2);
    fQualityEntry->UnmapWindow();
    fCommentEntry->UnmapWindow();
    flabel2->UnmapWindow();
    flabel1->UnmapWindow();
    
    RemoveFrame(fhorframe1);
    
    RemoveAll();
    Cleanup();
    UnmapWindow();
}


void SidsQualityTagField::GetTagField(char* val)
{
    int value=atoi(val);
    fQualityTag=value;
}


void SidsQualityTagField::GetCommentField(char* val)
{
    fComment=string(val);
}

ClassImp(SidsQualityTagField)
        