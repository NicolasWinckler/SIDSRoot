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


class SidsQualityTagField : public TGGroupFrame 
{
public:
    SidsQualityTagField(const TGWindow *p, const char* name, Int_t ButtonID=666);
    virtual ~SidsQualityTagField();
    
    TGTextEntry *GetTagEntry() const { return fQualityEntry->GetNumberEntry(); }
    void SetTagEntry(Int_t number){fQualityEntry->SetIntNumber(number);}
    
    TGTextEntry *GetCommentEntry() const { return fCommentEntry; }
    void SetCommentEntry(string comment){fQualityEntry->SetText(comment.c_str());}
    
    void GetTagField(char* val);
    int GetTag() const {return fQualityTag;}
    
    void GetCommentField(char* val){fComment(val);}
    string GetTag() const {return fComment;}
    
    void ClearWindow();
    
protected:

    TGHorizontalFrame *fhorframe1; 
    TGNumberEntry *fQualityEntry;
    TGTextEntry* fCommentEntry;
    TGLabel *fLabel;
    TGLabel *fLabel2;

    int fQualityTag;
    string fComment;
    
};

#endif	/* SIDSQUALITYTAGFIELD_H */

