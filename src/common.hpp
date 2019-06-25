
#ifndef APIDB_COMMON_HPP
#define APIDB_COMMON_HPP
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
 
#include <string>
#include <list>
#include <vector>
#include <map>
#include <clientdb-mysql.hpp>
#include <libxml/xmlreader.h>
#include <iostream>
#include <cstring>

namespace apidb
{
	/*enum InputLenguajes
	{
		MySQL_Server,
		//MySQL_Script,
		//PostgresSQL
	};*/
        typedef octetos::toolkit::clientdb::Datconnect::ServerType InputLenguajes;
	
        enum PackingLenguajes
        {
                CMake
        };
	enum OutputLenguajes
	{
		C,
		CPP,
		//JAVA,
		//CSHARP,
		//PERL,
		//PYTHON
	};
	
	enum MVC
	{
		NO,
		//X,
		GTK2,
		GTK3
		//WIN32
	};
        enum Compiled
        {
                STATIC,
                SHARED
        };
        
	class BuildException : public std::exception
	{
	public:
		virtual ~BuildException() throw();
		virtual const char* what() const throw();
		BuildException(const std::string &description) throw();
        //Exception()throw();
	private:
		std::string description;
	};
    

	namespace symbols
	{
		struct Table;
		struct Tables;
		
                struct cmp_str
                {
                bool operator()(char const *a, char const *b) const
                {
                        return std::strcmp(a, b) < 0;
                }
                };
        
		/**
		 * Informacion sobre cada symbolo
		 * */
		struct Symbol 
		{
                        friend class Table;
            
			enum KeyType
			{
				NOKEY,
				PRIMARY,
				UNIQUE,
				FOREIGN_UNIQUE,
			};
			
					
			//in input lenguaje
                        std::string inType;
                        //in input lenguaje
                        std::string name;                
                        //in out lenguaje
                        std::string get;
			std::string upperName;
			bool required;
                        KeyType keyType;
                        Table* classReferenced;
                        Table* classParent;
                        std::string outType; 
                        Symbol* symbolReferenced;
                        bool isPrimaryKey();
                        bool isForeignKey();
                        bool isAutoIncrement();
            
                        Symbol();
                        int getID()const;
                
                private:
			static int counter;
			int id;	
                        bool isPK;
                        bool isFK;
                        bool isAutoInc;
		};
		
		struct Key : public std::vector<Symbol*>
		{
		};
		/**
		 * Simbolos por alcance(tabla en SQL) 
		 **/
		struct Table : public std::map<const char*,Symbol*,cmp_str>
		{
			std::string name;
                        std::string upperName;
                        Key key;
                        std::list<Symbol*> required;//ademas de porner en true su abtributo se agrega a esta lista    
                        std::string space;
                        
                        Table();
                        ~Table();
			bool basicSymbols(octetos::toolkit::clientdb::Connector& connect);
                        bool fillKeyType(octetos::toolkit::clientdb::Connector& connect, std::map<const char*,symbols::Tables*,symbols::cmp_str>& tables);
                        short getCountRefereces() const; 
                        //std::list<Symbol*>::iterator search(const std::string&);
            
		private:
			short countRef;
		};
		
		/**
		 * Conjunto de tablas
		 * */
		struct Tables : public std::list<Table*>
		{
		public:
			~Tables();	
                        Table* search(const std::string&); 
			std::list<Table*>::iterator find(const std::string& tableName);
                        std::string name;      
			
			short getMaxCountRef();
                        const std::string& getName();                        
                private:                  
		};
                
                bool listing(octetos::toolkit::clientdb::mysql::Connector& connect, std::map<const char*,symbols::Tables*,symbols::cmp_str>& tables);
                std::string getSpaceName(std::string fullname);
                std::string getSpacePatch(std::string fullname);
	}
    class ConfigureProject
    {
    private:
        bool processNode(xmlTextReaderPtr);
        bool getProjectNodes(xmlTextReaderPtr);
        
    public:        
        class Parameters : public std::vector<const char*>
        {
            
        };        
        class Function
        {
        public:
        	enum Skeleton
        	{
        		DOWNLOAD,
        		SELECT,
        		INSERT,
        		UPDATE,
        		COSTUM
        	};
                const std::string& getName() const;
                Function(const std::string&,Skeleton skeleton);
                Function();
                Skeleton getSkeleton() const;
                void setHeader(const Parameters*);
                void setBody(const std::string&);
                const Parameters& getParameters() const;
            
        private:
                std::string name;
                Skeleton skeleton;
                const Parameters* header; 
                std::string body;
        };
        class Table : public std::map<const char*, const Function*>
        {
        private:
                std::string name;
        public:
                const std::string& getName() const;
                Table(const std::string&);
                Table();
        };
        
        std::string name; 
        std::string directory;
        octetos::toolkit::Version version;
        octetos::toolkit::clientdb::mysql::Datconnect* conectordb;
        InputLenguajes inputLenguaje;
        OutputLenguajes outputLenguaje;
        PackingLenguajes packing;
        Compiled compiled;
        MVC mvc;
        std::vector<Table> downloads;
        std::vector<Table> selects;
		
        ConfigureProject(std::string filename);
        ConfigureProject(const ConfigureProject& configProy);
        ConfigureProject();
        bool saveConfig();
        bool checkXML(xmlTextReaderPtr);
		
        const std::string& getName()const;
        const std::string& getDirectory()const;
        const octetos::toolkit::Version& getVersion()const;
        const octetos::toolkit::clientdb::mysql::Datconnect& getConector()const;
    };
    
  
}

#endif
