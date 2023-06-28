#define TEST
#ifndef TEST
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
#else

#include <iostream>
#include "nod.hpp"
#include "binding.h"
#include "property.h"
#include <QString>
#include <QDebug>

void slot1(){ std::cout << "slot1()" << std::endl; }
void slot2(){ std::cout << "slot2()" << std::endl; }

struct Test{ static void method(){ std::cout << "method()" << std::endl; } };

int main1()
{
    nod::signal<void()> signal;
    Test t;
    auto conn1 = signal.connect(slot1);
    auto conn2 = signal.connect(t.method);
    signal();
    conn2.disconnect();
    signal();
    return 0;
}
template<typename T> void show(const T& t){ qDebug() << t; }
int main()
{
    bindey::property<QString> var1_prop1;
    {
        bindey::property<QString> var2_prop1;
        bindey::property<QString> var3_prop1;
        bindey::property<QString> var4_prop1;
        auto con1 = bindey::bind(var1_prop1,var2_prop1);
        auto con2 = bindey::bind(var1_prop1,var3_prop1);
        var1_prop1.set("value");
        auto con3 = bindey::bind(var1_prop1,var4_prop1);
        show(var2_prop1.get());
        show(var3_prop1.get());
        show(var4_prop1.get());
        con1.disconnect();
        con2.disconnect();
        con3.disconnect();
    }
    var1_prop1.set("value2");
    return 0;
}
#endif
