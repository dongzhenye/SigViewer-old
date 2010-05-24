#include "label_widget.h"

#include <cmath>

#include <QPainter>

namespace BioSig_
{

// constructor
LabelWidget::LabelWidget(SignalBrowserModel& model)
: signal_browser_model_(model),
  y_start_ (0)
{
    // nothing to do here
}

//-----------------------------------------------------------------------------
void LabelWidget::changeYStart (int32 y_start)
{
    y_start_ = y_start;
    update ();
}


//-----------------------------------------------------------------------------
void LabelWidget::paintEvent(QPaintEvent*)
{
    float64 signal_height = signal_browser_model_.getSignalHeight();
    int32 y_end = y_start_ + height();

    if (signal_height < 1)
        return;

    QPainter p(this);
    p.translate(0, -y_start_);
    p.drawLine(0, y_start_, 0, y_end);

    // labels
    float64 float_y_start = y_start_;//floor(static_cast<float64>(y_start) / intervall) * intervall;
    float64 float_y_end = y_end;//ceil(y_end / intervall) * intervall;
    QMap<int32, QString>::iterator iter = channel_nr2label_.begin();

    for (float32 float_y = signal_height / 2;
         float_y < float_y_end && iter != channel_nr2label_.end();
         float_y += signal_height, iter++)
    {
        if (float_y > float_y_start)
        {
            int32 y = (int32)(float_y + 0.5);
            p.drawText(5, (int32)(y - signal_height /2) , width() - 10, (int32)signal_height,
                       Qt::AlignHCenter | Qt::AlignVCenter, iter.value());
        }
    }

    for (float32 float_y = 0;
         float_y <= signal_height * channel_nr2label_.size();
         float_y += signal_height)
    {
        p.drawLine(0, float_y, width() - 1, float_y);
    }
}

//-----------------------------------------------------------------------------
void LabelWidget::addChannel(int32 channel_nr, const QString& label)
{
    channel_nr2label_[channel_nr] = label;
    QPixmap dummy(1,1);
    QPainter p(&dummy);
    QRect bounding = p.boundingRect(0, 0, 500, 500,
                                    Qt::AlignHCenter | Qt::AlignVCenter, label);
    int32 max_width = minimumWidth() - 10;
    max_width = bounding.width() > max_width ? bounding.width() : max_width;
    setMinimumWidth(10 + max_width);
}

//-----------------------------------------------------------------------------
void LabelWidget::removeChannel(int32 channel_nr)
{
    channel_nr2label_.erase(channel_nr2label_.find(channel_nr));
    int32 max_width = -1;
    QPixmap dummy(1,1);
    QPainter p(&dummy);

    for (QMap<int32, QString>::iterator iter = channel_nr2label_.begin();
         iter != channel_nr2label_.end();
         iter++)
    {
        QRect bounding = p.boundingRect(0, 0, 200, p.font().pixelSize(),
                                        Qt::AlignHCenter | Qt::AlignVCenter,
                                        iter.value());

        max_width = max_width < bounding.width() ? bounding.width() : max_width;
    }

    setMinimumWidth(10 + max_width);
}

} // namespace BioSig_

