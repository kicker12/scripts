// File_7z - Info for 7z files
// Copyright (C) 2005-2007 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_7Z_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Archive/File_7z.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_7z::Read_Buffer_Continue()
{
    //Integrity
    if (Buffer_Size<=6)
        return;

    //Header
    if (CC2(Buffer)!=CC2("7z") || CC4(Buffer+2)!=0xBCAF271C)
    {
        File_Offset=File_Size;
        return;
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "7Z");

    //No need of more
    File_Offset=File_Size;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_7z::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

} //NameSpace

#endif //MEDIAINFO_7Z_YES
