#ifndef TABLE_OF_COMPONENT_RATES_H
#define TABLE_OF_COMPONENT_RATES_H
#include"function_for_component_rate.h"
#include <QtCore>
namespace Ui {
    class Table_of_component_rates;
}


namespace simbad{
namespace gui{




class Table_of_component_rates{


public:
    int Number_of_types = 1;
    QVector<function_for_component_rate> Table;

//constructor
    Table_of_component_rates ();
    Table_of_component_rates (int Number_of_types_in_model_for_component_rates);
    Table_of_component_rates (const Table_of_component_rates& that);
    Table_of_component_rates & operator = (const Table_of_component_rates& that)
    {

       Number_of_types = that.Number_of_types;
//+++
       Table = that.Table;
//+++
       return *this;
    }

    // Overloading operators

    bool operator == (const Table_of_component_rates& that)
        {
            if (       (this->Table == that.Table) and
                       (this->Number_of_types == that.Number_of_types)
                ) return true;

            return false;
        }



};


}//namespace gui
}//namespace simbad


#endif // TABLE_OF_COMPONENT_RATES_H
