#pragma once

#include <QString>
#include <QSize>

#include "bindey/binding.h"
#include "bindey/property.h"
#include "blockmodel.h"
#include <QPoint>

class BlockViewModel
{
public:
    struct MouseEventMetadata
    {
//        MouseEventMetadata() : {}
        uint32_t x;
        uint32_t y;
        union
        {
            struct
            {
                uint8_t left    : 1;
                uint8_t right   : 1;
                uint8_t middle  : 1;
                uint8_t shift   : 1;
                uint8_t alt     : 1;
                uint8_t ctrl    : 1;
            };
            uint8_t raw;
        };
    };
public:
    BlockViewModel(BlockModel& model);

public: //commands
    void setSize(QSize size);
    void onClick(QPoint pos,MouseEventMetadata meta=MouseEventMetadata{});
    void onMouseMove(QPoint pos,MouseEventMetadata meta=MouseEventMetadata{});
public: //properties
    bindey::property<QSize>     _size;
    bindey::property<QPoint>    _markPoint;
    bindey::property<bool>      _drawMark;

public: //events
    nod::signal<void()> changedSignal;

private: //models
    BlockModel& _model;
};

