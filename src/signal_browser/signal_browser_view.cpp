// signal_browser.cpp

#include "signal_browser_view.h"
#include "signal_browser_model.h"
#include "signal_graphics_item.h"
#include "event_graphics_item.h"
#include "y_axis_graphics_item.h"
#include "x_axis_graphics_item.h"
#include "../label_widget.h"


#include <QGridLayout>
#include <QScrollBar>
#include <QPointF>

#include <iostream>


namespace BioSig_
{

namespace PortingToQT4_
{

//-----------------------------------------------------------------------------
// constructor
SignalBrowserView::SignalBrowserView(SignalBrowserModel* signal_browser_model, QWidget* parent)
: QFrame(parent),
  center_x_for_scrolling_ (0),
  center_y_for_scrolling_ (0)
{
    resize(parent->contentsRect().width(), parent->contentsRect().height()-300);
    graphics_scene_ = new QGraphicsScene (0,0,parent->contentsRect().width(), parent->contentsRect().height(), this);
    graphics_view_ = new QGraphicsView(graphics_scene_, this);
    graphics_view_->scroll(0,0);
    graphics_view_->horizontalScrollBar()->hide();
    graphics_view_->verticalScrollBar()->hide();

    y_axis_widget_ = new YAxisWidget (this, *signal_browser_model, this);
    y_axis_widget_->resize(70, height());
    y_axis_widget_->setMinimumSize(70, 0);

    x_axis_widget_ = new XAxisWidget (this, *signal_browser_model, this);
    x_axis_widget_->resize(100, 40);
    x_axis_widget_->setMinimumSize(100, 40);

#ifdef QT4_PORTED
    label_widget_ = new LabelWidget (*signal_browser_model, this);
    label_widget_->setMinimumWidth(40);
    label_widget_->setMinimumHeight(20);
#endif

    horizontal_scrollbar_ = new QScrollBar (Qt::Horizontal, this);
    vertical_scrollbar_ = new QScrollBar (Qt::Vertical, this);

    connect(horizontal_scrollbar_, SIGNAL(valueChanged(int)),
            graphics_view_->horizontalScrollBar(), SLOT(setValue(int)));
    connect(graphics_view_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            horizontal_scrollbar_, SLOT(setValue(int)));
    connect(graphics_view_->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)),
            this, SLOT(horizontalScrollBarRangeChaned(int,int)));
    connect(horizontal_scrollbar_, SIGNAL(valueChanged(int)),
            this, SLOT(horizontalSrollbarMoved(int)));

    connect(vertical_scrollbar_, SIGNAL(valueChanged(int)),
            graphics_view_->verticalScrollBar(), SLOT(setValue(int)));
    connect(graphics_view_->verticalScrollBar(), SIGNAL(valueChanged(int)),
            vertical_scrollbar_, SLOT(setValue(int)));
    connect(graphics_view_->verticalScrollBar(), SIGNAL(rangeChanged(int,int)),
            this, SLOT(verticalScrollBarRangeChaned(int,int)));
    connect(vertical_scrollbar_, SIGNAL(valueChanged(int)),
            this, SLOT(verticalSrollbarMoved(int)));

    graphics_view_->resize(this->width()-100, this->height()-50);
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    createLayout();


//    connect(canvas_, SIGNAL(resized()), this, SLOT(updateScrollBarRange()));
//    connect(canvas_view_->horizontalScrollBar(), SIGNAL(valueChanged(int)),
//            h_scrollbar_, SLOT(setValue(int)));
//    connect(h_scrollbar_, SIGNAL(valueChanged(int)),
//            canvas_view_->horizontalScrollBar(), SLOT(setValue(int)));
//    connect(canvas_view_->verticalScrollBar(), SIGNAL(valueChanged(int)),
//            v_scrollbar_, SLOT(setValue(int)));
//    connect(v_scrollbar_, SIGNAL(valueChanged(int)),
//            canvas_view_->verticalScrollBar(), SLOT(setValue(int)));
//    connect(h_scrollbar_, SIGNAL(valueChanged(int)),
//            this, SLOT(horizontalSrollbarMoved(int)));
//    connect(v_scrollbar_, SIGNAL(valueChanged(int)),
//            this, SLOT(verticalSrollbarMoved(int)));
}

//-----------------------------------------------------------------------------
SignalBrowserView::~SignalBrowserView ()
{

}

//-----------------------------------------------------------------------------
void SignalBrowserView::resizeScene (int32 width, int32 height)
{
    graphics_scene_->setSceneRect(0, 0, width, height);
    graphics_view_->centerOn(0,0);
    //graphics_view_->viewport()->set
}

//-----------------------------------------------------------------------------
void SignalBrowserView::addSignalGraphicsItem (int32 channel_nr, SignalGraphicsItem* graphics_item)
{
    // TODO: really remove before add????
    graphics_scene_->removeItem(graphics_item);
    graphics_scene_->addItem(graphics_item);
    y_axis_widget_->addChannel(channel_nr, graphics_item);
    label_widget_->addChannel(channel_nr, QString::number(channel_nr));
 //   graphics_scene_->addLine(200, 0, 200, 200);

    graphics_view_->update();
}

//-----------------------------------------------------------------------------
void SignalBrowserView::removeSignalGraphicsItem (int32 channel_nr)
{
//    graphics_scene_->removeItem(graphics_item);
//    graphics_scene_->addItem(graphics_item);
//    y_axis_widget_->addChannel(channel_nr, graphics_item);
//    graphics_view_->update();
}

//-----------------------------------------------------------------------------
void SignalBrowserView::addEventGraphicsItem (EventGraphicsItem* event_graphics_item)
{
    // TODO: really remove before add????
    graphics_scene_->removeItem(event_graphics_item);
    graphics_scene_->addItem(event_graphics_item);
    std::cout << "SignalBrowserView::addEventGraphicsItem" << std::endl;

    graphics_view_->update();
}


//-----------------------------------------------------------------------------
int32 SignalBrowserView::getVisibleWidth () const
{
    return graphics_view_->viewport()->width();
}

//-----------------------------------------------------------------------------
int32 SignalBrowserView::getVisibleHeight () const
{
    return graphics_view_->viewport()->height();
}

//-----------------------------------------------------------------------------
int32 SignalBrowserView::getVisibleX () const
{
    return graphics_view_->mapToScene(0,0).x();
}

//-----------------------------------------------------------------------------
int32 SignalBrowserView::getVisibleY () const
{
    return graphics_view_->mapToScene(0,0).y();
}

//-----------------------------------------------------------------------------
YAxisWidget& SignalBrowserView::getYAxisWidget () const
{
    return *y_axis_widget_;
}

//-----------------------------------------------------------------------------
LabelWidget& SignalBrowserView::getLabelWidget () const
{
    return *label_widget_;
}

//-----------------------------------------------------------------------------
void SignalBrowserView::goTo (int32 x, int32 y)
{
    center_x_for_scrolling_ = x;
    center_y_for_scrolling_ = y;
    std::cout << "x = " << x << "; y = " << y << std::endl;
    graphics_view_->centerOn(center_x_for_scrolling_, center_y_for_scrolling_);
}

//-----------------------------------------------------------------------------
void SignalBrowserView::scrollContente (int32 dx, int32 dy)
{
    center_y_for_scrolling_ -= dy;
    center_x_for_scrolling_ -= dx;
    // FIXME: dieser call löst vermutlich (?) wieder ein mousemoveevent aus... :(
    graphics_view_->centerOn(center_x_for_scrolling_, center_y_for_scrolling_);
    //graphics_view_->horizontalScrollBar()->setValue(graphics_view_->horizontalScrollBar()->value() - dx);
    //graphics_view_->verticalScrollBar()->setValue(graphics_view_->verticalScrollBar()->value() - dy);
    //updateWidgets();
}

//-----------------------------------------------------------------------------
void SignalBrowserView::updateWidgets ()
{
    y_axis_widget_->update();
    x_axis_widget_->update();
    label_widget_->update();
}


//-----------------------------------------------------------------------------
void SignalBrowserView::initScroll ()
{
    int32 center_x = graphics_view_->viewport()->width() / 2;
    int32 center_y = graphics_view_->viewport()->height() / 2;
    QPointF scene_center = graphics_view_->mapToScene(center_x, center_y);
    center_y_for_scrolling_ = scene_center.y();
    center_x_for_scrolling_ = scene_center.x();
}

//-----------------------------------------------------------------------------
void SignalBrowserView::verticalSrollbarMoved(int value)
{
    y_axis_widget_->repaint();
    label_widget_->repaint();
}

//-----------------------------------------------------------------------------
void SignalBrowserView::horizontalSrollbarMoved(int value)
{
    x_axis_widget_->repaint();
}

//-----------------------------------------------------------------------------
void SignalBrowserView::horizontalScrollBarRangeChaned (int min, int max)
{
    x_axis_widget_->repaint();
    horizontal_scrollbar_->setRange(min, max);
}

//-----------------------------------------------------------------------------
void SignalBrowserView::verticalScrollBarRangeChaned (int min, int max)
{
    y_axis_widget_->repaint();
    label_widget_->repaint();
    vertical_scrollbar_->setRange(min, max);
    vertical_scrollbar_->setPageStep(vertical_scrollbar_->height()-max);
}



//-----------------------------------------------------------------------------
void SignalBrowserView::resizeEvent (QResizeEvent * event)
{
    graphics_view_->resize(this->width()-150, this->height()-70);
}

//-----------------------------------------------------------------------------
void SignalBrowserView::createLayout()
{
    layout_ = new QGridLayout(this);

    layout_->setMargin(1);
    layout_->setSpacing(0);

    layout_->addWidget(y_axis_widget_, 1, 1);
    layout_->addWidget(graphics_view_, 1, 2);
    layout_->addWidget(x_axis_widget_, 2, 2);
    layout_->addWidget(horizontal_scrollbar_, 3, 2);
    layout_->addWidget(label_widget_, 1, 3);
    layout_->addWidget(vertical_scrollbar_, 1,4);
//    layout_->addWidget(label_widget_, 1, 3);
//    layout_->addWidget(x_axis_widget_, 2, 2);
//    layout_->addWidget(h_scrollbar_, 3, 1, 1, 3);
//    layout_->addWidget(v_scrollbar_, 1, 4, 2, 1);
//
//    QWidget* dummy = new QWidget(this);
//    dummy->setBackgroundColor(XAxisWidget::getAxisColor());
//    layout_->addWidget(dummy, 2, 1);
//
//    dummy = new QWidget(this);
//    dummy->setBackgroundColor(XAxisWidget::getAxisColor());
//    layout_->addWidget(dummy, 2, 3);
}
/*
// get canvas
SmartCanvas* SignalBrowserView::getCanvas()
{
    return canvas_;
}

// get canvas view
SmartCanvasView* SignalBrowserView::getCanvasView()
{
    return canvas_view_;
}

// get x axis widget
XAxisWidget* SignalBrowserView::getXAxisWidget()
{
    return x_axis_widget_;
}

// get y axis widget
YAxisWidget* SignalBrowserView::getYAxisWidget()
{
    return y_axis_widget_;
}

// get label widget
LabelWidget* SignalBrowserView::getLabelWidget()
{
    return label_widget_;
}

// resize event
void SignalBrowserView::resizeEvent(QResizeEvent* e)
{
    updateScrollBarRange();
}

// update scrollbar range
void SignalBrowserView::updateScrollBarRange()
{
    int32 visible_width = canvas_view_->visibleWidth();
    int32 visible_height = canvas_view_->visibleHeight();
    int32 h_max = canvas_->width() - visible_width;
    int32 v_max = canvas_->height() - visible_height;

    h_scrollbar_->setRange(0, h_max > 0 ? h_max : 0);
    h_scrollbar_->setSteps(10, visible_width);
    v_scrollbar_->setRange(0, v_max > 0 ? v_max : 0);
    v_scrollbar_->setSteps(10, visible_height);
    x_axis_widget_->update();
}

// horizontal scrollbar moved
void SignalBrowserView::horizontalSrollbarMoved(int)
{
    x_axis_widget_->repaint();
}

// vertical scrollbar moved
void SignalBrowserView::verticalSrollbarMoved(int)
{
    label_widget_->repaint();
    y_axis_widget_->repaint();
}
*/
} //namespace PortingToQT4_

} // namespace BioSig_

