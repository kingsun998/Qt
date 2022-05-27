#include "datastruct.h"
#include "settings.h"

DataStruct::DataStruct()
{

}

DataStruct::~DataStruct(){

}

void DataStruct::setParameter(QString company_type){
    this->company_type=company_type;
    ttnames=settings.company_col_title;
    use_names=settings.company_usedate_names;
    reset();
}

void DataStruct::reset(){
     obj.clear();
     for(int j=0;j<use_names.size();j++) {
         QMap<QString,QVector<QString>> vec;
         for (int k=0;k<ttnames.size();k++) {
             QVector<QString> mes;
             vec[ttnames[k]]=mes;
         }
         obj[use_names[j]]=vec;
     }

}


