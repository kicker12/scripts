// File_OtherText - Use magic number to detect only the format (Text)
// Copyright (C) 2006-2007 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation condition
#include <MediaInfo/Setup.h>
#if defined(MEDIAINFO_OTHERTEXT_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_OtherText.h"
#include "ZenLib/ZtringList.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Functions
//***************************************************************************

void File_OtherText::Read_Buffer_Continue()
{
    if (Buffer_Size<0x200)
        return;

    Ztring Format, FormatMore, Url, Extensions, Codec;
    Ztring File;
    ZtringList Lines;

    //Feed File and Lines
    File.From_Local((char*)Buffer, Buffer_Size); //Ansi
    if (File.size()<0x100)
    {
        File.From_Unicode((wchar_t*)Buffer+1, 0, Buffer_Size/sizeof(wchar_t)); //Unicode with BOM
        //TODO: Order of bytes (big or Little endian)
        if (File.size()<0x100)
        {
            File_Offset=File_Size;
            return;
        }
    }
    if (File.size()>0x1000)
        File.resize(0x1000); //To not work on too big
    if (File.find(_T("\x0D\x0A"))!=Error)
    {
        Lines.Separator_Set(0, _T("\x0D\x0A"));
        Lines.Write(File);
    }
    else if (File.find(_T("\x0D"))!=Error)
    {
        Lines.Separator_Set(0, _T("\x0D"));
        Lines.Write(File);
    }
    else if (File.find(_T("\x0A"))!=Error)
    {
        Lines.Separator_Set(0, _T("\x0A"));
        Lines.Write(File);
    }
    else
    {
        File_Offset=File_Size;
        return;
    }
    if (Lines.size()<0x10)
    {
        File_Offset=File_Size;
        return;
    }
    Lines.resize(0x10);

         if (Lines[0].size()==1
          && Lines[0][0]==_T('1')
          && Lines[1].size()==29
          && Lines[1][ 0]==_T('0') && Lines[1][ 1]==_T('0')
          && Lines[1][ 2]==_T(':') && Lines[1][ 5]==_T(':') && Lines[1][ 8]==_T(',')
          && Lines[1][12]==_T(' ') && Lines[1][13]==_T('-') && Lines[1][14]==_T('-') && Lines[1][15]==_T('>') && Lines[1][16]==_T(' ')
          && Lines[1][17]==_T('0') && Lines[1][18]==_T('0')
          && Lines[1][19]==_T(':') && Lines[1][22]==_T(':') && Lines[1][25]==_T(',')
          && Lines.Find(_T("2"))!=Error
          )
    {
        Format=_T("SubRip");
        Extensions=_T("srt");
        Url=_T("http://ffdshow.sourceforge.net/tikiwiki/tiki-index.php?page=Getting+ffdshow");
        Codec=_T("SubRip");
    }
    else if (Lines[0]!=_T("[Script Info]")
          && Lines.Find(_T("ScriptType: v4.00"))!=Error
          && Lines.Find(_T("[V4 Styles]"))!=Error
          )
    {
       Format=_T("SSA");
       FormatMore=_T("SubStation Alpha");
       Extensions=_T("ssa");
       Url=_T("http://ffdshow.sourceforge.net/tikiwiki/tiki-index.php?page=Getting+ffdshow");
       Codec=_T("SSA");
    }
    else if (Lines[0]!=_T("[Script Info]")
          && Lines.Find(_T("ScriptType: v4.00+"))!=Error
          && Lines.Find(_T("[V4+ Styles]"))!=Error
          )
    {
       Format=_T("ASS");
       FormatMore=_T("Advanced SubStation Alpha");
       Extensions=_T("ssa");
       Url=_T("http://ffdshow.sourceforge.net/tikiwiki/tiki-index.php?page=Getting+ffdshow");
       Codec=_T("ASS");
    }
         if (Lines[0].size()>24
          && Lines[0][ 0]==_T('0') && Lines[0][ 1]==_T('0')
          && Lines[0][ 2]==_T(':') && Lines[0][ 5]==_T(':') && Lines[0][ 8]==_T(':')
          && Lines[0][11]==_T(' ')
          && Lines[0][12]==_T('0') && Lines[0][13]==_T('0')
          && Lines[0][14]==_T(':') && Lines[0][17]==_T(':') && Lines[0][20]==_T(':')
          && Lines[0][23]==_T(' ')
          )
    {
       Format=_T("Adobe encore DVD");
       Extensions=_T("txt");
       Url=_T("http://www.adobe.fr/products/encore/");
       Codec=_T("Adobe");
    }
    else if (Lines[0].size()==11
          && Lines[0][0]==_T('-') && Lines[0][1]==_T('-') && Lines[0][2]==_T('>') && Lines[0][3]==_T('>') && Lines[0][4]==_T(' ')
          && Lines[0][5]==_T('0')
          && Lines[1].empty()!=true
          )
    {
       Format=_T("AQTitle");
       Extensions=_T("aqt");
       Url=_T("http://www.volny.cz/aberka/czech/aqt.html");
       Codec=_T("AQTitle");
    }
         if (Lines[0].size()>28
          && Lines[0][ 0]==_T('0') && Lines[0][ 1]==_T('0')
          && Lines[0][ 2]==_T(':') && Lines[0][ 5]==_T(':') && Lines[0][ 8]==_T(':')
          && Lines[0][11]==_T(' ') && Lines[0][12]==_T(',') && Lines[0][13]==_T(' ')
          && Lines[0][14]==_T('0') && Lines[0][15]==_T('0')
          && Lines[0][16]==_T(':') && Lines[0][19]==_T(':') && Lines[0][22]==_T(':')
          && Lines[0][25]==_T(' ') && Lines[0][16]==_T(',') && Lines[0][27]==_T(' ')
          )
    {
       Format=_T("Captions 32");
       Extensions=_T("txt");
       Codec=_T("Caption 32");
    }
    else if (Lines[0].size()==23
          && Lines[0]==_T("*Timecode type: PAL/EBU")
          && Lines[1].empty()
          && Lines[2].size()==23
          && Lines[2][ 0]==_T('0') && Lines[2][ 1]==_T('0')
          && Lines[2][ 2]==_T(':') && Lines[2][ 5]==_T(':') && Lines[2][ 8]==_T(':')
          && Lines[2][11]==_T(' ')
          && Lines[2][12]==_T('0') && Lines[2][13]==_T('0')
          && Lines[2][14]==_T(':') && Lines[2][17]==_T(':') && Lines[2][20]==_T(':')
          && Lines[2].size()>0
          )
    {
       Format=_T("Captions Inc");
       Extensions=_T("txt");
       Codec=_T("Captions inc");
    }
    else if (Lines[0].size()>1
          && Lines[0][0]==_T('*')
          && Lines.Find(_T("** Caption Number 1"))!=Error
    )
    {
       Format=_T("Cheeta");
       Extensions=_T("asc");
    }
    else if (Lines[0].size()>10
          && Lines[0][0]==_T('~') && Lines[0][1]==_T('C') && Lines[0][2]==_T('P') && Lines[0][3]==_T('C') && Lines[0][9]==_T('~')
          && Lines[1][ 0]==_T('0') && Lines[1][ 1]==_T('0')
          && Lines[1][ 2]==_T(':') && Lines[1][ 5]==_T(':') && Lines[1][ 8]==_T(':')
    )
    {
       Format=_T("CPC Captioning");
       Extensions=_T("txt");
       Url=_T("http://www.cpcweb.com/Captioning/cap_software.htm");
       Codec=_T("CPC Captioning");
    }
    else if (false
    )
    {
       Format=_T("");
       Extensions=_T("");
       Url=_T("");
       Codec=_T("");
    }
    else
        return;

    if (Format.empty())
        return;

    Stream_Prepare(Stream_General);
    General[0](_T("Format"))=Format;
    General[0](_T("Format/More"))=FormatMore;
    General[0](_T("Format/Url"))=Url;
    General[0](_T("Format/Extensions"))=Extensions;
    Stream_Prepare(Stream_Text);
    Text[0](_T("Codec"))=Codec;

    //No need of more
    File_Offset=File_Size;
}

} //NameSpace

#endif
