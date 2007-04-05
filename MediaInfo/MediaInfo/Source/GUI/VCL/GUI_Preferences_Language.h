// GUI_Preferences_Language - Preferences interface of MediaInfo
// Copyright (C) 2002-2005 Jerome Martinez, Zen@MediaArea.net
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Preferences interface of MediaInfo (Language definition part)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//---------------------------------------------------------------------------
#ifndef GUI_Preferences_LanguageH
#define GUI_Preferences_LanguageH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <TntStdCtrls.hpp>
#include <TntGrids.hpp>
#include <TntForms.hpp>
#include <ZenLib/ZtringListListF.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

class TPreferences_LanguageF : public TTntForm
{
__published:    // IDE-managed Components
    TTntStringGrid *Grille;
    TTntButton *OK;
    TTntButton *Cancel;
    void __fastcall GrilleKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:    // User declarations
    ZtringListListF EditedLanguage;
public:     // User declarations
    __fastcall TPreferences_LanguageF(TComponent* Owner);
    int Run(const Ztring &Name);
};
#endif
