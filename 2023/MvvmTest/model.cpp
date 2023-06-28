#include "model.h"
#include <iostream>

void Model::showPropertiesName() const
{
    for( const auto& prop : _props )
        std::cout << prop.name() << std::endl;
}
void Model::showAll() const
{
    for( const auto& prop : _props )
    {
        std::cout << prop.name() << ": ";
        switch( prop.value().index() )
        {
        case 0:
            std::cout << std::get<0>(prop.value()) << std::endl;
            break;
        case 1:
            std::cout << std::get<1>(prop.value()) << std::endl;
            break;
        case 2:
            std::cout << std::get<2>(prop.value()) << std::endl;
            break;
        case 3:
            std::cout << std::get<3>(prop.value()) << std::endl;
            break;
        case 4:
            std::cout << std::get<4>(prop.value()) << std::endl;
            break;
        case std::variant_npos:
            std::cout << std::endl;
            break;
        }
    }
}
