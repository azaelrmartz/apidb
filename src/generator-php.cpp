/**
 * 
 *  This file is part of apidb.
 *  APIDB do Make easy to connect your Database
 *  Copyright (C) 2018  Azael Reyes
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
 * */

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <locale>

#include "analyzer.hpp"
#include "generator.hpp"
#include "Errors.hpp"

namespace octetos
{
namespace apidb
{
namespace generators
{
    void PHP::writeRemoves(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Remove remove(configureProject,table,ofile);
        remove.setImplementation(true);
        remove.generate();
    }
    void PHP::getInheritKey(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << "->get" << k->getUpperName() << "()";
            getInheritKey(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << "->get" << k->getUpperName() << "()";
        }
    }
    void PHP::getKey(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {
            ofile << "->get" << k->getUpperName() << "()";
            getKey(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << "->get" << k->getUpperName() << "()";
        }
    }
    void PHP::writeUppdaters(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        if(table.getKey().size() == 0) return;
        
        /*for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(not it->second->isPrimaryKey())
			{
		        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
		            ofile << "\tpublic function up" << it->second->getUpperName() << "($connector,";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		        {
		            ofile << "\tpublic function up" << it->second->getUpperName() << "($connector,";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		        {
		            ofile << "\tpublic function up" << it->second->getUpperName() << "($connector,";
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }

                ofile << " $" << it->second->getName() ;
		        
				ofile <<") "<< std::endl;
				ofile << "\t{"<<std::endl;
				ofile << "\t\t$sqlString = \"\";"<<std::endl;
				if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << "\t\t$sqlString = \"UPDATE \\\"\" ; . $this->TABLE_NAME . \"\\\"\";"<<std::endl;
				}
				else
				{
					ofile << "\t\t$sqlString = \"UPDATE \" . $this->TABLE_NAME;"<<std::endl;
					//ofile << "\t\tsqlString += \" FROM \";"<<std::endl;
				}
				ofile << "\t\t$sqlString = $sqlString . \" SET " ;
		        
				ofile << it->second->getName()  << " = " ;
                if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() != NULL)
                {
                    ofile << "'\" .  $" << it->second->getName()   << ".getKey" << it->second->getSymbolReferenced()->getUpperName() << "() . \"'\";" << std::endl;                                    
                }
                else if( it->second->getOutType().compare("int") == 0 && it->second->getSymbolReferenced() == NULL)
                {
                    ofile << "'\" .  $" << it->second->getName()   << " + \"'\";" << std::endl;                            
                }
                else if(it->second->getOutType().compare("String") == 0 && it->second->getSymbolReferenced() != NULL)
                {
                    ofile << "'\" . $" << it->second->getName()  << " . \"'\";" << std::endl;
                }
                else if(it->second->getOutType().compare("String") == 0  && it->second->getSymbolReferenced() == NULL)
                {
                    ofile << "'\" . $" << it->second->getName()  << " . \"'\";" << std::endl;
                }
                else
                {
                    ofile << "\" . $" << it->second->getName()  << ";" << std::endl;
                }
								
                if(table.getKey().size() > 0)
                {
                    ofile << "\t\t$sqlString = $sqlString . \" WHERE ";
                    auto kEnd = table.getKey().end();
                    kEnd--;
                    for(auto k : table.getKey())
                    {
                            ofile << k->getName()  << " = \" . ";  
                            if(k->classReferenced != NULL) //es un objeto
                            {
                                if(k->outType.compare("String") == 0)
                                {
                                    ofile << "$this->" << k->getName() << ";\n";
                                }
                                else
                                {
                                    ofile << "$this->" << k->getName();
                                    getKey(ofile,k->symbolReferenced);
                                    ofile<< ";\n";
                                }
                            }
                            else if(k->outType.compare("String") == 0)
                            {
                                ofile << " '\" . $this->" <<  k->getName()  << " . \"'\";\n";
                            }
                            else
                            {
                                ofile << "$this->" << k->getName()  <<";\n";
                            }
                            if(k != *kEnd)
                            {
                                ofile << " . \" and \" ";
                            }
                    }
                }
                else
                {
                    throw BuildException("No hay soporte para table sin llave",__FILE__,__LINE__);
                }
				
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
		        {
                    ofile <<"\t\t$rs  = null;\n";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
		        {
		            ofile <<"\t\t$rs = null;\n";
		        }
		        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
		        {
		            ofile <<"\t\t$rs = null;\n";
		        }
		        else
		        {
		            std::string msg = "Lenguaje no soportado " ;
		            throw BuildException(msg);
		        }
                
				ofile <<"\t\treturn $connector->update($sqlString,$rs);\n";
				ofile << "\t}"<<std::endl;	
            } 
        }*/
        Update updater(configureProject,table,ofile);
        updater.setImplementation(true);
        updater.generate();
    }
    
    void PHP::writeGetters(const apidb::symbols::Table& table, std::ofstream& ofile)
    {   
        Getter getter(configureProject,table,ofile);
        getter.setImplementation(true);
        getter.generate();
    }
    
	PHP::~PHP()
	{
		//delete[] writeResults;
	}
	/*const std::string& Java::getHeaderName() const
	{
		return projectH;
	}
	std::ofstream& Java::getSourceOutput()
	{
		return writeResults[1];
	}
	std::ofstream& Java::getHeaderOutput()
	{
		return writeResults[0];
	}*/
	PHP::PHP(apidb::Analyzer& d,const ConfigureProject& config) : apidb::generators::Generator(config,d)
	{
        if(config.outputLenguaje != OutputLenguajes::PHP)
        {
            std::string msg = "La configuracion indica '" ;
            msg = msg + getOutputLenguajeString()+ "', pero el componente es PHP.";
            throw BuildException(msg);
        }
                
		//outputLenguaje = d.getOutputLenguaje();
		/*
        writeResults = new std::ofstream[2];
        if((configureProject.builDirectory.empty()) | (configureProject.builDirectory.compare(".") == 0)) 
		{
			projectH = configureProject.name + ".hpp";
			writeResults[0].open(projectH);
			projectCPP = configureProject.name + ".cpp";
			writeResults[1].open(projectCPP);
		}
		else
		{
			projectH = configureProject.name + ".hpp";
			projectCPP = configureProject.name + ".cpp";
			writeResults[0].open(configureProject.builDirectory + "/" + projectH);
			writeResults[1].open(configureProject.builDirectory + "/" + projectCPP);
		}
		*/
	}
	bool PHP::generate(bool log)
	{
		if(log)analyzer.getOutput().add("Generando archivos de codigo fuente... \n");
		std::string msg1 = "\tLenguaje resultado: " ;
		msg1 += getOutputLenguajeString() ;
		if(log)analyzer.getOutput().add(msg1);;
        std::string strFilename = configureProject.builDirectory;
        strFilename += "/" + configureProject.name + ".php";
        std::ofstream file(strFilename, std::ofstream::out);
        
        file << "<?php \n\n";
		if(create(file,log,getSymbolsTable()) == false)
		{
			return false;
		}
        file << "\n\n?>";
        return true;    
    }
		
    void PHP::writeSelects(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        Select select(configureProject,table,ofile);
        select.setImplementation(true);
        select.generate();
    }
	void PHP::writeDownloads(const apidb::symbols::Table& table, std::ofstream& ofile)
    {
        for( std::map<const char*,ConfigureProject::Table*>::const_iterator itT = configureProject.downloads.begin(); itT != configureProject.downloads.end(); itT++)//std::vector<Table>
        {
            if(table.getName().compare(itT->second->getName()) != 0) 
            {
                continue;//buscar la configuracion de la tabla correspondiente
            }
                        
            //for (auto const& [key, val] : *(itT->second))//class Table : public std::map<const char*, const Function*>
            for(ConfigureProject::Table::iterator itCfTb = itT->second->begin(); itCfTb != itT->second->end(); itCfTb++)
            {
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {       
                    ofile << "\tpublic function " << itCfTb->first << "($connector)"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\tpublic function " << itCfTb->first << "($connector)"<<std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\tpublic function " << itCfTb->first << "($connector)"<<std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                    throw BuildException(msg);
                }
                
                ofile << "\t{ " << std::endl;
                ofile << "\t\t$sqlString = \"SELECT ";
                const apidb::ConfigureProject::Parameters* params = itCfTb->second->getParameters();

                {
                apidb::ConfigureProject::Parameters::const_iterator itParamEnd = params->end();
                itParamEnd--;
                for(const std::string& param : *params)
                {
					ofile << param; 
					if(param != *itParamEnd)
					{
						ofile << ",";
					}
                }  
                if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
				{
					ofile << " FROM \\\"" << table.getName() << "\\\" WHERE \";\n" ;
				}
				else
				{
					ofile << " FROM " << table.getName() << " WHERE \";\n" ;
				}
				
				auto kEnd = table.getKey().end();
                kEnd--;
                for(auto k : table.getKey())
                {
                    ofile << "\t\t$sqlString = $sqlString . \"" << k->getName()  << " = \" . ";  
                    if(k->classReferenced != NULL) //es un objeto
                    {
                        if(k->outType.compare("String") == 0)
                        {
                            ofile << "'" << k->getName() << " . \"'\";\n";
                        }
                        else
                        {
                            ofile << " \"'\" . $this->" << k->getName();
                            getKey(ofile,k->symbolReferenced);
                            ofile<< ") . \"'\";\n";
                        }
                    }
                    else if(k->outType.compare("String") == 0)
                    {
                        ofile << " '\" . $this->" <<  k->getName()  << " . \"'\";\n";
                    }
                    else
                    {
                        ofile << "$this->" << k->getName()  <<";\n";
                    }
                    
                    if(k != *kEnd)
                    {
                        ofile << " . \" and \" ";
                    }          
                }
                
                if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
                {        
                    ofile << "\t\t$rs = null;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
                {
                    ofile << "\t\t$rs = null;"  << std::endl;
                }
                else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
                {
                    ofile << "\t\t$rs = null;"  << std::endl;
                }
                else
                {
                    std::string msg = "Lenguaje no soportado " ;
                                    throw BuildException(msg,__FILE__,__LINE__);
                }
                
                ofile << "\t\t$rs = $connector->select(sqlString);"  << std::endl;
                ofile << "\t\tif(!is_null(rs))"  << std::endl;
                ofile << "\t\t{" << std::endl;
                ofile << ""<< std::endl;
                ofile << "\t\t\twhile ($rs->nextRow()) "<< std::endl;
                ofile << "\t\t\t{"<< std::endl;
                //ofile << "\t\t\t\tfor(int i = 0; i < num_fields; i++)"<< std::endl;
                //ofile << "\t\t\t\t{"<< std::endl;
                itParamEnd = params->end();
                int countparam = 1;
                for(const std::string& param : *params)
                {
                    //ofile << param; 
                    if(param != *itParamEnd)
                    {
                        //ofile << "\t\t\t\t\tthis->" << param << " = (row[i] ? row[i] : NULL);"<< std::endl;
                        ofile << "\t\t\t\t\t$this->" << param << " = ";
                        auto fl = table.find(param.c_str());
                        if(fl != table.end())
                        {
							if((*fl).second->getClassReferenced() != NULL)
							{
								if((*fl).second->outType.compare("int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getInt(" << countparam << ")" << ";" << std::endl ;
								}
								else if((*fl).second->outType.compare("long") or (*fl).second->outType.compare("long int"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getl(" << countparam << ")" << ";" << std::endl ;
								}
								else if((*fl).second->outType.compare("String"))
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getString(" << countparam << ")" << ";" << std::endl ;
								}
								else
								{
									ofile << " new " << (*fl).second->getClassReferenced()->getName() << "(rs.getString(" << countparam << ")" << ";" << std::endl ;
								}
							}
							else if((*fl).second->getOutType().compare("int") == 0)
							{
								ofile << " $rs->getInt(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("long") == 0)
							{
								ofile << " $rs->getl(" << countparam << ")" << ";"<< std::endl ;
							}
							else if((*fl).second->getOutType().compare("String") == 0 || (*fl).second->getOutType().compare("const char*") == 0)
							{
								ofile << " $rs->getString(" << countparam << ")" << ";" << std::endl ;
							}
							else
							{
								ofile << " $rs->getString(" << countparam << ")" << ";" << std::endl ;
							}
                        }
                        else
                        {
                            std::string strmsg = "No se encontro el campo ";
                            strmsg = strmsg + "'" + param + "' en la tabla '" + table.getName() + "' File : generator-c++";
                            throw BuildException(strmsg);
                        }
					}
					countparam++;
				}
                                
				//ofile << "\t\t\t;"<< std::endl;
				//ofile << "\t\t\t\t}"<< std::endl;
				ofile << "\t\t\t}"<< std::endl;
				ofile << "\t\t\treturn true;" << std::endl;
				ofile << "\t\t}" << std::endl;
				ofile << "\t\telse" << std::endl;
				ofile << "\t\t{" << std::endl;
				ofile << "\t\t\treturn false;" << std::endl;
				ofile << "\t\t}" << std::endl;
				}
			ofile << "\t} " << std::endl;
			}         
		}    
		
		
        for(auto symbol : table)
        {
            if(symbol.second->isAutoIncrement() and symbol.second->isPrimaryKey()) continue;
            else
            {
                symbols::Symbol* symroot = getRootSymbol(symbol.second);
                if(symroot->isAutoIncrement() and symroot->isPrimaryKey()) continue;            
            }
                
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\tpublic function down" << symbol.second->upperName << "($connector) \n";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {        
                ofile << "\tpublic function down" << symbol.second->upperName << "($connector) \n";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\tpublic function down" << symbol.second->upperName << "($connector) \n";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
            
            ofile << "\t{\n";
            ofile << "\t\t$sqlString = \"SELECT " << symbol.second->name << " \";\n";
             
            if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\t\t$sqlString = $sqlString . \" FROM \\\"" << table.getName() << "\\\" WHERE \";\n" ;
            }
            else
            {
                ofile << "\t\t$sqlString = $sqlString . \" FROM " << table.getName() << " WHERE \";\n" ;
            }
            
            auto kEnd = table.getKey().end();
            kEnd--;
            for(auto k : table.getKey())
            {
                ofile << "\t\t$sqlString = $sqlString . \"" << k->getName()  << " = \" . ";  
                if(k->classReferenced != NULL) //es un objeto
                {
                    if(k->outType.compare("String") == 0)
                    {
                        ofile << "'" << k->getName() << " . \"'\";\n";
                    }
                    else
                    {
                        ofile << " \"'\" . $this->" << k->getName();
                        getKey(ofile,k->symbolReferenced);
                        ofile<< " . \"'\";\n";
                    }
                }
                else if(k->outType.compare("String") == 0)
                {
                    ofile << " '\" . $this->" <<  k->getName()  << " . \"'\";\n";
                }
                else
                {
                    ofile << "$this->" << k->getName()  <<";\n";
                }
                    
                if(k != *kEnd)
                {
                    ofile << " . \" and \" ";
                }          
            }
            
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                ofile << "\t\t$rs = null;"  << std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {
                ofile << "\t\t$rs = null;"  << std::endl;
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                ofile << "\t\t$rs = null;"  << std::endl;
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                throw BuildException(msg,__FILE__,__LINE__);
            }
                
            ofile << "\t\t$rs = $connector->select($sqlString);"  << std::endl;
            ofile << "\t\tif($rs->nextRow())"  << std::endl;
            ofile << "\t\t{\n";            
            if(symbol.second->symbolReferenced != NULL)
            {
                ofile << "\t\t\t$this->" << symbol.second->name << " = new " << symbol.second->classReferenced->name << "(";
                if(symbol.second->outType.compare("String") == 0)
                {
                    ofile << symbol.second->getName();
                }
                else
                {
                    ofile << "(*" << symbol.second->getName() << ")";
                    getKey(ofile,symbol.second->symbolReferenced);
                }
                ofile << ");\n";
            }
            else
            {
                ofile << "\t\t\t$this->" << symbol.second->name << " = $rs->";
                if(symbol.second->outType.compare("String") == 0)
                {
                    ofile << "getString(\"" << symbol.second->name << "\")";
                }
                else if(symbol.second->outType.compare("Integer") == 0)
                {
                    ofile << "getInt(\"" << symbol.second->name << "\")";
                }
                else if(symbol.second->outType.compare("Float") == 0)
                {
                    ofile << "getFloat(\"" << symbol.second->name << "\")";
                }
                else if(symbol.second->outType.compare("Boolean") == 0)
                {
                    ofile << "getDouble(\"" << symbol.second->name << "\")";
                }
                else
                {
                    std::string msg = "El tipo de dato '";
                    msg += symbol.second->outType + "' no tiene conversion en el metodo de descarga.";
                    throw BuildException(msg,__FILE__,__LINE__);
                }
            }
            ofile << ";\n";
            ofile << "\t\t\treturn true;\n";
            ofile << "\t\t}\n";
            ofile << "\t\treturn false;\n";
            ofile << "\t}\n";
            
        }
    }
    /*
    void PHP::getKey2Java(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << ".get" << k->getUpperName() << "()";
            getKey2Java(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << ".get" << k->getUpperName() << "()";
        }
    }
    */
    /*void PHP::writeSelectInstancetObjectData(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
		ofile << "\tpublic boolean ";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            ofile << "select(octetos.db.mysql.Connector connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "select(octetos.db.maria.Connector connector";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "select(octetos.db.postgresql.Connector connector";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }
        for(symbols::Symbol* k : table.getKey())
        {
			if(k->symbolReferenced != NULL)
			{
				ofile << "," << k->classReferenced->getName() << " ";
			}
			else
			{
				ofile << "," << k->getOutType() << " " ;
			}  
			
			ofile << k->getName();
		}
        ofile << ") throws SQLException"<<std::endl;
        ofile << "\t{\n";
        ofile << "\t\tString sql = \"SELECT ";
        symbols::Key::const_iterator penultimo = --table.getKey().end();
        //symbols::Symbol* penultimo = (--(table.end()))->second;
        for(symbols::Symbol* k : table.getKey())
        {
			ofile << " " << k->getName();
            if(k != (*penultimo))
            {
                ofile << ",";
            }
		}
		ofile << "\";\n\t\tsql = sql + \" FROM \" + TABLE_NAME ";
        for(symbols::Symbol* k : table.getKey())
        {
			ofile <<  " + \" WHERE " << k->getName() << " = \" + " ;  
            if(k->symbolReferenced != NULL)
            {
                if(k->outType.compare("String") == 0)
                {
                    ofile << k->getName();
                    getKey2(ofile,k->symbolReferenced);
                }
                else
                {
                    
                    ofile << k->getName();
                    getKey2(ofile,k->symbolReferenced);
                }
            }
            else if(k->outType.compare("String") == 0)
            {
                ofile << k->getName();
            }
            else
            {
                ofile << k->getName();
            }
            if(k != (*penultimo) )
            {
                ofile << " and ";
            }
		}
		ofile << ";\n";
        
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            ofile << "\t\tResultSet rs = null;\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            ofile << "\t\tResultSet rs = null;\n";
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            ofile << "\t\tResultSet rs = null;\n";
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg,__FILE__,__LINE__);
        }
		ofile << "\t\trs = connector.select(sql);\n";
        ofile << "\t\tif(rs != null)\n";
        ofile << "\t\t{\n";
        for(symbols::Symbol* k : table.getKey())
        {
            if(k->symbolReferenced != NULL)
            {
                ofile << "\t\t\t$this->" << k->name << " = new " << k->classReferenced->name << "(" << k->name << ");\n";
            }
            else
            {
                ofile << "\t\t\t$this->" << k->name << " = " << k->name << ";\n";
            }
        }        
        ofile << "\t\t\treturn true;\n";
        ofile << "\t\t}\n";
        //ofile << "\t\t\n";
        ofile << "\t\treturn false;\n";
        ofile << "\t}\n";
	}*/
    void PHP::writeInsert(const apidb::symbols::Table& table,std::ofstream& ofile)	
	{
        Insert insert(configureProject,table,ofile);
        insert.setImplementation(true);
        insert.generate();
	}
	void PHP::writeDefaultContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
		ofile <<"\tpublic "<<table.getName()<<"()"<<std::endl;
		ofile <<"\t{"<<std::endl;
		ofile <<"\t}"<<std::endl;
	}
	void PHP::writeCopyContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor de copias 
		ofile << "\tpublic " << table.getName() <<"(" << table.getName() <<" obj)"<<std::endl;
		ofile << "\t{"<<std::endl;                
        //for (auto const& [key, attr] : table)
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
		{
			ofile << "\t\tthis."<< it->second->getName()  << " = obj." << it->second->getName() <<";"<<std::endl;
		}
		ofile << "\t}"<<std::endl;
	}
	void PHP::writeKeyRawDataContructor(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//constructor que toma key como parametro
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\tpublic function create(";
            auto endIt = table.getKey().end();
            endIt--;
            for(auto k : table.getKey())
            {
                if(k->getOutType().compare("String") == 0)
                {
                    ofile << "$" << k->getName() ;
                }
                else
                {
                    ofile << "$" << k->getName() ;                    
                }
                if(k != *endIt)
                {
                    ofile << ",";
                }
            }
            ofile << ")"<<std::endl;
		}
		else
		{
			// no tiene constructor con llaves
		}
        if(table.getKey().size() > 0)//tiene key
        {
            ofile << "\t{" <<std::endl;
            if(table.getKey().size() > 1) throw core::Exception("Llave complet aun no soportada",__FILE__,__LINE__);
            for(auto k : table.getKey())
            {//TODO: La catidad de parametros deve variar de acuerdo a la cantidad de elementos en la llave: soport para llave compleja.
                if(k->getClassReferenced() != NULL)
                {
                    ofile << "\t\t$this->" << k->getName()  << " = new " << k->getClassReferenced()->getName() << "();" << std::endl;
                    ofile << "\t\t$this->" << k->getName() << "->create($" << k->getName()  << ");" << std::endl;
                }
                else
                {
                    ofile << "\t\t$this->" << k->getName()  << " = $" << k->getName()  << ";" << std::endl;
                }
            }
            ofile << "\t}" <<std::endl;
        }
        else 
        {
            
        }
	}
	/*void PHP::getKeyJava(std::ofstream& ofile, const symbols::Symbol* k)
    {
        if(k->symbolReferenced != NULL)
        {            
            ofile << ".get" << k->getUpperName() << "()";
            getKey2Java(ofile,k->symbolReferenced);
        }
        else
        {
            ofile << ".get" << k->getUpperName() << "()";
        }
    }*/
	void PHP::createClassMethodes(const apidb::symbols::Table& table,std::ofstream& ofile)
	{
		//writeDefaultContructor(table,ofile);
		writeKeyRawDataContructor(table,ofile);
		//writeCopyContructor(table,ofile);
        
        ofile << "\n\n";
        writeGetters(table,ofile);
        ofile << "\n\n";
        writeUppdaters(table,ofile);
		ofile << "\n\n";
		writeInsert(table,ofile);
        ofile << "\n\n";        
        //writeSelectInstancetObjectData(table,ofile);
		writeSelects(table,ofile);
        ofile << "\n\n";        
        writeDownloads(table,ofile);
        ofile << "\n\n";       
        writeRemoves(table,ofile);
        ofile << "\n\n"; 
    }
        
    void PHP::createClass(const apidb::symbols::Table& cl,std::ofstream& file,const std::string& nameClass)
    {
        file << "\nclass " << cl.getUpperName() << "\n{\n";
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL or configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            file << "\tprivate $TABLE_NAME = \"`"<<  cl.getFullName() << "`\";" << std::endl;
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            file << "\tprivate $TABLE_NAME = \""<<  cl.getFullName() << "\";" << std::endl;
        }
        else
        {
            std::string msg = "Lenguaje de entrado no soportado.";
			throw BuildException(msg,__FILE__,__LINE__);
        }
        file<< "\n";
        createClassAttributes(cl,file);
        file<< "\n\n";
		createClassMethodes(cl,file);   
        file<< "}\n";
		file<< std::endl;
    }
    bool PHP::create(std::ofstream& file,bool log,const symbols::ISpace* ispace)
	{
		if(ispace->what() == symbols::SpaceType::TABLE)
        {
            symbols::Table* table = (symbols::Table*) ispace;
            //std::string strFilename = configureProject.builDirectory;
            //strFilename += "/" + table->getName() + ".java";
            //std::ofstream file(strFilename, std::ofstream::out);
            createClass(*table,file,table->getName());
            //file.flush();
            //file.close();
        }
        else if(ispace->what() == symbols::SpaceType::SPACE)
        {
            symbols::Space* space = (symbols::Space*) ispace;
            //std::cout << "Es Espacio " << space->getFullName() << std::endl;
            //file << "namespace " ;
            if(space->getName().empty())
            {
                //file << configureProject.name;
            }
            else
            {
                //file << space->getName() << std::endl;
            }
            //file << "\n{\n";
            //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
            for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
            {
                create(file,log,it->second);
            }
            //file << "\n}\n";
            //file << std::endl;				
        }
		
		return true;
	}
    bool PHP::create(std::ofstream& file,bool log,const symbols::SymbolsTable& stb)
	{
        if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
        {
            throw core::Exception("Faltas los encabazados de MySQL",__FILE__,__LINE__);
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
        {
            
        }
        else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
        {
            throw core::Exception("Faltas los encabazados de PosrgreSQL",__FILE__,__LINE__);
        }
        else
        {
            std::string msg = "Lenguaje no soportado " ;
            throw BuildException(msg);
            return false;
        }

		//getSourceOutput()<< "#include \"" << getHeaderName() <<"\""<<std::endl<<std::endl;
        
		for(symbols::SymbolsTable::const_iterator it = stb.begin(); it != stb.end(); it++)
		{
			symbols::ISpace* ispace = it->second;
			if(ispace->what() == symbols::SpaceType::TABLE)
            {
                symbols::Table* table = (symbols::Table*) ispace;
                //std::string strFilename = configureProject.builDirectory;
                //strFilename += "/" + table->getName() + ".java";
                //std::ofstream file(strFilename, std::ofstream::out);
                createClass(*table,file,table->getName());
                //file.flush();
                //file.close();
            }
            else if(ispace->what() == symbols::SpaceType::SPACE)
            {
                symbols::Space* space = (symbols::Space*) ispace;
                //file << "namespace " ;
                //if(space->getName().empty())
                {
                    //file << configureProject.name;
                }
                //else
                {
                    //file << space->getName() << std::endl;
                }
                //file << "{\n";
                //std::cout << "Espacio '" << space->getFullName() << "'" << std::endl;
                //file << "\n\n";
                for(symbols::Space::iterator it = space->begin(); it != space->end(); it++)
                {
                    create(file,log,it->second);
                }
                //file << "\n}\n";
                //file << std::endl;
            }
		}
		return true;
	}
    
	/*
    symbols::Symbol* getRootSymbolJava(symbols::Symbol* k)
    {
        if(k == NULL) return NULL;
        
        if(k->symbolReferenced != NULL)
        {
            return getRootSymbol(k->symbolReferenced);
        }
        else
        {
            return k;
        }
    }
    
	void insertParamsRawJava(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertParamsRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << l->outType << " " << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }
    
	void insertValueRawJava(std::ofstream& ofile,symbols::Symbol* k,symbols::Symbol* parent)
    {
        if(k->symbolReferenced != NULL)
        {
            if(k->symbolReferenced->symbolReferenced != NULL)
            {
                insertValueRaw(ofile,k->symbolReferenced,parent);
            }     
            else
            {
                auto penultimo = k->symbolReferenced->classParent->getRequired().begin();
                penultimo--;
                penultimo--;
                for(symbols::Symbol* l : k->symbolReferenced->classParent->getRequired())
                {
                    ofile << parent->name << l->upperName;
                    if(*penultimo != l)
                    {
                        ofile << ",";
                    }
                }
            }
        }
    }*/
    
    	
    void PHP::createClassAttributes(const apidb::symbols::Table& table,std::ofstream& ofile)
    {
        for(std::map<const char*,symbols::Symbol*,symbols::cmp_str>::const_iterator it = table.begin(); it != table.end(); it++)
        {
            if(it->second->getClassReferenced() != NULL && (it->second->getOutType().compare("int") == 0 || it->second->getOutType().compare("String") == 0))
            {
                ofile << "\tprivate $" << it->second->getName()<<";"<< std::endl;
            }
            else
            {
                //ofile <<"[3]"<<std::endl;
                ofile << "\tprivate $" << it->second->getName() <<";"<< std::endl;
            }
        }
    }
    
    bool PHP::createDatconnect(std::ofstream& file,bool log)
	{
        //is function?
        int lgf = configureProject.writeDatconnect.length();
        std::string strF = configureProject.writeDatconnect.substr(lgf - 2,lgf);
        if(strF.compare("()") == 0)
        {
            file << configureProject.writeDatconnect  << "\n";
            return true;
        }
        
		if(!configureProject.writeDatconnect.empty() and configureProject.writeDatconnect.compare("¿?") != 0)
		{
            if(configureProject.getInputLenguaje() == InputLenguajes::MySQL)
            {        
                file << "\tstatic const octetos.db.mysql.Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::MariaDB)
            {                    
                file << "\tstatic const octetos.db.maria.Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else if(configureProject.getInputLenguaje() == InputLenguajes::PostgreSQL)
            {
                file << "\tstatic const octetos.db.postgresql.Datconnect " << configureProject.writeDatconnect  << "(";
            }
            else
            {
                std::string msg = "Lenguaje no soportado " ;
                core::Error::write(core::Error(msg,ErrorCodes::GENERATOR_FAIL,__FILE__,__LINE__));
                return false;
            }

			file << "\"" << configureProject.getDatconnection()->getHost() << "\",";
			file << configureProject.getDatconnection()->getPort() << ",";
			file << "\"" << configureProject.getDatconnection()->getDatabase() << "\",";
			file << "\"" << configureProject.getDatconnection()->getUser() << "\",";
			file << "\"" << configureProject.getDatconnection()->getPassword() << "\"";
			file << ");\n";
		}
		
		return true;
	}
}
}
}
