/**
 * 
 *  This file is part of apidb.
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *  author: Azael Reyes
 * */

#ifndef APIDB_ERRORS_HPP
#define APIDB_ERRORS_HPP

#include <stdio.h>
#include <errno.h>
#include <string.h>


#include <toolkit/common/common.hpp>

namespace octetos
{
namespace apidb
{
        extern toolkit::Error* error;
        enum ErrorCodes
        {                        
                unattendedError,
                ReadFile_TempUnpackFail,//No se puede crear el directorio tempora para desempauqetar el archivo de proyecto.
                Read_FileFailParseNode,
                Read_UncomConfigFile,
                ReadFile_OpenXMLFile,
                ReadFile_OpenVerFile,
                ReadFile_InvlidPath//La direecion especificada  del archivo especificada no es valida
        };
        
        toolkit::Error getError();
        bool checkError();
        bool writeError(toolkit::Error* e);
}
}
#endif