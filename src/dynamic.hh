
#include <octetos/db/clientdb.hh>


extern "C" octetos::db::Connector* createConnector();
extern "C" void destroyConector(octetos::db::Connector*); 

extern "C" octetos::db::Datconnect* createDatconnect();
extern "C" void destroyDatconnect(octetos::db::Datconnect*);