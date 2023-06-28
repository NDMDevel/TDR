#ifndef BLOCKCONNECTOR_H
#define BLOCKCONNECTOR_H

#include <QPainter>

class BlockConnector
{
public:
    enum class Direction : uint8_t
    {
        Input,  /**< */
        Output, /**< */
    };
    enum class Type : uint8_t
    {
        Data,   /**< */
        Signal, /**< */
        Flow    /**< */
    };
    enum class State : uint8_t
    {
        Connected,
        Disconnected
    };
    Direction dir;
    Type type;
    State state;
    QString name;
public:
    BlockConnector(Direction dir,
                   Type type,
                   const QString & name,
                   State state=State::Disconnected);
};

#endif // BLOCKCONNECTOR_H
