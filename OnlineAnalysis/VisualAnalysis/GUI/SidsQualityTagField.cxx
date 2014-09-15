/* 
 * File:   SidsQualityTagField.cxx
 * Author: winckler
 * 
 * Created on September 15, 2014, 12:18 PM
 */

#include "SidsQualityTagField.h"

SidsQualityTagField::SidsQualityTagField(const TGWindow *p, const char* name, Int_t ButtonID) :
    fhorframe1(NULL), fQualityEntry(NULL), fLabel(NULL), fQualityTag(0)
{
    SetTitlePos(TGGroupFrame::kCenter);
    fhorframe1 = new TGHorizontalFrame(this, 200, 50);
    
    //label
    fLabel = new TGLabel(fhorframe1, "Quality Tag");
    fhorframe1->AddFrame(fLabel, new TGLayoutHints( kLHintsExpandY,2,2,2,2));
    
    fQualityEntry = new TGNumberEntry(fhorframe1, 0, 6, ButtonID, TGNumberFormat::kNESInteger);
    fhorframe1->AddFrame(fQualityEntry, new TGLayoutHints( kLHintsExpandY,6,2,2,2));
    
    //parent frame
    AddFrame(fhorframe1, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));
    

}



SidsQualityTagField::~SidsQualityTagField() 
{
    
}

void SidsQualityTagField::GetQualityTagField(char* val)
{
    int value=atoi(val);
    fQualityTag=value;
    
}


void SidsQualityTagField::ClearWindow()
{
    fhorframe1->RemoveFrame(fQualityEntry);
    fhorframe1->RemoveFrame(fCommentEntry);
    fhorframe1->RemoveFrame(fLabel);
    fhorframe1->RemoveFrame(fLabel2);
    fQualityEntry->UnmapWindow();
    fCommentEntry->UnmapWindow();
    fLabel->UnmapWindow();
    fLabel2->UnmapWindow();
    
    RemoveFrame(fhorframe1);
    
    RemoveAll();
    Cleanup();
    UnmapWindow();
}

