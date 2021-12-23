#ifndef LOADINGSPINNER_H
#define LOADINGSPINNER_H

#include <QLabel>
#include <QObject>
#include <QVariantAnimation>

class LoadingSpinner : public QLabel {

    Q_OBJECT
public:
    explicit LoadingSpinner(QWidget* parent = Q_NULLPTR) :
        QLabel(parent),
        pixmap(32, 32),
        animation(new QVariantAnimation)
    {
        pixmap.load(":/icons/icons/loading.png");
        resize(100, 100);


        animation->setDuration(1000);
        animation->setStartValue(0.0f);
        animation->setEndValue(90.0f);
        animation->setLoopCount(-1);
        connect(
            animation,
            &QVariantAnimation::valueChanged,
            [=](const QVariant &value){
                QTransform t;
                t.rotate(value.toReal());
                setPixmap(pixmap.transformed(t));
            }
        );
        animation->start();
    }
private:
    QPixmap             pixmap;
    QVariantAnimation  *animation;
};


#endif // LOADINGSPINNER_H
