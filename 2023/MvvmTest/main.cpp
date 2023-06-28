#include "mainwindow.h"

#include <QApplication>

#include "bindey/binding.h"
#include "bindey/property.h"
#include <iostream>
#include <variant>
#include <string>
#include "NamedProperty.h"
#include "json/json.h"

#include "model.h"

//template<typename T>
//void function(const T& value)
//{
//    using strt = std::conditional_t<   std::is_same_v<T,std::string>
//                                    || std::is_same_v<T,char*>
//                                    || std::is_same_v<T,const char*>
//                                    || std::is_same_v<std::remove_all_extents_t<T>,char>
//                                    ,  std::string,void>;
//    std::cout << strt(value) << std::endl;
////    static_assert( std::is_same_v<T,std::string> , "no std::string");
////    static_assert( std::is_same_v<T,const char*> , "no const char*");
////    static_assert( std::is_same_v<T,char*>       , "no char*");
////    static_assert( std::is_same_v<T,char[2]>       , "no char[]");
//}

int main(int argc, char *argv[])
{
////    function("23");
////    return 0;
//    Model model;
//    model.addProp("age",15.2f);
//    model.addProp("height",335);
//
//    model.showAll();
//    return 0;

//    const std::string json = R"(
//{
//    "age" : 39,
//    "name": "me",
//    "obj" :
//    {
//        "value" : 23,
//        "other" : "other_value"
//`    }
//}
//)";
//    Json::Value root;
//    Json::Reader reader;
//    reader.parse(json,root);
////    root["obj"]["value"] = 495;
//    std::cout << root["age"] << std::endl;
//    std::cout << root["name"] << std::endl;
//    std::cout << root["obj"] << std::endl;
//    std::cout << root["obj"]["value"] << std::endl;
//
//    return 0;

//    std::cout << sizeof(nod::unsafe_signal<void()>) << std::endl;
//    std::cout << sizeof(NamedProperty<nod::unsafe_signal<void()>>) << std::endl;
//    std::cout << sizeof(std::vector<NamedProperty<nod::signal<void()>>>)
//              << std::endl;
//    return 0;

//    using PropType = std::variant<std::string,
//                                  uint64_t,
//                                  int64_t,
//                                  double,
//                                  bool>;
//    std::cout << sizeof(std::vector<bindey::property<PropType>>)
//              << std::endl;
//
//    return 0;

//    std::variant<std::string*,
//                 int*,
//                 double*,
//                 bool*,
//                 NamedProperty<std::vector<uint8_t>>*> var;
//    var = new NamedProperty<std::vector<uint8_t>>{"type_id"};
//    if( auto v = std::get_if<NamedProperty<std::vector<uint8_t>>*>(&var) )
//    {
//        (*v)->value().push_back(23);
//        (*v)->value().push_back(55);
//        (*v)->value().push_back(77);
//        (*v)->value().push_back(89);
//    }
//    if( auto v = std::get_if<NamedProperty<std::vector<uint8_t>>*>(&var) )
//        for( const auto& item : (*v)->value() )
//            std::cout << int(item) << std::endl;
//    std::cout << "sizeof(var): " << sizeof(var) << std::endl;
//    std::cout << "sizeof(std::pair<std::string,std::vector<uint8_t>>*): " << sizeof(std::pair<std::string,std::vector<uint8_t>>*) << std::endl;
//    if( auto v = std::get_if<NamedProperty<std::vector<uint8_t>>*>(&var) )
//        delete (*v);
//    return 0;

//    bindey::property<QString> p1;
//    bindey::property<QString> p2;
//    bindey::bind(p1,p2);
//    bindey::bind(p2,p1);
//    p1.set("value");
//    std::cout << "p2: " << p2.get().toStdString() << std::endl;
//    p2.set("other value");
//    std::cout << "p1: " << p1.get().toStdString() << std::endl;
//    return 0;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

